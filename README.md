# NANOX
CMSSW package for producing NANOaod eXtended (NANOX) files containing additional jet data required for training and evaluation.

![build tests](https://travis-ci.org/LLPDNNX/NANOX.svg?branch=master)

## Setup
Requires CMSSW_9_X, e.g.
```
cmsrel CMSSW_9_4_2
cd CMSSW_9_4_2/src
cmsenv
git clone git@github.com:LLPDNNX/NANOX.git
scram b
```

## Example configurations
* generic MC: `cmsRun NANOX/NANOXProducer/test/test80X_NANO.py`
* generic data: `cmsRun NANOX/NANOXProducer/test/test80X_NANO_data.py`
* for LLP analysis: `cmsRun NANOX/LLPSpecific/test/produce_80X_NANOX.py` (see avaiable options in file)

## Short description

* **DataFormats**: Common data containers
* **DisplacedVertex**: Groups particles sharing the same displacement together  
* **GhostTagging**: Alternative tagging algorithm using gluino daughters (not recommended)
* **Jet**: Plugins & data containers to generate and store additional jet variables for training
* **LLPSpecific**: Gathers and stores additional information about simulated gluinos in events
* **NANOXProducer**: Convertes data containters into nanoaod flat tables
* **Utils**: Helpful scripts/plugins
