#define APP_NAME "MyxAODAnalysis"
#define MyInfo(a,b)   if(m_debugMode<=MSG::INFO  ) std::cout<<"Info  in <MyxAODAnalysis::"<<(a)<<">: "<<(b)<<std::endl;
#define MyError(a,b)  if(m_debugMode<=MSG::ERROR ) std::cout<<"Error in <MyxAODAnalysis::"<<(a)<<">: "<<(b)<<std::endl;
#define MyDebug(a,b)  if(m_debugMode<=MSG::DEBUG ) std::cout<<"Debug in <MyxAODAnalysis::"<<(a)<<">: "<<(b)<<std::endl;
#define MyAlways(a,b) if(m_debugMode<=MSG::ALWAYS) std::cout<<"In       <MyxAODAnalysis::"<<(a)<<">: "<<(b)<<std::endl;

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>
#include <MyAnalysis/MyxAODAnalysis.h>

//added by minoru
#include"TauAnalysisTools/TauTruthMatchingTool.h"
#include"SUSYTools/SUSYObjDef_xAOD.h"
#include"SUSYTools/SUSYCrossSection.h"
#include"xAODCutFlow/CutBookkeeper.h"
#include"xAODCutFlow/CutBookkeeperContainer.h"

#include"MyAnalysis/EventSelector.h"
#include"TStopwatch.h"
//end adding

// this is needed to distribute the algorithm to the workers
ClassImp(MyxAODAnalysis)

MyxAODAnalysis :: MyxAODAnalysis ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  // Event selection list
  m_vec_eveSelec = new std::vector<std::string>();
  m_vec_eveSelec->clear();

}



