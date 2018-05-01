// system include files
#include <memory>
#include <vector>
#include <unordered_map>
// user include files

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/one/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "NANOX/NANOXProducer/interface/NANOXPlugin.h"
#include "NANOX/NANOXProducer/interface/NANOXPluginFactory.h"

#include <vector>
#include <iostream>

class NANOXProducer:
    public edm::one::EDProducer<>
    
{
    private:
    
        std::vector<std::shared_ptr<nanox::NANOXPlugin>> plugins_;
        
        virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;
    public:
    
        explicit NANOXProducer(const edm::ParameterSet&);
        ~NANOXProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};


//
//constructors and destructor
NANOXProducer::NANOXProducer(const edm::ParameterSet& iConfig)
{
    edm::ConsumesCollector consumeCollector = consumesCollector();
    const edm::ParameterSet& tagProducerConfigs = iConfig.getParameter<edm::ParameterSet>("plugins");
    std::vector<std::string> names = tagProducerConfigs.getParameterNames();
    for (auto name: names)
    {
        edm::ParameterSet tagDataConfig = tagProducerConfigs.getParameter<edm::ParameterSet>(name);
        std::string pluginName = tagDataConfig.getParameter<std::string>("type");
        nanox::NANOXPlugin* plugin = nanox::NANOXPluginFactory::get()->tryToCreate(
            pluginName,
            name,
            tagDataConfig,
            consumeCollector,
            *this
        );
        if (not plugin)
        {
            throw cms::Exception("Cannot find plugin with name '"+pluginName+"'");
        }
        plugins_.emplace_back(plugin);
    }
}


NANOXProducer::~NANOXProducer()
{
}


// ------------ method called to produce the data  ------------
void
NANOXProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    for (auto plugin: plugins_)
    {
        plugin->produce(iEvent,iSetup);
    }
    /*
    edm::Handle<edm::View<xtag::DisplacedGenVertex>> displacedGenVertexCollection;
    iEvent.getByToken(_displacedGenVertexToken, displacedGenVertexCollection);
    
    edm::Handle<edm::View<pat::Jet>> jetCollection;
    iEvent.getByToken(_jetToken, jetCollection);
    
    std::vector<uint8_t> ncpf(jetCollection->size(),0);
    std::vector<float> cpf_ptrel;
    for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
    {
        const pat::Jet& jet = jetCollection->at(ijet);
        unsigned int ncpf_sum = 0;
        for (unsigned int iconstituent = 0; iconstituent < jet.numberOfDaughters(); ++iconstituent)
        {
            const pat::PackedCandidate* constituent = dynamic_cast<const pat::PackedCandidate*>(jet.daughter(iconstituent));
            if (constituent and constituent->charge ()!=0)
            {
                cpf_ptrel.push_back(constituent->pt()/jet.pt());
                ++ncpf_sum;
            }
        }
        ncpf[ijet] = ncpf_sum;
    }
    
    
    auto tab  = std::make_unique<nanoaod::FlatTable>(displacedGenVertexCollection->size(), "displaced", false, false);
    std::vector<float> decayLength(displacedGenVertexCollection->size(),0);
    
    auto cpf_size_tab  = std::make_unique<nanoaod::FlatTable>(ncpf.size(), "jets", false, false);
    cpf_size_tab->addColumn<float>("cpf_size", ncpf, "POG highPt muon ID (1 = tracker high pT, 2 = global high pT, which includes tracker high pT)", nanoaod::FlatTable::FloatColumn);
    iEvent.put(std::move(cpf_size_tab),"cpfsize");
    
    auto cpf_ptrel_tab  = std::make_unique<nanoaod::FlatTable>(cpf_ptrel.size(), "cpf_ptrel", false, false);
    cpf_ptrel_tab->addColumn<float>("cpf_ptrel", cpf_ptrel, "POG highPt muon ID (1 = tracker high pT, 2 = global high pT, which includes tracker high pT)", nanoaod::FlatTable::FloatColumn);
    iEvent.put(std::move(cpf_ptrel_tab),"cpfptrel");
    */
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
NANOXProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(NANOXProducer);

