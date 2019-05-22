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
    if (medi.size() == 0) return 0;
    
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
        edm::InputTag inputTag_;
        edm::EDGetTokenT<edm::View<pat::Jet>> jetToken_;
        
    public:
        LegacyTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            NANOXPlugin(name,pset,collector,prod),
            inputTag_(pset.getParameter<edm::InputTag>("jets")),
            jetToken_(collector.consumes<edm::View<pat::Jet>>(inputTag_))
        {
            prod.produces<std::vector<nanox::LegacyTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup& setup) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(jetToken_, jetCollection);

            std::unique_ptr<std::vector<nanox::LegacyTagData>> output(
                new std::vector<nanox::LegacyTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
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
                    
                } 

                float alpha = 1.;
                if (alltracks_ > 0.) {
                    alpha = pvtracks_/alltracks_;
                }

                output->at(0).legacyTagData.emplace_back(median(dxy_), median(trackSip2dSig_), alpha);
            }

            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(nanox::NANOXPluginFactory, nanox::LegacyTagDataPlugin, "LegacyTagData");
