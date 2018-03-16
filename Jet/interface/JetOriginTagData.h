#ifndef XTAG_DATAFORMATS_JETORIGINTAGDATA_H
#define XTAG_DATAFORMATS_JETORIGINTAGDATA_H

#include "XTag/DataFormats/interface/TagData.h"
#include <iostream>

namespace xtag
{

class JetOriginTagData:
    public TagData
{
    public:
        
    
    
        class Data:
            public PropertyContainer
        {
            public:
                
                uint8_t partonFlavor;
                uint8_t hadronFlavor;
                uint8_t processFlavor; //TODO
                
                bool isPU;
                
                bool isB;
                bool isBB;
                bool isGBB;
                
                bool isLeptonic_B;
                bool isLeptonic_C;
                
                bool isC;
                bool isCC;
                bool isGCC;
                
                bool isS;
                bool isUD;
                bool isG;
                
                bool isUndefined;
                
                bool fromLLP;
                float log_displacement;
                float decay_angle;
                float vertexFraction;
                
                Data():
                    
                    partonFlavor(0),
                    hadronFlavor(0),
                    processFlavor(0),
                    isPU(0),
                    isB(0),
                    isBB(0),
                    isGBB(0),
                    isLeptonic_B(0),
                    isLeptonic_C(0),
                    isC(0),
                    isCC(0),
                    isGCC(0),
                    isS(0),
                    isUD(0),
                    isG(0),
                    isUndefined(0),
                    fromLLP(0),
                    log_displacement(-10),
                    decay_angle(-1),
                    vertexFraction(-1)
                {
                }
                
        };
        std::vector<Data> jetData;

        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            
            
            ArrayInterface& dataArray = archive.initArray("jetorigin",jetData.size());
            
            dataArray.bookProperty("partonFlavor",&Data::partonFlavor);
            dataArray.bookProperty("hadronFlavor",&Data::hadronFlavor);
            dataArray.bookProperty("processFlavor",&Data::processFlavor);
            
            dataArray.bookProperty("isPU",&Data::isPU);
            
            dataArray.bookProperty("isB",&Data::isB);
            dataArray.bookProperty("isBB",&Data::isBB);
            dataArray.bookProperty("isGBB",&Data::isGBB);
            dataArray.bookProperty("isLeptonic_B",&Data::isLeptonic_B);
            dataArray.bookProperty("isLeptonic_C",&Data::isLeptonic_C);
            
            dataArray.bookProperty("isC",&Data::isC);
            dataArray.bookProperty("isCC",&Data::isCC);
            dataArray.bookProperty("isGCC",&Data::isGCC);
            
            dataArray.bookProperty("isS",&Data::isS);
            dataArray.bookProperty("isUD",&Data::isUD);
            dataArray.bookProperty("isG",&Data::isG);
            dataArray.bookProperty("isUndefined",&Data::isUndefined);
            
            dataArray.bookProperty("fromLLP",&Data::fromLLP);
            dataArray.bookProperty("log_displacement",&Data::log_displacement);
            dataArray.bookProperty("decay_angle",&Data::decay_angle);
            
            for (unsigned int ijet = 0; ijet < jetData.size(); ++ijet)
            {
                dataArray.fill(&jetData[ijet],ijet);
            }
        }
        
        virtual ~JetOriginTagData()
        {
        }
};

}

#endif