EL::StatusCode MyxAODAnalysis :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  //Added by minoru
  MyDebug("setupJob()", "Setting up EL::Job");
  MyDebug("setupJob()", Form("#EventSelec=%d",(int)m_vec_eveSelec->size()));

  job.useXAOD();
  xAOD::Init("MyxAODAnalysis").ignore(); //call before opening first file
  for(UInt_t eveSelec=0; eveSelec<m_vec_eveSelec->size(); eveSelec++){
    EL::OutputStream out(Form("%d.%s.AnaHists",m_dsid,m_vec_eveSelec->at(eveSelec).c_str()));
    job.outputAdd(out);
  }
  //end adding

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MyxAODAnalysis :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyxAODAnalysis :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  //Added by minoru
  m_processedEventsInFile = 0;
  m_processedEventsBeforeSkim = 0;
  m_sumWeightBeforeSkim = 0;
  m_event = wk()->xaodEvent();
  MyAlways("initialize()", Form("Input file changed... Number of events in this file = %lli.", m_event->getEntries())); //print in long long int

  ///////////////////////////////////////////
  //Read the CutBookkeeper container
  const xAOD::CutBookkeeperContainer* completeCBC = 0;
  if(!m_event->retrieveMetaInput(completeCBC, "CutBookkeepers").isSuccess()){
    MyError(APP_NAME, "Failed to retrieve CutBookkeepers from MetaData! Exiting.");
  }
  // First, let's find the smallest cycle number,
  // i.e., the original first processing step/cycle
  int minCycle = 10000;
  for(auto cbk : *completeCBC){
    if(minCycle>cbk->cycle()) minCycle = cbk->cycle();
  }
  // Now, let's actually find the right one that contains all the needed info...
  const xAOD::CutBookkeeper* allEventsCBK = 0;
  for(auto cbk : *completeCBC){
    if(minCycle==cbk->cycle() && cbk->name()=="AllExecutedEvents"){
      allEventsCBK = cbk;
      break;
    }
  }
  if(allEventsCBK){
    uint64_t nEventsProcessed  = allEventsCBK->nAcceptedEvents();
    double sumOfWeights        = allEventsCBK->sumOfEventWeights();
    double sumOfWeightsSquared = allEventsCBK->sumOfEventWeightsSquared();
    MyAlways(APP_NAME, Form("CutBookkeepers Accepted %lu SumWei %f sumWei2 %f ",nEventsProcessed,sumOfWeights,sumOfWeightsSquared));
    m_processedEventsBeforeSkim = nEventsProcessed;
    m_sumWeightBeforeSkim = sumOfWeights;
  }else{
    MyError(APP_NAME, "No relevent CutBookKeepers found"); 
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyxAODAnalysis :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyxAODAnalysis :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  CP::SystematicCode::enableFailure();

  //Added by minoru
  MyDebug("initialize()", "Initializing MyxAODAnalysis");
  MyAlways("initialize()", Form("%lli events will be processed.", m_maxEvent)); //print in long long int

  m_eventCounter = 0;
  m_totalProcessedEventsBeforeSkim = 0;
  m_totalSumWeightBeforeSkim = 0;
  m_processedEvents = 0;
  m_numCleanEvents = 0;
  m_eventWeight = 1.;

  // GRL tool initialization
  MyDebug("initialize()", "Preparing GRL tool");
  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  // vecStringGRL.push_back("$ROOTCOREBIN/data/MyAnalysis/GRL/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml");
  vecStringGRL.push_back("/gpfs/fs2001/dhdaiki/MM3L/RealData/All_Good/data15_13TeV.periodAllYear_DetStatus-v63-pro18-01_DQDefects-00-01-02_PHYS_StandardGRL_All_Good.xml");
  CHECK(m_grl->setProperty("GoodRunsListVec", vecStringGRL));
  CHECK(m_grl->setProperty("PassThrough", false)); // if true (default) will ignore result of GRL and will just pass all events
  if (!m_grl->initialize().isSuccess()) { // check this isSuccess
    MyError("initialize()", "Failed to properly initialize the GRL. Exiting.");
    return EL::StatusCode::FAILURE;
  }

  //Get meta-data from Sample Handler
  MyDebug("initialize()", "Preparing meta data from Sample Handler");
  TFile *file = wk()->inputFile();
  TString sample_name = file->GetName();
  MyInfo( "initialize()", Form("We will process the events in the dataset : %s", sample_name.Data()));
  int posAOD = sample_name.First("AOD");
  int posLastSlash = sample_name.Last('/');
  TString dstag = sample_name(posAOD+3,posLastSlash-posAOD-3);
  TString tmpname = sample_name(0,posLastSlash);
  int posSecondLastSlash = tmpname.Last('/');
  TString dsname = tmpname(posSecondLastSlash+1,tmpname.Length()-posSecondLastSlash-1);
  MyInfo( "initialize()", Form("Dataset Name : %s", dsname.Data()));

  std::string SampleType = wk()->metaData()->getString("SampleType");
  std::string MCType     = wk()->metaData()->getString("MCType"    );
  int isData    = (SampleType=="data"   ?1:0);
  int isAtlFast = (MCType    =="AtlFast"?1:0);
  MyInfo( "initialize()", Form("SampleType=%s, MCType=%s",SampleType.c_str(),MCType.c_str()));

  ST::SettingDataSource datasource = isData ? ST::Data : (isAtlFast ? ST::AtlfastII : ST::FullSim);

  // Tau Truth matching tool:
  m_tauTruthMatchingTool = NULL;
  if(!isData){
    m_tauTruthMatchingTool = new TauAnalysisTools::TauTruthMatchingTool("TauTruthMatchingTool");
    m_tauTruthMatchingTool->msg().setLevel( MSG::INFO );
    //    m_tauTruthMatchingTool->setProperty("WriteTruthTaus",true);
    CHECK(m_tauTruthMatchingTool->initialize());
  }

  ///////////////////////////////////////////
  //  Create the tool(s) to test:
  MyDebug("initialize()", "Preparing SUSYTools");
  m_susyObjTool = new ST::SUSYObjDef_xAOD("SUSYObjDef_xAOD");
  //  Configure the SUSYObjDef instance
  m_susyObjTool->msg().setLevel( m_debugMode );

  CHECK(m_susyObjTool->setProperty("DataSource",datasource) ) ;

  CHECK(m_susyObjTool->setProperty("JetInputType", xAOD::JetInput::EMTopo) );
  
  CHECK(m_susyObjTool->setProperty("EleId","TightLH") );
  CHECK(m_susyObjTool->setProperty("EleIdBaseline",m_eleIdBaseline.c_str()) );
  CHECK(m_susyObjTool->setProperty("MuId",xAOD::Muon::Medium) );
  CHECK(m_susyObjTool->setProperty("TauId","Tight") );
  //  CHECK(m_susyObjTool->setProperty("IsoWP",m_isoWP.c_str()) );

  //  CHECK(m_susyObjTool->setProperty("EleIsoWP",m_eleIso_WP = "EL0p06"));
  //  CHECK(m_susyObjTool->setProperty("MuIsoWP", m_muIso_WP  = "MU0p06"));
  //  CHECK(m_susyObjTool->setProperty("EleIsoWP",m_eleIso_WP = "GradientLoose"));
  //  CHECK(m_susyObjTool->setProperty("MuIsoWP", m_muIso_WP  = "GradientLoose"));
  CHECK(m_susyObjTool->setProperty("EleIsoWP",m_eleIso_WP = m_isoWP.c_str()) );
  CHECK(m_susyObjTool->setProperty("MuIsoWP", m_muIso_WP  = m_isoWP.c_str()) );

  // Set to true for DxAOD, false for primary xAOD
  CHECK(m_susyObjTool->setProperty("DoJetAreaCalib",true) ); //somehow "false" craches the process.
  // Set to false if not doing JetAreaCalib
  CHECK(m_susyObjTool->setProperty("DoJetGSCCalib",true) );
  
  // Set 0 for 14NP, 1,2,3,4 for 3NP sets
  CHECK(m_susyObjTool->setProperty("JESNuisanceParameterSet",1) );
  /////////////////////////////////////////////
  if( m_susyObjTool->SUSYToolsInit().isFailure() ) {
    MyError( "initialize()", "Failed to initialise tools in SUSYToolsInit()..." );
    MyError( "initialize()", "Exiting..." );
    return EL::StatusCode::FAILURE;
  }
  if( m_susyObjTool->initialize() != StatusCode::SUCCESS){
    MyError( "initialize()", "Cannot intialize SUSYObjDef_xAOD..." );
    MyError( "initialize()", "Exiting... " );
    return EL::StatusCode::FAILURE;
  }else{
    MyInfo( "initialize()", "SUSYObjDef_xAOD initialized... " );
  }

  /////////////////////////////////////////////////////
  MyDebug("initialize()", "Preparing Cross-Section DB");
  std::string xsecFileName  = gSystem->ExpandPathName("$ROOTCOREBIN/data/SUSYTools");
  m_XSDB = new SUSY::CrossSectionDB(xsecFileName);
  if(isData) m_crossSection = 0.;
  else       m_crossSection = m_XSDB->xsectTimesEff(m_dsid);
  MyInfo("initialize()", Form("Cross section times filter efficiency for DSID #%i: %f pb", m_dsid, m_crossSection));
  if(m_crossSection<0.){
    MyError("initialize()",Form("Failed to obtain the cross section for DSID=%i.",m_dsid));
    return EL::StatusCode::FAILURE;
  }

  // Now we can look at systematics:
  MyDebug("initialize()", "Preparing recommended systematics");
  const CP::SystematicRegistry& registry = CP::SystematicRegistry::getInstance();
  const CP::SystematicSet& recommendedSystematics = registry.recommendedSystematics();
  // this is the nominal set
  m_sysList.push_back(CP::SystematicSet());
  for(CP::SystematicSet::const_iterator sysItr = recommendedSystematics.begin();
      sysItr != recommendedSystematics.end(); ++sysItr){
    
    MyInfo("initialize()", Form("Found syst in global registry: %s", (sysItr->basename()).c_str()));
    
    TString tmpSysName = sysItr->basename();
    if(IsConsideredSyst(tmpSysName)){
      if (*sysItr == CP::SystematicVariation (sysItr->basename(), CP::SystematicVariation::CONTINUOUS)){
        // for continuous systematics just evaluate +/-1 sigma
        m_sysList.push_back(CP::SystematicSet());
        m_sysList.back().insert(CP::SystematicVariation (sysItr->basename(), 1));
        m_sysList.push_back(CP::SystematicSet());
        m_sysList.back().insert(CP::SystematicVariation (sysItr->basename(), -1));
      }else{
        // otherwise just add it flat
        m_sysList.push_back(CP::SystematicSet());
        m_sysList.back().insert(*sysItr);
      }
    }
  }
  MyDebug("initialize()", Form("================ Considered #systematics = %d", (int)m_sysList.size()) );

  //Preparing event counter
#define MAKE_COUNTER_VEC(val)                                           \
  n_ ## val = new std::vector<std::vector<int> >();                     \
  for(UInt_t eveSelec=0; eveSelec<m_vec_eveSelec->size(); eveSelec++){  \
    std::vector<int> tmp_ ## val ; n_ ## val ->push_back(tmp_ ## val ); \
    for(uint syst=0; syst<m_sysList.size(); syst++){                  \
      n_ ## val ->at(eveSelec).push_back(0);                            \
    }                                                                   \
  }                                                                     \
  
  MAKE_COUNTER_VEC(initial     );
  MAKE_COUNTER_VEC(pass_hotSpot);
  MAKE_COUNTER_VEC(pass_badJet );
  MAKE_COUNTER_VEC(pass_feb    );
  MAKE_COUNTER_VEC(pass_badMu  );
  MAKE_COUNTER_VEC(pass_cosmic );
  MAKE_COUNTER_VEC(pass_clean  );
  MAKE_COUNTER_VEC(pass_nBLep  );
  MAKE_COUNTER_VEC(pass_mllBase);
  MAKE_COUNTER_VEC(pass_nLep   );
  MAKE_COUNTER_VEC(pass_nTau   );
  MAKE_COUNTER_VEC(pass_ssEmul );
  MAKE_COUNTER_VEC(pass_trig   );
  MAKE_COUNTER_VEC(pass_truth  );
  MAKE_COUNTER_VEC(pass_sfos   );
  MAKE_COUNTER_VEC(pass_lz     );
  MAKE_COUNTER_VEC(pass_z      );
  MAKE_COUNTER_VEC(pass_met    );
  MAKE_COUNTER_VEC(pass_vbf    );
  MAKE_COUNTER_VEC(pass_jet    );
  MAKE_COUNTER_VEC(pass_mt     );
  MAKE_COUNTER_VEC(pass_mljj   );
  MAKE_COUNTER_VEC(pass_meff   );
  MAKE_COUNTER_VEC(pass_lepPt  );
  MAKE_COUNTER_VEC(pass_lepDR  );
  MAKE_COUNTER_VEC(pass_other  );

#undef MAKE_COUNTER_VEC

  n_passAC_badMuon    = 0;
  n_passAC_jetClean   = 0;
  n_passAC_primVtx    = 0;
  n_passAC_cosmic     = 0;
  n_passAC_oneBaseLep = 0;
  n_passAC_oneSigLep  = 0;
  n_passAC_oneBaseJet = 0;
  n_passAC_oneSigJet  = 0;
  n_passAC_twoBaseLep = 0;
  n_passAC_twoSigLep  = 0;
  n_passAC_oneBaseEl  = 0;
  n_passAC_oneSigEl   = 0;
  n_passAC_oneBaseMu  = 0;
  n_passAC_oneSigMu   = 0;
  n_passAC_oneBaseTau = 0;
  n_passAC_oneSigTau  = 0;
  n_passAC_oneBjet    = 0;

  //Preparing plotter and stopwatch for each event selection region and systematic variation
  m_vec_plotter = new std::vector<std::vector<Plotter*> >();
  m_vec_watch = new std::vector<std::vector<TStopwatch*> >();
  for(UInt_t eveSelec=0; eveSelec<m_vec_eveSelec->size(); eveSelec++){
    std::vector<Plotter*> tmp_vec_plotter;
    m_vec_plotter->push_back(tmp_vec_plotter);
    std::vector<TStopwatch*> tmp_vec_watch;
    m_vec_watch->push_back(tmp_vec_watch);
    size_t isys=0;
    std::vector<CP::SystematicSet>::const_iterator sysListItr;
    for(sysListItr = m_sysList.begin(); sysListItr != m_sysList.end(); ++sysListItr){
      std::string eveSelecName = m_vec_eveSelec->at(eveSelec);
      std::string systName     = sysListItr->name();
      Plotter* tmp_plotter = new Plotter(eveSelecName.c_str(), systName.c_str(), m_debugMode);
      tmp_plotter->SetRunMM(m_runMM);
      tmp_plotter->SetEffFile(m_effFile.c_str());
      m_vec_plotter->at(eveSelec).push_back(tmp_plotter);
      if(systName==""){
        TFile *outfile = wk()->getOutputFile(Form("%d.%s.AnaHists",m_dsid,m_vec_eveSelec->at(eveSelec).c_str()));
        m_vec_plotter->at(eveSelec).at(isys)->initialize(m_outputDir.c_str(),m_dsid,m_crossSection,outfile);
      }
      else m_vec_plotter->at(eveSelec).at(isys)->initialize(m_outputDir.c_str(),m_dsid,m_crossSection);
      TStopwatch* tmp_watch = new TStopwatch();
      m_vec_watch->at(eveSelec).push_back(tmp_watch);
      if(m_noSyst) break; //break if NoSyst flag is true;
      isys++;
    }
  }

  //end adding

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyxAODAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  //Added by minoru
  if(m_eventCounter==0) MyInfo("execute()", "Starting event by event processing.");
  // print every 5000 events, so we know where we are:
  if(m_eventCounter%2000==0) MyAlways("execute()", Form("Event number = %lli", m_eventCounter));
  m_eventCounter++; //Incrementing here since event might be rejected by some quality checks below.
  if( m_maxEvent>=0 && 
      ( m_eventCounter<=m_nSkipNum || (m_maxEvent+m_nSkipNum)<m_eventCounter ) ) return EL::StatusCode::SUCCESS;
  m_processedEventsInFile++;
  m_processedEvents++;

  //----------------------------
  // Event information
  //--------------------------- 
  const xAOD::EventInfo *eventInfo = 0;
  if(!m_event->retrieve(eventInfo, "EventInfo").isSuccess()){
    MyError("execute()", "Failed to retrieve event info collection. Exiting.");
    return EL::StatusCode::FAILURE;
  }

  // check if the event is data or MC
  m_isMC = eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) ? true : false;

  // check run number and luminosity block in data
  Int_t RunNumber   = -999;
  Int_t LumiBlock   = -999;
  Int_t EventNumber = -999;
  Int_t mcChannelNumber = -999; //DSID
  Int_t mcEventNumber   = -999; //Event number in generator?
  m_eventWeight = 0.;
  if(!m_isMC){ //For data
    RunNumber   = eventInfo->runNumber();
    LumiBlock   = eventInfo->lumiBlock();
    EventNumber = eventInfo->eventNumber();
    m_eventWeight = 1.;
    MyInfo("execute()", Form("RunNumber : %i, LumiBlock : %i, EventNumber : %i", RunNumber, LumiBlock, EventNumber));
  }else{ //For MC, check DSID and 
    mcChannelNumber = eventInfo->mcChannelNumber(); //DSID
    mcEventNumber   = eventInfo->mcEventNumber(); //Event number in generator?
    m_eventWeight   = eventInfo->mcEventWeight();
    MyInfo("execute()", Form("ChannelNumber : %i, EventNumber : %i, EventWeight : %f", mcChannelNumber, mcEventNumber, m_eventWeight));
    if(m_dsid!=mcChannelNumber){
      MyError("execute()",Form("mcChannelNumber(%d) by EventInfo is different from the one in testRun arugument(%d).",mcChannelNumber,m_dsid));
    }
  }

  //Filling event into h_nEve.
  for(uint eve=0; eve<m_vec_eveSelec->size(); eve++){
    for(uint syst=0; syst<m_sysList.size(); syst++){
      //      if(m_noSyst==false || syst==0) m_vec_plotter->at(eve).at(syst)->FillNEvent(m_eventWeight);// do not work for skimmed datasets.
      if(m_noSyst==false || syst==0){
        if(m_maxEvent>0){
          m_vec_plotter->at(eve).at(syst)->FillNEvent(m_eventWeight);
        }else if(m_processedEventsInFile==1){
          m_vec_plotter->at(eve).at(syst)->FillNEvent(m_sumWeightBeforeSkim);
          m_totalProcessedEventsBeforeSkim+=m_processedEventsBeforeSkim;
          m_totalSumWeightBeforeSkim+=m_sumWeightBeforeSkim;
        }
      }
    }
  }

  if(not PassPreSelection(eventInfo)) return EL::StatusCode::SUCCESS;
  MyInfo( "initialize()", "Preselection : Done.");

  ///////////////////////////////////////////////////////////////////////////
  // End of preselection
  ///////////////////////////////////////////////////////////////////////////
  if(m_debugMode<=MSG::DEBUG) m_store.print();

  ///////////////////////////////////////////////////////////////////////////
  // Prepare the tau truth matching before the systematic variations
  ///////////////////////////////////////////////////////////////////////////
  const xAOD::TruthParticleContainer* truthP = 0;
  if(m_isMC){
    // Get the Truth Particles from the event:
    CHECK( m_event->retrieve(truthP, "TruthParticles"));
    if(m_debugMode<=MSG::DEBUG) MyInfo( APP_NAME, Form("Number of truth particles: %i", static_cast<int>(truthP->size())));
    CHECK( m_tauTruthMatchingTool->initializeEvent() );
  }

  /////////////////////////////////////////////////////////////////////////
  // Now loop over all the systematic variations and event selections
  /////////////////////////////////////////////////////////////////////////
  std::vector<CP::SystematicSet>::const_iterator sysListItr;
  for(UInt_t eveSelec=0; eveSelec<m_vec_eveSelec->size(); eveSelec++){
    std::string eveSelecName = m_vec_eveSelec->at(eveSelec);
    //Systematic loop should be nested in the event selection loop.
    //This is due to the fact that we have to clear m_store after executing one set of systematic loop.
    ////////////////////////////
    size_t isys=0;
    for(sysListItr = m_sysList.begin(); sysListItr != m_sysList.end(); ++sysListItr){
      m_vec_watch->at(eveSelec).at(isys)->Start(m_processedEvents==1?kTRUE:kFALSE);
      MyInfo("execute()", Form(">>>> Working on variation: sys=%i, \"%s\"", (int)isys, (sysListItr->name()).c_str()));
      // Tell the SUSYObjDef_xAOD which variation to apply
      if(m_susyObjTool->applySystematicVariation(*sysListItr) != CP::SystematicCode::Ok){
        MyError("execute()", Form("Cannot configure SUSYTools for systematic var. %s", (sysListItr->name()).c_str()) );
      }else{
        MyInfo("execute()", Form("Variation \"%s\" configured...", (sysListItr->name()).c_str()) );
      }

      EventSelector* myEveSelec = 
        new EventSelector(m_susyObjTool, eveSelecName.c_str(), (sysListItr->name()).c_str(), (m_isMC?1:0), m_debugMode);
      myEveSelec->setRunMM(m_runMM);//Need to be called before initialize();
      myEveSelec->setMeasureEff(m_measureEff);//Need to be called before initialize();
      bool initDone = myEveSelec->initialize();
      if(initDone==false){
        if(m_processedEvents==1) MyError("execute()", Form("Not supported event selection was detected!! : %s",eveSelecName.c_str()));
        continue;
      } else {
        MyDebug("execute()", Form("Event selection has been initialized."));
      }
      if(eveSelecName=="ac"){
        myEveSelec->setBaseElMuPtThreshold(10000, 10000);
        myEveSelec->setSigElMuPtThreshold(25000, 25000);
      }else{
        myEveSelec->setBaseElMuPtThreshold(5000, 5000);
        myEveSelec->setSigElMuPtThreshold(5000, 5000);
      }
      myEveSelec->setBaseJetPtEtaThreshold(20000,2.8);
      myEveSelec->setSigJetPtEtaThreshold(20000,2.8);
      myEveSelec->setStore(&m_store);
      myEveSelec->setTauTruthMatchingTool(m_tauTruthMatchingTool);
      myEveSelec->selectObject();
      m_vec_plotter->at(eveSelec).at(isys)->FillHistoPreSelec(myEveSelec,m_eventWeight);
      Bool_t passSelec = myEveSelec->selectEvent();
      if(passSelec) m_vec_plotter->at(eveSelec).at(isys)->FillHistograms(myEveSelec,m_eventWeight);
      SetEventCounter(myEveSelec,eveSelec,isys);
      if(eveSelecName=="ac" && isys==0)SetEventCounterAC(myEveSelec);
      myEveSelec->finalize();
      delete myEveSelec;

      if(m_susyObjTool->resetSystematics()!=CP::SystematicCode::Ok){
        MyError("execute()", "Cannot reset SUSYTools systematics" );
        exit(-2);
      }
      m_vec_watch->at(eveSelec).at(isys)->Stop();
      if(m_noSyst) break; //break if NoSyst flag is true;
      ++isys;
    }

    MyDebug("execute()", "Store before .clear()");
    if(m_debugMode<=MSG::DEBUG) m_store.print();
    m_store.clear();
    MyDebug("execute()", "Store after .clear()");
    if(m_debugMode<=MSG::DEBUG) m_store.print();

  }
    
  if(m_debugMode<=MSG::VERBOSE){
    std::cout<<"Hit enter to proceed to the next event."<<std::endl;
    getchar();
  }
  //end adding

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyxAODAnalysis :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  //Added by minoru
  //Here, executed just after execute(). (i.e. event by event)
  //end adding

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyxAODAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  //Added by minoru
  MyAlways("finalize()", Form("Total #events in the sample dataset before skimming: %lli", m_totalProcessedEventsBeforeSkim) );
  MyAlways("finalize()", Form("Total #events given to this algorithm              : %lli", m_eventCounter) );
  MyAlways("finalize()", Form("#(Processed Events)                                : %lli", m_processedEvents) );
  MyAlways("finalize()", Form("#(Healthy events)                                  : %lli", m_numCleanEvents) );
  dumpEventCounters();
  dumpEventCountersAC();

  if(m_susyObjTool){
    delete m_susyObjTool;
    m_susyObjTool = 0;
  }
  if(m_grl){
    delete m_grl;
    m_grl = 0;
  }
  if(m_tauTruthMatchingTool){
    delete m_tauTruthMatchingTool;
    m_tauTruthMatchingTool = 0;
  }

  for(uint eve=0; eve<m_vec_eveSelec->size(); eve++){
    for(uint syst=0; syst<m_sysList.size(); syst++){
      if(m_noSyst==false || syst==0){
        m_vec_plotter->at(eve).at(syst)->finalize();
        delete m_vec_plotter->at(eve).at(syst);
        delete m_vec_watch->at(eve).at(syst);
      }
    }
  }
  delete m_vec_plotter;
  delete m_vec_watch;
  //end adding

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MyxAODAnalysis :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  return EL::StatusCode::SUCCESS;
}


