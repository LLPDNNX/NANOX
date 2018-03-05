#ifndef XTAG_DATAFORMATS_GHOSTFLAVOURINFO_H
#define XTAG_DATAFORMATS_GHOSTFLAVOURINFO_H

#include "DataFormats/Common/interface/RefProd.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


#include <vector>

namespace xtag
{

class GhostFlavourInfo
{
    public:
        std::vector<reco::GenParticleRef> ghosts;
        GhostFlavourInfo()
        {
        }
        
        void addGhost(const reco::GenParticleRef& ghost)
        {
            ghosts.push_back(ghost);
        }
};

}

#endif
