// system include files
#include <memory>
#include <vector>
#include <unordered_map>
// user include files

#include <fstream> 

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/JetReco/interface/GenJet.h"


#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"


class EventViewer:
    public edm::one::EDAnalyzer<>
    
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
        edm::InputTag _genJetInputTag;
        edm::EDGetTokenT<reco::GenParticleCollection> _genParticleToken;
        edm::EDGetTokenT<std::vector<reco::GenJet>> _genJetToken;
        StringCutObjectSelector<reco::GenParticle> _particleSelector;
        
        virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
        
        std::ofstream ofs;
            
        unsigned int nevents;
        
    public:
        explicit EventViewer(const edm::ParameterSet&);
        ~EventViewer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};


//
// constructors and destructor

//
EventViewer::EventViewer(const edm::ParameterSet& iConfig):
    _genParticleInputTag(iConfig.getParameter<edm::InputTag>("genParticles")),
    _genJetInputTag(iConfig.getParameter<edm::InputTag>("genJets")),
    _genParticleToken(consumes<reco::GenParticleCollection>(_genParticleInputTag)),
    _genJetToken(consumes<std::vector<reco::GenJet>>(_genJetInputTag)),
    _particleSelector(iConfig.getParameter<std::string>("selection")),
    ofs("test.txt", std::ofstream::out),
    nevents(0)
{
    ofs<<"{\"Events\":["<<std::endl;
}


EventViewer::~EventViewer()
{
    ofs<<"]}"<<std::endl;
    ofs.close();
}

struct DisplacedVertex
{
    std::vector<const reco::GenParticle*> particles;
    reco::Candidate::Point position;
    bool isHardInteraction;
    
    DisplacedVertex():
        position(0,0,0),
        isHardInteraction(false)
    {
    }
};


// ------------ method called to produce the data  ------------
void
EventViewer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::Handle<reco::GenParticleCollection> genParticleCollection;
    iEvent.getByToken(_genParticleToken, genParticleCollection);
    
    edm::Handle<std::vector<reco::GenJet>> genJetCollection;
    iEvent.getByToken(_genJetToken, genJetCollection);
    
    if (nevents>0)
    {
        ofs<<","<<std::endl;
    }
    ++nevents;
    
    ofs<<"{"<<std::endl;
   
    std::unique_ptr<reco::Candidate::Point> hardInteraction(nullptr);
    
    std::vector<DisplacedVertex> vertices;
    std::unordered_map<size_t,size_t> particleIndices;
    std::unordered_map<size_t,std::string> particleName;
    std::unordered_map<size_t,std::vector<size_t>> daughterRelations;
        
    ofs<<"  \"Particles\":["<<std::endl;
    for (unsigned int igenParticle = 0; igenParticle < genParticleCollection->size(); ++igenParticle)
    {
        const reco::GenParticle& genParticle = genParticleCollection->at(igenParticle);
        particleIndices[(size_t)&genParticle]=igenParticle;
        if (igenParticle>0)
        {
            ofs<<","<<std::endl;
        }
        ofs<<"     {";
        ofs<<"\"index\":"<<igenParticle;
        ofs<<", \"pdg\":"<<genParticle.pdgId();
        ofs<<", \"p\":"<<genParticle.p();
        ofs<<", \"eta\":"<<genParticle.eta();
        ofs<<", \"phi\":"<<genParticle.phi();
        ofs<<", \"status\":"<<genParticle.status();
        ofs<<"}";
        DisplacedVertex* foundVertex = nullptr;
        for (auto& vertex: vertices)
        {
            if (distance(vertex.position,genParticle.vertex())<10e-10)
            {
                vertex.particles.push_back(&genParticle);
                foundVertex = &vertex;
                break;
            }
        }
        
        if (not foundVertex)
        {
            DisplacedVertex vertex;
            vertex.particles.push_back(&genParticle);
            vertex.position = genParticle.vertex();
            vertices.push_back(vertex);
            foundVertex = &vertices.back();
        }
        
        
        if (genParticle.isHardProcess() and genParticle.numberOfMothers()==2)
        {
            if (!hardInteraction)
            {
                hardInteraction.reset(new reco::Candidate::Point(genParticle.vertex()));
                foundVertex->isHardInteraction = true;
            }
        }
    }
    ofs<<std::endl;
    ofs<<"  ],"<<std::endl;
    
    ofs<<"  \"Relations\":["<<std::endl;
    for (unsigned int igenParticle = 0; igenParticle < genParticleCollection->size(); ++igenParticle)
    {
        const reco::GenParticle& genParticle = genParticleCollection->at(igenParticle);
        size_t motherIndex = particleIndices[(size_t)&genParticle];
        if (igenParticle>0)
        {
            ofs<<","<<std::endl;
        }
        ofs<<"     {\"index\":"<<motherIndex<<", \"daughters\": [";
        for (unsigned int idaughter=0; idaughter<genParticle.numberOfDaughters();++idaughter)
        {
            if (idaughter>0)
            {
                ofs<<",";
            }
            size_t daughterIndex = particleIndices[(size_t)genParticle.daughter(idaughter)];
            daughterRelations[motherIndex].push_back(daughterIndex);
            ofs<<daughterIndex;
        }
        ofs<<"]}";
    }
    ofs<<std::endl;
    ofs<<"  ],"<<std::endl;
    
    
    ofs<<"  \"GenJets\":["<<std::endl;
    
    std::vector<const reco::GenJet*> sortedJets;
    for (unsigned int ijet = 0; ijet < genJetCollection->size(); ++ijet)
    {
        sortedJets.push_back(&genJetCollection->at(ijet));
    }
    std::sort(sortedJets.begin(),sortedJets.end(),
        [](const reco::GenJet* j1, const reco::GenJet* j2){ return j1->p()>j2->p();}
    );
    
    for (unsigned int ijet = 0; ijet < sortedJets.size(); ++ijet)
    {
        const reco::GenJet& jet = *sortedJets.at(ijet);
        if (jet.p()<40) continue;
        if (ijet>0)
        {
            ofs<<","<<std::endl;
        }
        ofs<<"     {\"index\":"<<ijet<<", \"p\":"<<jet.p()<<", \"eta\":"<<jet.eta()<<", \"phi\":"<<jet.phi()<<", \"distance\":"<<jet.maxDistance()<<"}";
    }
    
    ofs<<std::endl;
    ofs<<"  ]"<<std::endl;
    
    ofs<<"}";
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EventViewer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(EventViewer);


