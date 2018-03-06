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

#include "XTag/XTagProducer/interface/XTagPlugin.h"
#include "XTag/XTagProducer/interface/XTagPluginFactory.h"
#include "XTag/Jet/interface/SVTagData.h"


#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>

#include "TVector3.h"

namespace xtag
{

class SVTagDataPlugin:
    public XTagPlugin
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
            XTagPlugin(name,pset,collector,prod),
            jetToken_(collector.consumes<edm::View<pat::Jet>>(pset.getParameter<edm::InputTag>("jets"))),
            pvToken_(collector.consumes<edm::View<reco::Vertex>>(pset.getParameter<edm::InputTag>("pvVertices"))),
            svToken_(collector.consumes<edm::View<reco::VertexCompositePtrCandidate>>(pset.getParameter<edm::InputTag>("svVertices")))
        {
            prod.produces<std::vector<xtag::SVTagData>>(name);
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
           
            std::unique_ptr<std::vector<xtag::SVTagData>> output(
                new std::vector<xtag::SVTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                const float jet_e_uncorr = jet.correctedJet("Uncorrected").energy();
                
                std::vector<xtag::SVTagData::Data> svData;
                for (unsigned int isv = 0; isv < svCollection->size(); ++isv)
                {
                    const reco::VertexCompositePtrCandidate& sv = svCollection->at(isv);
                    if (reco::deltaR(sv,jet)>0.4)
                    {
                        continue;
                    }
                    
                    xtag::SVTagData::Data data;
                    data.pt = std::log10(sv.pt());
                    
                    data.deltaR = reco::deltaR(sv,jet);
                    data.mass = sv.mass();
                    data.ntracks = sv.numberOfDaughters();
                    data.chi2 = std::log10(sv.vertexChi2());
                    data.normchi2 = std::log10(sv.vertexChi2()/sv.vertexNdof());
                    
                    reco::Vertex::CovarianceMatrix covsv; 
                    sv.fillVertexCovariance(covsv);
                    reco::Vertex svtx(sv.vertex(), covsv);
                    
                    VertexDistanceXY distXY;
                    Measurement1D distanceXY = distXY.distance(svtx, pv);
                    data.dxy = 0.1/(0.1+distanceXY.value());
                    data.dxysig = std::log10(distanceXY.value()/distanceXY.error());
                    
                    VertexDistance3D dist3D;
                    Measurement1D distance3D = dist3D.distance(svtx, pv);
                    data.d3d = 0.1/(0.1+distance3D.value());
                    data.d3dsig = std::log10(distance3D.value()/distance3D.error());
                    
                    reco::Candidate::Vector distance(sv.vx() - pv.x(), sv.vy() - pv.y(), sv.vz() - pv.z());
                    data.costhetasvpv = 0.01/(1.01-sv.momentum().Unit().Dot(distance.Unit()));

                    data.enratio = 0.1/(0.1+sv.energy()/jet_e_uncorr);
                    
                    
                    svData.emplace_back(data);
                }
                output->at(0).jetData.push_back(svData);
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(xtag::XTagPluginFactory, xtag::SVTagDataPlugin, "SVTagData");

