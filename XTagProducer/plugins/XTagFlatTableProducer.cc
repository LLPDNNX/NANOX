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

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "XTag/DataFormats/interface/TagData.h"

#include "DataFormats/NanoAOD/interface/FlatTable.h"

#include <algorithm>

class XTagFlatTableProducer:
    public edm::stream::EDProducer<>
    
{
    private:    
        struct TagDataToWrite
        {
            std::string basename;
            std::vector<edm::InputTag> inputTags;
            std::vector<edm::EDGetTokenT<edm::View<xtag::TagData>>> tokens;
            bool extend;
            
            TagDataToWrite(const std::string basename):
                basename(basename),
                extend(false)
            {
            }
            
            inline unsigned int size() const
            {
                return inputTags.size();
            }
            
            void addTagData(
                edm::InputTag& inputTag, 
                edm::EDGetTokenT<edm::View<xtag::TagData>> token
            )
            {
                inputTags.emplace_back(inputTag);
                tokens.emplace_back(token);
            }
        };
        std::vector<TagDataToWrite> tagDataToWrite_;
        
        class FlatTableArchive:
            public xtag::ArchiveInterface
        {
            public:
                class FlatTableArray:
                    public xtag::ArrayInterface
                {
                    protected:
                        unsigned int size_;
                        unsigned int fillIndex_;
                        std::unordered_map<std::string, std::shared_ptr<xtag::Accessor>> accessors_;
                        
                        std::unordered_map<std::string, std::vector<float>> floatData_;
                    public:
                        FlatTableArray(unsigned int size):
                            size_(size),
                            fillIndex_(0)
                        {
                        }
                        
                        virtual unsigned int size() const
                        {
                            return size_;
                        }
                        virtual void bookProperty(const std::string& name,std::shared_ptr<xtag::Accessor> acc)
                        {
                            accessors_[name]=acc;
                            floatData_.emplace(
                                std::piecewise_construct,
                                std::forward_as_tuple(name),
                                std::make_tuple(size_,0.f)
                            );
                        }
                        
                        virtual void fill(const xtag::Property* property)
                        {
                            for (auto itPair: accessors_)
                            {
                                itPair.second->fill(property,itPair.first,*this);
                            }
                            fillIndex_++; //TODO: what if user calls this function twice per object?
                        }
                        
                        virtual void fillFloat(const std::string& name, float value)
                        {
                            if (fillIndex_>=size_) throw cms::Exception("Attempt to fill array index '"+std::to_string(fillIndex_)+"' which is larger than its size '"+std::to_string(size_)+"'");
                            floatData_[name][fillIndex_]=value;
                        }
                        
                        std::unique_ptr<nanoaod::FlatTable> makeTable(
                            const std::string& name, 
                            bool extend
                        )
                        {
                            std::unique_ptr<nanoaod::FlatTable> table = 
                                std::make_unique<nanoaod::FlatTable>(
                                    size_, 
                                    name, 
                                    false, 
                                    extend
                                );
                            for (auto floatData: floatData_)
                            {
                                table->addColumn<float>(
                                    floatData.first, 
                                    floatData.second, 
                                    "", 
                                    nanoaod::FlatTable::FloatColumn
                                );
                            }
                            
                            return std::move(table);
                        }
                };

                std::unique_ptr<FlatTableArray> arrayData_;
                
                FlatTableArchive(bool extend)
                {
                }
                
                virtual xtag::ArrayInterface& initArray(
                    unsigned int size
                )
                {
                    arrayData_.reset(new FlatTableArray(size));
                    return *arrayData_;
                }
                
                
        };
 

    public:
        explicit XTagFlatTableProducer(const edm::ParameterSet&);
        ~XTagFlatTableProducer();

        virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;
        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};


//
//constructors and destructor
XTagFlatTableProducer::XTagFlatTableProducer(const edm::ParameterSet& iConfig)
{
    const edm::ParameterSet& tagDataConfigs = iConfig.getParameter<edm::ParameterSet>("tagData");
    std::vector<std::string> names = tagDataConfigs.getParameterNames();
    for (auto name: names)
    {
        edm::ParameterSet tagDataConfig = tagDataConfigs.getParameter<edm::ParameterSet>(name);
        TagDataToWrite tagDataToWrite(name);
        std::vector<edm::InputTag> inputTags = tagDataConfig.getParameter<std::vector<edm::InputTag>>("srcs");
        for (unsigned int i = 0; i < inputTags.size(); ++i)
        {
            tagDataToWrite.addTagData(
                inputTags[i],
                consumes<edm::View<xtag::TagData>>(inputTags[i])
            );
        }
        if (tagDataConfig.exists("extend"))
        {
            tagDataToWrite.extend = tagDataConfig.getParameter<bool>("extend");
        }
        tagDataToWrite_.emplace_back(std::move(tagDataToWrite));
        produces<nanoaod::FlatTable>(name);
    }
}


XTagFlatTableProducer::~XTagFlatTableProducer()
{
}


// ------------ method called to produce the data  ------------
void
XTagFlatTableProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    for (const TagDataToWrite& tagDataToWrite: tagDataToWrite_)
    {
        
        FlatTableArchive ar(tagDataToWrite.extend);
        for (unsigned int idata = 0; idata < tagDataToWrite.size(); ++idata)
        {
            edm::Handle<edm::View<xtag::TagData>> tagDataCollection;
            iEvent.getByToken(tagDataToWrite.tokens[idata], tagDataCollection);

            const xtag::TagData& tagData = tagDataCollection->at(0);
            tagData.saveTagData(ar);
        }
        iEvent.put(std::move(ar.arrayData_->makeTable(tagDataToWrite.basename,false)),tagDataToWrite.basename);
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
XTagFlatTableProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(XTagFlatTableProducer);

