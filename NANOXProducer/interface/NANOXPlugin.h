#ifndef NANOX_NANOXPRODUCER_NANOXPLUGIN_H
#define NANOX_NANOXPRODUCER_NANOXPLUGIN_H

#include <string>

namespace edm
{
    class Event;
    class EventSetup;
    class ParameterSet;
    class ConsumesCollector;
    class ProducerBase;
}

namespace nanox
{

class NANOXPlugin
{
    protected:
        std::string name_;
    public:
        NANOXPlugin(
            const std::string& name, 
            const edm::ParameterSet&, 
            edm::ConsumesCollector&,
            edm::ProducerBase&
        ):
            name_(name)
        {
        }
        
        inline const std::string& name() const
        {
            return name_;
        }
        
        virtual void produce(edm::Event&, const edm::EventSetup&) const = 0;
        
        virtual ~NANOXPlugin()
        {
        }
};

}

#endif
