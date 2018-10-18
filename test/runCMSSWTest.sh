function cms-merge-commit()
{
    local github_user="${1}"
    local topic_branch="${2}"
    local commit_hash="${3}"
    echo "-----> Merging commit ${commit_hash} on branch ${topic_branch} from ${github_user}"

    git remote add -t "${topic_branch}" "${github_user}" "https://github.com/${github_user}/cmssw.git"
    git fetch --no-tags "${github_user}" "${topic_branch}:refs/remotes/${github_user}/${topic_branch}" || { echo "Could not fetch branch ${topic_branch} from ${github_user}"; return 1; }
    local current_branch="$(git rev-parse --abbrev-ref HEAD)"
    local merge_base="$(git merge-base ${commit_hash} ${current_branch})"
    git cms-sparse-checkout "${merge_base}" "${commit_hash}" || return 1
    git read-tree -mu HEAD || return 1
    git merge --no-ff -m "Merged ${commit_hash} on branch ${topic_branch} from ${github_user} into ${current_branch}" "${commit_hash}" || { echo "Could not merge ${commit_hash} on branch ${topic_branch} from ${github_user} into ${current_branch}"; return 1; }
}

function run_test()
{
    #check siteconfig for GT
    ls -l /cvmfs/cms.cern.ch/SITECONF | grep local
    
    touch /var/lib/rpm/* || return 1
    yum -y install wget rsync yum-plugin-ovl || return 1
    yum -y install glibc-devel.x86_64 --disablerepo=adobe* || return 1
    cd ~
    source ~/.bashrc
    export SCRAM_ARCH=slc6_amd64_gcc630 || return 1
    scramv1 project CMSSW CMSSW_9_4_10 || return 1
    cd CMSSW_9_4_10/src || return 1
    eval `scramv1 runtime -sh` || return 1
    git config --global user.email "root.rooster@chicken.com"
    git config --global user.name "Root rooster"
    git cms-init --upstream-only
    cms-merge-commit cms-nanoAOD master 7c6bce65ee16a01c0d991fdbf40c43dbcfa6202a || return 1
    git cms-checkdeps -a || return 1
    mkdir NANOX
    rsync -r --stats /scripts/ NANOX/. || return 1
    scram b || return 1
    
    wget -nv https://github.com/LLPDNNX/test-files/raw/master/miniaod/RunIISummer16MiniAODv2_MC.root || return 1
    cmsRun NANOX/LLPSpecific/test/produce_80X_NANOX.py inputFiles=file:RunIISummer16MiniAODv2_MC.root || return 1
}

run_test