//Added by minoru
bool MyxAODAnalysis::PassPreSelection(const xAOD::EventInfo* eventInfo){
  //------------------------------------------------------------
  // if data check if event passes GRL
  //------------------------------------------------------------
  if(!m_isMC){ // it's data!
    if(!m_grl->passRunLB(*eventInfo)){
      MyInfo("EventPreSelection()", "The event is not in the GRL!! going to next event...");
      return false;
    }
  } // end if not MC

  //------------------------------------------------------------
  // Apply event cleaning to remove events due to 
  // problematic regions of the detector, and incomplete events.
  // Apply to data.
  //------------------------------------------------------------
  // reject event if:
  if(!m_isMC){
    if((eventInfo->errorState(xAOD::EventInfo::LAr )==xAOD::EventInfo::Error) ||
       (eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error) ||
       (eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18)) ){
      MyInfo("EventPreSelection()", "Event is rejected due to detector imperfections.");
      return false;
    } // end if event flags check
  } // end if the event is data
  m_numCleanEvents++;

  return true;
}


bool MyxAODAnalysis::IsConsideredSyst(TString sysBasename){
  bool retval = false;

  if(sysBasename.Contains("EG_RESOLUTION_ALL") ||
     sysBasename.Contains("EG_RESOLUTION_NOMINAL") ||
     sysBasename.Contains("EG_SCALE_ALL") ||
     sysBasename.Contains("EG_SCALE_NOMINAL") ||
     sysBasename.Contains("EG_SCALE_NONE") ||
     sysBasename.Contains("EL_EFF_CorrUncertainty") ||
     sysBasename.Contains("EL_EFF_UncorrUncertainty") ||
     sysBasename.Contains("JER") ||
     sysBasename.Contains("MUONS_SCALE") ||
     sysBasename.Contains("MUONSF") ||
     sysBasename.Contains("JET_BJES_Response") ||
     sysBasename.Contains("MET_SoftCalo_Scale") ||
     sysBasename.Contains("MET_SoftTrk_Scale")
     ) retval = true;
  
  return retval;
}

