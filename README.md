# NANOX
CMSSW package for producing NANOaod eXtended (NANOX) files containing additional jet data required for training and evaluation.

![build tests](https://travis-ci.org/LLPDNNX/NANOX.svg?branch=master)

## Setup
Requires CMSSW_10_2_18
```
cmsrel CMSSW_10_2_18
cd CMSSW_10_2_18/src
cmsenv
git cms-merge-topic cms-nanoAOD:master-102X
git checkout -b nanoAOD cms-nanoAOD/master-102X
git clone git@github.com:llpdnnx/nanox.git
scram b
```

## Example configurations
* generic MC: `cmsRun NANOX/NANOXProducer/test/test102X_NANO.py`
* generic data: `cmsRun NANOX/NANOXProducer/test/test102X_NANO_data.py`
* for LLP analysis: `cmsRun NANOX/LLPSpecific/test/produce_102X_NANOX.py`

## Short description

* **DataFormats**: Common data containers
* **DisplacedVertex**: Groups particles sharing the same displacement together  
* **GhostTagging**: Alternative tagging algorithm using gluino daughters (deprecated)
* **Jet**: Plugins & data containers to generate and store additional jet variables for training
* **LLPSpecific**: Gathers and stores additional information about simulated gluinos in events
* **NANOXProducer**: Convertes data containters into nanoaod flat tables
* **Utils**: Helpful scripts/plugins
