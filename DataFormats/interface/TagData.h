#ifndef XTAG_DATAFORMATS_TAGDATA_H
#define XTAG_DATAFORMATS_TAGDATA_H

#include <string>
#include <vector>

namespace xtag
{

class ArchiveInterface
{
    public:
        virtual void saveSingleFloat(
            float value, 
            const std::string& name
        ) = 0;
        virtual void saveVectorFloat(
            const std::vector<float>& values, 
            const std::string& name, 
            const std::string& sizename
        ) = 0;
};

class TagData
{
    protected:
    public:
        virtual void saveTagData(ArchiveInterface& archive) const
        {
        }
        virtual ~TagData()
        {
        }
};



//template<class T> typename TagDataTmpl<T>::Base::registerFields();
}

#endif
