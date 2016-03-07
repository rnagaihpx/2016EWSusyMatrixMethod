###########################################################
# Usage:
# $python share/mkFileList.py [Path to the datasets]
###########################################################
import os,sys,commands

print 'Making filelist for testRun...'

#defining the target directory
targetDir = '/gpfs/fs2001/hirose/MC15/SUSY2'
if len(sys.argv)==2: targetDir = os.path.abspath(sys.argv[1])

#checking if the specified target is a directory
isDir = os.path.isdir(targetDir)
if not isDir:
    print 'Please specify the directory. Exitting...'
    exit(1)

#deleting filelist existing before
os.system('rm -f '+targetDir+"/Local/*.txt")

directory = commands.getoutput('ls '+targetDir)
dirlist = directory.split('\n')

#Loop for the directory list
ndirs = len(dirlist)
processedDSIDs = []
isNewDsid = 0
preDsid = 0
nAddedFiles = 0
nthFile = 0
nMaxFiles = 5
for dirid in range(ndirs):
    isDir = os.path.isdir(targetDir+'/'+dirlist[dirid])
    if not isDir: continue
    if len(dirlist[dirid])<20: continue
    print 'For directory of ...', dirlist[dirid]
    projectName = 'mc15_13TeV.'
    dsidStartPos = dirlist[dirid].find(projectName)
    if dsidStartPos==-1: #case of the real data
        projectName = 'data15_13TeV.'
        dsidStartPos = dirlist[dirid].find(projectName)
        dsid = dirlist[dirid][dsidStartPos+len(projectName):dsidStartPos+len(projectName)+7]
        isData = 1
    else:
        dsid = dirlist[dirid][dsidStartPos+len(projectName):dsidStartPos+len(projectName)+6]
        isData = 0

    #checking if dataset is a AtlFast sample or not.
    dataTypePos = dirlist[dirid].find('AOD')
    configTag = dirlist[dirid][dataTypePos+4:]
    aTagPos = configTag.find('_a')
    if aTagPos==-1: #case of full sim sample
        print 'FullSim sample'
        isAFII = ''
    else:
        print 'AFII sample'
        isAFII = '_AtlFast'

    #checking if this is a new dataset
    if preDsid==dsid:
        isNewDsid = 0
        print 'Continueing with the previous DSID.'
    else:
        isNewDsid = 1
        print 'Starting with a new DSID!!'
        nAddedFiles = 0
        nthFile = 0

    preDsid = dsid

#Preparing filelist...
    ToBeSplit = 1
#    if isData==0 or isTtbar==1: ToBeSplit = 0 #in case we have a dataset not to be split...
    if not(dsid in processedDSIDs) or (ToBeSplit==1 and nAddedFiles>=nMaxFiles):
        nthFile+=1
        nAddedFiles = 0
        if isNewDsid==1: processedDSIDs.append(dsid)
        if ToBeSplit==1: filelistFileName = targetDir+"/Local/"+dsid+isAFII+'_'+str(nthFile)+".txt"
        else: filelistFileName = targetDir+"/"+dsid+".txt"
        filelistFile = open(filelistFileName,"w")
        print 'Create a new filelist file : '+filelistFileName
    else:
        if ToBeSplit==1: filelistFileName = targetDir+"/Local/"+dsid+isAFII+'_'+str(nthFile)+".txt"
        else: filelistFileName = targetDir+"/"+dsid+".txt"
        filelistFile = open(filelistFileName,"a")
        print 'Appending to the existing file : '+filelistFileName

#Loop for the file list
    files = commands.getoutput('ls '+targetDir+'/'+dirlist[dirid])
    filelist = files.split('\n')
    nfiles = len(filelist)
    for fileid in range(nfiles):
        isRootFile = ".root" in filelist[fileid]
        if isRootFile:
            if ToBeSplit==1 and nAddedFiles>=nMaxFiles:
                nthFile+=1
                nAddedFiles = 0
                filelistFileName = targetDir+"/Local/"+dsid+isAFII+'_'+str(nthFile)+".txt"
                filelistFile.close()
                filelistFile = open(filelistFileName,"w")
                print 'Create a new filelist file : '+filelistFileName
            nAddedFiles+=1
            print 'Adding '+filelist[fileid]+' to the filelist... ('+str(nAddedFiles)+')'
            filelistFile.write(targetDir+'/'+dirlist[dirid]+'/'+filelist[fileid]+'\n')
    print ''

print 'File lists for DSIDs below are generated...'
print processedDSIDs
