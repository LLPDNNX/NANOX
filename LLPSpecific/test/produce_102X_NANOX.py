import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from PhysicsTools.NanoAOD.common_cff import *
from Configuration.StandardSequences.Eras import eras

options = VarParsing ('analysis')
options.register(
    'isData',
    False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "is data"
)

options.register(
    'addLLPInfo',
    True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "add llp info"
)

options.register(
    'LLPtype',
    "T1qqqqLL",
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "add llp type"
)

options.register(
    'year',
    2016,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "add year file"
)

options.parseArguments() 

if options.year == 2016:
    process = cms.Process('NANO',eras.Run2_2016,eras.run2_nanoAOD_94X2016)
elif options.year == 2017:
    process = cms.Process('NANO',eras.Run2_2017,eras.run2_nanoAOD_94XMiniAODv2)
elif options.year == 2018:
    process = cms.Process('NANO',eras.Run2_2018,eras.run2_nanoAOD_102Xv1)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('PhysicsTools.NanoAOD.nano_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

if options.isData:
    process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
else:
    process.load('SimGeneral.MixingModule.mixNoPU_cfi')
    process.load('Configuration.StandardSequences.MagneticField_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(),
    secondaryFileNames = cms.untracked.vstring()
)

process.source.fileNames = [
        'root://cms-xrd-global.cern.ch///store/mc/RunIISummer16MiniAODv3/gluinoGMSB_M2500_ctau1000p0_TuneCP2_13TeV_pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3-v1/80000/FC634AC8-7D0B-E911-98E2-AC1F6B0DE2F4.root'
        ]

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('test102X nevts:1000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

process.plain=cms.Path()

def addModule(m):
    process.plain+=m

if not options.isData:
    process.TFileService = cms.Service("TFileService", 
        fileName = cms.string("info.root")
    )

    process.eventAndPuInfo = cms.EDAnalyzer("EventInfoCollector",
        GenEventInfo=cms.InputTag("generator"),
        PileupSummaryInfo=cms.InputTag("slimmedAddPileupInfo")
    )
    process.plain+=process.eventAndPuInfo

#Output definition

process.NANOAODSIMoutput = cms.OutputModule("NanoAODOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    saveProvenance = cms.untracked.bool(True),
    fakeNameForCrab = cms.untracked.bool(True),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring([
            "filtered*",
            "plain*", #this deactivates filtering
        ])
    ), 
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('NANOAODSIM'),
        #dataTier = cms.untracked.string('NANOAOD'), for DATA! TODO
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('nano.root'),
    outputCommands = cms.untracked.vstring(
         'drop *', 
        'keep nanoaodFlatTable_*Table_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep nanoaodMergeableCounterTable_*Table_*_*', 
        'keep nanoaodUniqueString_nanoMetadata_*_*',
        
        'drop *_caloMetTable_*_*',
        'drop *_fatJetTable_*_*',
        'drop *_genJetAK8FlavourTable_*_*',
        'drop *_genJetAK8Table_*_*',
        'drop *_genVisTauTable_*_*',
        'drop *_genJetTauTable_*_*',
        'drop *_subJetTable_*_*',
        'drop *_tkMetTable_*_*',
        'drop *_puppiMetTable_*_*',
        'drop *_ttbarCategoryTable_*_*',
        
        'drop *_photonTable_*_*',
        'drop *_photonMCTable_*_*',
        
        'drop *_tauTable_*_*',
        'drop *_tauMCTable_*_*' ,
        
        'drop *_saJetTable_*_*',
        'drop *_saTable_*_*',
        
        'drop *_simpleCleanerTable_photons_*',
        'drop *_simpleCleanerTable_taus_*',
        
        'drop *_rivetMetTable_*_*',
        'drop *_rivetLeptonTable_*_*',
        'drop *_rivetProducerHTXS_*_*',
    )
)

process.MINIAODoutput = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('output.root'),
    outputCommands = cms.untracked.vstring(
        "keep *",
        
     ),#+process.NANOAODSIMoutput.outputCommands,
    dropMetaData = cms.untracked.string('ALL'),
)


#process.load("PhysicsTools.PatAlgos.patSequences_cff")

# load the coreTools of PAT
from PhysicsTools.PatAlgos.tools.jetTools import *

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

