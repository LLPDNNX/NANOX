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

#include "NANOX/NANOXProducer/interface/NANOXPlugin.h"
#include "NANOX/NANOXProducer/interface/NANOXPluginFactory.h"
#include "NANOX/Jet/interface/LegacyTagData.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>

#include "TVector3.h"

double median(std::vector<float> medi) 
{
    sort(medi.begin(), medi.end());     // sort
            
    double tmedian;
    if (medi.size() % 2 == 0)           // even
        tmedian = (medi[medi.size() / 2 - 1] + medi[medi.size() / 2]) / 2;
    else                                // odd
        tmedian = medi[medi.size() / 2];
    
    return tmedian;
}

namespace nanox
{

class LegacyTagDataPlugin:
    public NANOXPlugin
{
    private:
        edm::EDGetTokenT<edm::View<pat::Jet>> jetToken_;
        edm::EDGetTokenT<edm::View<reco::Vertex>> pvToken_;
        edm::EDGetTokenT<edm::View<reco::VertexCompositePtrCandidate>> svToken_;
        
    public:
        LegacyTagDataPlugin(
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
            prod.produces<std::vector<nanox::LegacyTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup& setup) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(jetToken_, jetCollection);

            edm::Handle<edm::View<reco::Vertex>> pvCollection;
            event.getByToken(pvToken_, pvCollection);

            const reco::Vertex& pv = pvCollection->at(0);
            
            edm::Handle<edm::View<reco::VertexCompositePtrCandidate>> svCollection;
            event.getByToken(svToken_, svCollection);
            
            std::unique_ptr<std::vector<nanox::LegacyTagData>> output(
                new std::vector<nanox::LegacyTagData>(1)
            );

            std::vector<nanox::LegacyTagData::Data> legacyTagData;
            
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                const float jet_pt_uncorr = jet.correctedJet("Uncorrected").pt();

                std::vector<nanox::LegacyTagData::Data> legacyTagData;
                float pvtracks_ = 0;
                float alltracks_ = 0;
                std::vector<float> dxy_;
                std::vector<float> trackSip2dSig_;

                for (unsigned int idaughter = 0; idaughter < jet.numberOfDaughters(); ++idaughter)
                {
                    const pat::PackedCandidate* constituent = dynamic_cast<const pat::PackedCandidate*>(jet.daughter(idaughter));
                    if ((not constituent) or constituent->charge()==0)
                    {
                        continue;
                    }

                    if (constituent->hasTrackDetails())
                    {
                        trackSip2dSig_.emplace_back(constituent->dxyError());

                    }

                    alltracks_ += constituent->pt();

                    if (constituent->fromPV() > 1){
                        pvtracks_ += constituent->pt();
                    }



                    dxy_.emplace_back(constituent->dxy());

                    nanox::LegacyTagData::Data data;
                    
                    legacyTagData.emplace_back(data);
                } 

                float alpha = 1.;
                if (alltracks_ > 0.) {
                    alpha = pvtracks_/alltracks_;
                }

                float median_dxy = median(dxy_);
                std::cout << median_dxy << std::endl;

                
                std::cout << alpha << std::endl;
                //output->at(0).jetData.push_back(legacyTagData);
            }
            

            
            //event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(nanox::NANOXPluginFactory, nanox::LegacyTagDataPlugin, "LegacyTagData");
