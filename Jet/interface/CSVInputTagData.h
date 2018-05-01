#ifndef NANOX_DATAFORMATS_CSVINPUTTAGDATA_H
#define NANOX_DATAFORMATS_CSVINPUTTAGDATA_H

#include "NANOX/DataFormats/interface/TagData.h"
#include <iostream>

namespace nanox
{

class CSVInputTagData:
    public TagData
{
    public:
        class Data:
            public PropertyContainer
        {
            public:
                float trackSumJetEtRatio;
                float trackSumJetDeltaR;
                float vertexCategory;
                float trackSip2dValAboveCharm;
                float trackSip2dSigAboveCharm;
                float trackSip3dValAboveCharm;
                float trackSip3dSigAboveCharm;
                float jetNSelectedTracks;
                float jetNTracksEtaRel;
        };
        
        std::vector<Data> jetData;
        
        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            ArrayInterface& csvDataArray = archive.initArray("csv",jetData.size());
            csvDataArray.bookProperty("trackSumJetEtRatio",&Data::trackSumJetEtRatio);
            csvDataArray.bookProperty("trackSumJetDeltaR",&Data::trackSumJetDeltaR);
            csvDataArray.bookProperty("vertexCategory",&Data::vertexCategory);
            csvDataArray.bookProperty("trackSip2dValAboveCharm",&Data::trackSip2dValAboveCharm);
            csvDataArray.bookProperty("trackSip2dSigAboveCharm",&Data::trackSip2dSigAboveCharm);
            csvDataArray.bookProperty("trackSip3dValAboveCharm",&Data::trackSip3dValAboveCharm);
            csvDataArray.bookProperty("trackSip3dSigAboveCharm",&Data::trackSip3dSigAboveCharm);
            csvDataArray.bookProperty("jetNSelectedTracks",&Data::jetNSelectedTracks);
            csvDataArray.bookProperty("jetNTracksEtaRel",&Data::jetNTracksEtaRel);
            
            for (unsigned int i = 0; i < jetData.size(); ++i)
            {
                csvDataArray.fill(&jetData[i],i);
            }
        }
        
        virtual ~CSVInputTagData()
        {
        }
};

}

#endif
