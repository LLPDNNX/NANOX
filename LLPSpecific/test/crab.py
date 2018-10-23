from WMCore.Configuration import Configuration
import datetime,sys,os
import copy
import math

requestName = "NANOX_180727"
isData = False
userName = "vcepaiti"

if isData==False:
    myJobs = {
        "SMS-T1qqqq_ctau-0p001_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-0p001_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],

        "SMS-T1qqqq_ctau-0p01_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-0p01_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],
        "SMS-T1qqqq_ctau-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],
        "SMS-T1qqqq_ctau-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],
        "SMS-T1qqqq_ctau-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],
        "SMS-T1qqqq_ctau-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],
        "SMS-T1qqqq_ctau-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],
        "SMS-T1qqqq_ctau-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],
        "SMS-T1qqqq_ctau-100000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-100000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_GridpackScan_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2))),
            lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True")
        ],

        # === ZJetsToNuNu === 
        "ZJetsToNuNu_HT-100To200_13TeV-madgraph":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "ZJetsToNuNu_HT-200To400_13TeV-madgraph":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "ZJetsToNuNu_HT-400To600_13TeV-madgraph":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "ZJetsToNuNu_HT-600To800_13TeV-madgraph":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ZJetsToNuNu_HT-600To800_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "ZJetsToNuNu_HT-800to1200_13TeV-madgraph":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ZJetsToNuNu_HT-800To1200_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "ZJetsToNuNu_HT-1200To2500_13TeV-madgraph":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],
 
        # === WJetsToLNu === 
        "WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WJetsToLNu_HT-70To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],
 
        "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],
 
        "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],
 
        "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],
 
        "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],
 
        "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],
 
        "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        
        # === GJets === 
 
        "GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],
 
        "GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

        "GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
         ],

         
        # === DYJetsToLL === 

        "DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
         
        "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
 
        "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
 
        "DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
  
        "DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],


        # === TTJets === 

        "TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM"),
        ],

        "TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],

        "TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],

        "TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],

        "TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],

        "TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],

        "TTJets_SingleLeptFromT_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_SingleLeptFromT_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
        
        "TTJets_SingleLeptFromTbar_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TTJets_SingleLeptFromTbar_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],


        # === QCD ===
  
        "QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
 
        "QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
 
        "QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
 
        "QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

 
        # === SingleTop ====
        "ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
        ],
        "ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
        ],
        "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        # === TTX === #

        # === DiBoson == #
        
        "WW_TuneCUETP8M1_13TeV-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WW_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "WZ_TuneCUETP8M1_13TeV-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "ZZ_TuneCUETP8M1_13TeV-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
    }

        # === EWK V+jets === #
    '''
        "EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/EWKWMinus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/EWKWPlus2Jets_WToLNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "EWKZ2Jets_ZToLL_M-50_13TeV-madgraph-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/EWKZ2Jets_ZToLL_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],

        "EWKZ2Jets_ZToNuNu_M-50_13TeV-madgraph-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/EWKZ2Jets_ZToNuNu_M-50_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],


        # === LEGACY === #
        "TT_TuneCUETP8M2T4_13TeV-powheg-pythia8-evtgen":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8-evtgen/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
        ],
        
        
        
        "QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
        "QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
        "QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM"),
        ],
        "QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
        "QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
        ],
        "QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        ],
        "QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/MINIAODSIM"),
        ],
        "WToLNu_0J_13TeV-amcatnloFXFX-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WToLNu_0J_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
        ],
        "WToLNu_1J_13TeV-amcatnloFXFX-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WToLNu_1J_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
        ],
        "WToLNu_2J_13TeV-amcatnloFXFX-pythia8":[
            lambda cfg: setattr(cfg.Data,"inputDataset","/WToLNu_2J_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM"),
            lambda cfg: setattr(cfg.Data,"unitsPerJob",int(math.ceil(cfg.Data.unitsPerJob/3.*2)))
        ],
   ''' 
       
