#ifndef XTAG_DATAFORMATS_CHARGEDPFTAGDATA_H
#define XTAG_DATAFORMATS_CHARGEDPFTAGDATA_H

#include "XTag/DataFormats/interface/TagData.h"
#include <iostream>

namespace xtag
{

class ChargedPFTagData:
    public TagData
{
    public:
        std::vector<float> ptrel;
        std::vector<unsigned int> ncpf;
        /*
        'jet_pt',
        'jet_eta',
        'nCpfcand',
        'nNpfcand',
        'nsv',
        'npv',
        'TagVarCSV_trackSumJetEtRatio', 
        'TagVarCSV_trackSumJetDeltaR', 
        'TagVarCSV_vertexCategory', 
        'TagVarCSV_trackSip2dValAboveCharm', 
        'TagVarCSV_trackSip2dSigAboveCharm', 
        'TagVarCSV_trackSip3dValAboveCharm', 
        'TagVarCSV_trackSip3dSigAboveCharm', 
        'TagVarCSV_jetNSelectedTracks', 
        'TagVarCSV_jetNTracksEtaRel'
        */
        
        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            archive.saveVectorUInt(ncpf,"ncpf");
            //archive.saveVectorFloat(jetPt,"jetPt");
            
        }
        
        virtual ~ChargedPFTagData()
        {
        }
};

}

#endif