if options.isData:
    if options.year == 2016:
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_v11', '')
    if options.year == 2017:
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_v11', '')
    if options.year == 2018:
        # Add 102X_dataRun2_Prompt_v14 for 2018D!
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_dataRun2_v11', '')
    jetCorrectionsAK4PFchs = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute','L2L3Residual'], 'None')
    jetCorrectionsAK4PF = ('AK4PF', ['L1FastJet', 'L2Relative', 'L3Absolute','L2L3Residual'], 'None')
else:
    if options.year == 2016:
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_mcRun2_asymptotic_v7', '')
    if options.year == 2017:
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_mc2017_realistic_v7', '')
    if options.year == 2018:
        process.GlobalTag = GlobalTag(process.GlobalTag, '102X_upgrade2018_realistic_v19', '')
    jetCorrectionsAK4PFchs = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None')
    jetCorrectionsAK4PF = ('AK4PF', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None')
   
bTagDiscriminators = [
    'pfDeepFlavourJetTags:probb',
    'pfDeepFlavourJetTags:probbb',
    'pfDeepFlavourJetTags:problepb',
    'pfDeepFlavourJetTags:probc',
    'pfDeepFlavourJetTags:probuds',
    'pfDeepFlavourJetTags:probg'
]

updateJetCollection(
        process,
        labelName = "XTag",
        jetSource = cms.InputTag('slimmedJets'),#'ak4Jets'
        jetCorrections = jetCorrectionsAK4PFchs,
        pfCandidates = cms.InputTag('packedPFCandidates'),
        pvSource = cms.InputTag("offlineSlimmedPrimaryVertices"),
        svSource = cms.InputTag('slimmedSecondaryVertices'),
        muSource = cms.InputTag('slimmedMuons'),
        elSource = cms.InputTag('slimmedElectrons'),
        btagDiscriminators = bTagDiscriminators,
        #btagInfos = bTagInfos,
        
)

#process.updatedPatJetsTransientCorrectedXTag.addDeepBostedJet = cms.bool(False)
#process.updatedPatJetsTransientCorrectedXTag.addDeepDoubleX = cms.bool(False)


process.updateJetXTagSequence = cms.Sequence(
    process.patJetCorrFactorsXTag
    +process.updatedPatJetsXTag
    #+process.updatedPatJetsWithDeepInfoXTag
    +process.pfImpactParameterTagInfosXTag
    +process.pfInclusiveSecondaryVertexFinderTagInfosXTag
    +process.patJetCorrFactorsTransientCorrectedXTag
    +process.pfDeepCSVTagInfosXTag
    +process.pfDeepFlavourTagInfosXTag
    +process.pfDeepFlavourJetTagsXTag
    +process.updatedPatJetsTransientCorrectedXTag
)

addModule(process.updateJetXTagSequence)

process.selectJetsInBarrel = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("updatedPatJetsTransientCorrectedXTag"),
    cut = cms.string("pt > 20")
)
addModule(process.selectJetsInBarrel)

process.nanoxProducer = cms.EDProducer("NANOXProducer",
    plugins = cms.PSet(
        globalVars = cms.PSet(
            type = cms.string("GlobalJetTagData"),
            #src = cms.InputTag("linkedObjects","jets")
            jets = cms.InputTag("selectJetsInBarrel"),
        ),
        csv = cms.PSet(
            type = cms.string("CSVInputTagData"),
            jets = cms.InputTag("selectJetsInBarrel"),
            tagName = cms.string('pfDeepCSV')
        ),
        cpf = cms.PSet(
            type = cms.string("ChargedPFTagData"),
            jets = cms.InputTag("selectJetsInBarrel"),
            pvVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
            svVertices = cms.InputTag("slimmedSecondaryVertices"),
        ),
        npf = cms.PSet(
            type = cms.string("NeutralPFTagData"),
            jets = cms.InputTag("selectJetsInBarrel"),
            svVertices = cms.InputTag("slimmedSecondaryVertices"),
        ),
        sv = cms.PSet(
            type = cms.string("SVTagData"),
            jets = cms.InputTag("selectJetsInBarrel"),
            pvVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
            svVertices = cms.InputTag("slimmedSecondaryVertices"),
        ),
        legacyTag = cms.PSet(
            type = cms.string("LegacyTagData"),
            jets = cms.InputTag("selectJetsInBarrel"),
            pvVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
        ),
    )
)

