#include "XTag/Jet/interface/GlobalJetTagData.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace { 

struct dictionary_xtag_jet
{
    xtag::GlobalJetTagData dummy1;
    edm::Wrapper<xtag::GlobalJetTagData> dummy2;
};

}
