#ifndef XTAG_DATAFORMATS_GLOBALJETTAGDATA_H
#define XTAG_DATAFORMATS_GLOBALJETTAGDATA_H

#include "XTag/DataFormats/interface/TagData.h"
#include <iostream>

namespace xtag
{

class GlobalJetTagData:
    public TagData
{
    public:
        float jetPt;
        float jetEta;
        
        virtual void saveTagData(ArchiveInterface& archive) const
        {
            archive.saveSingleFloat(jetPt,"jetPt");
            archive.saveSingleFloat(jetEta,"jetEta");
        }
        
        virtual ~GlobalJetTagData()
        {
        }
};

}

#endif