process.nanoxFlatTable = cms.EDProducer("NANOXFlatTableProducer",
    tagData = cms.VPSet([
        cms.PSet(
            src = cms.InputTag("nanoxProducer","globalVars"),
            arrayNames = cms.vstring(["global"])
        ),
        cms.PSet(
            src = cms.InputTag("nanoxProducer","csv"),
            arrayNames = cms.vstring(["csv"])
        ),
        cms.PSet(
            src = cms.InputTag("nanoxProducer","cpf"),
            arrayNames = cms.vstring(["cpflength","cpf"])
        ),
        cms.PSet(
            src = cms.InputTag("nanoxProducer","npf"),
            arrayNames = cms.vstring(["npflength","npf"])
        ),
        cms.PSet(
            src = cms.InputTag("nanoxProducer","sv"),
            arrayNames = cms.vstring(["svlength","sv"])
        ),
        cms.PSet(
            src = cms.InputTag("nanoxProducer","legacyTag"),
            arrayNames = cms.vstring(["legacyTag"])
        ),
        
    ])
)

if not options.isData:
    process.nanoxProducer.plugins.origin = cms.PSet(
        type = cms.string("JetOriginTagData"),
        jets = cms.InputTag("selectJetsInBarrel"),
        displacedGenVertices = cms.InputTag("displacedGenVertices"),
    )
    
    process.nanoxFlatTable.tagData.append(cms.PSet(
        src = cms.InputTag("nanoxProducer","origin"),
        arrayNames = cms.vstring(["jetorigin"])
    ))
    
    if options.addLLPInfo:
        process.nanoxProducer.plugins.llpinfo = cms.PSet(
            type = cms.string("LLPInfo"),
            displacedGenVertices = cms.InputTag("displacedGenVertices"),
            LLPtype = cms.string(options.LLPtype)
        )
process.nanoxFlatTable.tagData.append(cms.PSet( src = cms.InputTag("nanoxProducer","llpinfo"),
            arrayNames = cms.vstring(["llpinfo"])
        ))
    
#process.eventView = cms.EDAnalyzer(
#    "EventViewer",
#    genParticles = cms.InputTag("genParticlesMerged"),
#    genJets = cms.InputTag("genJetsReclustered"),
#    selection = cms.string('(abs(pdgId)<6) && (abs(mother.pdgId)==1000021)')
#)

