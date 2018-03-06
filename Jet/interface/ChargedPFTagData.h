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
                float trackEtaRel;
                float trackPtRel;
                float trackPPar;
                float trackDeltaR;
                float trackPtRatio;
                float trackPParRatio;
                float trackSip2dVal;
                float trackSip2dSig;
                float trackSip3dVal;
                float trackSip3dSig;
                float trackJetDistVal;
                float trackJetDistSig;
                
                float ptrel;
                float drminsv;
                float vertex_association;
                float puppi_weight;
                float track_chi2;
                float track_quality;
               
                float jetmassdroprel;
                float relIso01;
                
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
            
            cpfDataArray.bookProperty("trackEtaRel",&Data::trackEtaRel);
            cpfDataArray.bookProperty("trackPtRel",&Data::trackPtRel);
            cpfDataArray.bookProperty("trackPPar",&Data::trackPPar);
            cpfDataArray.bookProperty("trackDeltaR",&Data::trackDeltaR);
            cpfDataArray.bookProperty("trackPtRatio",&Data::trackPtRatio);
            cpfDataArray.bookProperty("trackPParRatio",&Data::trackPParRatio);
            cpfDataArray.bookProperty("trackSip2dVal",&Data::trackSip2dVal);
            cpfDataArray.bookProperty("trackSip2dSig",&Data::trackSip2dSig);
            cpfDataArray.bookProperty("trackSip3dVal",&Data::trackSip3dVal);
            cpfDataArray.bookProperty("trackSip3dSig",&Data::trackSip3dSig);
            cpfDataArray.bookProperty("trackJetDistVal",&Data::trackJetDistVal);
            cpfDataArray.bookProperty("trackJetDistSig",&Data::trackJetDistSig);
                
            cpfDataArray.bookProperty("ptrel",&Data::ptrel);
            cpfDataArray.bookProperty("drminsv",&Data::drminsv);
            cpfDataArray.bookProperty("vertex_association",&Data::vertex_association);
            cpfDataArray.bookProperty("puppi_weight",&Data::puppi_weight);
            cpfDataArray.bookProperty("track_chi2",&Data::track_chi2);
            cpfDataArray.bookProperty("track_quality",&Data::track_quality);
            
            cpfDataArray.bookProperty("jetmassdroprel",&Data::jetmassdroprel);
            cpfDataArray.bookProperty("relIso01",&Data::relIso01);
            
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