/*--------------------------------------------------------------------------------*/
void MyxAODAnalysis::SetEventCounter(EventSelector *EveSelec, int eveSelec, int sys){
  n_initial     ->at(eveSelec).at(sys) += EveSelec->Get_initial     ();
  n_pass_hotSpot->at(eveSelec).at(sys) += EveSelec->Get_pass_hotSpot();
  n_pass_badJet ->at(eveSelec).at(sys) += EveSelec->Get_pass_badJet ();
  n_pass_feb    ->at(eveSelec).at(sys) += EveSelec->Get_pass_feb    ();
  n_pass_badMu  ->at(eveSelec).at(sys) += EveSelec->Get_pass_badMu  ();
  n_pass_cosmic ->at(eveSelec).at(sys) += EveSelec->Get_pass_cosmic ();
  n_pass_clean  ->at(eveSelec).at(sys) += EveSelec->Get_pass_clean  ();
  n_pass_nBLep  ->at(eveSelec).at(sys) += EveSelec->Get_pass_nBLep  ();
  n_pass_mllBase->at(eveSelec).at(sys) += EveSelec->Get_pass_mllBase();
  n_pass_nLep   ->at(eveSelec).at(sys) += EveSelec->Get_pass_nLep   ();
  n_pass_nTau   ->at(eveSelec).at(sys) += EveSelec->Get_pass_nTau   ();
  n_pass_ssEmul ->at(eveSelec).at(sys) += EveSelec->Get_pass_ssEmul ();
  n_pass_trig   ->at(eveSelec).at(sys) += EveSelec->Get_pass_trig   ();
  n_pass_truth  ->at(eveSelec).at(sys) += EveSelec->Get_pass_truth  ();
  n_pass_sfos   ->at(eveSelec).at(sys) += EveSelec->Get_pass_sfos   ();
  n_pass_lz     ->at(eveSelec).at(sys) += EveSelec->Get_pass_lz     ();
  n_pass_z      ->at(eveSelec).at(sys) += EveSelec->Get_pass_z      ();
  n_pass_met    ->at(eveSelec).at(sys) += EveSelec->Get_pass_met    ();
  n_pass_vbf    ->at(eveSelec).at(sys) += EveSelec->Get_pass_vbf    ();
  n_pass_jet    ->at(eveSelec).at(sys) += EveSelec->Get_pass_jet    ();
  n_pass_mt     ->at(eveSelec).at(sys) += EveSelec->Get_pass_mt     ();
  n_pass_mljj   ->at(eveSelec).at(sys) += EveSelec->Get_pass_mljj   ();
  n_pass_meff   ->at(eveSelec).at(sys) += EveSelec->Get_pass_meff   ();
  n_pass_lepPt  ->at(eveSelec).at(sys) += EveSelec->Get_pass_lepPt  ();
  n_pass_lepDR  ->at(eveSelec).at(sys) += EveSelec->Get_pass_lepDR  ();
  n_pass_other  ->at(eveSelec).at(sys) += EveSelec->Get_pass_other  ();

  return;
}

