#ifndef XTAG_DATAFORMATS_CSVINPUTTAGDATA_H
#define XTAG_DATAFORMATS_CSVINPUTTAGDATA_H

#include "XTag/DataFormats/interface/TagData.h"
#include <iostream>

namespace xtag
{

class CSVInputTagData:
    public TagData
{
    public:
        std::vector<float> trackSumJetEtRatio;
        std::vector<float> trackSumJetDeltaR;
        std::vector<float> vertexCategory;
        std::vector<float> trackSip2dValAboveCharm;
        std::vector<float> trackSip2dSigAboveCharm;
        std::vector<float> trackSip3dValAboveCharm;
        std::vector<float> trackSip3dSigAboveCharm;
        std::vector<float> jetNSelectedTracks;
        std::vector<float> jetNTracksEtaRel;
        
        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            /*
            archive.saveVectorFloat(trackSumJetEtRatio,"trackSumJetEtRatio");
            archive.saveVectorFloat(trackSumJetDeltaR,"trackSumJetDeltaR");
            archive.saveVectorFloat(vertexCategory,"vertexCategory");
            archive.saveVectorFloat(trackSip2dValAboveCharm,"trackSip2dValAboveCharm");
            archive.saveVectorFloat(trackSip2dSigAboveCharm,"trackSip2dSigAboveCharm");
            archive.saveVectorFloat(trackSip3dValAboveCharm,"trackSip3dValAboveCharm");
            archive.saveVectorFloat(trackSip3dSigAboveCharm,"trackSip3dSigAboveCharm");
            archive.saveVectorFloat(jetNSelectedTracks,"jetNSelectedTracks");
            archive.saveVectorFloat(jetNTracksEtaRel,"jetNTracksEtaRel");
            */
        }
        
        virtual ~CSVInputTagData()
        {
        }
};

}

#endif
