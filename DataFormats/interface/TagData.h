#ifndef XTAG_DATAFORMATS_TAGDATA_H
#define XTAG_DATAFORMATS_TAGDATA_H

#include <string>
#include <vector>
#include <memory>
#include <iostream> 

#include "TObject.h"

namespace xtag
{

class TagData;
class ArrayInterface;

class Property
{
    public:
        virtual ~Property()
        {
        }
};

class Accessor
{
    public:
        virtual void fill(Property* property, ArrayInterface& array) = 0;
};

class ArrayInterface
{
    public:
        virtual unsigned int size() const = 0;
        virtual void bookFloat(const std::string& name, Accessor* acc) = 0;
        virtual void fill(Property* property) = 0;
};



template<class PROPERTY, class TYPE>
class AccessorTmpl:
    public Accessor
{
    public:
        TYPE PROPERTY::*data_;
        AccessorTmpl(TYPE PROPERTY::*data):
            data_(data)
        {
        }
        virtual void fill(Property* property, ArrayInterface& array)
        {
            PROPERTY* obj = dynamic_cast<PROPERTY*>(property);
            std::cout<<"fill: "<<obj->*data_<<std::endl;
        }
};



class ArchiveInterface
{
    public:
        /*
        virtual void addSingleFloat(
            float value, 
            const std::string& name
        ) = 0;
        virtual void addVectorFloat(
            const std::vector<float>& values, 
            const std::string& name
        ) = 0;
        virtual void addVectorUInt(
            const std::vector<unsigned int>& values, 
            const std::string& name
        ) = 0;
        */
        virtual ArrayInterface& bookArray(
            const std::string& name,
            unsigned int size
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

}

#endif
