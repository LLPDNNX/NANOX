#ifndef NANOX_DATAFORMATS_LEGACYTAGDATA_H
#define NANOX_DATAFORMATS_LEGACYTAGDATA_H

#include "NANOX/DataFormats/interface/TagData.h"
#include <iostream>

namespace nanox
{

class LegacyTagData:
    public TagData
{
    public:
        class Data:
            public PropertyContainer
        {
            public:
                float _;
        };
        
        std::vector<Data> jetData;
        
        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            ArrayInterface& legacyDataArray = archive.initArray("_",jetData.size());
            //legacyDataArray.bookProperty("_",&Data::_);
            
            for (unsigned int i = 0; i < jetData.size(); ++i)
            {
                //legacyDataArray.fill(&jetData[i],i);
            }
        }
        
        virtual ~LegacyTagData()
        {
        }
};

}

#endif