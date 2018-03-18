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

#include "XTag/DataFormats/interface/DisplacedGenVertex.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/angle.h"

using xtag::DisplacedGenVertex;
using xtag::DisplacedGenVertexCollection;

class DisplacedGenVertexProducer:
    public edm::stream::EDProducer<>
    
{
    private:    
        static double distance(const reco::Candidate::Point& p1, const reco::Candidate::Point& p2)
        {
            return std::sqrt((p1-p2).mag2());
        }
        
        static int getHadronFlavor(const reco::Candidate& genParticle)
        {
            int absPdgId = std::abs(genParticle.pdgId());
            if (absPdgId<100)
            {
                return 0; //not a hadron
            }
            int nq3 = (absPdgId/     10)%10; //quark content
            int nq2 = (absPdgId/    100)%10; //quark content
            int nq1 = (absPdgId/   1000)%10; //quark content
            int nL  = (absPdgId/  10000)%10;
            int n   = (absPdgId/1000000)%10;
            return std::max({nq1,nq2,nq3})+n*10000+(n>0 and nL==9)*100;
        }
        
        static bool ignoreDisplacement(const reco::Candidate& genParticle)
        {
            
            int absPdgId = std::abs(genParticle.pdgId());
            if (absPdgId==111)
            {
                return true;
            }
            
            return false;
        }
        
        static reco::Candidate::Point correctedDisplacement(const reco::Candidate& genParticle)
        {
            //return mother vertex if displacement is ignored
            if (genParticle.mother() and ignoreDisplacement(*genParticle.mother()) and  (distance(genParticle.mother()->vertex(),genParticle.vertex())>1e-10))
            {
                return correctedDisplacement(*genParticle.mother()); //call recursively
            }
            return genParticle.vertex();
        }
        
        static bool displacedDecay(const reco::Candidate& genParticle)
        {
            for (unsigned int idaughter = 0; idaughter<genParticle.numberOfDaughters(); ++idaughter)
            {
                if (distance(genParticle.daughter(idaughter)->vertex(),genParticle.vertex())>1e-10)
                {
                    return true;
                }
            }
            return false;
        }
        
    
        edm::InputTag _genParticleInputTag;
        edm::EDGetTokenT<edm::View<reco::GenParticle>> _genParticleToken;
        
        edm::InputTag _genJetInputTag;
        edm::EDGetTokenT<edm::View<reco::GenJet>> _genJetToken;
        
        
        virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;
            
            
        template<typename HANDLE, typename TYPE>
        void writeValueMap(edm::Event &out, const HANDLE& handle, const std::vector<TYPE> values, const std::string &name) const 
        {
             typedef edm::ValueMap<TYPE> Map;
             std::unique_ptr<Map> map(new Map());
             typename Map::Filler filler(*map);
             filler.insert(handle, values.begin(), values.end());
             filler.fill();
             out.put(std::move(map), name);
        }
 

    public:
        explicit DisplacedGenVertexProducer(const edm::ParameterSet&);
        ~DisplacedGenVertexProducer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};


//
// constructors and destructor

//
DisplacedGenVertexProducer::DisplacedGenVertexProducer(const edm::ParameterSet& iConfig):
    _genParticleInputTag(iConfig.getParameter<edm::InputTag>("srcGenParticles")),
    _genParticleToken(consumes<edm::View<reco::GenParticle>>(_genParticleInputTag)),
    _genJetInputTag(iConfig.getParameter<edm::InputTag>("srcGenJets")),
    _genJetToken(consumes<edm::View<reco::GenJet>>(_genJetInputTag))
{
    produces<std::vector<DisplacedGenVertex>>();
}


DisplacedGenVertexProducer::~DisplacedGenVertexProducer()
{
}


