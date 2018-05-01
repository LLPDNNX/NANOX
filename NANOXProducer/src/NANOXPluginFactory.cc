#include "FWCore/PluginManager/interface/PluginFactory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "NANOX/NANOXProducer/interface/NANOXPluginFactory.h"


EDM_REGISTER_PLUGINFACTORY(nanox::NANOXPluginFactory,"nanox::NANOXPluginFactory");
