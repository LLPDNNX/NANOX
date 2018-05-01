

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"
#include "NANOX/DataFormats/interface/GhostFlavourInfo.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "PhysicsTools/JetMCUtils/interface/CandMCTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/PseudoJet.hh"

//
// constants, enums and typedefs
//
typedef boost::shared_ptr<fastjet::ClusterSequence>  ClusterSequencePtr;
typedef boost::shared_ptr<fastjet::JetDefinition>    JetDefPtr;

//
// class declaration
//
class GhostInfo : public fastjet::PseudoJet::UserInfoBase
{
  public:
    GhostInfo(const reco::GenParticleRef& ghost):
      ghost_(ghost)
    {
    }
    inline const reco::GenParticleRef& ghost() const
    { 
        return ghost_; 
    }

  protected:
    const reco::GenParticleRef ghost_;
};

class GhostTagJetProducer : public edm::EDProducer {
   public:
      explicit GhostTagJetProducer(const edm::ParameterSet&);
      ~GhostTagJetProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void produce(edm::Event&, const edm::EventSetup&);

      void insertGhosts(const edm::Handle<reco::GenParticleRefVector>& particles,
                        const double ghostRescaling,
                        std::vector<fastjet::PseudoJet>& constituents);

      void matchReclusteredJets(const edm::Handle<edm::View<pat::Jet> >& jets,
                                const std::vector<fastjet::PseudoJet>& matchedJets,
                                std::vector<int>& matchedIndices);


      // ----------member data ---------------------------
      const edm::EDGetTokenT<edm::View<pat::Jet> >      jetsToken_;        // Input jet collection
      const edm::EDGetTokenT<reco::GenParticleRefVector> ghostToken_;    // Input b hadron collection

      const std::string   jetAlgorithm_;
      const double        rParam_;
      const double        jetPtMin_;
      const double        ghostRescaling_;
      const double        relPtTolerance_;

      ClusterSequencePtr  fjClusterSeq_;
      JetDefPtr           fjJetDefinition_;
};

//
// static data member definitions
//
template<typename HANDLE, typename TYPE>
void writeValueMap(edm::Event &out, const HANDLE& handle, const std::vector<TYPE> values, const std::string &name)  
{
     typedef edm::ValueMap<TYPE> Map;
     std::unique_ptr<Map> map(new Map());
     typename Map::Filler filler(*map);
     filler.insert(handle, values.begin(), values.end());
     filler.fill();
     out.put(std::move(map), name);
}


void getAllDaughers(const reco::GenParticle* p, std::vector<const reco::GenParticle*>& daughters)
{
    for (const auto& d: p->daughterRefVector ())
    {
        const reco::GenParticle* dcast = dynamic_cast<const reco::GenParticle*>(d.get());
        for (const auto& m: dcast->motherRefVector())
        {
            if (m.get()!=p)
            {
                //std::cout<<"interconnection: "<<m->pdgId()<<", pt="<<m->pt()<<std::endl;
            }
        }
        if (not d)
        {
            std::cout<<"ERROR while casting genparticle"<<std::endl;
        }
        if (dcast->	numberOfDaughters ()==0)
        {
            daughters.push_back(dcast);
        }
        else
        {
            getAllDaughers(dcast, daughters);
        }
    }
}

//
// constructors and destructor
//
GhostTagJetProducer::GhostTagJetProducer(const edm::ParameterSet& iConfig) :

   jetsToken_(consumes<edm::View<pat::Jet> >( iConfig.getParameter<edm::InputTag>("jets")) ),
   ghostToken_(consumes<reco::GenParticleRefVector>( iConfig.getParameter<edm::InputTag>("ghosts") )),
   jetAlgorithm_(iConfig.getParameter<std::string>("jetAlgorithm")),
   rParam_(iConfig.getParameter<double>("rParam")),
   jetPtMin_(0.), // hardcoded to 0. since we simply want to recluster all input jets which already had some PtMin applied
   ghostRescaling_(iConfig.exists("ghostRescaling") ? iConfig.getParameter<double>("ghostRescaling") : 1e-18),
   relPtTolerance_(iConfig.exists("relPtTolerance") ? iConfig.getParameter<double>("relPtTolerance") : 1e-04) // 0.1% relative difference in Pt should be sufficient to detect possible misconfigurations

