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
            float phi;
            Data(float pt, float eta, float phi):
                pt(pt),
                eta(eta),
                phi(phi)
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
            array.bookProperty("phi", &Data::phi);
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
