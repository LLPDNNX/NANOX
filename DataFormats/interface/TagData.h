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
        virtual void fill(const Property* property, const std::string& name, ArrayInterface& array) = 0;
};

class ArrayInterface
{
    public:
        virtual unsigned int size() const = 0;
        virtual void bookProperty(const std::string& name, std::shared_ptr<Accessor> acc) = 0;
        virtual void fill(const Property* property) = 0;
       
        
        //for convenience
        template<class PROPERTY, class TYPE> void bookProperty(const std::string& name,const TYPE PROPERTY::*data);
};

template<class PROPERTY, class TYPE>
class AccessorTmpl:
    public Accessor
{
    public:
        const TYPE PROPERTY::*data_;
        AccessorTmpl(const TYPE PROPERTY::*data):
            data_(data)
        {
        }
        virtual void fill(const Property* property, const std::string& name, ArrayInterface& array)
        {
            const PROPERTY* obj = dynamic_cast<const PROPERTY*>(property);
            std::cout<<"fill: "<<name<<" = "<<obj->*data_<<std::endl;
            
        }
};

template<class PROPERTY, class TYPE> void ArrayInterface::bookProperty(const std::string& name,const TYPE PROPERTY::*data)
{
    this->bookProperty(
        name, 
        std::shared_ptr<Accessor>(new AccessorTmpl<PROPERTY,TYPE>(data))
    );
}


class ArchiveInterface
{
    public:
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
