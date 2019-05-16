#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ProducerBase.h"


#include "DataFormats/PatCandidates/interface/Jet.h"

#include "NANOX/NANOXProducer/interface/NANOXPlugin.h"
#include "NANOX/NANOXProducer/interface/NANOXPluginFactory.h"
#include "NANOX/Jet/interface/GlobalJetTagData.h"

#include <iostream>

namespace nanox
{

class GlobalJetTagDataPlugin:
    public NANOXPlugin
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
            NANOXPlugin(name,pset,collector,prod),
            inputTag_(pset.getParameter<edm::InputTag>("jets")),
            token_(collector.consumes<edm::View<pat::Jet>>(inputTag_))
        {
            prod.produces<std::vector<nanox::GlobalJetTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup&) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(token_, jetCollection);
            

            std::unique_ptr<std::vector<nanox::GlobalJetTagData>> output(
                new std::vector<nanox::GlobalJetTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                output->at(0).jetData.emplace_back(std::log10(jet.pt()), jet.eta(), jet.phi());
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(nanox::NANOXPluginFactory, nanox::GlobalJetTagDataPlugin, "GlobalJetTagData");