else:

    myJobs = {}
    
    for dataset in [

        '/HTMHT/Run2016B-03Feb2017_ver2-v2/MINIAOD',
        '/HTMHT/Run2016C-03Feb2017-v1/MINIAOD',
        '/HTMHT/Run2016D-03Feb2017-v1/MINIAOD',
        '/HTMHT/Run2016E-03Feb2017-v1/MINIAOD',
        '/HTMHT/Run2016F-03Feb2017-v1/MINIAOD',
        '/HTMHT/Run2016G-03Feb2017-v1/MINIAOD',
        '/HTMHT/Run2016H-03Feb2017_ver2-v1/MINIAOD',
        '/HTMHT/Run2016H-03Feb2017_ver3-v1/MINIAOD',

        '/SingleElectron/Run2016B-03Feb2017_ver2-v2/MINIAOD',
        '/SingleElectron/Run2016C-03Feb2017-v1/MINIAOD',
        '/SingleElectron/Run2016D-03Feb2017-v1/MINIAOD',
        '/SingleElectron/Run2016E-03Feb2017-v1/MINIAOD',
        '/SingleElectron/Run2016F-03Feb2017-v1/MINIAOD',
        '/SingleElectron/Run2016G-03Feb2017-v1/MINIAOD',
        '/SingleElectron/Run2016H-03Feb2017_ver2-v1/MINIAOD',
        '/SingleElectron/Run2016H-03Feb2017_ver3-v1/MINIAOD',

        '/SingleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD',
        '/SingleMuon/Run2016C-03Feb2017-v1/MINIAOD',
        '/SingleMuon/Run2016D-03Feb2017-v1/MINIAOD',
        '/SingleMuon/Run2016E-03Feb2017-v1/MINIAOD',
        '/SingleMuon/Run2016F-03Feb2017-v1/MINIAOD',
        '/SingleMuon/Run2016G-03Feb2017-v1/MINIAOD',
        '/SingleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD',
        '/SingleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD',

        '/MET/Run2016B-03Feb2017_ver2-v2/MINIAOD',
        '/MET/Run2016C-03Feb2017-v1/MINIAOD',
        '/MET/Run2016D-03Feb2017-v1/MINIAOD',
        '/MET/Run2016E-03Feb2017-v1/MINIAOD',
        '/MET/Run2016F-03Feb2017-v1/MINIAOD',
        '/MET/Run2016G-03Feb2017-v1/MINIAOD',
        '/MET/Run2016H-03Feb2017_ver2-v1/MINIAOD',
        '/MET/Run2016H-03Feb2017_ver3-v1/MINIAOD',
        

        #'/SingleElectron/Run2016B-03Feb2017_ver2-v2/MINIAOD',
        #'/SingleElectron/Run2016C-03Feb2017-v1/MINIAOD',
        #'/SingleElectron/Run2016D-03Feb2017-v1/MINIAOD',
        #'/SingleElectron/Run2016E-03Feb2017-v1/MINIAOD',
        #'/SingleElectron/Run2016F-03Feb2017-v1/MINIAOD',
        #'/SingleElectron/Run2016G-03Feb2017-v1/MINIAOD',
        #'/SingleElectron/Run2016H-03Feb2017_ver2-v1/MINIAOD',
        #'/SingleElectron/Run2016H-03Feb2017_ver3-v1/MINIAOD'
    ]:
        datasetSplit = dataset.split('/')
        datasetName = datasetSplit[1]+"_"+datasetSplit[2]
        #NOTE: need to bind by value, not by reference
        myJobs[datasetName] = [
            lambda cfg,dataset=dataset: setattr(cfg.Data,"inputDataset", dataset),
            lambda cfg,datasetSplit=datasetSplit: setattr(cfg.Data,"outputDatasetTag",datasetSplit[2]+"_"+cfg.Data.outputDatasetTag),
            lambda cfg,datasetSplit=datasetSplit: cfg.JobType.pyCfgParams.append("isData=True")
        ]


configTmpl = Configuration()

configTmpl.section_('General')
configTmpl.General.transferOutputs = True
configTmpl.General.transferLogs = False

configTmpl.section_('JobType')
configTmpl.JobType.psetName = "NANOX/LLPSpecific/test/produce_80X_NANOX.py"
configTmpl.JobType.pluginName = 'Analysis'
configTmpl.JobType.outputFiles = ['nano.root']
configTmpl.JobType.maxJobRuntimeMin=40*60
configTmpl.JobType.pyCfgParams = []
#configTmpl.JobType.inputFiles = ['QGL_cmssw8020_v2.db']
configTmpl.JobType.allowUndistributedCMSSW = True 
configTmpl.JobType.maxMemoryMB = 4000

configTmpl.section_('Data')
configTmpl.Data.inputDataset = ""
#configTmpl.Data.ignoreLocality = True
configTmpl.Data.inputDBS = 'global'
#configTmpl.Data.allowNonValidInputDataset = True
configTmpl.Data.publication = True
configTmpl.Data.useParent = False

if isData:
    configTmpl.Data.splitting = 'LumiBased'
    configTmpl.Data.unitsPerJob = 75
    #configTmpl.Data.runRange = '306091,306092,306095' #fill: 6358, maxpu: 78
    configTmpl.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification//Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
    #configTmpl.Data.lumiMask = 'lumiMask.json'
else:
    configTmpl.Data.splitting = 'FileBased'
    configTmpl.Data.unitsPerJob = 6

configTmpl.section_('Site')
configTmpl.Site.storageSite = 'T2_UK_London_IC'
#configTmpl.Site.whitelist = ["T2_CH*"]
configTmpl.Site.blacklist = ["T2_US_UCSD","T2_IN_TIFR"]

print(__name__)

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    from httplib import HTTPException
    from multiprocessing import Process

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################


    for i,jobName in enumerate(sorted(myJobs.keys())):

        jobModifiers=myJobs[jobName]
        i=i+1
        config = copy.deepcopy(configTmpl)
        config.General.requestName = requestName
        config.General.workArea = "crab/"+config.General.requestName+"/"+jobName
        config.Data.outLFNDirBase = "/store/user/"+userName+"/LLP/"+config.General.requestName+"/"+jobName
        config.Data.outputDatasetTag = config.General.requestName
        
        for modifier in jobModifiers:
            modifier(config)
        print config
            
        if not os.path.exists(configTmpl.JobType.psetName):
            print "\nConfiguration file ", pSet, "does not exist.  Aborting..."
            sys.exit(1)
        
        print "Submitting job ",i," of ",len(myJobs.keys()),":",config.General.workArea
        
        if os.path.isdir(os.path.join(os.getcwd(),config.General.workArea)): 
            print "Output directory ",os.path.join(os.getcwd(),config.General.workArea)," exists -> skipping"
            continue
        print config
        
        p = Process(target=submit, args=(config,))
        p.start()
        p.join()
        
