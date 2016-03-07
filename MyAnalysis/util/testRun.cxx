#include "xAODRootAccess/Init.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "EventLoopGrid/PrunDriver.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>
#include <TString.h>
#include <vector>

#include "MyAnalysis/MyxAODAnalysis.h"

void help();
char* GetMsgLevelStr(MSG::Level level);
MSG::Level GetMsgLevel(int level);

int main( int argc, char* argv[] ) {

  if(argc==1) help();
  std::cout << "=============================" << std::endl;
  std::cout << "Starting analysis code..."     << std::endl;
  std::cout << "=============================" << std::endl;
  std::cout << std::endl;

  long long int nEvt = -1;
  long long int nSkip = 0;
  MSG::Level dbg = MSG::ERROR;
  std::vector<std::string> sels;
  std::string file = "";
  std::string fileDir = "";
  std::string filelist = "";
  std::string fileDirBase = "/home/hirose/atlas/data/DC14/mc14_13TeV";
  std::string outputDir = "result/submitDir";
  std::string eleIdBaseline = "LooseLH";
  std::string isoWP = "GradientLoose";
  std::string effFile = "";
  bool doSys = false;
  bool runMM = false;
  bool measureEff = false;
  bool useFAX = false;
  bool useGRID = false;
  //  bool limitTree = false;
  
  /** Read inputs to program */  
  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-n") == 0)
      nEvt = atoi(argv[++i]);
    else if (strcmp(argv[i], "-k") == 0)
      nSkip = atoi(argv[++i]);
    else if (strcmp(argv[i], "-d") == 0)
      dbg = GetMsgLevel( atoi(argv[++i]) );
    else if (strcmp(argv[i], "-F") == 0)
      file = argv[++i];
    else if (strcmp(argv[i], "-D") == 0)
      fileDir = argv[++i];
    else if (strcmp(argv[i], "--filelist") == 0)
      filelist = argv[++i];
    else if (strcmp(argv[i], "--FileDirBase") == 0)
      fileDirBase = argv[++i];
    else if (strcmp(argv[i], "-o") == 0)
      outputDir = argv[++i];
    else if (strcmp(argv[i], "-S") == 0)
      sels.push_back(argv[++i]);
    else if (strcmp(argv[i], "--eleIdBaseline") == 0)
      eleIdBaseline = argv[++i];
    else if (strcmp(argv[i], "--isoWP") == 0)
      isoWP = argv[++i];
    else if (strcmp(argv[i], "--effFile") == 0)
      effFile = argv[++i];
    else if (strcmp(argv[i], "--sys") == 0)
      doSys = true;
    else if (strcmp(argv[i], "--runMM") == 0)
      runMM = true;
    else if (strcmp(argv[i], "--measureEff") == 0)
      measureEff = true;
    else if (strcmp(argv[i], "--useFAX") == 0)
      useFAX = true;
    else if (strcmp(argv[i], "--useGRID") == 0)
      useGRID = true;
    //    else if (strcmp(argv[i], "--limitTree") == 0)
    //      limitTree = true;
    //else if (strcmp(argv[i], "--sigCut") == 0)
      //sigCut = argv[++i];
    else if (strcmp(argv[i], "-h") == 0) help();
    else help();
  }

  std::cout << "flags:" << std::endl;
  std::cout <<Form("  outDir   : %s"  , outputDir.c_str()  ) << std::endl;
  std::cout <<Form("  nEvt     : %lli", nEvt               ) << std::endl;
  std::cout <<Form("  nSkip    : %lli", nSkip              ) << std::endl;
  std::cout <<Form("  dbg      : %s"  , GetMsgLevelStr(dbg)) << std::endl;
  if(!fileDirBase.empty()) std::cout << Form("  DataPath : %s", fileDirBase.c_str()) << std::endl;
  if(!fileDir    .empty()) std::cout << Form("  inputDir : %s", fileDir    .c_str()) << std::endl;
  if(!file       .empty()) std::cout << Form("  file     : %s", file       .c_str()) << std::endl;
  if(!filelist   .empty()) std::cout << Form("  filelist : %s", filelist   .c_str()) << std::endl;
  std::cout <<Form("  eleId    : %s"  , eleIdBaseline.c_str()  ) << std::endl;
  std::cout <<Form("  isoWP    : %s"  , isoWP.c_str()          ) << std::endl;
  std::cout <<Form("  useFAX   : %s"  , (useFAX?"true":"false")) << std::endl;
  std::cout <<Form("  useGRID  : %s"  , (useGRID?"true":"false")) << std::endl;
  std::cout <<Form("  doSys    : %s"  , (doSys ?"true":"false")) << std::endl;
  std::cout <<Form("  runMM    : %s"  , (runMM ?"true":"false")) << std::endl;
  std::cout <<Form("  effFile  : %s"  , effFile.c_str()        ) << std::endl;
  std::cout <<Form("  measureEff: %s"  , (measureEff ?"true":"false")) << std::endl;
  std::cout << std::endl;

  // Aborting if useFAX but filelist was not specified
  if(useFAX && filelist.empty()){
    std::cout<<"Error: Please specify input filelist by --filelist when you use \"--useFAX\"."<<std::endl;
    return 0;
  }

  // Aborting if useGRID but filelist was specified
  if(useGRID && (!filelist.empty() || !file.empty())){
    std::cout<<"Error: Please do not specify -F, --filelist, --FileDirBase with --useGRID."<<std::endl;
    return 0;
  }

  // Aborting if runMM and measureEff are asserted at the same time
  if(runMM && measureEff){
    std::cout<<"Error: Please do not specify --runMM and --measureEff at the same time."<<std::endl;
    return 0;
  }

  // Aborting if runMM but effFile is not specified
  if(runMM && effFile.empty()){
    std::cout<<"Error: Please specify --effFile [filename] when using --runMM."<<std::endl;
    return 0;
  }

  // If no input is specified, return with message.
  if(fileDir.empty() && filelist.empty()){
    std::cout << "Error: Please specify the input sample by the option \"-D\" or \"--filelist\"" <<std::endl;
    std::cout << "When you specify \"-D test\", SUSY signal sample written below will be used." <<std::endl;
    std::cout << "-- mc14_13TeV.205048.Herwigpp_UEEE4_CTEQ6L1_sM_wA_WZ_C1_100_N1_0.merge.AOD.e3479_a266_a265_r5853_tid04786763_00" << std::endl;
    return 0;
  }

  // Aborting if filelist and file directory are given at the same time.
  if(!fileDir.empty() && !filelist.empty()){
    std::cout<<"Error: Please do not use the options -D and --filelist at the same time."<<std::endl;
    return 0;
  }

  // Warning if "-k" or "-n" which won't work to scale MC with skimmed datasets is specified.
  if(nSkip!=0 || nEvt>0){
    std::cout<<std::endl;
    std::cout<<"============================================================================"<<std::endl;
    std::cout<<"Warning!!: \"-k\" or \"-n\" option was specified."                           <<std::endl;
    std::cout<<"           This option prevents proper scaling for MC without special care. "<<std::endl;
    std::cout<<"           Process interupted to check if you are sure."                     <<std::endl;
    std::cout<<"           Hit enter to proceed..."                                          <<std::endl;
    std::cout<<"============================================================================"<<std::endl;
    std::cout<<std::endl;
    getchar();
  }

  std::cout << "selections: ";
  for(uint i=0; i<sels.size(); i++) std::cout << "\"" << sels[i] << "\" ";
  std::cout << std::endl;
  std::cout << std::endl;

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Preparing your GRID certificate
  if(useFAX){
    gSystem->Exec("cat ${ROOTCOREBIN}/../share/pfile.txt | voms-proxy-init -voms atlas -valid 48:00");
  }

  // Construct the samples to run on:
  SH::SampleHandler sh;
  Bool_t useDir = kTRUE;
  if(fileDir.empty()) useDir = kFALSE;
  if(useGRID){
    std::cout<<"DatasetName : "<<fileDir.c_str()<<std::endl;;
    SH::scanDQ2(sh, fileDir); 
  }else if(useDir){
    //Check if input file path is existing.
    std::string dirFullPath = "";
    if(fileDir=="test") fileDir = "mc14_13TeV.205048.Herwigpp_UEEE4_CTEQ6L1_sM_wA_WZ_C1_100_N1_0.merge.AOD.e3479_a266_a265_r5853_tid04786763_00"; //Default dataset for testing.
    dirFullPath = fileDirBase+"/"+fileDir;
    if(!gSystem->AccessPathName(dirFullPath.c_str())){
      //    std::cout<<"InputPath Existing : "<<dirFullPath.c_str()<<std::endl;    
    }else{
      std::cout<<"Cannot find the input file path : "<<dirFullPath.c_str()<<std::endl;    
      std::cout<<"Exitting ..."<<std::endl;    
      exit(1);
    }

    std::cout<<"DatasetName  : "<<fileDir.c_str()    <<std::endl;;
    std::cout<<"DataFullPath : "<<dirFullPath.c_str()<<std::endl;;
    if(!file.empty()){
      FileStat_t info;
      if(gSystem->GetPathInfo((dirFullPath+"/"+file).c_str(), info)!=0) {
        std::cout<<"cannot find the input file in the specified directory."<<std::endl;
        std::cout<<"filename : "<<file.c_str()<<std::endl;
        exit(1);
      }else{
        std::cout<<"Found "<<file.c_str()<<" in the specified directory. "<<std::endl;
      }
      SH::DiskListLocal list(fileDirBase.c_str());
      SH::scanDir(sh, list, file.c_str()); // specifying one particular data file for testing 
    }else{
      SH::DiskListLocal list(dirFullPath.c_str());
      SH::scanSingleDir(sh, fileDir.c_str(), list);
    }
  }else{
    std::string filelistFullPath = fileDirBase+"/"+filelist;
    SH::readFileList(sh, "sample", filelistFullPath);
  }

  // Cloning output directory when it already exists.
  std::string submitDir = outputDir;
  TString olddir = submitDir.c_str();
  if(!gSystem->AccessPathName(olddir)){
    gSystem->Exec(Form("rm -rf %s~",olddir.Data()));
    gSystem->Rename(olddir,olddir+"~");
  }

  // Copying additional cross-section file into the SUSYTools directory under $ROOTCOREBIN.
  if(useGRID){
    gSystem->Exec("cp -f $ROOTCOREBIN/data/MyAnalysis/CrossSection/CrossSections_13TeV.txt $ROOTCOREBIN/data/SUSYTools/");
  }

  // Obtain DSID/RunNumber from file directory name
  int dsid = -1;
  if(useDir){
    if     (fileDir.substr(0,10)=="mc14_13TeV"  ) dsid = atoi(fileDir.substr(11,6).c_str());
    else if(fileDir.substr(0,10)=="mc15_13TeV"  ) dsid = atoi(fileDir.substr(11,6).c_str());
    else if(fileDir.substr(0,12)=="data14_13TeV") dsid = atoi(fileDir.substr(13,6).c_str());
    else if(fileDir.substr(0,12)=="data15_13TeV") dsid = atoi(fileDir.substr(13,6).c_str());
    else if(fileDir.substr(0, 9)=="mc14_8TeV"   ) dsid = atoi(fileDir.substr(10,6).c_str());
  }else{
    std::cout<<filelist.substr(0,6).c_str()<<std::endl;
    dsid = atoi(filelist.substr(0,6).c_str());
  }

  // Set the name of the input TTree. It's always "CollectionTree" for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );
  sh.print(); // Print what we found:

  // Set meta-data for the given sample.
  if(!filelist.empty()){
    TString tmpListName = filelist.c_str();
    sh.setMetaString("SampleType", (dsid<900000?"MC":"data"));//DSID==999999(egamma), 999998(muon)
    sh.setMetaString("MCType"    , (tmpListName.Contains("AtlFast")?"AtlFast":"FullSim"));
  }else{
    TString tmpDirName = fileDir.c_str();
    Ssiz_t aodPos = tmpDirName.Index("AOD");
    TString tmpTagName = tmpDirName(aodPos+4,tmpDirName.Length()-aodPos);
    sh.setMetaString("SampleType", (tmpDirName.BeginsWith("mc")?"MC"     :"data"   ));
    sh.setMetaString("MCType"    , (tmpDirName.Contains  ("_a")?"AtlFast":"FullSim"));
  }

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );

  // Add our analysis to the job:
  MyxAODAnalysis* alg = new MyxAODAnalysis();
  alg->SetDebugMode(dbg);
  alg->SetMaxEvent(nEvt);
  alg->SetNoSyst(!doSys);
  alg->SetDSID(dsid);
  alg->SetSkipNum(nSkip);
  alg->SetRunMM(runMM);
  alg->SetMeasureEff(measureEff);
  alg->SetEleIdBaseline(eleIdBaseline.c_str());
  alg->SetIsoWP(isoWP.c_str());
  alg->SetEffFile(effFile.c_str());
  if(useGRID) alg->SetOutputDir("./");
  else        alg->SetOutputDir(submitDir.c_str());
  for(uint i=0; i<sels.size(); i++) alg->SetSelectionRegion(sels.at(i).c_str());
  job.algsAdd( alg );

  // Run the job using the local/direct driver:
  if(useGRID){
    EL::PrunDriver driver;
    TString outputDS = Form("user.hirose.%d_%s",dsid,submitDir.c_str());
    std::cout<<"Output DS name: "<<outputDS.Data()<<std::endl;
    driver.options()->setString("nc_outputSampleName", outputDS.Data());
    //    driver.options()->setDouble(EL::Job::optGridNFilesPerJob, 20);
    driver.options()->setDouble(EL::Job::optGridNGBPerJob, 20);
    driver.submitOnly( job, ("result/"+outputDS).Data() );
  }else{
    EL::DirectDriver driver;
    driver.submit( job, submitDir );
  }
  std::cout<<std::endl;

  return 0;
}

