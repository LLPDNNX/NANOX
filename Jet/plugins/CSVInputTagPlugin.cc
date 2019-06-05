#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ProducerBase.h"

#include "DataFormats/BTauReco/interface/ShallowTagInfo.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "NANOX/NANOXProducer/interface/NANOXPlugin.h"
#include "NANOX/NANOXProducer/interface/NANOXPluginFactory.h"
#include "NANOX/Jet/interface/CSVInputTagData.h"

#include <iostream>

namespace nanox
{

class CSVInputTagDataPlugin:
    public NANOXPlugin
{
    private:
        edm::InputTag inputTag_;
        edm::EDGetTokenT<edm::View<pat::Jet>> token_;
        std::string tagName_;
    public:
        CSVInputTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            NANOXPlugin(name,pset,collector,prod),
            inputTag_(pset.getParameter<edm::InputTag>("jets")),
            token_(collector.consumes<edm::View<pat::Jet>>(inputTag_)),
            tagName_(pset.getParameter<std::string>("tagName"))
        {
            prod.produces<std::vector<nanox::CSVInputTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup&) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(token_, jetCollection);
            

            std::unique_ptr<std::vector<nanox::CSVInputTagData>> output(
                new std::vector<nanox::CSVInputTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                const reco::ShallowTagInfo* tagInfo = dynamic_cast<const reco::ShallowTagInfo*>(
                    jet.tagInfo(tagName_)
                );
                if (not tagInfo)
                {
                    std::string availableTagNames = "[";
                    for (auto name: jet.tagInfoLabels())
                    {
                        availableTagNames+=name+",";
                    }
                    availableTagNames+="]";
                    throw cms::Exception("Not shallow tag information '"+tagName_+"' found in jet. Only these tag labels are available: "+availableTagNames);
                }
                nanox::CSVInputTagData::Data jetTagData;
                reco::TaggingVariableList vars = tagInfo->taggingVariables();
                
                //http://cmslxr.fnal.gov/source/DataFormats/BTauReco/interface/TaggingVariable.h?v=CMSSW_9_4_0_pre1#0033
                float trackSumJetEtRatio = vars.get(reco::btau::trackSumJetEtRatio, -1);
                jetTagData.trackSumJetEtRatio = trackSumJetEtRatio;
                
                float trackSumJetDeltaR = vars.get(reco::btau::trackSumJetDeltaR, -1);
                jetTagData.trackSumJetDeltaR = trackSumJetDeltaR;
                 
                jetTagData.vertexCategory = vars.get(reco::btau::vertexCategory, -1);
                jetTagData.jetNSelectedTracks = vars.get(reco::btau::jetNSelectedTracks, -1);
                jetTagData.jetNTracksEtaRel = vars.get(reco::btau::jetNTracksEtaRel, -1);
                
                float trackSip2dValAboveCharm = vars.get(reco::btau::trackSip2dValAboveCharm, -1);
                float trackSip2dSigAboveCharm = vars.get(reco::btau::trackSip2dSigAboveCharm, -1);
                //the significances take often the values -1 
                //this maybe a bug and happen if trackSip2dValAboveCharm = -1 thus the reset value is ignored
                //cmslxr.fnal.gov/source/RecoBTag/SecondaryVertex/plugins/TemplatedSecondaryVertexProducer.cc?v=CMSSW_9_4_0
                jetTagData.trackSip2dValAboveCharm = trackSip2dValAboveCharm;
                jetTagData.trackSip2dSigAboveCharm = trackSip2dSigAboveCharm;
                
                float trackSip3dValAboveCharm = vars.get(reco::btau::trackSip2dValAboveCharm, -1);
                float trackSip3dSigAboveCharm = vars.get(reco::btau::trackSip3dSigAboveCharm, -1);

                jetTagData.trackSip3dValAboveCharm = trackSip3dValAboveCharm;
                jetTagData.trackSip3dSigAboveCharm = trackSip3dSigAboveCharm;

                output->at(0).jetData.push_back(jetTagData);
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(nanox::NANOXPluginFactory, nanox::CSVInputTagDataPlugin, "CSVInputTagData");

