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
#include "XTag/Jet/interface/ChargedPFTagData.h"

#include <iostream>

namespace xtag
{

class ChargedPFTagDataPlugin:
    public XTagPlugin
{
    private:
        edm::InputTag inputTag_;
        edm::EDGetTokenT<edm::View<pat::Jet>> token_;
        
    public:
        ChargedPFTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            XTagPlugin(name,pset,collector,prod),
            inputTag_(pset.getParameter<edm::InputTag>("src")),
            token_(collector.consumes<edm::View<pat::Jet>>(inputTag_))
        {
            prod.produces<std::vector<xtag::ChargedPFTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup&) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(token_, jetCollection);
            
            std::unique_ptr<std::vector<xtag::ChargedPFTagData>> output(
                new std::vector<xtag::ChargedPFTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                std::vector<xtag::ChargedPFTagData::Data> data;
                for (unsigned int idaughter = 0; idaughter < jet.numberOfDaughters(); ++idaughter)
                {
                    const reco::Candidate* constituent = jet.daughter(idaughter);
                    if (constituent->charge()==0)
                    {
                        continue;
                    }
                    data.emplace_back(constituent->pt()/jet.pt());
                    
                }
                output->at(0).jetData.push_back(data);
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(xtag::XTagPluginFactory, xtag::ChargedPFTagDataPlugin, "ChargedPFTagData");

