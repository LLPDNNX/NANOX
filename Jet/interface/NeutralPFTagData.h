#ifndef NANOX_DATAFORMATS_NEUTRALPFTAGDATA_H
#define NANOX_DATAFORMATS_NEUTRALPFTAGDATA_H

#include "NANOX/DataFormats/interface/TagData.h"
#include <iostream>

namespace nanox
{

class NeutralPFTagData:
    public TagData
{
    public:
        class Data:
            public PropertyContainer
        {
            public:
                float ptrel;
                float deltaR;
                float isGamma;
                float hcal_fraction;
                float drminsv;
                float puppi_weight;
               
                float jetmassdroprel;
                float relIso01;
                
        };
        std::vector<std::vector<Data>> jetData;

        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            
            ArrayInterface& dataLengthArray = archive.initArray("npflength",jetData.size());
            dataLengthArray.bookProperty("length");
            unsigned int jetDataLength = 0;
            for (unsigned int ijet = 0; ijet < jetData.size(); ++ijet)
            {
                dataLengthArray.fillFloat("length",jetData[ijet].size(),ijet);
                jetDataLength+=jetData[ijet].size();
            }
            
            ArrayInterface& npfDataArray = archive.initArray("npf",jetDataLength);
            
            npfDataArray.bookProperty("ptrel",&Data::ptrel);
            npfDataArray.bookProperty("deltaR",&Data::deltaR);
            npfDataArray.bookProperty("isGamma",&Data::isGamma);
            npfDataArray.bookProperty("hcal_fraction",&Data::hcal_fraction);
            npfDataArray.bookProperty("drminsv",&Data::drminsv);
            npfDataArray.bookProperty("puppi_weight",&Data::puppi_weight);
            
            npfDataArray.bookProperty("jetmassdroprel",&Data::jetmassdroprel);
            npfDataArray.bookProperty("relIso01",&Data::relIso01);
            
            unsigned int index = 0;
            for (unsigned int ijet = 0; ijet < jetData.size(); ++ijet)
            {
                for (unsigned int inpf = 0; inpf < jetData[ijet].size(); ++inpf)
                {
                    npfDataArray.fill(&jetData[ijet][inpf],index);
                    ++index;
                }
            }
        }
        
        virtual ~NeutralPFTagData()
        {
        }
};

}

#endif
