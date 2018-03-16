#ifndef XTAG_DATAFORMATS_TAGDATA_H
#define XTAG_DATAFORMATS_TAGDATA_H

#include "FWCore/Utilities/interface/Exception.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream> 

#include "TObject.h"
#include <unordered_map>

namespace xtag
{

class TagData;
class ArrayInterface;

enum class ArrayType {BOOLEAN,INT8,UINT8,INT16,UINT16,INT32,UINT32,INT64,UINT64,FLOAT,DOUBLE};

class PropertyContainer
{
    public:
        virtual ~PropertyContainer()
        {
        }
};

class Accessor
{
    public:
        virtual ArrayType type() const = 0;
        virtual void fill(const PropertyContainer* property, const std::string& name, ArrayInterface& array, unsigned int index) = 0;
};

class ArrayInterface
{
    public:
        
    protected:
        std::unordered_map<std::string, std::shared_ptr<xtag::Accessor>> accessors_;
    public:
        virtual unsigned int size() const = 0;
        virtual void bookProperty(const std::string& name, ArrayType type = ArrayType::FLOAT) = 0;
        virtual void fillFloat(const std::string& name, float value, unsigned int index) = 0;
        
        //for convenience
        virtual void bookProperty(const std::string& name, std::shared_ptr<Accessor> acc)
        {
            accessors_[name]=acc;
            this->bookProperty(name,acc->type());
        }
        
        virtual void fill(const PropertyContainer* property, unsigned int index)
        {
            for (auto itPair: accessors_)
            {
                itPair.second->fill(property,itPair.first,*this,index);
            }
        }
        
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
        
        virtual ArrayType type() const
        {
            //base types
            if (std::is_same<TYPE,bool>::value) return ArrayType::BOOLEAN;
            
            if (std::is_same<TYPE,int8_t>::value) return ArrayType::INT8;
            if (std::is_same<TYPE,uint8_t>::value) return ArrayType::UINT8;
            
            if (std::is_same<TYPE,int16_t>::value) return ArrayType::INT16;
            if (std::is_same<TYPE,uint16_t>::value) return ArrayType::UINT16;
            
            if (std::is_same<TYPE,int32_t>::value) return ArrayType::INT32;
            if (std::is_same<TYPE,uint32_t>::value) return ArrayType::UINT32;
            
            if (std::is_same<TYPE,int64_t>::value) return ArrayType::INT64;
            if (std::is_same<TYPE,uint64_t>::value) return ArrayType::UINT64;
            
            if (std::is_same<TYPE,float>::value) return ArrayType::FLOAT;
            if (std::is_same<TYPE,double>::value) return ArrayType::DOUBLE;
            
            //test if these types have the correct byte length
            static_assert(sizeof(char)==sizeof(int8_t),"Wrong size of type 'char' found");
            if (std::is_same<TYPE,char>::value) return ArrayType::INT8;
            if (std::is_same<TYPE,unsigned char>::value) return ArrayType::UINT8;
            
            static_assert(sizeof(int)==sizeof(int32_t),"Wrong size of type 'int' found");
            if (std::is_same<TYPE,int>::value) return ArrayType::INT32;
            if (std::is_same<TYPE,unsigned int>::value) return ArrayType::UINT32;
            
            static_assert(sizeof(long)==sizeof(int64_t),"Wrong size of type 'long' found");
            if (std::is_same<TYPE,long>::value) return ArrayType::INT64;
            if (std::is_same<TYPE,unsigned long>::value) return ArrayType::UINT64;
            
            static_assert(sizeof(size_t)==sizeof(int64_t),"Wrong size of type 'size_t' found");
            if (std::is_same<TYPE,size_t>::value) return ArrayType::UINT64;
            
            throw std::runtime_error(std::string("No suitable array type found for data member ")+typeid(TYPE).name());
            return ArrayType::FLOAT;
        }
        
        virtual void fill(const PropertyContainer* property, const std::string& name, ArrayInterface& array, unsigned int index)
        {
            const PROPERTY* obj = dynamic_cast<const PROPERTY*>(property);
            if (not obj) throw cms::Exception("Cannot cast property object to type "+std::string(typeid(PROPERTY).name()));
            //TODO: currently everything is treated as a float
            array.fillFloat(name,obj->*data_, index);
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
