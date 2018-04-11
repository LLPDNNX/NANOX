#ifndef XTAG_DATAFORMATS_SVTAGDATA_H
#define XTAG_DATAFORMATS_SVTAGDATA_H

#include "XTag/DataFormats/interface/TagData.h"
#include <iostream>

namespace xtag
{

class SVTagData:
    public TagData
{
    public:
        class Data:
            public PropertyContainer
        {
            public:
                float pt;
                float deltaR;
                float mass;
                float ntracks;
                float chi2;
                float normchi2;
                float dxy;
                float dxysig;
                float d3d;
                float d3dsig;
                float costhetasvpv;
                float enratio;
        };
        std::vector<std::vector<Data>> jetData;

        virtual void saveTagData(ArchiveInterface& archive) const override
        {
            
            ArrayInterface& dataLengthArray = archive.initArray("svlength",jetData.size());
            dataLengthArray.bookProperty("length");
            unsigned int jetDataLength = 0;
            for (unsigned int ijet = 0; ijet < jetData.size(); ++ijet)
            {
                dataLengthArray.fillFloat("length",jetData[ijet].size(),ijet);
                jetDataLength+=jetData[ijet].size();
            }
            
            ArrayInterface& svDataArray = archive.initArray("sv",jetDataLength);

            svDataArray.bookProperty("pt",&Data::pt);
            svDataArray.bookProperty("deltaR",&Data::deltaR);
            svDataArray.bookProperty("mass",&Data::mass);
            svDataArray.bookProperty("ntracks",&Data::ntracks);
            svDataArray.bookProperty("chi2",&Data::chi2);
            svDataArray.bookProperty("normchi2",&Data::normchi2);
            svDataArray.bookProperty("dxy",&Data::dxy);
            svDataArray.bookProperty("dxysig",&Data::dxysig);
            svDataArray.bookProperty("d3d",&Data::d3d);
            svDataArray.bookProperty("d3dsig",&Data::d3dsig);
            svDataArray.bookProperty("costhetasvpv",&Data::costhetasvpv);
            svDataArray.bookProperty("enratio",&Data::enratio);
            
            unsigned int index = 0;
            for (unsigned int ijet = 0; ijet < jetData.size(); ++ijet)
            {
                for (unsigned int inpf = 0; inpf < jetData[ijet].size(); ++inpf)
                {
                    svDataArray.fill(&jetData[ijet][inpf],index);
                    ++index;
                }
            }
        }
        
        virtual ~SVTagData()
        {
        }
};

}

#endif
