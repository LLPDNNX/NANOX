#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ProducerBase.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"

#include "NANOX/NANOXProducer/interface/NANOXPlugin.h"
#include "NANOX/NANOXProducer/interface/NANOXPluginFactory.h"
#include "NANOX/Jet/interface/SVTagData.h"


#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>

#include "TVector3.h"

namespace nanox
{

class SVTagDataPlugin:
    public NANOXPlugin
{
    private:
        edm::EDGetTokenT<edm::View<pat::Jet>> jetToken_;
        edm::EDGetTokenT<edm::View<reco::Vertex>> pvToken_;
        edm::EDGetTokenT<edm::View<reco::VertexCompositePtrCandidate>> svToken_;
        
    public:
        SVTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            NANOXPlugin(name,pset,collector,prod),
            jetToken_(collector.consumes<edm::View<pat::Jet>>(pset.getParameter<edm::InputTag>("jets"))),
            pvToken_(collector.consumes<edm::View<reco::Vertex>>(pset.getParameter<edm::InputTag>("pvVertices"))),
            svToken_(collector.consumes<edm::View<reco::VertexCompositePtrCandidate>>(pset.getParameter<edm::InputTag>("svVertices")))
        {
            prod.produces<std::vector<nanox::SVTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup& setup) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(jetToken_, jetCollection);
            
            edm::Handle<edm::View<reco::Vertex>> pvCollection;
            event.getByToken(pvToken_, pvCollection);
            
            edm::Handle<edm::View<reco::VertexCompositePtrCandidate>> svCollection;
            event.getByToken(svToken_, svCollection);
            
            const reco::Vertex& pv = pvCollection->at(0);
           
            std::unique_ptr<std::vector<nanox::SVTagData>> output(
                new std::vector<nanox::SVTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                const float jet_e_uncorr = jet.correctedJet("Uncorrected").energy();
                
                std::vector<nanox::SVTagData::Data> svData;
                for (unsigned int isv = 0; isv < svCollection->size(); ++isv)
                {
                    const reco::VertexCompositePtrCandidate& sv = svCollection->at(isv);
                    if (reco::deltaR(sv,jet)>0.4)
                    {
                        continue;
                    }
                    
                    nanox::SVTagData::Data data;
                    data.pt = std::log10(sv.pt());
                    
                    data.deltaR = reco::deltaR(sv,jet);
                    data.mass = sv.mass();
                    data.ntracks = sv.numberOfDaughters();
                    data.chi2 = sv.vertexChi2();
                    data.ndof = sv.vertexNdof();
                    
                    reco::Vertex::CovarianceMatrix covsv; 
                    sv.fillVertexCovariance(covsv);
                    reco::Vertex svtx(sv.vertex(), covsv);
                    
                    VertexDistanceXY distXY;
                    Measurement1D distanceXY = distXY.distance(svtx, pv);
                    data.dxy = distanceXY.value();
                    data.dxysig = distanceXY.value()/distanceXY.error();
                    
                    VertexDistance3D dist3D;
                    Measurement1D distance3D = dist3D.distance(svtx, pv);
                    data.d3d = distance3D.value();
                    data.d3dsig = distance3D.value()/distance3D.error();
                    
                    reco::Candidate::Vector distance(sv.vx() - pv.x(), sv.vy() - pv.y(), sv.vz() - pv.z());
                    data.costhetasvpv = sv.momentum().Unit().Dot(distance.Unit());

                    data.enratio = sv.energy()/jet_e_uncorr;
                    
                    
                    svData.emplace_back(data);
                }
                
                
                std::stable_sort(svData.begin(),svData.end(),[&pv](const auto& d1, const auto& d2)
                {
                    return d1.dxysig>d2.dxysig; //sort decreasing
                });

                output->at(0).jetData.push_back(svData);
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(nanox::NANOXPluginFactory, nanox::SVTagDataPlugin, "SVTagData");

