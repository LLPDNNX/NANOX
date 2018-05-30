function run_test()
{
    touch /var/lib/rpm/* || return 1
    yum -y install wget yum-plugin-ovl || return 1
    yum -y install glibc-devel.x86_64 --disablerepo=adobe* || return 1
    cd ~
    source ~/.bashrc
    export SCRAM_ARCH=slc6_amd64_gcc630 || return 1
    scramv1 project CMSSW CMSSW_9_4_8 || return 1
    cd CMSSW_9_4_8/src || return 1
    eval `scramv1 runtime -sh` || return 1
    git config --global user.email "root.rooster@chicken.com"
    git config --global user.name "Root rooster"
    git cms-merge-topic cms-nanoAOD:master || return 1
    git clone https://github.com/LLPDNNX/NANOX.git || return 1
    scram b || return 1
    
    #requires siteconfig for GT
    wget https://github.com/LLPDNNX/test-files/raw/master/miniaod/RunIISummer16MiniAODv2_MC.root || return 1
    echo "export CMS_LOCAL_SITE=T2_UK_London_IC" > /etc/cvmfs/config.d/cms.cern.ch.conf
    service autofs restart 
    cmsRun NANOX/LLPSpecific/test/produce_80X_NANOX.py inputFiles=file:RunIISummer16MiniAODv2_MC.root || return 1
}

run_test
