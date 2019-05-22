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
                float median_dxy;
                float median_trackSip2dSig;
                float alpha;
                Data(float median_dxy, float median_trackSip2dSig, float alpha):
                    median_dxy(median_dxy),
                    median_trackSip2dSig(median_trackSip2dSig),
                    alpha(alpha)
            {
            }
        
        };

        public:
            std::vector<Data> legacyTagData;
           
            virtual void saveTagData(ArchiveInterface& archive) const override
            {
                ArrayInterface& legacyDataArray = archive.initArray("legacyTag",legacyTagData.size());
                legacyDataArray.bookProperty("median_dxy",&Data::median_dxy);
                legacyDataArray.bookProperty("median_trackSip2dSig",&Data::median_trackSip2dSig);
                legacyDataArray.bookProperty("alpha",&Data::alpha);

                for (unsigned int i = 0; i < legacyTagData.size(); ++i)
                {
                    legacyDataArray.fill(&legacyTagData[i],i);
                }
            }
                        
 
        virtual ~LegacyTagData()
        {
        }
};

}

#endif
