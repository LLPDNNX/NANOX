#ifndef XTAG_XTAGPRODUCER_XTAGPLUGIN_H
#define XTAG_XTAGPRODUCER_XTAGPLUGIN_H

#include <string>

namespace edm
{
    class Event;
    class EventSetup;
    class ParameterSet;
    class ConsumesCollector;
    class ProducerBase;
}

namespace xtag
{

class XTagPlugin
{
    protected:
        std::string name_;
    public:
        XTagPlugin(
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
        
        virtual ~XTagPlugin()
        {
        }
};

}

#endif
