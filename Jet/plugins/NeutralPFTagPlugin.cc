#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ProducerBase.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "NANOX/NANOXProducer/interface/NANOXPlugin.h"
#include "NANOX/NANOXProducer/interface/NANOXPluginFactory.h"
#include "NANOX/Jet/interface/NeutralPFTagData.h"

#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>

#include "TVector3.h"

namespace nanox
{

class NeutralPFTagDataPlugin:
    public NANOXPlugin
{
    private:
        edm::EDGetTokenT<edm::View<pat::Jet>> jetToken_;
        edm::EDGetTokenT<edm::View<reco::VertexCompositePtrCandidate>> svToken_;
        
    public:
        NeutralPFTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            NANOXPlugin(name,pset,collector,prod),
            jetToken_(collector.consumes<edm::View<pat::Jet>>(pset.getParameter<edm::InputTag>("jets"))),
            svToken_(collector.consumes<edm::View<reco::VertexCompositePtrCandidate>>(pset.getParameter<edm::InputTag>("svVertices")))
        {
            prod.produces<std::vector<nanox::NeutralPFTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup& setup) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(jetToken_, jetCollection);
            
            edm::Handle<edm::View<reco::VertexCompositePtrCandidate>> svCollection;
            event.getByToken(svToken_, svCollection);
           
            std::unique_ptr<std::vector<nanox::NeutralPFTagData>> output(
                new std::vector<nanox::NeutralPFTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                const float jet_pt_uncorr = jet.correctedJet("Uncorrected").pt();
                //const float jet_e_uncorr = jet.correctedJet("Uncorrected").energy();
                
                std::vector<nanox::NeutralPFTagData::Data> npfData;
                for (unsigned int idaughter = 0; idaughter < jet.numberOfDaughters(); ++idaughter)
                {
                    const pat::PackedCandidate* constituent = dynamic_cast<const pat::PackedCandidate*>(jet.daughter(idaughter));
                    if ((not constituent) or constituent->charge()!=0)
                    {
                        continue;
                    }
                    
                    nanox::NeutralPFTagData::Data data;
                    
                    data.ptrel = 0.01/(0.01+constituent->pt()/jet_pt_uncorr);
                    data.puppi_weight = constituent->puppiWeight();
                    data.deltaR = reco::deltaR(*constituent,jet);
                    data.isGamma = fabs(constituent->pdgId())==22;
                    data.hcal_fraction = constituent->hcalFraction();
                    
                    data.drminsv = 0.4;
                    for (const auto& sv: *svCollection.product())
                    {
                        float dR = reco::deltaR(sv,*constituent);
                        data.drminsv = std::min(data.drminsv,dR);
                    }
                    
                    if (jet.mass()<1e-10) data.jetmassdroprel = 0;
                    else data.jetmassdroprel = std::log10(1-(jet.p4()-constituent->p4()).mass()/jet.mass());
                  
                    float sumPt = 0.;
                    for (unsigned int jdaughter = 0; jdaughter < jet.numberOfDaughters(); ++jdaughter)
                    {
                        const pat::PackedCandidate* other = dynamic_cast<const pat::PackedCandidate*>(jet.daughter(jdaughter));
                        if (other and other!=constituent and reco::deltaR(*other,*constituent)<0.1)
                        {
                            sumPt += other->pt();
                        }
                    }
                    data.relIso01 = 10./(10.+sumPt/constituent->pt());
                    
                    npfData.emplace_back(data);
                }
                
                std::stable_sort(npfData.begin(),npfData.end(),[](const auto& d1, const auto& d2)
                {
                    if (!std::isnan(d1.drminsv) and !std::isinf(d1.drminsv))
                    {
                        if (!std::isnan(d2.drminsv) and !std::isinf(d2.drminsv))
                        {
                            if (std::fabs(d1.drminsv-d2.drminsv)>std::numeric_limits<float>::epsilon())
                            {
                                return d1.drminsv<d2.drminsv; //sort increasing
                            }
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (!std::isnan(d2.drminsv) and !std::isinf(d2.drminsv))
                    {
                        return true;
                    }
                    
                    if (!std::isnan(d1.ptrel) and !std::isinf(d1.ptrel))
                    {
                        if (!std::isnan(d2.ptrel) and !std::isinf(d2.ptrel))
                        {
                            return d1.ptrel>d2.ptrel; //sort decreasing
                        }
                        else
                        {
                            return true;
                        }
                    }
                    else if (!std::isnan(d2.ptrel) and !std::isinf(d2.ptrel))
                    {
                        return false;
                    }
                    return false;
                });
                
                output->at(0).jetData.push_back(npfData);
            }
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(nanox::NANOXPluginFactory, nanox::NeutralPFTagDataPlugin, "NeutralPFTagData");

