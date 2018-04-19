import FWCore.ParameterSet.Config as cms


#merge GenParticles for easy matching of GenJets to vertices
genParticlesMerged = cms.EDProducer("MergedGenParticleProducer",
    inputPruned = cms.InputTag("prunedGenParticles"),
    inputPacked = cms.InputTag("packedGenParticles")
)


#select only stable particles (status=1) for reclustering GenJets for the matching; exclude neutrinos
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
genParticlesForGenJets = cms.EDFilter(
    "CandPtrSelector", 
    src = cms.InputTag("genParticlesMerged"),
    cut = cms.string("status==1 && abs(pdgId) != 12 && abs(pdgId) != 14 && abs(pdgId) != 16 && abs(pdgId) < 1000000")
)


#recluster GenJets
genJetsReclustered = ak4GenJets.clone(
    src = cms.InputTag('genParticlesForGenJets')
)
'''
genXJetsReclustered = cms.EDProducer(
    'XJetProducer',
    src = cms.InputTag('genParticlesForGenJets'),
    genJets = cms.InputTag("genJetsReclustered")
)
'''

#TFileService = cms.Service("TFileService", fileName = cms.string("histo5.root") )

#produce DisplacedGenVertices and match to GenJets
displacedGenVertices = cms.EDProducer(
    "DisplacedGenVertexProducer",
    srcGenParticles = cms.InputTag("genParticlesMerged"),
    srcGenJets = cms.InputTag("genJetsReclustered")
)

DisplacedGenVertexSequence = cms.Sequence(
    genParticlesMerged     +
    genParticlesForGenJets +
    genJetsReclustered     +
    #genXJetsReclustered    +
    displacedGenVertices
)