// ------------ method called to produce the data  ------------
void
DisplacedGenVertexProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::Handle<edm::View<reco::GenParticle>> genParticleCollection;
    iEvent.getByToken(_genParticleToken, genParticleCollection);
    
    edm::Handle<edm::View<reco::GenJet>> genJetCollection;
    iEvent.getByToken(_genJetToken, genJetCollection);
    
    std::unordered_map<size_t,unsigned int> genParticleToVertexGroupMap;
    std::shared_ptr<reco::Candidate::Point> hardInteractionVertex(nullptr);
    
    std::unique_ptr<DisplacedGenVertexCollection> displacedGenVertices(new DisplacedGenVertexCollection());
    edm::RefProd<DisplacedGenVertexCollection> displacedGenVertices_refProd = iEvent.getRefBeforePut<DisplacedGenVertexCollection>();
    
    for (unsigned int igenParticle = 0; igenParticle < genParticleCollection->size(); ++igenParticle)
    {
        const reco::GenParticle& genParticle = genParticleCollection->at(igenParticle);
        if (genParticle.isHardProcess() and genParticle.numberOfMothers()==2)
        {
            if (!hardInteractionVertex)
            {
                hardInteractionVertex.reset(new reco::Candidate::Point(genParticle.vertex()));
            }
            else if (distance(*hardInteractionVertex,correctedDisplacement(genParticle))>1e-10)
            {
                std::cout<<"pdg="<<genParticle.pdgId()<<"; ";
                throw cms::Exception("DisplacedGenVertexProducer: multiple hard interaction vertices found!");
            }
        }
        
        //group particles by vertex position
        bool inserted = false;
        for (unsigned int ivertex = 0; ivertex<displacedGenVertices->size(); ++ivertex)
        {
            DisplacedGenVertex& displacedGenVertex = displacedGenVertices->at(ivertex);
            if (distance(displacedGenVertex.vertex,correctedDisplacement(genParticle))<1e-10)
            {
                displacedGenVertex.genParticles.push_back(genParticleCollection->ptrAt(igenParticle));
                genParticleToVertexGroupMap[(size_t)&genParticle]=ivertex;
                inserted=true;
                break;
            }
        }
        if (not inserted)
        {
            DisplacedGenVertex displacedGenVertex;
            displacedGenVertex.vertex = genParticle.vertex();
            displacedGenVertex.genParticles.push_back(genParticleCollection->ptrAt(igenParticle));
            displacedGenVertices->push_back(displacedGenVertex);
            genParticleToVertexGroupMap[(size_t)&genParticle]=displacedGenVertices->size()-1;
        }
    }
    
    if (not hardInteractionVertex)
    {
        throw cms::Exception("DisplacedVertexProducer: no hard interaction vertex found in event!");
    }
    
    for (unsigned int ivertex = 0; ivertex<displacedGenVertices->size(); ++ivertex)
    {
        displacedGenVertices->at(ivertex).hardInteraction = *hardInteractionVertex;
        if (hardInteractionVertex and distance(displacedGenVertices->at(ivertex).vertex,*hardInteractionVertex)<1e-10)
        {
            displacedGenVertices->at(ivertex).isHardInteraction=true;
        }
    }
    
    //use longlived particles to link vertex groups
    for (unsigned int igenParticle = 0; igenParticle < genParticleCollection->size(); ++igenParticle)
    {
        const reco::GenParticle& genParticle = genParticleCollection->at(igenParticle);
        //if (not ((isGluino(genParticle) or isGluinoHadron(genParticle) or isHadron(genParticle,5) or isHadron(genParticle,4)) and genParticle.isLastCopy() and displacedDecay(genParticle)))
        if (genParticle.mass()<=0)
        {
            continue;
        }
        if (ignoreDisplacement(genParticle) or (not displacedDecay(genParticle)))
        {
            continue;
        }

        unsigned int originVertexGroupIndex = genParticleToVertexGroupMap.at((size_t)&genParticle);
        //book keep summed p4 per vertex group
        std::unordered_map<unsigned int,reco::Candidate::LorentzVector> momentumDistribution;

        for (unsigned int idaughter = 0; idaughter<genParticle.numberOfDaughters(); ++idaughter)
        {
            const reco::Candidate* daughter = genParticle.daughter(idaughter);
            unsigned int daughterVertexGroupIndex = genParticleToVertexGroupMap.at((size_t)daughter);
            if (originVertexGroupIndex!=daughterVertexGroupIndex)
            {
                momentumDistribution[daughterVertexGroupIndex]+=daughter->p4();
            }
        }
        if (genParticle.numberOfDaughters()==0) throw cms::Exception("DisplacedGenVertexProducer: Particle has no daughters in vertex groups");
        //find vertex which shares most of the invariant mass with the long lived particle
        double maxMassRatio = -1;
        int decayVertexIndex = -1;
        for (auto idMomentumPair: momentumDistribution)
        {
            double massRatio = idMomentumPair.second.mass()/genParticle.mass()+1e-8;
            if (massRatio>maxMassRatio)
            {
                maxMassRatio=massRatio;
                decayVertexIndex = idMomentumPair.first;
            }
        }
        if (decayVertexIndex<0)
        {
            edm::LogError("DisplacedGenVertexProducer")<<"A long lived particle should always connect two vertices";
        }
        else
        {
            //make link
            displacedGenVertices->at(originVertexGroupIndex).sharedMassFraction = maxMassRatio;
            displacedGenVertices->at(originVertexGroupIndex).daughterVertices.push_back(edm::Ref<DisplacedGenVertexCollection>(displacedGenVertices_refProd,decayVertexIndex));
            edm::Ref<DisplacedGenVertexCollection> motherRef(displacedGenVertices_refProd,originVertexGroupIndex);
            displacedGenVertices->at(decayVertexIndex).motherVertex = std::move(edm::Ptr<DisplacedGenVertex>(motherRef.id(),motherRef.key(),motherRef.productGetter()));
            //store long lived particle in daughter vertex
            displacedGenVertices->at(decayVertexIndex).motherLongLivedParticle = std::move(edm::Ptr<reco::GenParticle>(genParticleCollection,igenParticle));
        }
    }
    
    
    //calculate overlap of gen particles in gen jets with displaced vertices and associate them
    //IMPORTANT: the following will only work if the GenJets were constructed from the SAME GenParticle collection as the DisplacedGenVertices!!!
    //Note: cannot use ghost tagging since jets may not be pointing along long lived particle direction
    //std::cout<<iEvent.id ()<<std::endl;
    for (unsigned int ijet = 0; ijet < genJetCollection->size(); ++ijet)
    {
        
        if (genJetCollection->at(ijet).pt()<20)
        {
            continue;
        }
        
        std::unordered_map<unsigned int,unsigned int> particlesPermatchedVerticesIndex;
        std::unordered_map<unsigned int,reco::Candidate::LorentzVector> p4PermatchedVerticesIndex;
        //std::vector<std::vector<const reco::GenParticle*>> matchedParticles;
        for (const reco::GenParticle* genParticle: genJetCollection->at(ijet).getGenConstituents())
        {
            
            auto foundGenParticleIt = genParticleToVertexGroupMap.find((size_t)genParticle); 
            if (foundGenParticleIt!=genParticleToVertexGroupMap.end())
            {
                auto vertexIndexIt = particlesPermatchedVerticesIndex.find(foundGenParticleIt->second);
                
                if (vertexIndexIt==particlesPermatchedVerticesIndex.end())
                {
                    particlesPermatchedVerticesIndex[foundGenParticleIt->second]=1;
                    p4PermatchedVerticesIndex[foundGenParticleIt->second] = genParticle->p4();
                    //matchedParticles.push_back(std::vector<const reco::GenParticle*>({genParticle}));
                    //break; //gen particles ordered by pt -> so keep only the hardest one
                    //displacedGenVertices->at(foundGenParticleIt->second).genJets.push_back(genJetCollection->ptrAt(ijet)); //no cross cleaning -> multiple jets can originate from one vertex
                }
                else
                {
                    particlesPermatchedVerticesIndex[foundGenParticleIt->second]+=1;
                    p4PermatchedVerticesIndex[foundGenParticleIt->second]+=genParticle->p4();
                    //matchedParticles[vertexIndexIt-matchedVerticesIndices.begin()].push_back(genParticle);
                }
            }
        }
        
        //std::cout<<"gen jet pt="<<genJetCollection->at(ijet).pt()<<", eta="<<genJetCollection->at(ijet).eta()<<", phi="<<genJetCollection->at(ijet).phi()<<std::endl;
        if (particlesPermatchedVerticesIndex.size()==0)
        {
            continue;
        }
        else
        {
            float maxShared = 0;
            int maxIndex = -1;
            
            //int maxFlavour = 0;
            //float maxPt = 0;
            for (const auto& indexPair: particlesPermatchedVerticesIndex)
            {
                reco::Candidate::Vector vertexVec =  p4PermatchedVerticesIndex[indexPair.first].Vect();
                reco::Candidate::Vector jetVec =  genJetCollection->at(ijet).p4().Vect();
                //calculate projection
                float shared = vertexVec.Dot(jetVec)/jetVec.mag2();
                
                //float pt = p4PermatchedVerticesIndex[indexPair.first].pt();
   
                
                //if (pt>maxPt)
                if (shared>maxShared)
                //if (flavour>maxFlavour)
                {
                    //maxFlavour = flavour; 
                    //maxPt= pt;
                    maxShared = shared;
                    maxIndex = indexPair.first;
                }
            }
            //std::cout<<"sum: "<<sumF<<std::endl;
            if (maxIndex>=0)
            {
 
                displacedGenVertices->at(maxIndex).genJets.push_back(genJetCollection->ptrAt(ijet));
                displacedGenVertices->at(maxIndex).jetFractions.push_back(maxShared);
            }
        }
    }
    /*
    for (const auto& vertex: *displacedGenVertices)
    {
        std::cout<<"pos="<<vertex.vertex<<", particle="<<vertex.genParticles.size()<<", njets="<<vertex.genJets.size()<<", llp=";
        if (vertex.motherLongLivedParticle.isNonnull())
        {
            std::cout<<vertex.motherLongLivedParticle->pdgId()<<std::endl;
        }
        else
        {
            std::cout<<"-"<<std::endl;
        }
    } 
    */
    iEvent.put(std::move(displacedGenVertices));
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DisplacedGenVertexProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(DisplacedGenVertexProducer);
