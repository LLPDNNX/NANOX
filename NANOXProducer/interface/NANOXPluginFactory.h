#ifndef NANOX_NANOXPRODUCER_NANOXPLUGINFACTORY_H
#define NANOX_NANOXPRODUCER_NANOXPLUGINFACTORY_H

#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "NANOX/NANOXProducer/interface/NANOXPlugin.h"

#include <string>

namespace edm
{
    class ParameterSet;
    class ConsumesCollector;
    class ProducerBase;
}

namespace nanox
{
    typedef edmplugin::PluginFactory<
        nanox::NANOXPlugin*(
            const std::string&, 
            const edm::ParameterSet&, 
            edm::ConsumesCollector&,
            edm::ProducerBase&
        )
    > NANOXPluginFactory;
}

#endif
