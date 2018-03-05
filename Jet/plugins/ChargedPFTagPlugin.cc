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

#include "XTag/XTagProducer/interface/XTagPlugin.h"
#include "XTag/XTagProducer/interface/XTagPluginFactory.h"
#include "XTag/Jet/interface/ChargedPFTagData.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include <iostream>

#include "TVector3.h"

namespace xtag
{

class ChargedPFTagDataPlugin:
    public XTagPlugin
{
    private:
        edm::EDGetTokenT<edm::View<pat::Jet>> jetToken_;
        edm::EDGetTokenT<edm::View<reco::Vertex>> vertexToken_;
        
    public:
        ChargedPFTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            XTagPlugin(name,pset,collector,prod),
            jetToken_(collector.consumes<edm::View<pat::Jet>>(pset.getParameter<edm::InputTag>("jets"))),
            vertexToken_(collector.consumes<edm::View<reco::Vertex>>(pset.getParameter<edm::InputTag>("vertices")))
        {
            prod.produces<std::vector<xtag::ChargedPFTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup& setup) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(jetToken_, jetCollection);
            
            edm::Handle<edm::View<reco::Vertex>> vertexCollection;
            event.getByToken(vertexToken_, vertexCollection);
            
            const reco::Vertex& pv = vertexCollection->at(0);
            
            edm::ESHandle<TransientTrackBuilder> builder;
            setup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
            
            std::unique_ptr<std::vector<xtag::ChargedPFTagData>> output(
                new std::vector<xtag::ChargedPFTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                std::vector<xtag::ChargedPFTagData::Data> cpfData;
                for (unsigned int idaughter = 0; idaughter < jet.numberOfDaughters(); ++idaughter)
                {
                    const pat::PackedCandidate* constituent = dynamic_cast<const pat::PackedCandidate*>(jet.daughter(idaughter));
                    if ((not constituent) or constituent->charge()==0 or (not constituent->hasTrackDetails()))
                    {
                        continue;
                    }
                    
                    xtag::ChargedPFTagData::Data data;
                    reco::TransientTrack transientTrack = builder->build(constituent->pseudoTrack());
                    reco::Candidate::Vector jetDir = jet.momentum().Unit();
                    GlobalVector jetRefTrackDir(jet.px(),jet.py(),jet.pz());
                    
                    
                    Measurement1D meas_ip2d=IPTools::signedTransverseImpactParameter(transientTrack, jetRefTrackDir, pv).second;
                    Measurement1D meas_ip3d=IPTools::signedImpactParameter3D(transientTrack, jetRefTrackDir, pv).second;
                    Measurement1D jetdist=IPTools::jetTrackDistance(transientTrack, jetRefTrackDir, pv).second;
                    reco::Candidate::Vector trackMom = constituent->pseudoTrack().momentum();
                    double trackMag = std::sqrt(trackMom.Mag2());
                    TVector3 trackMom3(trackMom.x(),trackMom.y(),trackMom.z());
                    TVector3 jetDir3(jetDir.x(),jetDir.y(),jetDir.z());

                    data.trackEtaRel=reco::btau::etaRel(jetDir, trackMom);
                    data.trackPtRel=trackMom3.Perp(jetDir3);
                    data.trackPPar=jetDir.Dot(trackMom);
                    data.trackDeltaR=reco::deltaR(trackMom, jetDir);
                    data.trackPtRatio=data.trackPtRel / trackMag;
                    data.trackPParRatio=data.trackPPar / trackMag;
                    
                    data.trackSip2dVal=(meas_ip2d.value());
                    data.trackSip2dSig=(meas_ip2d.significance());
                    data.trackSip3dVal=(meas_ip3d.value());
                    data.trackSip3dSig=meas_ip3d.significance();
                    
                    data.trackJetDistVal= jetdist.value();
                    data.trackJetDistSig= jetdist.significance();
        
                    cpfData.emplace_back(data);
                    
                }
                output->at(0).jetData.push_back(cpfData);
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(xtag::XTagPluginFactory, xtag::ChargedPFTagDataPlugin, "ChargedPFTagData");

