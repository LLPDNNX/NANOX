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

#include "XTag/XTagProducer/interface/XTagPlugin.h"
#include "XTag/XTagProducer/interface/XTagPluginFactory.h"
#include "XTag/Jet/interface/ChargedPFTagData.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>

#include "TVector3.h"

namespace xtag
{

class ChargedPFTagDataPlugin:
    public XTagPlugin
{
    private:
        edm::EDGetTokenT<edm::View<pat::Jet>> jetToken_;
        edm::EDGetTokenT<edm::View<reco::Vertex>> pvToken_;
        edm::EDGetTokenT<edm::View<reco::VertexCompositePtrCandidate>> svToken_;
        
    public:
        ChargedPFTagDataPlugin(
            const std::string& name, 
            const edm::ParameterSet& pset, 
            edm::ConsumesCollector& collector,
            edm::ProducerBase& prod
        ):
            XTagPlugin(name,pset,collector,prod),
            jetToken_(collector.consumes<edm::View<pat::Jet>>(pset.getParameter<edm::InputTag>("jets"))),
            pvToken_(collector.consumes<edm::View<reco::Vertex>>(pset.getParameter<edm::InputTag>("pvVertices"))),
            svToken_(collector.consumes<edm::View<reco::VertexCompositePtrCandidate>>(pset.getParameter<edm::InputTag>("svVertices")))
        {
            prod.produces<std::vector<xtag::ChargedPFTagData>>(name);
        }
        
        virtual void produce(edm::Event& event, const edm::EventSetup& setup) const
        {
            edm::Handle<edm::View<pat::Jet>> jetCollection;
            event.getByToken(jetToken_, jetCollection);
            
            edm::Handle<edm::View<reco::Vertex>> pvCollection;
            event.getByToken(pvToken_, pvCollection);
            
            edm::Handle<edm::View<reco::VertexCompositePtrCandidate>> svCollection;
            event.getByToken(svToken_, svCollection);
            
            const reco::Vertex& pv = pvCollection->at(0);
            
            edm::ESHandle<TransientTrackBuilder> builder;
            setup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
            
            std::unique_ptr<std::vector<xtag::ChargedPFTagData>> output(
                new std::vector<xtag::ChargedPFTagData>(1)
            );
            
            for (unsigned int ijet = 0; ijet < jetCollection->size(); ++ijet)
            {
                const pat::Jet& jet = jetCollection->at(ijet);
                const float jet_pt_uncorr = jet.correctedJet("Uncorrected").pt();
                //const float jet_e_uncorr = jet.correctedJet("Uncorrected").energy();
                
                std::vector<xtag::ChargedPFTagData::Data> cpfData;
                for (unsigned int idaughter = 0; idaughter < jet.numberOfDaughters(); ++idaughter)
                {
                    const pat::PackedCandidate* constituent = dynamic_cast<const pat::PackedCandidate*>(jet.daughter(idaughter));
                    if ((not constituent) or constituent->charge()==0 or (not constituent->hasTrackDetails()))
                    {
                        continue;
                    }
                    
                    xtag::ChargedPFTagData::Data data;
                    
                    data.ptrel = 0.01/(0.01+constituent->pt()/jet_pt_uncorr);
                    
                    data.drminsv = 0.4;
                    for (const auto& sv: *svCollection.product())
                    {
                        float dR = reco::deltaR(sv,*constituent);
                        data.drminsv = std::min(data.drminsv,dR);
                    }
                    
                    data.vertex_association = constituent->pvAssociationQuality();
                    data.puppi_weight = constituent->puppiWeight();
                    data.track_chi2 = std::log10(constituent->pseudoTrack().chi2()/constituent->pseudoTrack().ndof()+1);
                    data.track_quality = constituent->pseudoTrack().qualityMask();
                    if (jet.mass()<1e-10) data.jetmassdroprel = 0;
                    else data.jetmassdroprel = std::log10(1-(jet.p4()-constituent->p4()).mass()/jet.mass());
                    
                    reco::TransientTrack transientTrack = builder->build(constituent->pseudoTrack());
                    reco::Candidate::Vector jetDir = jet.momentum().Unit();
                    GlobalVector jetRefTrackDir(jet.px(),jet.py(),jet.pz());
                    
                    Measurement1D meas_ip2d=IPTools::signedTransverseImpactParameter(transientTrack, jetRefTrackDir, pv).second;
                    Measurement1D meas_ip3d=IPTools::signedImpactParameter3D(transientTrack, jetRefTrackDir, pv).second;
                    Measurement1D jetdist=IPTools::jetTrackDistance(transientTrack, jetRefTrackDir, pv).second;
                    reco::Candidate::Vector trackMom = constituent->pseudoTrack().momentum();
                    double trackMag = std::sqrt(trackMom.Mag2());
                    TVector3 trackMom3(trackMom.x(),trackMom.y(),trackMom.z());
                    TVector3 jetDir3(jetDir.x(),jetDir.y(),jetDir.z());

                    data.trackEtaRel=reco::btau::etaRel(jetDir, trackMom);
                    data.trackPtRel=std::log10(trackMom3.Perp(jetDir3));
                    data.trackPPar=std::log10(1+jetDir.Dot(trackMom));
                    data.trackDeltaR=reco::deltaR(trackMom, jetDir);
                    data.trackPtRatio=std::log10(1-data.trackPtRel / trackMag);
                    data.trackPParRatio=std::log10(1+data.trackPPar / trackMag);
                    
                    data.trackSip2dVal=std::copysign(std::log10(std::fabs(meas_ip2d.value())),meas_ip2d.value());
                    data.trackSip2dSig=std::copysign(std::log10(std::fabs(meas_ip2d.significance())),meas_ip2d.significance());
                    data.trackSip3dVal=std::copysign(std::log10(std::fabs(meas_ip3d.value())),meas_ip3d.value());
                    data.trackSip3dSig=std::copysign(std::log10(std::fabs(meas_ip3d.significance())),meas_ip3d.significance());
                    
                    data.trackJetDistVal = std::log10(1-jetdist.value());
                    data.trackJetDistSig = jetdist.significance();
                    
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
                    
                    data.lostInnerHits = constituent->lostInnerHits(); //http://cmsdoxygen.web.cern.ch/cmsdoxygen/CMSSW_9_4_4/doc/html/d8/d79/classpat_1_1PackedCandidate.html#ab9ef9a12f92e02fa61653ba77ee34274
        
                    cpfData.emplace_back(data);
                }
                
                std::stable_sort(cpfData.begin(),cpfData.end(),[](const auto& d1, const auto& d2)
                {
                    if (!std::isnan(d1.trackSip2dSig) and !std::isinf(d1.trackSip2dSig))
                    {
                        if (!std::isnan(d2.trackSip2dSig) and !std::isinf(d2.trackSip2dSig))
                        {
                            if (std::fabs(d1.drminsv-d2.drminsv)>std::numeric_limits<float>::epsilon())
                            {
                                return std::fabs(d1.trackSip2dSig)>std::fabs(d2.trackSip2dSig); //sort decreasing
                            }
                        }
                        else
                        {
                            return true;
                        }
                    }
                    else if (!std::isnan(d2.trackSip2dSig) and !std::isinf(d2.trackSip2dSig))
                    {
                        return false;
                    }
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
                
                
                
                output->at(0).jetData.push_back(cpfData);
            }
            
            
            
            event.put(std::move(output),this->name());
        }
};

}

DEFINE_EDM_PLUGIN(xtag::XTagPluginFactory, xtag::ChargedPFTagDataPlugin, "ChargedPFTagData");

