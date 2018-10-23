from WMCore.Configuration import Configuration
import datetime,sys,os
import copy
import math

requestName = "NANOX_181022"
userName = "vcepaiti"

myJobs = {

    # === Topping Up the Grid ===

    "SMS-T1qqqq_ctau-0p001_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-0p001_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],


    "SMS-T1qqqq_ctau-0p01_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-0p01_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],


    "SMS-T1qqqq_ctau-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-0p1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],


    "SMS-T1qqqq_ctau-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-1_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],


    "SMS-T1qqqq_ctau-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],


    "SMS-T1qqqq_ctau-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],


    "SMS-T1qqqq_ctau-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],


    "SMS-T1qqqq_ctau-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-10000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],


    "SMS-T1qqqq_ctau-100000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/SMS-T1qqqq_ctau-100000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/mkomm-LLP_miniaodv180920-312db0fc9b855c6adca5525185ce0591/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 5),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],

    # === Private Thomas' ===

    "T1qqqqLL_ctau-0p001_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-0p001_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-0p001_Madgraph_Pythia_MINIAODSIM_18_01_11-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    
    "T1qqqqLL_ctau-0p01_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-0p01_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-0p01_Madgraph_Pythia_MINIAODSIM_18_01_12-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    
    "T1qqqqLL_ctau-0p1_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-0p1_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-0p1_Madgraph_Pythia_MINIAODSIM_18_01_12-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    
    "T1qqqqLL_ctau-1_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-1_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-1_Madgraph_Pythia_MINIAODSIM_18_01_11-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    
    
    "T1qqqqLL_ctau-10_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-10_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-10_Madgraph_Pythia_MINIAODSIM_18_01_12-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    
    
    "T1qqqqLL_ctau-100_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-100_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-100_Madgraph_Pythia_MINIAODSIM_18_01_12-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    
    
    "T1qqqqLL_ctau-1000_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-1000_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-1000_Madgraph_Pythia_MINIAODSIM_17_12_23-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    
    
    "T1qqqqLL_ctau-10000_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-10000_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-10000_Madgraph_Pythia_MINIAODSIM_18_01_12-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    
    
    "T1qqqqLL_ctau-100000_Madgraph_Pythia_GEN-SIM":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/T1qqqqLL_ctau-100000_Madgraph_Pythia_GEN-SIM/tstreble-T1qqqqLL_ctau-100000_Madgraph_Pythia_MINIAODSIM_17_12_29-28028af67189b3de7224b79195bd0e1d/USER"),
        lambda cfg: setattr(cfg.Data,"unitsPerJob", 100),
        lambda cfg: setattr(cfg.Data,"inputDBS", 'phys03'),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=T1qqqqLL")
    ],
    

    # === Gluino decay to gravitino (GMSB model) === 

    "gluinoGMSB_M2000_ctau10000p0_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/gluinoGMSB_M2000_ctau10000p0_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=GMSB")
    ],

    "gluinoGMSB_M2000_ctau1000p0_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/gluinoGMSB_M2000_ctau1000p0_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=GMSB")
    ],

    "gluinoGMSB_M2000_ctau100p0_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/gluinoGMSB_M2000_ctau100p0_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=GMSB")
    ],

    "gluinoGMSB_M2000_ctau10p0_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/gluinoGMSB_M2000_ctau10p0_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=GMSB")
    ],

    "gluinoGMSB_M2000_ctau1p0_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/gluinoGMSB_M2000_ctau1p0_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=GMSB")
    ],

    "gluinoGMSB_M2000_ctau300p0_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/gluinoGMSB_M2000_ctau300p0_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=GMSB")
    ],

    ## === RPV SUSY STOP TO B L === #

    "DisplacedSUSY_StopToBL_M-1000_CTau-1_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/DisplacedSUSY_StopToBL_M-1000_CTau-1_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=RPV")
    ],


    "DisplacedSUSY_StopToBL_M-1000_CTau-10_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/DisplacedSUSY_StopToBL_M-1000_CTau-10_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=RPV")
    ],


    "DisplacedSUSY_StopToBL_M-1000_CTau-100_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/DisplacedSUSY_StopToBL_M-1000_CTau-100_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=RPV")
    ],


    "DisplacedSUSY_StopToBL_M-1000_CTau-1000_TuneCUETP8M1_13TeV_pythia8":[
        lambda cfg: setattr(cfg.Data,"inputDataset","/DisplacedSUSY_StopToBL_M-1000_CTau-1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM"),
        lambda cfg: cfg.JobType.pyCfgParams.append("addLLPInfo=True"),
        lambda cfg: cfg.JobType.pyCfgParams.append("LLPtype=RPV")
    ],
}
  
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

configTmpl.Data.splitting = 'FileBased'
configTmpl.Data.unitsPerJob = 6

configTmpl.section_('Site')
configTmpl.Site.storageSite = 'T2_UK_London_IC'
#configTmpl.Site.blacklist = ["T2_US_UCSD","T2_IN_TIFR"]

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
        
