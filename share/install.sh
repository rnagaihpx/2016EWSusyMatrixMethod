#!/bin/sh
git clone git://github.com/hrsmnr/MMTool.git
source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh
rcSetup SUSY,2.3.45
#rc checkout_pkg $SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-06-22
#rc checkout SUSYTools/doc/packages.txt
rc checkout_pkg svn+ssh://svn.cern.ch/reps/atlasphys/Physics/SUSY/Tools/CalcGenericMT2
cp CalcGenericMT2/src/MT2.h MyAnalysis/MyAnalysis/
cp CalcGenericMT2/src/MT2_ROOT.h MyAnalysis/MyAnalysis/
rm -rf CalcGenericMT2
rc checkout_pkg atlasoff/PhysicsAnalysis/MCTruthClassifier/tags/MCTruthClassifier-00-01-31
cp MCTruthClassifier/MCTruthClassifier/MCTruthClassifierDefs.h MyAnalysis/MyAnalysis/
rm -rf MCTruthClassifier
rc find_packages
rc compile
\cp -f $ROOTCOREBIN/../MyAnalysis/share/CrossSection/CrossSections_13TeV.txt $ROOTCOREBIN/data/SUSYTools/
