from WMCore.Configuration import Configuration
import datetime,sys,os
import copy
import math
import urllib, json
from CRABClient.UserUtilities import getUsernameFromSiteDB, getLumiListInValidFiles


url = "https://raw.githubusercontent.com/LLPDNNX/misc/master/miniAOD_102X_Run2.json"
response = urllib.urlopen(url)
myJobs = json.loads(response.read())

requestName = "NANOX_09Oct19"
userName = getUsernameFromSiteDB() 
configTmpl = Configuration()

configTmpl.section_('General')
configTmpl.General.transferOutputs = True
configTmpl.General.transferLogs = False

configTmpl.section_('JobType')
configTmpl.JobType.psetName = "NANOX/LLPSpecific/test/produce_102X_NANOX.py"
configTmpl.JobType.pluginName = 'Analysis'
configTmpl.JobType.outputFiles = ['nano.root']
configTmpl.JobType.maxJobRuntimeMin= 24*60
configTmpl.JobType.pyCfgParams = []
configTmpl.JobType.inputFiles = []
configTmpl.JobType.allowUndistributedCMSSW = True
configTmpl.JobType.maxMemoryMB = 3000
configTmpl.section_('Data')
configTmpl.Data.inputDataset = ""
configTmpl.Data.publication = True
configTmpl.Data.useParent = False
configTmpl.section_('Site')
configTmpl.Site.storageSite = 'T2_UK_London_IC'

print(__name__)

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    from httplib import HTTPException
    from multiprocessing import Process

    def submit(config):
        try:
            crabCommand('submit',  config = config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################


    for i,jobName in enumerate(sorted(myJobs.keys())):

        isData = False
        myJob = myJobs[jobName]
        i=i+1
        config = copy.deepcopy(configTmpl)
        config.General.requestName = jobName+"_"+requestName
        config.General.workArea = "crab/"+requestName+"/"+jobName
        config.Data.outLFNDirBase = "/store/user/"+userName+"/LLP/"+requestName+"/"+jobName
        config.Data.inputDataset = myJob["inputDataset"]
        config.Data.outputDatasetTag = jobName
            
        isData = myJob.get('isData', False)
        year = int(myJob.get('year', 0))
        config.Data.inputDBS = myJob.get('inputDBS', 'global')

        print "year:", year
        config.JobType.pyCfgParams.append("year="+str(year))

        if isData:
            config.JobType.pyCfgParams.append("isData=True")
            config.Data.splitting = 'LumiBased'
            config.Data.unitsPerJob = myJob.get('unitsPerJob', 50)
            if year == 2016:
                config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification//Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
            if year == 2017:
                config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt'
            if year == 2018 or year == 2019:
                config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt' 
        else:
            config.JobType.pyCfgParams.append("isData=False")
            config.Data.splitting = 'FileBased'
            config.Data.unitsPerJob = myJob.get('unitsPerJob', 3)

        if "params" in myJob:
            params = myJob["params"]

            for param in params:
                config.JobType.pyCfgParams.append(str(param))

        print config,
        
        if not os.path.exists(configTmpl.JobType.psetName):
            print "\nConfiguration file ", pSet, "does not exist.  Aborting..."
            sys.exit(1)
        
        print "Submitting job ",i," of ",len(myJobs.keys()),":",config.General.workArea
        
        if os.path.isdir(os.path.join(os.getcwd(),config.General.workArea)):
            print "Output directory ",os.path.join(os.getcwd(),config.General.workArea)," exists -> skipping"
            print
            continue

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()
        print
        print
