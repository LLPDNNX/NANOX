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
#include "XTag/Jet/interface/JetOriginTagData.h"

#include "XTag/DataFormats/interface/DisplacedGenVertex.h"

#include "DataFormats/Math/interface/angle.h"

#include "TRandom.h"
#include "TMath.h"

#include <iostream>

namespace xtag
{

class JetOriginTagDataPlugin:
    public XTagPlugin
{
    private:
        edm::EDGetTokenT<edm::View<pat::Jet>> token_;
        
        edm::EDGetTokenT<edm::View<xtag::DisplacedGenVertex>> displacedGenVertexToken_;
        
        
        //edm::EDGetTokenT<edm::View<reco::GenJet>> genJetToken_;
        
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
        
    public:
        JetOriginTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            XTagPlugin(name,pset,collector,prod),
            token_(collector.consumes<edm::View<pat::Jet>>(pset.getParameter<edm::InputTag>("jets")))
            //genJetToken_(collector.consumes<edm::View<reco::GenJet>>(pset.getParameter<edm::InputTag>("genJets")))
        {
            if (pset.exists("displacedGenVertices"))
            {
                displacedGenVertexToken_ = 
                    collector.consumes<edm::View<xtag::DisplacedGenVertex>>(pset.getParameter<edm::InputTag>("displacedGenVertices"));
            }
            prod.produces<std::vector<xtag::JetOriginTagData>>(name);
        }
        
        
        
        
        virtual void produce(edm::Event& event, const edm::EventSetup&) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(token_, jetCollection);

            std::unique_ptr<std::vector<xtag::JetOriginTagData>> output(
                new std::vector<xtag::JetOriginTagData>(1)
            );
            
            edm::Handle<edm::View<xtag::DisplacedGenVertex>> displacedGenVertexCollection;
            if (not displacedGenVertexToken_.isUninitialized())
            {
                event.getByToken(displacedGenVertexToken_, displacedGenVertexCollection);
            }
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                
                xtag::JetOriginTagData::Data data;
                if (not jet.genJet())
                {
                    data.isPU = true;
                }
                else
                {
                    data.partonFlavor = abs(jet.partonFlavour());
                    data.hadronFlavor = abs(jet.hadronFlavour());
                    
                    unsigned int nbHadrons = jet.jetFlavourInfo().getbHadrons().size();
                    unsigned int ncHadrons = jet.jetFlavourInfo().getcHadrons().size();
                    
                    unsigned int nbHadronsToLeptons = 0;
                    unsigned int ncHadronsToLeptons = 0;
                    
                    unsigned int nGluons = 0; 
                    if (jet.genParton() and (data.partonFlavor==5 or data.partonFlavor==4))
                    {
                        const reco::Candidate* mother = jet.genParton();
                        //walk up
                        while (mother->mother() and mother->mother()->pdgId()==mother->pdgId())
                        {
                            mother = mother->mother();
                        }
                        //find gluon anchestor
                        if (mother->mother() and mother->mother()->pdgId()==21)
                        {
                            nGluons+=1;
                        }
                    }
                   
                    
                    for (const auto* constituent: jet.genJet()->getJetConstituentsQuick())
                    {   
                        int absId = std::abs(constituent->pdgId());
                        if (constituent->mother() and (absId==11 or absId==13))
                        {
                            //account for photon/Z FSR walk up the decay tree
                            const reco::Candidate* mother = constituent->mother();
                            while (mother->mother() and mother->pdgId()==mother->mother()->pdgId())
                            {
                                mother = mother->mother();
                            }
                            int hadFlavor = getHadronFlavor(*constituent->mother());
                            if (hadFlavor==5) nbHadronsToLeptons+=1;
                            if (hadFlavor==4) ncHadronsToLeptons+=1;
                        }
                    }
                    
                    if (data.hadronFlavor==5)
                    {
                        if (nbHadronsToLeptons==0 and ncHadronsToLeptons==0)
                        {
                            data.isBB = nbHadrons>1 and nGluons==0;
                            data.isGBB = nbHadrons>1 and nGluons>0;
                            data.isB = nbHadrons==1;
                        }
                        else
                        {
                            data.isLeptonic_B = nbHadronsToLeptons>0;
                            data.isLeptonic_C = nbHadronsToLeptons==0 and ncHadronsToLeptons>0;
                        }
                    }
                    else if (data.hadronFlavor==4)
                    {
                        data.isCC = ncHadrons>1 and nGluons==0;
                        data.isGCC = ncHadrons>1 and nGluons>0;
                        data.isC = ncHadrons==1;
                    }
                    else if (data.partonFlavor!=0)
                    {
                        data.isB = data.partonFlavor==5;
                        data.isC = data.partonFlavor==4;
                        data.isS = data.partonFlavor==3;
                        data.isUD = data.partonFlavor==2 or data.partonFlavor==1;
                        data.isG = data.partonFlavor==21;
                    }  
                    else
                    {
                        /*
                        std::array<float,3> fractionsPerFlavor{{0,0,0}}; //0: photon (22); 1: 1: ud flavors; 2: s flavors;
                        for (const auto* constituent: jet.genJet()->getJetConstituentsQuick())
                        {
                            //categorize by momentum fraction
                            float fraction = constituent->p4().Vect().Dot(jet.genJet()->p4().Vect())/jet.genJet()->p4().Vect().mag2();
                            int hadFlavor = getHadronFlavor(*constituent->mother());
                            if (constituent->pdgId()==22) fractionsPerFlavor[0] += fraction;
                            else if (hadFlavor==2) fractionsPerFlavor[1] += fraction;
                            else if (hadFlavor==3) fractionsPerFlavor[2] += fraction;
                            data.isUndefined = true;
                        }
                        
                        std::cout<<"Weird: photon="<<fractionsPerFlavor[0]<<", ud:"<<fractionsPerFlavor[1]<<", s:"<<fractionsPerFlavor[2];
                        std::cout<<", missing: "<<(1-fractionsPerFlavor[0]-fractionsPerFlavor[1]-fractionsPerFlavor[2])<<std::endl;
                        */
                        data.isUndefined = true;
                    }
                    
                    

                    if (displacedGenVertexCollection.product())
                    {
                        
                        for (const auto& vertex: *displacedGenVertexCollection)
                        {
                            float dRmin = 1000;
                            for(unsigned int igenJet = 0; igenJet<vertex.genJets.size();++igenJet)
                            {
                                const reco::GenJet* genJet = vertex.genJets[igenJet].get();
                                float dRGenJets = reco::deltaR(*genJet,*jet.genJet());
                                if(dRGenJets<0.02 and dRGenJets<dRmin) //numerical stability
                                {
                                    dRmin = dRGenJets;
                                    if (not vertex.motherLongLivedParticle.isNull())
                                    {
                                        const auto &mother = *(vertex.motherLongLivedParticle);
                                        data.fromLLP = getHadronFlavor(mother)>10000;
                                        data.decay_angle = angle(genJet->p4(),mother.p4());
                                        data.displacement = std::log10(std::max<float>(vertex.d3d(),1e-10));
                                        data.displacement_xy = std::log10(std::max<float>(vertex.dxy(),1e-10));
                                        data.displacement_z = std::log10(std::max<float>(vertex.dz(),1e-10));	
                                        data.vertexFraction = vertex.jetFractions[igenJet];
                                    }
                                }
                            }		
                        }
                    }
                }
                
                int test = data.isUndefined+data.isPU+data.isB+data.isBB+data.isGBB+data.isLeptonic_B+data.isLeptonic_C+data.isC+data.isCC+data.isGCC+data.isUD+data.isS+data.isG;
                if (test!=1)
                {
                    edm::LogWarning("JetOriginTagPlugin") <<"Inconsitent labelling: "<<", isPU:"<<data.isPU<<
                        ", isB:"<<data.isB<<", isBB:"<<data.isBB<<", isGBB:"<<data.isGBB<<", isLeptonic_B:"<<data.isLeptonic_B<<
                        ", isLeptonic_C:"<<data.isLeptonic_C<<", isC:"<<data.isC<<", isCC:"<<data.isCC<<
                        ", isGCC:"<<data.isGCC<<", isS:"<<data.isS<<", isUD:"<<data.isUD<<", isG:"<<data.isG<<
                        ", isUndefined: "<<data.isUndefined;
                    data.isUndefined = true;
                }
                
                
                
                
                
                output->at(0).jetData.push_back(data);
            }
            
            
            
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(xtag::XTagPluginFactory, xtag::JetOriginTagDataPlugin, "JetOriginTagData");