#remove unneeded modules
for moduleName in [
    "chsForSATkJets",
    "softActivityJets",
    "softActivityJets2",
    "softActivityJets5",
    "softActivityJets10",
    
    "fatJetTable",
    "subJetTable",
    "saJetTable",
    "saTable",
    
    "genJetAK8Table",
    "genJetAK8FlavourAssociation",
    "genJetAK8FlavourTable",
   
    "particleLevel",
    "rivetLeptonTable",
    "HTXSCategoryTable",
    "rivetMetTable",
    "rivetLeptonTable",
    "rivetProducerHTXS",
    "tautagger",
    "patTauDiscriminationByElectronRejectionMVA62018Raw",
    "patTauDiscriminationByIsolationMVArun2v1DBnewDMwLTraw",
    "patTauDiscriminationByIsolationMVArun2v1DBoldDMdR0p3wLTraw",
    "patTauDiscriminationByIsolationMVArun2v1DBoldDMwLTraw",
    "patTauDiscriminationByIsolationMVArun2v1DBoldDMwLTraw2015",
    "patTauDiscriminationByLooseElectronRejectionMVA62018",
    "patTauDiscriminationByLooseIsolationMVArun2v1DBnewDMwLT",
    "patTauDiscriminationByLooseIsolationMVArun2v1DBoldDMdR0p3wLT",
    "patTauDiscriminationByLooseIsolationMVArun2v1DBoldDMwLT",
    "patTauDiscriminationByLooseIsolationMVArun2v1DBoldDMwLT2015",
    "patTauDiscriminationByMediumElectronRejectionMVA62018",
    "patTauDiscriminationByMediumIsolationMVArun2v1DBnewDMwLT",
    "patTauDiscriminationByMediumIsolationMVArun2v1DBoldDMdR0p3wLT",
    "patTauDiscriminationByMediumIsolationMVArun2v1DBoldDMwLT",
    "patTauDiscriminationByMediumIsolationMVArun2v1DBoldDMwLT2015",
    "patTauDiscriminationByTightElectronRejectionMVA62018",
    "patTauDiscriminationByTightIsolationMVArun2v1DBnewDMwLT",
    "patTauDiscriminationByTightIsolationMVArun2v1DBoldDMdR0p3wLT",
    "patTauDiscriminationByTightIsolationMVArun2v1DBoldDMwLT",
    "patTauDiscriminationByTightIsolationMVArun2v1DBoldDMwLT2015",
    "patTauDiscriminationByVLooseElectronRejectionMVA62018",
    "patTauDiscriminationByVLooseIsolationMVArun2v1DBnewDMwLT",
    "patTauDiscriminationByVLooseIsolationMVArun2v1DBoldDMdR0p3wLT",
    "patTauDiscriminationByVLooseIsolationMVArun2v1DBoldDMwLT",
    "patTauDiscriminationByVLooseIsolationMVArun2v1DBoldDMwLT2015",
    "patTauDiscriminationByVTightElectronRejectionMVA62018",
    "patTauDiscriminationByVTightIsolationMVArun2v1DBnewDMwLT",
    "patTauDiscriminationByVTightIsolationMVArun2v1DBoldDMdR0p3wLT",
    "patTauDiscriminationByVTightIsolationMVArun2v1DBoldDMwLT",
    "patTauDiscriminationByVTightIsolationMVArun2v1DBoldDMwLT2015",
    "patTauDiscriminationByVVLooseIsolationMVArun2v1DBnewDMwLT",
    "patTauDiscriminationByVVLooseIsolationMVArun2v1DBoldDMdR0p3wLT",
    "patTauDiscriminationByVVLooseIsolationMVArun2v1DBoldDMwLT",
    "patTauDiscriminationByVVTightIsolationMVArun2v1DBnewDMwLT",
    "patTauDiscriminationByVVTightIsolationMVArun2v1DBoldDMdR0p3wLT",
    "patTauDiscriminationByVVTightIsolationMVArun2v1DBoldDMwLT",
    "patTauDiscriminationByVVTightIsolationMVArun2v1DBoldDMwLT2015",
]:
    if hasattr(process,moduleName):
        print "removing module: ",moduleName
        if options.isData:
            process.nanoSequence.remove(getattr(process,moduleName))
        else:
            process.nanoSequenceMC.remove(getattr(process,moduleName))
        
if options.isData:
    addModule(process.nanoSequence)
else:
    addModule(process.nanoSequenceMC)

if not options.isData:
    process.load('NANOX.DisplacedVertex.GenDisplacedVertices_cff')
    addModule(process.DisplacedGenVertexSequence)
    
from PhysicsTools.NanoAOD.common_cff import *
    
#process.jetTable.variables.myVariable = Var("chargedMultiplicity()", int, doc="n(charged)")

process.jetTable.variables.CHM = Var("chargedHadronMultiplicity()", int, doc="chargedHadronMultiplicity ")
#process.jetTable.variables.NumNeutralParticle = Var("numberofDaughters() - chargedHadronMultiplicity()", int, doc = "Number of neutral constituents?")

process.nanoxSequence = cms.Sequence( 
    process.nanoxProducer
    +process.nanoxFlatTable
    #+process.eventView
)
addModule(process.nanoxSequence)

process.endjob_step = cms.EndPath(process.endOfProcess)
process.NANOAODSIMoutput_step = cms.EndPath(
    process.NANOAODSIMoutput
)

# Schedule definition
#process.schedule = cms.Schedule(process.nanoAOD_step,process.endjob_step,process.NANOAODSIMoutput_step)
#from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
#associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from PhysicsTools.NanoAOD.nano_cff
from PhysicsTools.NanoAOD.nano_cff import nanoAOD_customizeData,nanoAOD_customizeMC 

#call to customisation function nanoAOD_customizeMC imported from PhysicsTools.NanoAOD.nano_cff
if options.isData:
    process = nanoAOD_customizeData(process)
else:
    process = nanoAOD_customizeMC(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
