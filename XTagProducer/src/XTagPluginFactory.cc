#include "FWCore/PluginManager/interface/PluginFactory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "XTag/XTagProducer/interface/XTagPluginFactory.h"


EDM_REGISTER_PLUGINFACTORY(xtag::XTagPluginFactory,"xtag::XTagPluginFactory");