void help()
{
  std::cout << "  Options:"                          << std::endl;

  std::cout << "  -n number of events to process"    << std::endl;
  std::cout << "     defaults: -1 (all events)"      << std::endl;

  std::cout << "  -k number of events to skip"       << std::endl;
  std::cout << "     defaults: 0"                    << std::endl;

  std::cout << "  --FileDirBase : input file directory or filelist location" << std::endl;
  std::cout << "     defaults: '/home/hirose/atlas/data/DC14/mc14_13TeV'" << std::endl;

  std::cout << "  -D : name of input dataset directory" << std::endl;
  std::cout << "     defaults: ''"                      << std::endl;

  std::cout << "  -F name of single input file"      << std::endl;
  std::cout << "     defaults: ''"                   << std::endl;

  std::cout << "  --filelist name of the input filelist" << std::endl;
  std::cout << "     defaults: ''"                   << std::endl;

  std::cout << "  --useFAX will use FAX as an input source" << std::endl;

  std::cout << "  --useGRID will submit jobs onto GRID" << std::endl;

  std::cout << "  -o output file directory"          << std::endl;
  std::cout << "     defaults: result/submitDir"     << std::endl;

  std::cout << "  -d debug printout level"           << std::endl;
  std::cout << "     defaults: MSG::ERROR"           << std::endl;

  std::cout << "  -S adds a selection region"        << std::endl;
  std::cout << "     each usage adds one region"     << std::endl;

  std::cout << "  --sys turns on MC systematics"     << std::endl;

  std::cout << "  --runMM uses MM weight for histograms" << std::endl;

  std::cout << "  --effFile [filename]: efficiency file for MM (should be in MMTool/share)" << std::endl;

  std::cout << "  --eleIdBaseline [(e.g. LooseLLH)]: baseline electron quality requirement (default LooseLLH)" << std::endl;

  std::cout << "  --isoWP [(e.g. GradientLoose)]: isolation working point for IsolationTool (default GradienLoose)" << std::endl;

  std::cout << "  --measureEff to measure efficiency in specified regions" << std::endl;

  //  std::cout << "  --limitTree write limit tree"      << std::endl;

  std::cout << "  -h print this help"                << std::endl;

  exit(1);
}