/*--------------------------------------------------------------------------------*/
void MyxAODAnalysis::SetEventCounterAC(EventSelector *EveSelec){
  n_passAC_badMuon    += EveSelec->GetPassAC_badMuon   ()?1:0;
  n_passAC_jetClean   += EveSelec->GetPassAC_jetClean  ()?1:0;
  n_passAC_primVtx    += EveSelec->GetPassAC_primVtx   ()?1:0;
  n_passAC_cosmic     += EveSelec->GetPassAC_cosmic    ()?1:0;
  n_passAC_oneBaseLep += EveSelec->GetPassAC_oneBaseLep()?1:0;
  n_passAC_oneSigLep  += EveSelec->GetPassAC_oneSigLep ()?1:0;
  n_passAC_oneBaseJet += EveSelec->GetPassAC_oneBaseJet()?1:0;
  n_passAC_oneSigJet  += EveSelec->GetPassAC_oneSigJet ()?1:0;
  n_passAC_twoBaseLep += EveSelec->GetPassAC_twoBaseLep()?1:0;
  n_passAC_twoSigLep  += EveSelec->GetPassAC_twoSigLep ()?1:0;
  n_passAC_oneBaseEl  += EveSelec->GetPassAC_oneBaseEl ()?1:0;
  n_passAC_oneSigEl   += EveSelec->GetPassAC_oneSigEl  ()?1:0;
  n_passAC_oneBaseMu  += EveSelec->GetPassAC_oneBaseMu ()?1:0;
  n_passAC_oneSigMu   += EveSelec->GetPassAC_oneSigMu  ()?1:0;
  n_passAC_oneBaseTau += EveSelec->GetPassAC_oneBaseTau()?1:0;
  n_passAC_oneSigTau  += EveSelec->GetPassAC_oneSigTau ()?1:0;
  n_passAC_oneBjet    += EveSelec->GetPassAC_oneBjet   ()?1:0;
  
  return;
}

