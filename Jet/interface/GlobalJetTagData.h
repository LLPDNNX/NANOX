#ifndef NANOX_DATAFORMATS_GLOBALJETTAGDATA_H
#define NANOX_DATAFORMATS_GLOBALJETTAGDATA_H

#include "NANOX/DataFormats/interface/TagData.h"
#include <iostream>

namespace nanox
{


class GlobalJetTagData:
    public TagData
{
    class Data:
        public PropertyContainer
    {
        public:
            float pt;
            float eta;
            Data(float pt, float eta):
                pt(pt),
                eta(eta)
            {
            }
        
    };
    
    public:
        std::vector<Data> jetData;
        
        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            ArrayInterface& array = archive.initArray("global",jetData.size());
            array.bookProperty("pt", &Data::pt);
            array.bookProperty("eta", &Data::eta);
            for (unsigned int i = 0; i < jetData.size(); ++i)
            {
                array.fill(&jetData[i], i);
            }
        }
        
        virtual ~GlobalJetTagData()
        {
        }
};

}

#endif
