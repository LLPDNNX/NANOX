#ifndef XTAG_XTAGPRODUCER_XTAGPLUGINFACTORY_H
#define XTAG_XTAGPRODUCER_XTAGPLUGINFACTORY_H

#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "XTag/XTagProducer/interface/XTagPlugin.h"

#include <string>

namespace edm
{
    class ParameterSet;
    class ConsumesCollector;
    class ProducerBase;
}

namespace xtag
{
    typedef edmplugin::PluginFactory<
        xtag::XTagPlugin*(
            const std::string&, 
            const edm::ParameterSet&, 
            edm::ConsumesCollector&,
            edm::ProducerBase&
        )
    > XTagPluginFactory;
}

#endif