/*--------------------------------------------------------------------------------*/
// Event counters
/*--------------------------------------------------------------------------------*/
void MyxAODAnalysis::dumpEventCounters()
{
  for(UInt_t eveSelec=0; eveSelec<m_vec_eveSelec->size(); eveSelec++){
    size_t isys=0;
    std::vector<CP::SystematicSet>::const_iterator sysListItr;
    for(sysListItr = m_sysList.begin(); sysListItr != m_sysList.end(); ++sysListItr){
      std::string eveSelecName = m_vec_eveSelec->at(eveSelec);
      std::string systName     = sysListItr->name();
      std::cout << std::endl;
      std::cout << "Event selection counters for " << eveSelecName.c_str() << ", " << systName.c_str() << std::endl;
      std::cout << "  initial:      " << n_initial      ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass hotSpot: " << n_pass_hotSpot ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass badJet:  " << n_pass_badJet  ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass FEB:     " << n_pass_feb     ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass badMu:   " << n_pass_badMu   ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass cosmic:  " << n_pass_cosmic  ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass clean:   " << n_pass_clean   ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass nBLep:   " << n_pass_nBLep   ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass mllBase: " << n_pass_mllBase ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass nLep:    " << n_pass_nLep    ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass nTau:    " << n_pass_nTau    ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass trig:    " << n_pass_trig    ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass truth:   " << n_pass_truth   ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass flavQ:   " << n_pass_sfos    ->at(eveSelec).at(isys)<< std::endl;
      //std::cout << "  pass loose Z: " << n_pass_lz      ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass jet:     " << n_pass_jet     ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass vbf:     " << n_pass_vbf     ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass lepPt:   " << n_pass_lepPt   ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass z:       " << n_pass_z       ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass lep dR:  " << n_pass_lepDR   ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass met:     " << n_pass_met     ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass mt:      " << n_pass_mt      ->at(eveSelec).at(isys)<< std::endl;
      std::cout << "  pass meff:    " << n_pass_meff    ->at(eveSelec).at(isys)<< std::endl;
      //std::cout << "  pass mljj:    " << n_pass_mljj    ->at(eveSelec).at(isys)<< std::endl;
      //std::cout << "  pass other:   " << n_pass_other   ->at(eveSelec).at(isys)<< std::endl;
      m_vec_watch->at(eveSelec).at(isys)->Print();
      if(m_noSyst) break; //break if NoSyst flag is true;
      isys++;
    }
  }
}

