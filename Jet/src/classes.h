#include "XTag/Jet/interface/GlobalJetTagData.h"
#include "XTag/Jet/interface/CSVInputTagData.h"
#include "XTag/Jet/interface/ChargedPFTagData.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace { 

struct dictionary_xtag_jet
{
    xtag::GlobalJetTagData dummy1;
    edm::Wrapper<xtag::GlobalJetTagData> dummy2;
    
    xtag::CSVInputTagData dummy3;
    edm::Wrapper<xtag::CSVInputTagData> dummy4;
    
    xtag::ChargedPFTagData dummy5;
    edm::Wrapper<xtag::ChargedPFTagData> dummy6;
};

}
