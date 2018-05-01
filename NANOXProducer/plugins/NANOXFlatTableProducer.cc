// system include files
#include <memory>
#include <vector>
#include <unordered_map>
// user include files

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"


#include "NANOX/DataFormats/interface/DisplacedGenVertex.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "NANOX/DataFormats/interface/TagData.h"

#include "DataFormats/NanoAOD/interface/FlatTable.h"

#include <algorithm>

class NANOXFlatTableProducer:
    public edm::stream::EDProducer<>
    
{
    private:    
        struct TagDataToWrite
        {
            edm::InputTag inputTag;
            std::vector<std::string> arrayNames;
            edm::EDGetTokenT<edm::View<nanox::TagData>> token;
            std::vector<std::string> tableNames;
            
            TagDataToWrite(
                const edm::InputTag& inputTag, 
                const std::vector<std::string>& arrayNames,
                edm::ProducerBase& prod,
                edm::ConsumesCollector collector
            ):
                inputTag(inputTag),
                arrayNames(arrayNames),
                token(collector.consumes<edm::View<nanox::TagData>>(inputTag))
            {
                std::string baseName = inputTag.label()+inputTag.instance();
                for (auto name: arrayNames)
                {
                    tableNames.push_back(baseName+name);
                    prod.produces<nanoaod::FlatTable>(baseName+name);
                }
            }
            
            void put(edm::Event& event, std::vector<std::unique_ptr<nanoaod::FlatTable>>&& tables)
            {
                for (unsigned int iname = 0; iname < arrayNames.size(); ++iname)
                {
                    bool inserted = false;
                    for (auto& table: tables)
                    {
                        //table may have moved on already
                        if (table and table->name()==arrayNames[iname])
                        {
                            event.put(std::move(table),tableNames[iname]);
                            inserted = true;
                            break;
                        }
                    }
                    if (not inserted)
                    {
                        throw cms::Exception("Requested array with name '"+arrayNames[iname]+"' not found in tables");
                    }
                }
            }
            
        };
        std::vector<TagDataToWrite> tagDataToWrite_;
        
        
        class FlatTableArchive:
            public nanox::ArchiveInterface
        {
            public:
                class FlatTableArray:
                    public nanox::ArrayInterface
                {
                    protected:
                        std::string name_;
                        unsigned int size_;
                        
                        std::unordered_map<std::string, std::vector<float>> floatData_;
                    public:
                        FlatTableArray(const std::string& name, unsigned int size):
                            name_(name),
                            size_(size)
                        {
                        }
                        
                        virtual unsigned int size() const
                        {
                            return size_;
                        }
                        
                        
                        virtual void bookProperty(const std::string& name, nanox::ArrayType type=nanox::ArrayType::FLOAT)
                        {
                            floatData_.emplace(
                                std::piecewise_construct,
                                std::forward_as_tuple(name),
                                std::make_tuple(size_,0.f)
                            );
                        }
                        
                        virtual void fillFloat(const std::string& name, float value, unsigned int index)
                        {
                            if (index>=size_) throw cms::Exception("Attempt to fill array index '"+std::to_string(index)+"' which is larger than its size '"+std::to_string(size_)+"'");
                            floatData_[name][index]=value;
                        }
                        
                        
                        std::unique_ptr<nanoaod::FlatTable> makeTable()
                        {
                            std::unique_ptr<nanoaod::FlatTable> table = std::make_unique<nanoaod::FlatTable>(
                                size_, 
                                name_, 
                                false,  //singleton
                                false //extend
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

                std::vector<std::unique_ptr<FlatTableArray>> arrayData_;
                
                FlatTableArchive()
                {
                }
                
                virtual nanox::ArrayInterface& initArray(
                    const std::string& name,
                    unsigned int size
                )
                {
                    arrayData_.emplace_back(new FlatTableArray(name,size));
                    return *arrayData_.back();
                }
                
                std::vector<std::unique_ptr<nanoaod::FlatTable>> makeTables()
                {
                    std::vector<std::unique_ptr<nanoaod::FlatTable>> output;
                    
                    for (auto& array: arrayData_)
                    {
                        output.emplace_back(array->makeTable());
                    }
                    
                    return std::move(output);
                }
        };
        
 

    public:
        explicit NANOXFlatTableProducer(const edm::ParameterSet&);
        ~NANOXFlatTableProducer();

        virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;
        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

};


//
//constructors and destructor
NANOXFlatTableProducer::NANOXFlatTableProducer(const edm::ParameterSet& iConfig)
{
    const std::vector<edm::ParameterSet>& tagDataConfigs = iConfig.getParameter<std::vector<edm::ParameterSet>>("tagData");
    for (const edm::ParameterSet& tagDataConfig: tagDataConfigs)
    {
        edm::InputTag inputTag = tagDataConfig.getParameter<edm::InputTag>("src");
        std::vector<std::string> arrayNames = tagDataConfig.getParameter<std::vector<std::string>>("arrayNames");
        tagDataToWrite_.emplace_back(
            inputTag,
            arrayNames,
            *this,
            this->consumesCollector()
        );
        
    }
}


NANOXFlatTableProducer::~NANOXFlatTableProducer()
{
}


// ------------ method called to produce the data  ------------
void
NANOXFlatTableProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    
    for (TagDataToWrite& tagDataToWrite: tagDataToWrite_)
    {
        FlatTableArchive ar;

        edm::Handle<edm::View<nanox::TagData>> tagDataCollection;
        iEvent.getByToken(tagDataToWrite.token, tagDataCollection);

        const nanox::TagData& tagData = tagDataCollection->at(0);
        tagData.saveTagData(ar);
        
        tagDataToWrite.put(iEvent,ar.makeTables());
    }
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
NANOXFlatTableProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(NANOXFlatTableProducer);

