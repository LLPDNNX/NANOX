#ifndef XTAG_DATAFORMATS_TAGDATA_H
#define XTAG_DATAFORMATS_TAGDATA_H

#include "FWCore/Utilities/interface/Exception.h"

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
       
        virtual void fillFloat(const std::string& name, float value) = 0;
        
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
            if (not obj) throw cms::Exception("Cannot cast property object to type "+std::string(typeid(PROPERTY).name()));
            array.fillFloat(name,obj->*data_);
            
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
        virtual ArrayInterface& initArray(
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
