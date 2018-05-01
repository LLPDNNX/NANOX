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

#include <iostream>
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;


class XJetProducer: 
    public edm::stream::EDProducer<>
{
    private:
        edm::InputTag _genParticleInputTag;
        edm::EDGetTokenT<edm::View<reco::Candidate>> _genParticleToken;
        
        edm::InputTag _genJetsInputTag;
        edm::EDGetTokenT<edm::View<reco::GenJet>> _genJetsToken;

        template<class CTYPE>
        struct ProtoJet
        {
            reco::Candidate::LorentzVector p4;
            reco::Candidate::Point vertex;
            std::vector<CTYPE> constituents;
            
            ProtoJet()
            {
            }
            
            ProtoJet(
                const reco::Candidate::LorentzVector& p4,
                const reco::Candidate::Point& vertex,
                const CTYPE& constituent
            ): 
                p4(p4), vertex(vertex), constituents{{constituent}}
            {
            }
            
            inline double pt() const
            {
                return p4.pt();
            }
            inline double E() const
            {
                return p4.E();
            }
            inline double mass() const
            {
                return p4.mass();
            }
            inline double px() const
            {
                return p4.px();
            }
            inline double py() const
            {
                return p4.py();
            }
            inline double pz() const
            {
                return p4.pz();
            }
            inline double eta() const
            {
                return p4.eta();
            }
            inline double phi() const
            {
                return p4.phi();
            }
            
            inline double x() const
            {
                return vertex.x();
            }
            inline double y() const
            {
                return vertex.y();
            }
            inline double z() const
            {
                return vertex.z();
            }
            
            
            void combine(const ProtoJet<CTYPE>& j)
            {
                vertex = reco::Candidate::Point(
                    (x()*pt()+j.x()*j.pt())/(pt()+j.pt()),
                    (y()*pt()+j.y()*j.pt())/(pt()+j.pt()),
                    (z()*pt()+j.z()*j.pt())/(pt()+j.pt())
                );
                p4 += j.p4;
                for (const CTYPE& constituent: j.constituents)
                {
                    constituents.push_back(constituent);
                }
            }
        };
        
        template<class CTYPE>
        double calc_cluster_distance(double dR, double dX, const ProtoJet<CTYPE>& j1, const ProtoJet<CTYPE>& j2)
        {
            return std::min<double>(1./(j1.pt()*j1.pt()),1./(j2.pt()*j2.pt()))*deltaR(j1.p4,j2.p4)/dR;
        }
        
        
        template<class CTYPE>
        double calc_single_distance(double dR, double dX, const ProtoJet<CTYPE>& j1)
        {
            return 1./(j1.pt()*j1.pt());
        }
        
        template<class CTYPE>
        std::vector<ProtoJet<CTYPE>> cluster(double minPt, double dR, double dX, std::vector<ProtoJet<CTYPE>> sequence)
        {
            std::vector<bool> masked(sequence.size(),false);
            
            size_t min_single_index;
            double min_single_distance = 1e10;
            std::pair<size_t,size_t> min_cluster_index;
            double min_cluster_distance = 1e10;
            
            for (size_t i = 0; i < sequence.size(); ++i)
            {
                double single_distance = calc_single_distance(dR,dX,sequence[i]);
                if (single_distance<min_single_distance)
                {
                    min_single_index = i;
                    min_single_distance = single_distance;
                }
                for (size_t j = i+1; j < sequence.size(); ++j)
                {
                    double cluster_distance = calc_cluster_distance(dR,dX,sequence[i],sequence[j]);
                    if (cluster_distance<min_cluster_distance)
                    {
                        min_cluster_index.first = i;
                        min_cluster_index.second = j;
                        min_cluster_distance = cluster_distance;
                    }
                }
            }
            std::vector<ProtoJet<CTYPE>> finalJets;
            size_t remainingJets = sequence.size();
            
            while (remainingJets>0)
            {
                if (min_single_distance<min_cluster_distance)
                {
                    //std::cout<<"min dist "<<min_single_index<<", d="<<min_single_distance<<std::endl;
                    finalJets.push_back(sequence[min_single_index]);
                    masked[min_single_index] = true;
                    if (sequence[min_single_index].pt()<minPt)
                    {
                        break;
                    }
                }
                else
                {
                    //std::cout<<"min cluster ("<<min_cluster_index.first<<","<<min_cluster_index.second<<"), d="<<min_cluster_distance<<std::endl;
                    sequence[min_cluster_index.first].combine(sequence[min_cluster_index.second]);
                    masked[min_cluster_index.second] = true;
                    
                }
                
                //recalculate distances
                min_single_distance = 1e10;
                min_cluster_distance = 1e10;
                
                for (size_t i = 0; i < sequence.size(); ++i)
                {
                    if (masked[i]) continue;
                    double single_distance = calc_single_distance(dR,dX,sequence[i]);
                    if (single_distance<min_single_distance)
                    {
                        min_single_index = i;
                        min_single_distance = single_distance;
                    }
                    for (size_t j = i+1; j < sequence.size(); ++j)
                    {
                        if (masked[j]) continue;
                        double cluster_distance = calc_cluster_distance(dR,dX,sequence[i],sequence[j]);
                        if (cluster_distance<min_cluster_distance)
                        {
                            min_cluster_index.first = i;
                            min_cluster_index.second = j;
                            min_cluster_distance = cluster_distance;
                        }
                    }
                }
                
                
                --remainingJets;
            }
            
            //std::cout<<"... end clustering"<<std::endl;
            return finalJets;
        }

    public:
        explicit XJetProducer(const edm::ParameterSet& iConfig):
            _genParticleInputTag(iConfig.getParameter<edm::InputTag>("src")),
            _genParticleToken(consumes<edm::View<reco::Candidate>>(_genParticleInputTag)),
            _genJetsInputTag(iConfig.getParameter<edm::InputTag>("genJets")),
            _genJetsToken(consumes<edm::View<reco::GenJet>>(_genJetsInputTag))
        {
            produces<std::vector<reco::GenJet>>();
        }
        
        virtual ~XJetProducer()
        {
        }

        virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup)
        {
            edm::Handle<edm::View<reco::Candidate>> genParticleCollection;
            iEvent.getByToken(_genParticleToken, genParticleCollection);
            
            edm::Handle<edm::View<reco::GenJet>> genJetCollection;
            iEvent.getByToken(_genJetsToken, genJetCollection);
            
            std::vector<ProtoJet<edm::Ptr<reco::Candidate>>> protoJets;
            for (size_t i = 0; i < genParticleCollection->size(); ++i)
            { 
                protoJets.emplace_back(
                    genParticleCollection->at(i).p4(),
                    genParticleCollection->at(i).vertex(),
                    genParticleCollection->ptrAt(i)
                );
            }
            protoJets = cluster(10, 0.4,1e-6,protoJets);
            /*
            for (size_t i = 0; i < protoJets.size(); ++i)
            {
                std::cout<<"xjet: "<<i<<", pt="<<protoJets[i].pt()<<", eta="<<protoJets[i].eta()<<", phi="<<protoJets[i].phi()<<std::endl;
            }
            
            for (size_t i = 0; i < genJetCollection->size(); ++i)
            {
                std::cout<<"genjet: "<<i<<", pt="<<genJetCollection->at(i).pt()<<", eta="<<genJetCollection->at(i).eta()<<", phi="<<genJetCollection->at(i).phi()<<std::endl;
            }
            */
            auto output = std::make_unique<std::vector<reco::GenJet>>();
            iEvent.put(std::move(output));
        }
};


DEFINE_FWK_MODULE(XJetProducer);