{
   // register your products
   produces<edm::ValueMap<nanox::GhostFlavourInfo>>();

   // set jet algorithm
   if (jetAlgorithm_=="Kt")
     fjJetDefinition_= JetDefPtr( new fastjet::JetDefinition(fastjet::kt_algorithm, rParam_) );
   else if (jetAlgorithm_=="CambridgeAachen")
     fjJetDefinition_= JetDefPtr( new fastjet::JetDefinition(fastjet::cambridge_algorithm, rParam_) );
   else if (jetAlgorithm_=="AntiKt")
     fjJetDefinition_= JetDefPtr( new fastjet::JetDefinition(fastjet::antikt_algorithm, rParam_) );
   else
     throw cms::Exception("InvalidJetAlgorithm") << "Jet clustering algorithm is invalid: " << jetAlgorithm_ << ", use CambridgeAachen | Kt | AntiKt" << std::endl;
}


GhostTagJetProducer::~GhostTagJetProducer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
GhostTagJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   edm::Handle<edm::View<pat::Jet> > jets;
   iEvent.getByToken(jetsToken_, jets);
   
   edm::Handle<reco::GenParticleRefVector> ghosts;
   iEvent.getByToken(ghostToken_, ghosts);

    std::vector<nanox::GhostFlavourInfo> ghostFlavourInfo(jets->size());

   
   // vector of constituents for reclustering jets and "ghosts"
   std::vector<fastjet::PseudoJet> fjInputs;
   // loop over all input jets and collect all their constituents
   for(auto it = jets->begin(); it != jets->end(); ++it)
   {
      
     for(unsigned int iconstituent = 0; iconstituent < it->numberOfDaughters(); ++iconstituent)
     {
       const reco::Candidate* constituent = it->daughter(iconstituent);
       if(constituent->pt() == 0)
       {
         edm::LogWarning("NullTransverseMomentum") << "dropping input candidate with pt=0";
         continue;
       }
       fjInputs.push_back(fastjet::PseudoJet(constituent->px(),constituent->py(),constituent->pz(),constituent->energy()));
     }
   }
   // insert "ghost" b hadrons in the vector of constituents
   insertGhosts(ghosts,ghostRescaling_, fjInputs);
   
   
   //std::cout<<"ghosts: "<<ghosts->size()<<std::endl;
   std::vector<const reco::GenParticle*> mothers;
   std::vector<std::vector<const reco::Candidate*>> ghostsPerMother;
   std::vector<const reco::Candidate*> lsp;
   for (unsigned int i = 0; i<ghosts->size(); ++i)
   {
   
    std::vector<const reco::GenParticle*> daughters;
    getAllDaughers(dynamic_cast<const reco::GenParticle*>(ghosts->at(i).get()),daughters);
    reco::Candidate::LorentzVector v(0,0,0,0);
    for (const auto* daughter: daughters)
    {
        v+=daughter->p4();
    }
    
    //std::cout<<"ghost "<<i<<" pt="<<ghosts->at(i)->pt()<<", eta="<<ghosts->at(i)->eta()<<", ndaughters="<<daughters.size()<<", ptsum="<<v.pt()<<std::endl; 
      
   }
   
   
   // define jet clustering sequence
   fjClusterSeq_ = ClusterSequencePtr( new fastjet::ClusterSequence( fjInputs, *fjJetDefinition_ ) );
   // recluster jet constituents and inserted "ghosts"
   std::vector<fastjet::PseudoJet> inclusiveJets = fastjet::sorted_by_pt( fjClusterSeq_->inclusive_jets(ghostRescaling_*0.001) );

   if( inclusiveJets.size() < jets->size() )
     edm::LogError("TooFewReclusteredJets") << "There are fewer reclustered (" << inclusiveJets.size() << ") than original jets (" << jets->size() << "). Please check that the jet algorithm and jet size match those used for the original jet collection.";

   // match reclustered and original jets
   std::vector<int> reclusteredIndices;
   matchReclusteredJets(jets,inclusiveJets,reclusteredIndices);
    
   // determine jet flavour
   for(size_t i=0; i<jets->size(); ++i)
   {
    
      if (reclusteredIndices.at(i)<0)
      {
          continue;
      }
     if( jets->at(i).pt() == 0 )
     {
       edm::LogWarning("NullTransverseMomentum") << "The original jet " << i << " has Pt=0. This is not expected so the jet will be skipped.";
     }
     else
     {
        
       // since the "ghosts" are extremely soft, the configuration and ordering of the reclustered and original jets should in principle stay the same
       if(jets->at(i).pt() > 20. and ( std::abs( inclusiveJets.at(reclusteredIndices.at(i)).pt() - jets->at(i).correctedP4("Uncorrected").pt() ) / jets->at(i).correctedP4("Uncorrected").pt() ) > relPtTolerance_ )
       {
           edm::LogError("JetPtMismatch") << "The reclustered and original jet " << i << " have different Pt's (" << inclusiveJets.at(reclusteredIndices.at(i)).pt() << " vs " << jets->at(i).correctedP4("Uncorrected").pt() << " GeV, respectively).\n"
                                          << "Please check that the jet algorithm and jet size match those used for the original jet collection and also make sure the original jets are uncorrected. In addition, make sure you are not using CaloJets which are presently not supported.\n"
                                          << "\nIn extremely rare instances the mismatch could be caused by a difference in the machine precision in which case make sure the original jet collection is produced and reclustering is performed in the same job.";
       }
       
       // get jet constituents (sorted by Pt)
       std::vector<fastjet::PseudoJet> constituents = fastjet::sorted_by_pt( inclusiveJets.at(reclusteredIndices.at(i)).constituents() );

       // loop over jet constituents and try to find "ghosts"
       for(std::vector<fastjet::PseudoJet>::const_iterator it = constituents.begin(); it != constituents.end(); ++it)
       {
         if( !it->has_user_info() ) continue; // skip if not a "ghost"
            
         ghostFlavourInfo[i].addGhost(it->user_info<GhostInfo>().ghost());
       }
    }
   }
   /*
   //try to find all ghosts back even if not clustered into high pt jet
   for (size_t i=0; i<inclusiveJets.size(); ++i)
   {
     std::vector<fastjet::PseudoJet> constituents = fastjet::sorted_by_pt( inclusiveJets[i].constituents() );

       // loop over jet constituents and try to find "ghosts"
       if (i<jets->size())
       {
        std::cout<<"reco/reclustered jet: pt="<<jets->at(i).pt()<<", uncorpt="<<jets->at(i).correctedP4("Uncorrected").pt()<<"/"<<inclusiveJets[i].pt();
        if (jets->at(i).genJet())
        {
            std::cout<<", genpt="<<jets->at(i).genJet()->pt();
        }
        std::cout<<std::endl;
       }
       for(std::vector<fastjet::PseudoJet>::const_iterator it = constituents.begin(); it != constituents.end(); ++it)
       {
        if( !it->has_user_info() ) continue;
        //std::cout<<"Found ghost in reclustered jet ("<<inclusiveJets[i].pt()<<"): "<<it->user_info<GhostInfo>().ghost()->pt()<<std::endl;
       }
   }
   */
   std::vector<std::vector<const pat::Jet*>> jetsPerMother(mothers.size());
   for (size_t i=0; i<jets->size(); ++i)
   {
      if (ghostFlavourInfo[i].ghosts.size()==1)
      {
        auto it = std::find(mothers.begin(),mothers.end(),ghostFlavourInfo[i].ghosts[0]->mother());
        if (it!=mothers.end())
        {
         jetsPerMother[it-mothers.begin()].push_back(&jets->at(i));   
        }
        else
        {
            //std::cout<<"no mother match!"<<std::endl;
        }
      }
      if (ghostFlavourInfo[i].ghosts.size()>=2)
      {
        //std::cout<<"double match"<<std::endl;
      }
   }
   for (size_t im =0 ; im <jetsPerMother.size(); ++im)
   {
     if (jetsPerMother[im].size()==2 and ghostsPerMother[im].size()==2)
     {
        reco::Candidate::LorentzVector vec(0,0,0,0);
        reco::Candidate::LorentzVector vecGen(0,0,0,0);
        vec+=jetsPerMother[im][0]->p4();
        vec+=jetsPerMother[im][1]->p4();
        
        vecGen+=ghostsPerMother[im][0]->p4();
        vecGen+=ghostsPerMother[im][1]->p4();
        vec+=lsp[im]->p4();
        vecGen+=lsp[im]->p4();
        //std::cout<<"Vertex mass: "<<vec.mass()<<"/"<<vecGen.mass()<<"/"<<mothers[im]->mass()<<std::endl;
     }
   }
   
   //std::cout<<" ------------- "<<std::endl;
   writeValueMap(iEvent,jets,ghostFlavourInfo,"");
}