void MyxAODAnalysis::dumpEventCountersAC()
{
  for(UInt_t eveSelec=0; eveSelec<m_vec_eveSelec->size(); eveSelec++){
    std::string eveSelecName = m_vec_eveSelec->at(eveSelec);
    if(eveSelecName=="ac"){
      std::cout << std::endl;
      std::cout << "Event selection counters for the cutflow comparison" << std::endl;
      std::cout << "  initial   :      " << n_initial->at(eveSelec).at(0)<< std::endl;
      std::cout << "  GRL       :      fix me" << std::endl;
      std::cout << "  errorflags:      fix me" << std::endl;
      std::cout << "  trigger   :      fix me" << std::endl;
      std::cout << "  badMuon   :      " << n_passAC_badMuon   << std::endl;
      std::cout << "  jetClean  :      " << n_passAC_jetClean  << std::endl;
      std::cout << "  primVtx   :      " << n_passAC_primVtx   << std::endl;
      std::cout << "  cosmic    :      " << n_passAC_cosmic    << std::endl;
      std::cout << "  oneBaseLep:      " << n_passAC_oneBaseLep<< std::endl;
      std::cout << "  oneSigLep :      " << n_passAC_oneSigLep << std::endl;
      std::cout << "  oneBaseJet:      " << n_passAC_oneBaseJet<< std::endl;
      std::cout << "  oneSigJet :      " << n_passAC_oneSigJet << std::endl;
      std::cout << "  twoBaseLep:      " << n_passAC_twoBaseLep<< std::endl;
      std::cout << "  twoSigLep :      " << n_passAC_twoSigLep << std::endl;
      std::cout << "  oneBaseEl :      " << n_passAC_oneBaseEl << std::endl;
      std::cout << "  oneSigEl  :      " << n_passAC_oneSigEl  << std::endl;
      std::cout << "  oneBaseMu :      " << n_passAC_oneBaseMu << std::endl;
      std::cout << "  oneSigMu  :      " << n_passAC_oneSigMu  << std::endl;
      std::cout << "  oneBaseTau:      " << n_passAC_oneBaseTau<< std::endl;
      std::cout << "  oneSigTau :      " << n_passAC_oneSigTau << std::endl;
      std::cout << "  oneBjet   :      " << n_passAC_oneBjet   << std::endl;
      std::cout<<std::endl;
    }
  }
}