char* GetMsgLevelStr(MSG::Level level){
  if     (level==MSG::VERBOSE) return (char*)"MSG::VERBOSE";
  else if(level==MSG::DEBUG  ) return (char*)"MSG::DEBUG";
  else if(level==MSG::INFO   ) return (char*)"MSG::INFO";
  else if(level==MSG::WARNING) return (char*)"MSG::WARNING";
  else if(level==MSG::ERROR  ) return (char*)"MSG::ERROR";
  else if(level==MSG::FATAL  ) return (char*)"MSG::FATAL";
  else if(level==MSG::ALWAYS ) return (char*)"MSG::ALWAYS";

  return (char*)"NOT DEFINED";
}

MSG::Level GetMsgLevel(int level){
  if     (level==MSG::VERBOSE) return MSG::VERBOSE;
  else if(level==MSG::DEBUG  ) return MSG::DEBUG  ;
  else if(level==MSG::INFO   ) return MSG::INFO   ;
  else if(level==MSG::WARNING) return MSG::WARNING;
  else if(level==MSG::ERROR  ) return MSG::ERROR  ;
  else if(level==MSG::FATAL  ) return MSG::FATAL  ;
  else if(level==MSG::ALWAYS ) return MSG::ALWAYS ;

  return MSG::NIL;
}
