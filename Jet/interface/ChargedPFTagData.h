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
        class Data:
            public PropertyContainer
        {
            public:
                float ptrel;
                Data(float ptrel):
                    ptrel(ptrel)
                {
                }
        };
        std::vector<std::vector<Data>> jetData;

        
        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            
            ArrayInterface& dataLengthArray = archive.initArray("cpflength",jetData.size());
            dataLengthArray.bookProperty("length");
            unsigned int jetDataLength = 0;
            for (unsigned int ijet = 0; ijet < jetData.size(); ++ijet)
            {
                dataLengthArray.fillFloat("length",jetData[ijet].size(),ijet);
                jetDataLength+=jetData[ijet].size();
            }
            
            ArrayInterface& cpfDataArray = archive.initArray("cpf",jetDataLength);
            cpfDataArray.bookProperty("ptrel", &Data::ptrel);
            
            unsigned int index = 0;
            for (unsigned int ijet = 0; ijet < jetData.size(); ++ijet)
            {
                for (unsigned int icpf = 0; icpf < jetData[ijet].size(); ++icpf)
                {
                    cpfDataArray.fill(&jetData[ijet][icpf],index);
                    ++index;
                }
            }
        }
        
        virtual ~ChargedPFTagData()
        {
        }
};

}

#endif
