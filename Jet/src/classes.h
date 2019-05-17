#include "NANOX/Jet/interface/GlobalJetTagData.h"
#include "NANOX/Jet/interface/CSVInputTagData.h"
#include "NANOX/Jet/interface/ChargedPFTagData.h"
#include "NANOX/Jet/interface/NeutralPFTagData.h"
#include "NANOX/Jet/interface/SVTagData.h"
#include "NANOX/Jet/interface/JetOriginTagData.h"
#include "NANOX/Jet/interface/LegacyTagData.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace { 

struct dictionary_nanox_jet
{
    nanox::GlobalJetTagData dummy1;
    edm::Wrapper<nanox::GlobalJetTagData> dummy2;
    
    nanox::CSVInputTagData dummy3;
    edm::Wrapper<nanox::CSVInputTagData> dummy4;
    
    nanox::ChargedPFTagData dummy5;
    edm::Wrapper<nanox::ChargedPFTagData> dummy6;
    
    nanox::NeutralPFTagData dummy7;
    edm::Wrapper<nanox::NeutralPFTagData> dummy8;
    
    nanox::SVTagData dummy9;
    edm::Wrapper<nanox::SVTagData> dummy10;
    
    nanox::JetOriginTagData dummy11;
    edm::Wrapper<nanox::JetOriginTagData> dummy12;

    nanox::LegacyTagData dummy13;
    edm::Wrapper<nanox::LegacyTagData> dummy14;
};

}
