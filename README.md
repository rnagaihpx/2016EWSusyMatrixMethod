# xAOD_walkthrough
Analysis framework based on Base,2.3.23 with SUSYTools-00-06-22.

## Contents of the package ----
./MyAnalysis : analysis package for EventLoop.  
./lsfoutput  : empty directory to store LSF outputs (only for login.icepp.jp).  
./result     : empty directory to store results of each EventLoop process will be in this directory.  
./share      : contains misc. files (e.g. install.sh, GRL.xml, XSDB.txt).  
./analysis/  : analysis codes  
  --mkBasicPlots : ROOT script to make distributions of Data/MC.  

## Usage -----
kinit [your account]@CERN.CH #not necessary but recommended  
./share/install.sh #setting up RootCore and installing all necessary packages  

## Tips / example -----
$git clone git://github.com/hrsmnr/xAOD_walkthrough.git [destination]  
$git clone git://github.com/hrsmnr/MMTool.git  
$git remote add origin https://hrsmnr@github.com/hrsmnr/xAOD_walkthrough.git  
$git log --branches --not --remotes  
$source rcSetup.sh  
$rc find_packages  
$rc compile  
$testRun -D test -n 10  
$source share/runOnLocal.sh [Dataset directory] [Signal selection]  
$source share/runOnLSF.sh [Dataset directory] [Signal selection]  
$python share/mkFileList.py [optionally: target directory]  
$source share/runOnLSFSplitDS.sh [Dataset directory] [Signal selection]  
$source share/runOnLSFHybridInput.sh [Dataset directory] [Signal selection]  
$source share/mergeRootFiles.sh [tag(e.g. h0001)]  
$root -q -b -n 'mkPlots.C+O("h0015","3S3B")'  
$root -q -b -n 'mkFRMeas.C+O("h0015","3S3B")'  
$bkill -u [your account] -b 0 #kill all your jobs on LSF  
$bjobs -l  
$bjobs -u all  
$bjobs -a  
$bhosts  
$bqueues  

Deprecated!!@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  
== To use FAX as an input of the analysis  
$setupATLAS  
$localSetupFAX  
$voms-proxy-init --voms atlas:/atlas/jp  
$source rcSetup.sh  
$python share/mkFaxFileList.py [optionally: target dataset list]  
== Submitting jobs with FAX to LSF  
-- Make your GRID passward file as ./share/pfile.txt  
-- Attention: make sure no access right for others!! (chmod 0600 ./share/pfile.txt)  
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  
