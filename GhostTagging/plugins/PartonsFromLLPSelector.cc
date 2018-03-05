// system include files
#include <memory>
#include <vector>
#include <unordered_map>
// user include files

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"


#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include <functional>

class PartonsFromLLPSelector:
    public edm::stream::EDProducer<>
    
{
    private:    
        static double distance(const reco::Candidate::Point& p1, const reco::Candidate::Point& p2)
        {
            return std::sqrt((p1-p2).mag2());
        }
        
        static int getHadronFlavor(const reco::GenParticle& genParticle)
        {
            int absPdgId = std::abs(genParticle.pdgId());
            int nq3 = (absPdgId/     10)%10; //quark content
            int nq2 = (absPdgId/    100)%10; //quark content
            int nq1 = (absPdgId/   1000)%10; //quark content
            int nL  = (absPdgId/  10000)%10;
            int n   = (absPdgId/1000000)%10;
            return std::max({nq1,nq2,nq3})+n*10000+(n>0 and nL==9)*100;
        }
        
        static bool displacedDecay(const reco::GenParticle& genParticle)
        {
            for (unsigned int idaughter = 0; idaughter<genParticle.numberOfDaughters(); ++idaughter)
            {
                if (distance(genParticle.daughter(idaughter)->vertex(),genParticle.vertex())>10e-10)
                {
                    return true;
                }
            }
            return false;
        }
        
    
        edm::InputTag _genParticleInputTag;
        edm::EDGetTokenT<reco::GenParticleCollection> _genParticleToken;
        
        StringCutObjectSelector<reco::GenParticle> _particleSelector;
        
        virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;
            
            
        
    public:
        explicit PartonsFromLLPSelector(const edm::ParameterSet&);
        ~PartonsFromLLPSelector();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};

bool hasParent(const reco::GenParticle* genParticle, std::function<bool(const reco::GenParticle*)> fct)
{
    for (const auto& mother: genParticle->motherRefVector())
    {
        if (fct(mother.get()))
        {
            return true;
        }
        else if (hasParent(mother.get(),fct))
        {
            return true;
        }
    }
    return false;
}


//
// constructors and destructor

//
PartonsFromLLPSelector::PartonsFromLLPSelector(const edm::ParameterSet& iConfig):
    _genParticleInputTag(iConfig.getParameter<edm::InputTag>("src")),
    _genParticleToken(consumes<reco::GenParticleCollection>(_genParticleInputTag)),
    _particleSelector(iConfig.getParameter<std::string>("selection"))
{
    produces<reco::GenParticleRefVector>();
    //produces<reco::Candidate::Point>("hardInteraction");
}


PartonsFromLLPSelector::~PartonsFromLLPSelector()
{
}


// ------------ method called to produce the data  ------------
void
PartonsFromLLPSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::Handle<reco::GenParticleCollection> genParticleCollection;
    iEvent.getByToken(_genParticleToken, genParticleCollection);
    
   
    std::unique_ptr<reco::Candidate::Point> hardInteraction(nullptr);
        
    for (unsigned int igenParticle = 0; igenParticle < genParticleCollection->size(); ++igenParticle)
    {
        const reco::GenParticle& genParticle = genParticleCollection->at(igenParticle);
        if (genParticle.isHardProcess() and genParticle.numberOfMothers()==2)
        {
            if (!hardInteraction)
            {
                hardInteraction.reset(new reco::Candidate::Point(genParticle.vertex()));
            }
            else if (distance(*hardInteraction,genParticle.vertex())>10e-10)
            {
                throw cms::Exception("PartonsFromLLPSelector: multiple hard interaction vertices found!");
            }
        }
    }
    
    
    std::unique_ptr<reco::GenParticleRefVector> partons(new reco::GenParticleRefVector());
    
    if (hardInteraction)
    {
        for (unsigned int igenParticle = 0; igenParticle < genParticleCollection->size(); ++igenParticle)
        {
            const reco::GenParticle& genParticle = genParticleCollection->at(igenParticle);
            if (distance(*hardInteraction,genParticle.vertex())<10e-10)
            {
                continue;
            }
            try
            {
                /*
                if (!_particleSelector(genParticle))
                {
                    continue;
                }
                */
                if (genParticle.pdgId()==0 or std::abs(genParticle.pdgId())>6)
                {
                    continue;
                }
                bool lastCopy = true;
                for (const auto& daughter: genParticle.daughterRefVector())
                {
                    if (daughter->pdgId()==genParticle.pdgId())
                    {
                        lastCopy = false;
                        break;
                    }
                }
                if (not lastCopy)
                {
                    continue;
                }
                if (hasParent(&genParticle,[](const reco::GenParticle* genParticle) -> bool {
                    return genParticle->pdgId()==1000021;
                }))
                {
                    //std::cout<<"found ghost: "<<genParticle.pdgId()<<std::endl;
                    partons->push_back(reco::GenParticleRef(genParticleCollection,igenParticle));
                }
            }
            catch (const cms::Exception& e)
            {
                edm::LogError("Caught exception in selector") << e.what();
            }
        }
    }
    
    //iEvent.put(std::move(hardInteraction));
    iEvent.put(std::move(partons));
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PartonsFromLLPSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(PartonsFromLLPSelector);


