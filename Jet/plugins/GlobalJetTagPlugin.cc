#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ProducerBase.h"


#include "DataFormats/PatCandidates/interface/Jet.h"

#include "XTag/XTagProducer/interface/XTagPlugin.h"
#include "XTag/XTagProducer/interface/XTagPluginFactory.h"
#include "XTag/Jet/interface/GlobalJetTagData.h"

#include <iostream>

namespace xtag
{

class GlobalJetTagDataPlugin:
    public XTagPlugin
{
    private:
        edm::InputTag inputTag_;
        edm::EDGetTokenT<edm::View<pat::Jet>> token_;
        
    public:
        GlobalJetTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            XTagPlugin(name,pset,collector,prod),
            inputTag_(pset.getParameter<edm::InputTag>("src")),
            token_(collector.consumes<edm::View<pat::Jet>>(inputTag_))
        {
            prod.produces<std::vector<xtag::GlobalJetTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup&) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(token_, jetCollection);
            

            std::unique_ptr<std::vector<xtag::GlobalJetTagData>> output(
                new std::vector<xtag::GlobalJetTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                output->at(0).jetData.emplace_back(jet.pt(),jet.eta());
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(xtag::XTagPluginFactory, xtag::GlobalJetTagDataPlugin, "GlobalJetTagData");