// ------------ method that inserts "ghost" particles in the vector of jet constituents ------------
void
GhostTagJetProducer::insertGhosts(const edm::Handle<reco::GenParticleRefVector>& particles,
                                   const double ghostRescaling,
                                   std::vector<fastjet::PseudoJet>& constituents)
{
   // insert "ghost" particles in the vector of jet constituents
   for(reco::GenParticleRefVector::const_iterator it = particles->begin(); it != particles->end(); ++it)
   {
     if((*it)->pt() == 0)
     {
       edm::LogInfo("NullTransverseMomentum") << "dropping input ghost candidate with pt=0";
       continue;
     }
     fastjet::PseudoJet p((*it)->px(),(*it)->py(),(*it)->pz(),(*it)->energy());
     p*=ghostRescaling; // rescale particle momentum
     p.set_user_info(new GhostInfo(*it));
     constituents.push_back(p);
   }
}

// ------------ method that matches reclustered and original jets based on minimum dR ------------
void
GhostTagJetProducer::matchReclusteredJets(const edm::Handle<edm::View<pat::Jet> >& jets,
                                           const std::vector<fastjet::PseudoJet>& reclusteredJets,
                                           std::vector<int>& matchedIndices)
{
   std::vector<bool> matchedLocks(reclusteredJets.size(),false);

   for(size_t j=0; j<jets->size(); ++j)
   {
     double matchedDR2 = 1e9;
     int matchedIdx = -1;

     for(size_t rj=0; rj<reclusteredJets.size(); ++rj)
     {
       if( matchedLocks.at(rj) ) continue; // skip jets that have already been matched

       double tempDR2 = reco::deltaR2( jets->at(j).rapidity(), jets->at(j).phi(), reclusteredJets.at(rj).rapidity(), reclusteredJets.at(rj).phi_std() );
       if( tempDR2 < matchedDR2 )
       {
         matchedDR2 = tempDR2;
         matchedIdx = rj;
       }
     }

     if( matchedIdx>=0 )
     {
       if ( matchedDR2 > rParam_*rParam_ )
       {
         edm::LogError("JetMatchingFailed") << "Matched reclustered jet " << matchedIdx << " and original jet " << j <<" are separated by dR=" << sqrt(matchedDR2) << " which is greater than the jet size R=" << rParam_ << ".\n"
                                            << "This is not expected so please check that the jet algorithm and jet size match those used for the original jet collection.";
       }
       else
         matchedLocks.at(matchedIdx) = true;
     }
     else
       edm::LogError("JetMatchingFailed") << "Matching reclustered to original jets failed. Please check that the jet algorithm and jet size match those used for the original jet collection.";

     matchedIndices.push_back(matchedIdx);
   }
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GhostTagJetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GhostTagJetProducer);

