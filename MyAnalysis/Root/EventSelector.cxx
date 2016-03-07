#define APP_NAME "EventSelector"
#define MyInfo(a,b)    if(m_dbg<=MSG::INFO  ) std::cout<<"Info    in <EventSelector::"<<(a)<<">: "<<(b)<<std::endl;
#define MyError(a,b)   if(m_dbg<=MSG::ERROR ) std::cout<<"Error   in <EventSelector::"<<(a)<<">: "<<(b)<<std::endl;
#define MyDebug(a,b)   if(m_dbg<=MSG::DEBUG ) std::cout<<"Debug   in <EventSelector::"<<(a)<<">: "<<(b)<<std::endl;
#define MyVerbose(a,b) if(m_dbg<=MSG::DEBUG ) std::cout<<"Verbose in <EventSelector::"<<(a)<<">: "<<(b)<<std::endl;
#define MyAlways(a,b)  if(m_dbg<=MSG::ALWAYS) std::cout<<"In         <EventSelector::"<<(a)<<">: "<<(b)<<std::endl;

#include<iostream>
#include"MyAnalysis/EventSelector.h"

#include"SUSYTools/SUSYObjDef_xAOD.h"
#include"EventLoop/StatusCode.h"
#include"EventPrimitives/EventPrimitivesHelpers.h"
#include"xAODMissingET/MissingETAuxContainer.h"

#include"xAODTruth/xAODTruthHelpers.h"
#include"TauAnalysisTools/TauTruthMatchingTool.h"
#include"MyAnalysis/MCTruthClassifierDefs.h"//Copied from MCTruthClassifier-00-01-31
#include"MyAnalysis/MT2_ROOT.h"//MT2 tool by CalcGenericMT2

/*--------------------------------------------------------------------------------*/
// EventSelector Constructor
/*--------------------------------------------------------------------------------*/
EventSelector::EventSelector(ST::SUSYObjDef_xAOD *SUSYObjDef, const std::string &sel, const std::string &sys, const int &isMC, const MSG::Level &dbg) : 
  m_susyObjTool(SUSYObjDef),
  m_objReady(false),
  m_nSignalLeps(-1),
  m_nBaselineLeps(-1),
  m_nSignalJets(-1),
  m_nBaselineJets(-1),
  m_dbg(dbg),
  m_isMC(isMC),
  m_totalSF(1.),
  m_sel(sel),
  m_sys(sys),
  m_runMM(false),
  m_measureEff(false),
  m_availableSel(NULL),
  m_is3SigLepSel(true),
  m_sigElPtCut(5000),
  m_sigMuPtCut(5000),
  m_baseElPtCut(5000),
  m_baseMuPtCut(5000),
  // m_selFlag(0),
  // m_doWeightSys(false),
  // m_objSys(ObjSys::nom),
  // //m_subLepIso(true),
  // m_tauID(TauID_medium),
  // m_use2LTrig(false),
  // //m_2lTrigLogic(0),
  // //m_3lTrigLogic(0),
  // //m_chargeFlip(0),
  // m_qFlipTool(0),
  // m_doAB3(false),
  m_nBaseLepMin(-1),
  m_nBaseLepMax(-1),
  m_nLepMin(3),
  m_nLepMax(3),
  m_nEleMin(-1),
  m_nEleMax(-1),
  m_isoL3(false),
  m_isoBase(false),
  //m_nBaseTauMin(-1),
  //m_nBaseTauMax(-1),
  m_nTauMin(0),
  m_nTauMax(0),
  //m_nMedTauMin(-1),
  //m_nMedTauMax(-1),
  m_nTightTauMin(-1),
  m_nTightTauMax(-1),
  m_nLepTauMin(-1),
  m_nLepTauMax(-1),
  m_nBaseLepTauMin(-1),
  m_nBaseLepTauMax(-1),
  m_nJetMin(-1),
  m_nJetMax(-1),
  m_nBJetMin(-1),
  m_nBJetMax(-1),
  m_nFJetMax(-1),
  m_nCBJetMax(-1),
  m_nCLJetMin(-1),
  m_nCLJetMax(-1),
  m_useBaseLeps(false),
  m_applyTrig(true),
  m_trigAccOnly(false),
  m_useDilepTrigs(true),
  m_metMin(-1),
  m_metMax(-1),
  m_metRelMin(-1),
  m_vetoUpsilon(false),
  m_vetoZ(false),
  m_selZ(false),
  m_vetoExtZ(false),
  m_selExtZ(false),
  m_windowZ(10.),
  m_vetoLooseZ(false),
  m_vetoZeeSS(false),
  //  m_vetoB(false),
  //  m_selB(false),
  m_vetoSFOS(false),
  m_selSFOS(false),
  m_vetoSFSS(false),
  m_selSFSS(false),
  m_vetoOFOS(false),
  m_selOFOS(false),
  m_selOS(false),
  m_selSS(false),
  m_selSF(false),
  m_selDF(false),
  m_selDForSS4SigLep(false),
  m_vetoDForSS4SigLep(false),
  m_selDFandSS4SigLep(false),
  m_vetoDFandSS4SigLep(false),
  m_selOSTau(false),
  m_selOSLepTau(false),
  //m_specialCut(false),
  m_orthoCut(false),
  m_mllBaseMin(-1),
  m_minMllMin(4.),
  m_minMllMax(-1),
  m_mllMin(-1),
  m_mllMax(-1),
  m_vetoMllMin(-1),
  m_vetoMllMax(-1),
  m_msfosMin(-1),
  m_msfosMax(-1),
  m_mlllMin(-1),
  m_mlllMax(-1),
  m_mltMax(-1),
  m_mttMin(-1),
  m_mttMax(-1),
  m_mljjMin(-1),
  m_mljjMax(-1),
  m_mtMin(-1),
  m_mtMax(-1),
  m_mtllMin(-1),
  m_minDRBaseMin(0.3),
  m_minDPhiOSMax(-1),
  m_minDPhiOSMin(-1),
  m_lepDEtaMax(-1),
  m_jetPtMin(-1),
  m_jetPtMax(-1),
  m_lepPtMin(-1),
  m_lepPtMax(-1),
  m_lep1PtMin(-1),
  m_lep2PtMin(-1),
  m_baseLepEtaMax(-1),
  m_baseLepEtaMin(-1),
  m_sumLepPtMin(-1),
  m_tau1PtMin(-1),
  m_tau2PtMin(-1),
  m_sumTauPtMin(-1),
  m_meffMin(-1),
  m_meffMax(-1),
  m_momMetMin(-1),
  m_momMeffMin(-1),
  m_maxMT2Min(-1),
  m_maxMT2Max(-1),
  m_selRealLeps(false),
  m_vetoRealLep1(false),
  m_estimateQFlip(false),
  m_emulateSS(false),
  m_mjjMin(-1),
  m_mjjMax(-1),
  m_productEtaEtaMin(-1),
  m_productEtaEtaMax(-1),
  m_dEtaJJMin(-1),
  m_dEtaJJMax(-1),
  m_1stBaseIsSignal(false)
{
  // event counters
  n_initial     = 0;
  n_pass_hotSpot= 0;
  n_pass_badJet = 0;
  n_pass_feb    = 0;
  n_pass_badMu  = 0;
  n_pass_cosmic = 0;
  n_pass_clean  = 0;
  n_pass_nBLep  = 0;
  n_pass_mllBase= 0;
  n_pass_nLep   = 0;
  n_pass_nTau   = 0;
  n_pass_ssEmul = 0;
  n_pass_trig   = 0;
  n_pass_truth  = 0;
  n_pass_sfos   = 0;
  n_pass_lz     = 0;
  n_pass_z      = 0;
  n_pass_met    = 0;
  n_pass_vbf    = 0;
  n_pass_jet    = 0;
  n_pass_mt     = 0;
  n_pass_mljj   = 0;
  n_pass_meff   = 0;
  n_pass_lepPt  = 0;
  n_pass_lepDR  = 0;
  n_pass_other  = 0;

  b_passAC_badMuon    = true;
  b_passAC_jetClean   = true;
  b_passAC_primVtx    = true;
  b_passAC_cosmic     = true;
  b_passAC_oneBaseLep = true;
  b_passAC_oneSigLep  = true;
  b_passAC_oneBaseJet = true;
  b_passAC_oneSigJet  = true;
  b_passAC_twoBaseLep = true;
  b_passAC_twoSigLep  = true;
  b_passAC_oneBaseEl  = true;
  b_passAC_oneSigEl   = true;
  b_passAC_oneBaseMu  = true;
  b_passAC_oneSigMu   = true;
  b_passAC_oneBaseTau = true;
  b_passAC_oneSigTau  = true;
  b_passAC_oneBjet    = true;

  // n_evt_pileup  = 0;
  // n_evt_lepSF   = 0;
  // n_evt_btagSF  = 0;
  // n_evt_trigW   = 0;
  // n_evt_qFlip   = 0;
  // n_evt_tot     = 0;
  // n_evt_totLumi = 0;
}

/*--------------------------------------------------------------------------------*/
// Initialize the event selection
/*--------------------------------------------------------------------------------*/
bool EventSelector::initialize()
{
  MyDebug("initialize()","EventSelector::initialize()");

  /////////////////////////////////////////////////////////////////////
  // storing all available regions just for execution convenience...
  ////////////////////////////////////////////////////////////////////
  m_availableSel = new std::vector<std::string>();
  m_availableSel->push_back("none");
  m_availableSel->push_back("3SAnyB");
  m_availableSel->push_back("3SAnyBBveto");
  m_availableSel->push_back("2S3B"             );
  m_availableSel->push_back("2S3BBveto"        );
  m_availableSel->push_back("2S3BZveto"        );
  m_availableSel->push_back("2S3BMet"          );
  m_availableSel->push_back("2S3BZvetoBvetoMet");
  m_availableSel->push_back("2S3BDForSS"             );
  m_availableSel->push_back("2S3BDForSSBveto"        );
  m_availableSel->push_back("2S3BDForSSZveto"        );
  m_availableSel->push_back("2S3BDForSSMet"          );
  m_availableSel->push_back("2S3BDForSSZvetoBvetoMet");
  m_availableSel->push_back("2S3BDFSS"             );
  m_availableSel->push_back("2S3BDFSSBveto"        );
  m_availableSel->push_back("2S3BDFSSZveto"        );
  m_availableSel->push_back("2S3BDFSSMet"          );
  m_availableSel->push_back("2S3BDFSSZvetoBvetoMet");
  m_availableSel->push_back("2S3BTightBase"             );
  m_availableSel->push_back("2S3BTightBaseBveto"        );
  m_availableSel->push_back("2S3BTightBaseZveto"        );
  m_availableSel->push_back("2S3BTightBaseMet"          );
  m_availableSel->push_back("2S3BTightBaseZvetoBvetoMet");
  m_availableSel->push_back("3S3B"             );
  m_availableSel->push_back("3S3BBveto"        );
  m_availableSel->push_back("3S3BZveto"        );
  m_availableSel->push_back("3S3BMet"          );
  m_availableSel->push_back("3S3BZvetoBvetoMet");
  m_availableSel->push_back("3S3BDFSS"             );
  m_availableSel->push_back("3S3BDFSSBveto"        );
  m_availableSel->push_back("3S3BDFSSZveto"        );
  m_availableSel->push_back("3S3BDFSSMet"          );
  m_availableSel->push_back("3S3BDFSSZvetoBvetoMet");
  m_availableSel->push_back("3S3BNotDFSS"             );
  m_availableSel->push_back("3S3BNotDFSSBveto"        );
  m_availableSel->push_back("3S3BNotDFSSZveto"        );
  m_availableSel->push_back("3S3BNotDFSSMet"          );
  m_availableSel->push_back("3S3BNotDFSSZvetoBvetoMet");
  m_availableSel->push_back("3S3BDForSS"             );
  m_availableSel->push_back("3S3BDForSSBveto"        );
  m_availableSel->push_back("3S3BDForSSZveto"        );
  m_availableSel->push_back("3S3BDForSSMet"          );
  m_availableSel->push_back("3S3BDForSSZvetoBvetoMet");
  m_availableSel->push_back("3S3BNotDForSS"             );
  m_availableSel->push_back("3S3BNotDForSSBveto"        );
  m_availableSel->push_back("3S3BNotDForSSZveto"        );
  m_availableSel->push_back("3S3BNotDForSSMet"          );
  m_availableSel->push_back("3S3BNotDForSSZvetoBvetoMet");
  m_availableSel->push_back("3S4B"             );
  m_availableSel->push_back("3S4BBveto"        );
  m_availableSel->push_back("3S4BZveto"        );
  m_availableSel->push_back("3S4BMet"          );
  m_availableSel->push_back("3S4BZvetoBvetoMet");
  m_availableSel->push_back("GT1S3B");
  m_availableSel->push_back("GT1S3BForward");

  //////////////////////////////////////////////
  // Set selection cut variables
  //////////////////////////////////////////////

  // No selection, use every event in the ntuple
  if(m_sel=="ac"){ //for acceptance challenge

  }
  else if(m_sel=="none"){
    m_nLepMin = m_nLepMax = -1;
    m_nTauMin = m_nTauMax = -1;
    m_applyTrig = false;
    m_metRelMin = 10.;
    m_mtMin = 30.;
  }
  // 3 signal leptons, that's all
  else if(m_sel=="3SAnyB"){
    TypSel(3,0,-1,0,-1,-1);
    m_applyTrig = false;
  }
  else if(m_sel=="3SAnyBBveto"){
    TypSel(3,0,-1,0,0,0);
    m_applyTrig = false;
  }
  else if(m_sel=="2S3B"     ) Set2S3B();
  else if(m_sel=="2S3BBveto") Set2S3BBveto();
  else if(m_sel=="2S3BZveto") Set2S3BZveto();
  else if(m_sel=="2S3BMet"  ) Set2S3BMet();
  else if(m_sel=="2S3BZvetoBvetoMet") Set2S3BZvetoBvetoMet();
  else if(m_sel=="2S3BDForSS"     ) Set2S3BDForSS();
  else if(m_sel=="2S3BDForSSBveto") Set2S3BDForSSBveto();
  else if(m_sel=="2S3BDForSSZveto") Set2S3BDForSSZveto();
  else if(m_sel=="2S3BDForSSMet"  ) Set2S3BDForSSMet();
  else if(m_sel=="2S3BDForSSZvetoBvetoMet") Set2S3BDForSSZvetoBvetoMet();
  else if(m_sel=="2S3BDFSS"     ) Set2S3BDFSS();
  else if(m_sel=="2S3BDFSSBveto") Set2S3BDFSSBveto();
  else if(m_sel=="2S3BDFSSZveto") Set2S3BDFSSZveto();
  else if(m_sel=="2S3BDFSSMet"  ) Set2S3BDFSSMet();
  else if(m_sel=="2S3BDFSSZvetoBvetoMet") Set2S3BDFSSZvetoBvetoMet();
  else if(m_sel=="2S3BTightBase"     ) Set2S3BTightBase();
  else if(m_sel=="2S3BTightBaseBveto") Set2S3BTightBaseBveto();
  else if(m_sel=="2S3BTightBaseZveto") Set2S3BTightBaseZveto();
  else if(m_sel=="2S3BTightBaseMet"  ) Set2S3BTightBaseMet();
  else if(m_sel=="2S3BTightBaseZvetoBvetoMet") Set2S3BTightBaseZvetoBvetoMet();
  else if(m_sel=="3S3B"     ) Set3S3B();
  else if(m_sel=="3S3BBveto") Set3S3BBveto();
  else if(m_sel=="3S3BZveto") Set3S3BZveto();
  else if(m_sel=="3S3BMet"  ) Set3S3BMet();
  else if(m_sel=="3S3BZvetoBvetoMet") Set3S3BZvetoBvetoMet();
  else if(m_sel=="3S3BDFSS"     ) Set3S3BDFSS();
  else if(m_sel=="3S3BDFSSBveto") Set3S3BDFSSBveto();
  else if(m_sel=="3S3BDFSSZveto") Set3S3BDFSSZveto();
  else if(m_sel=="3S3BDFSSMet"  ) Set3S3BDFSSMet();
  else if(m_sel=="3S3BDFSSZvetoBvetoMet") Set3S3BDFSSZvetoBvetoMet();
  else if(m_sel=="3S3BNotDFSS"     ) Set3S3BNotDFSS();
  else if(m_sel=="3S3BNotDFSSBveto") Set3S3BNotDFSSBveto();
  else if(m_sel=="3S3BNotDFSSZveto") Set3S3BNotDFSSZveto();
  else if(m_sel=="3S3BNotDFSSMet"  ) Set3S3BNotDFSSMet();
  else if(m_sel=="3S3BNotDFSSZvetoBvetoMet") Set3S3BNotDFSSZvetoBvetoMet();
  else if(m_sel=="3S3BDForSS"     ) Set3S3BDForSS();
  else if(m_sel=="3S3BDForSSBveto") Set3S3BDForSSBveto();
  else if(m_sel=="3S3BDForSSZveto") Set3S3BDForSSZveto();
  else if(m_sel=="3S3BDForSSMet"  ) Set3S3BDForSSMet();
  else if(m_sel=="3S3BDForSSZvetoBvetoMet") Set3S3BDForSSZvetoBvetoMet();
  else if(m_sel=="3S3BNotDForSS"     ) Set3S3BNotDForSS();
  else if(m_sel=="3S3BNotDForSSBveto") Set3S3BNotDForSSBveto();
  else if(m_sel=="3S3BNotDForSSZveto") Set3S3BNotDForSSZveto();
  else if(m_sel=="3S3BNotDForSSMet"  ) Set3S3BNotDForSSMet();
  else if(m_sel=="3S3BNotDForSSZvetoBvetoMet") Set3S3BNotDForSSZvetoBvetoMet();
  else if(m_sel=="3S4B"     ) Set3S4B();
  else if(m_sel=="3S4BBveto") Set3S4BBveto();
  else if(m_sel=="3S4BZveto") Set3S4BZveto();
  else if(m_sel=="3S4BMet"  ) Set3S4BMet();
  else if(m_sel=="3S4BZvetoBvetoMet") Set3S4BZvetoBvetoMet();
  else if(m_sel=="GT1S3B") SetGT1S3B();               // Fake rate estimation
  else if(m_sel=="GT1S3BForward") SetGT1S3BForward(); // Fake rate estimation
  //Used for the legacy paper
  else if(m_sel=="VR0a") {
    TypSel(3,0,3,0,0,0);
    m_jetPtMax = 50;
    m_metMax = 30;
    m_minMllMin = 4;
    m_vetoExtZ = true;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="VR0b") {
    TypSel(3,0,3,0,1,1);
    m_jetPtMax = 50;
    m_metMin = 30;
    m_minMllMin = 4;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="VR0c") {
    TypSel(3,0,3,0,0,0);
    m_jetPtMax = 50;
    m_lepPtMax = 30;
    m_selSFOS = true;
    m_metMin = 50; 
    m_mlllMin = 60;
    m_minMllMin = 15;
    m_minMllMax = 25;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="VR1a") { 
    TypSel(3,0,3,0,0,0);
    m_nJetMin = 1;
    m_jetPtMin = 50;
    m_metMin = 30;
    m_metMax = 50;
    m_vetoExtZ = true;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="VR1b") { 
    TypSel(3,0,3,0,1,1);
    m_nJetMin = 1;
    m_jetPtMin = 50;
    m_metMin = 50;
    m_vetoExtZ = true;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="CRWZ") {
    TypSel(3,0,3,0,0,0);
    m_nJetMin = 1;
    m_jetPtMin = 50.;
    m_lepPtMin = 30.;
    m_metMin = 30.;
    m_metMax = 50.;
    m_selSFOS = true;
    m_vetoExtZ = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="SR0a") {
    TypSel(3,0,3,0,0,0);
    m_jetPtMax = 50;
    m_lepPtMax = 30;
    m_metMin = 30;
    m_mtMax = 20;
    m_mlllMin = 30;
    m_minMllMin = 4;
    m_minMllMax = 15;
    m_mlllMax = 60;
    m_selSFOS = true;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="SR0b") {
    TypSel(3,0,3,0,0,0);
    m_jetPtMax = 50;
    m_lepPtMax = 30;
    m_metMin = 30;
    m_mlllMin = 30;
    m_mlllMax = 60;
    m_minMllMin = 15;
    m_minMllMax = 25;
    m_selSFOS = true;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="SR1a") {
    TypSel(3,0,3,0,0,0);
    m_nJetMin = 1;
    m_jetPtMin = 50;
    m_lepPtMax = 30;
    m_metMin = 50;
    m_minMllMin = 5;
    m_minMllMax = 15;
    //    m_rl1j1ptMax = 0.2;
    //    m_metj1dphiMin = 0.86;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else if(m_sel=="SR1b") {
    TypSel(3,0,3,0,0,0);
    m_nJetMin = 1;
    m_jetPtMin = 50;
    m_lepPtMax = 30;
    m_metMin = 50;
    m_minMllMin = 15;
    m_minMllMax = 25;
    //    m_metWZdphiMax = 0.7;
    m_vetoUpsilon = true;
    //    m_isoL3 = true;
  }
  else{
    MyError("initialize()",Form("EventSelector ERROR - selection %s not supported!!",m_sel.c_str()));
    MyVerbose("initialize()","Available regions are...");
    if(m_dbg<=MSG::VERBOSE){
      for(UInt_t sel=0; sel<m_availableSel->size(); sel++){
        std::cout<<Form("-S %s ",m_availableSel->at(sel).c_str());
      }
      std::cout<<std::endl;
      for(UInt_t sel=0; sel<m_availableSel->size(); sel++){
        std::cout<<Form("%s ",m_availableSel->at(sel).c_str());
      }
      std::cout<<std::endl;
    }
    return false;
  }
  if(m_runMM || m_measureEff){
    m_nLepMin = 1;
    m_nLepMax = 3;
    m_1stBaseIsSignal = true;
    m_is3SigLepSel = false;
  }
  if(m_nLepMin!=3 || m_nLepMax!=3) m_is3SigLepSel = false;

  m_vec_signalElectron = new std::vector<xAOD::Electron>();
  m_vec_baseElectron   = new std::vector<xAOD::Electron>();
  m_vec_signalMuon = new std::vector<xAOD::Muon>();
  m_vec_baseMuon   = new std::vector<xAOD::Muon>();
  m_vec_signalJet = new std::vector<xAOD::Jet>();
  m_vec_baseJet   = new std::vector<xAOD::Jet>();
  m_vec_preJet    = new std::vector<xAOD::Jet>();
  m_vec_signalElectron->clear();
  m_vec_baseElectron  ->clear();
  m_vec_signalMuon->clear();
  m_vec_baseMuon  ->clear();
  m_vec_signalJet->clear();
  m_vec_baseJet  ->clear();
  m_vec_preJet   ->clear();

  return true;

}

/*--------------------------------------------------------------------------------*/
// Setter for each selection region
/*--------------------------------------------------------------------------------*/
void EventSelector::SetNSigNBase(int nSig, int nBase)
{
  m_nLepMin = m_nLepMax = nSig;
  m_nBaseLepMin = m_nBaseLepMax = nBase;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::SetBveto()
{
  m_nBJetMin = m_nBJetMax = 0;
  return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3B()
{
  SetNSigNBase(2,3);
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BBveto()
{
  SetNSigNBase(2,3);
  SetBveto();
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BZveto()
{
  SetNSigNBase(2,3);
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BMet()
{
  SetNSigNBase(2,3);
  m_metMin = 30;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BZvetoBvetoMet()
{
  SetNSigNBase(2,3);
  SetBveto();
  m_metMin = 30;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDForSS()
{
  SetNSigNBase(2,3);
  m_selSFOS = true;
  m_selDForSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDForSSBveto()
{
  SetNSigNBase(2,3);
  SetBveto();
  m_selSFOS = true;
  m_selDForSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDForSSZveto()
{
  SetNSigNBase(2,3);
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_selDForSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDForSSMet()
{
  SetNSigNBase(2,3);
  m_metMin = 30;
  m_selSFOS = true;
  m_selDForSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDForSSZvetoBvetoMet()
{
  SetNSigNBase(2,3);
  SetBveto();
  m_metMin = 30;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_selDForSS4SigLep = true;
  m_applyTrig = false;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDFSS()
{
  SetNSigNBase(2,3);
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDFSSBveto()
{
  SetNSigNBase(2,3);
  SetBveto();
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDFSSZveto()
{
  SetNSigNBase(2,3);
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDFSSMet()
{
  SetNSigNBase(2,3);
  m_metMin = 30;
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BDFSSZvetoBvetoMet()
{
  SetNSigNBase(2,3);
  SetBveto();
  m_metMin = 30;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BTightBase()
{
  SetNSigNBase(2,3);
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  m_isoBase = true;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BTightBaseBveto()
{
  SetNSigNBase(2,3);
  SetBveto();
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  m_isoBase = true;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BTightBaseZveto()
{
  SetNSigNBase(2,3);
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  m_isoBase = true;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BTightBaseMet()
{
  SetNSigNBase(2,3);
  m_metMin = 30;
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  m_isoBase = true;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set2S3BTightBaseZvetoBvetoMet()
{
  SetNSigNBase(2,3);
  SetBveto();
  m_metMin = 30;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_selDFandSS4SigLep = true;
  m_applyTrig = false;
  m_isoBase = true;
  return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3B()
{
  SetNSigNBase(3,3);
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BBveto()
{
  SetNSigNBase(3,3);
  SetBveto();
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BZveto()
{
  SetNSigNBase(3,3);
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BMet()
{
  SetNSigNBase(3,3);
  m_metMin = 70;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BZvetoBvetoMet()
{
  SetNSigNBase(3,3);
  SetBveto();
  m_metMin = 70;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDFSS()
{
  SetNSigNBase(3,3);
  m_selDFandSS4SigLep = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDFSSBveto()
{
  SetNSigNBase(3,3);
  m_selDFandSS4SigLep = true;
  SetBveto();
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDFSSZveto()
{
  SetNSigNBase(3,3);
  m_selDFandSS4SigLep = true;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDFSSMet()
{
  SetNSigNBase(3,3);
  m_selDFandSS4SigLep = true;
  m_metMin = 50;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDFSSZvetoBvetoMet()
{
  SetNSigNBase(3,3);
  m_selDFandSS4SigLep = true;
  SetBveto();
  m_metMin = 50;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDFSS()
{
  SetNSigNBase(3,3);
  m_vetoDFandSS4SigLep = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDFSSBveto()
{
  SetNSigNBase(3,3);
  m_vetoDFandSS4SigLep = true;
  SetBveto();
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDFSSZveto()
{
  SetNSigNBase(3,3);
  m_vetoDFandSS4SigLep = true;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDFSSMet()
{
  SetNSigNBase(3,3);
  m_vetoDFandSS4SigLep = true;
  m_metMin = 70;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDFSSZvetoBvetoMet()
{
  SetNSigNBase(3,3);
  m_vetoDFandSS4SigLep = true;
  SetBveto();
  m_metMin = 70;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDForSS()
{
  SetNSigNBase(3,3);
  m_selDForSS4SigLep = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDForSSBveto()
{
  SetNSigNBase(3,3);
  m_selDForSS4SigLep = true;
  SetBveto();
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDForSSZveto()
{
  SetNSigNBase(3,3);
  m_selDForSS4SigLep = true;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDForSSMet()
{
  SetNSigNBase(3,3);
  m_selDForSS4SigLep = true;
  m_metMin = 50;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BDForSSZvetoBvetoMet()
{
  SetNSigNBase(3,3);
  m_vetoDForSS4SigLep = true;
  SetBveto();
  m_metMin = 50;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDForSS()
{
  SetNSigNBase(3,3);
  m_vetoDForSS4SigLep = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDForSSBveto()
{
  SetNSigNBase(3,3);
  m_vetoDForSS4SigLep = true;
  SetBveto();
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDForSSZveto()
{
  SetNSigNBase(3,3);
  m_vetoDForSS4SigLep = true;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDForSSMet()
{
  SetNSigNBase(3,3);
  m_vetoDForSS4SigLep = true;
  m_metMin = 50;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S3BNotDForSSZvetoBvetoMet()
{
  SetNSigNBase(3,3);
  m_vetoDForSS4SigLep = true;
  SetBveto();
  m_metMin = 50;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S4B()
{
  SetNSigNBase(3,4);
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S4BBveto()
{
  SetNSigNBase(3,4);
  SetBveto();
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S4BZveto()
{
  SetNSigNBase(3,4);
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S4BMet()
{
  SetNSigNBase(3,4);
  m_metMin = 30;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::Set3S4BZvetoBvetoMet()
{
  SetNSigNBase(3,4);
  SetBveto();
  m_metMin = 30;
  m_vetoZ = true;
  m_vetoExtZ = true;
  m_selSFOS = true;
  m_applyTrig = false;
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::SetGT1S3B()
{
  //  SetBveto(); // require b veto
  m_nLepMin = 1;
  m_nLepMax = 3;
  m_nBaseLepMin = m_nBaseLepMax = 3;
  m_selSFOS = true;
  m_applyTrig = false;
  m_1stBaseIsSignal = true;
  //  m_baseLepEtaMax = 1.5; // require all baseline lepton's |eta| < 1.5
  return;
}
/*--------------------------------------------------------------------------------*/
void EventSelector::SetGT1S3BForward()
{
  m_nLepMin = 1;
  m_nLepMax = 3;
  m_nBaseLepMin = m_nBaseLepMax = 3;
  m_applyTrig = false;
  m_1stBaseIsSignal = true;
  m_baseLepEtaMin = 1.5; // require all baseline lepton's |eta| > 1.5
  return;
}

/*--------------------------------------------------------------------------------*/
// Setter to define electron and muon pt threshold
/*--------------------------------------------------------------------------------*/
void EventSelector::setSigElMuPtThreshold(double elPtCut, double muPtCut)
{
  m_sigElPtCut = elPtCut;
  m_sigMuPtCut = muPtCut;
  return;
}
void EventSelector::setBaseElMuPtThreshold(double elPtCut, double muPtCut)
{
  m_baseElPtCut = elPtCut;
  m_baseMuPtCut = muPtCut;
  return;
}
void EventSelector::setSigJetPtEtaThreshold(double jetPtCut, double jetEtaCut)
{
  m_sigJetPtCut = jetPtCut;
  m_sigJetEtaCut = jetEtaCut;
  return;
}
void EventSelector::setBaseJetPtEtaThreshold(double jetPtCut, double jetEtaCut)
{
  m_baseJetPtCut = jetPtCut;
  m_baseJetEtaCut = jetEtaCut;
  return;
}


/*--------------------------------------------------------------------------------*/
// Finalize event selection
/*--------------------------------------------------------------------------------*/
void EventSelector::finalize()
{
  MyDebug("finalize()","EventSelector::finalize()");

  delete m_availableSel;

  delete m_vec_signalElectron;
  delete m_vec_baseElectron;
  delete m_vec_signalMuon;
  delete m_vec_baseMuon;
  delete m_vec_signalJet;
  delete m_vec_baseJet;
  delete m_vec_preJet;

}

// /*--------------------------------------------------------------------------------*/
// // Event pre-selection
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::preSelectEvent()
// {
//   if(!passEventTrigger()) return false;
//   return true;
// }

/*--------------------------------------------------------------------------------*/
// Preparing physics objects
/*--------------------------------------------------------------------------------*/
bool EventSelector::IsMyBaselineElectron(const xAOD::Electron& el){
  if((int)el.auxdata<char>("baseline")!=1) return false;
  if(m_isoBase){ // will modify
  //   // ST::IsSignalElectronExpCutArgs args;
  //   // args.etcut(m_baseElPtCut);
  //   // if(not PassIsoElectron(el, ST::SignalIso::LooseIso, args)) return false;
  }
  return true;
}
bool EventSelector::IsMySignalElectron(const xAOD::Electron& el){
  if(not IsMyBaselineElectron(el)) return false;
  if((int)el.auxdata<char>("signal")!=1) return false;
  return true;
}
bool EventSelector::IsMyBaselineMuon(const xAOD::Muon& mu){
  if((int)mu.auxdata<char>("baseline")!=1) return false;
  if(m_isoBase){ // will modify
  //   ST::IsSignalMuonExpCutArgs args;
  //   args.ptcut(m_baseMuPtCut);
  //   if(not PassIsoMuon(mu, ST::SignalIso::LooseIso, args)) return false;
  }
  return true;
}
bool EventSelector::IsMySignalMuon(const xAOD::Muon& mu){
  if(not IsMyBaselineMuon(mu)) return false;
  if((int)mu.auxdata<char>("signal")!=1) return false;
  return true;
}
bool EventSelector::IsMySignalJet(xAOD::Jet& jet){
  return true;
}
bool EventSelector::IsMyBaselineJet(xAOD::Jet& jet){
  return true;
}
bool EventSelector::IsMyPreJet(xAOD::Jet& jet){
  return true;
}
// bool EventSelector::PassIsoElectron(const xAOD::Electron& input,
//                                     const ST::SignalIsoExp::IsoExp whichiso,
//                                     ST::IsSignalElectronExpCutArgs args)
// {
//   ///////////////////////////////////////////
//   //copied from SUSYTools-00-05-00-26
//   ///////////////////////////////////////////
//   const xAOD::TrackParticle* track = input.trackParticle();
//   const xAOD::Vertex* pv = m_susyObjTool->GetPrimVtx();
//   double primvertex_z = pv ? pv->z() : 0;
//   double el_d0sig = fabs(track->d0()) /  Amg::error(track->definingParametersCovMatrix(),0);
//   double el_z0 = track->z0() + track->vz() - primvertex_z;
//   float el_ptcone30(0.);
//   input.isolationValue(el_ptcone30,xAOD::Iso::ptcone30);
//   float el_topoEtcone30(0.);
//   input.isolationValue(el_topoEtcone30,xAOD::Iso::topoetcone30);
  
//   // Use a relative cut on the isolation by default and an aboslute cut on the isolation if pt_isoMax>0 && pt > pt_isoMax
//   if(args._pt_isoMax>0 && input.p4().Perp2()> args._pt_isoMax*args._pt_isoMax){
//     args._id_isocut *= args._pt_isoMax;
//     args._calo_isocut *= args._pt_isoMax;
//   }
//   else{
//     args._id_isocut *= input.pt();
//     args._calo_isocut *= input.pt();
//   }
  
//   if(whichiso == ST::SignalIso::LooseIso){
//     if((input.p4().Perp2() <= args._etcut*args._etcut || input.p4().Perp2() == 0)) return false; // eT cut (might be necessary for leading electron to pass trigger)
//     if(args._id_isocut > 0.0 && el_ptcone30 >= args._id_isocut) return false; // track isolation
//     if((args._z0cut > 0.0 && fabs(el_z0*TMath::Sin(input.p4().Theta())) > args._z0cut)) return false; // longitudinal IP cut
//   }
//   else if(whichiso == ST::SignalIso::MediumIso){
//     if((input.p4().Perp2() <= args._etcut*args._etcut || input.p4().Perp2() == 0)) return false; // eT cut (might be necessary for leading electron to pass trigger)
//     if((args._id_isocut > 0.0 && el_ptcone30 >= args._id_isocut)) return false; // track isolation
//     if((args._z0cut > 0.0 && fabs(el_z0*TMath::Sin(input.p4().Theta())) > args._z0cut)) return false; // longitudinal IP cut
//     if(el_d0sig != 0){
//       if((args._d0sigcut > 0.0 && fabs(el_d0sig) > args._d0sigcut)) return false; // transverse IP cut
//     }
//   }
//   else if(whichiso == ST::SignalIso::TightIso){
//     if((input.p4().Perp2() <= args._etcut*args._etcut || input.p4().Perp2() == 0)) return false; // eT cut (might be necessary for leading electron to pass trigger)
//     if((args._id_isocut > 0.0 && el_ptcone30 >= args._id_isocut)) return false; // track isolation
//     if((args._z0cut > 0.0 && fabs(el_z0*TMath::Sin(input.p4().Theta())) > args._z0cut)) return false; // longitudinal IP cut
//     if(el_d0sig != 0){
//       if((args._d0sigcut > 0.0 && fabs(el_d0sig) > args._d0sigcut)) return false; // transverse IP cut
//     }
//     if(args._calo_isocut > 0.0 && el_topoEtcone30 >= args._calo_isocut) return false; // calo isolation
//   }
//   else {
//     MyError("PassIsoElectron()", "wrong isolation quality specified for electron .... returning false");
//     return false;
//   }
//   return true;
// }
// bool EventSelector::PassIsoMuon(const xAOD::Muon& input,
//                                 const ST::SignalIso::IsoExp whichiso,
//                                 ST::IsSignalMuonExpCutArgs args)
// {
//   ///////////////////////////////////////////
//   //copied from SUSYTools-00-05-00-26
//   ///////////////////////////////////////////
//   const xAOD::TrackParticle* track =  input.primaryTrackParticle();
//   const xAOD::Vertex* pv = m_susyObjTool->GetPrimVtx();
//   double primvertex_z = pv ? pv->z() : 0;
//   double mu_d0sig = fabs(track->d0()) /  Amg::error(track->definingParametersCovMatrix(),0);
//   double mu_z0 = track->z0() + track->vz() - primvertex_z;
//   float mu_ptcone30 = input.auxdata<float>("ptcone30");
//   float mu_etcone30 = input.auxdata<float>("etcone30");

//   // Use a relative cut on the isolation by default and an aboslute cut on the isolation if pt_isoMax>0 && pt > pt_isoMax
//   if(args._pt_isoMax>0 && input.pt()> args._pt_isoMax){
//     args._id_isocut *= args._pt_isoMax;
//     args._calo_isocut *= args._pt_isoMax;
//   }
//   else{
//     args._id_isocut *= input.pt();
//     args._calo_isocut *= input.pt();
//   }

//   // if(whichiso == ST::SignalIso::LooseIso){
//   //   if(input.pt() <= args._ptcut || input.pt() == 0) return false; // pT cut (might be necessary for leading muon to pass trigger)
//   //   if(args._id_isocut > 0.0 && mu_ptcone30 >= args._id_isocut) return false; // track isolation
//   //   if(args._z0cut > 0.0 && fabs(mu_z0*TMath::Sin(input.p4().Theta())) > args._z0cut) return false; // longitudinal IP cut
//   // }
//   // else if(whichiso == ST::SignalIso::MediumIso){
//   //   if(input.pt() <= args._ptcut || input.pt() == 0) return false; // pT cut (might be necessary for leading muon to pass trigger)
//   //   if(args._id_isocut > 0.0 && mu_ptcone30 >= args._id_isocut) return false; // track isolation
//   //   if(args._z0cut > 0.0 && fabs(mu_z0*TMath::Sin(input.p4().Theta())) > args._z0cut) return false; // longitudinal IP cut
//   //   if(mu_d0sig != 0){
//   //     if(args._d0sigcut > 0.0 && fabs(mu_d0sig) > args._d0sigcut) return false; // transverse IP cut
//   //   }
//   // }
//   // else if(whichiso == ST::SignalIso::TightIso){
//   //   if(input.pt() <= args._ptcut || input.pt() == 0) return false; // pT cut (might be necessary for leading muon to pass trigger)
//   //   if(args._id_isocut > 0.0 && mu_ptcone30 >= args._id_isocut) return false; // track isolation
//   //   if(args._z0cut > 0.0 && fabs(mu_z0*TMath::Sin(input.p4().Theta())) > args._z0cut) return false; // longitudinal IP cut
//   //   if(mu_d0sig != 0){
//   //     if(args._d0sigcut > 0.0 && fabs(mu_d0sig) > args._d0sigcut) return false; // transverse IP cut
//   //   }
//   //   if(args._calo_isocut > 0.0 && mu_etcone30 >= args._calo_isocut) return false; // calo isolation
//   // }
//   // else{
//   //   MyError("PassIsoMuon()", "wrong isolation quality specified for muon .... returning false");
//   //   return false;
//   // }
//   return true;
// }

/*--------------------------------------------------------------------------------*/
// Object selection
/*--------------------------------------------------------------------------------*/
bool EventSelector::selectObject()
{
  MyDebug("selectObject()", "Start object selection");
  bool rtrvFail = false;
  ///////////////////////////////////////////////////
  // Get Electrons from the event
  ///////////////////////////////////////////////////
  xAOD::ElectronContainer   *electrons_copy(0);
  xAOD::ShallowAuxContainer *electrons_copyaux(0);
  if(m_susyObjTool->GetElectrons(electrons_copy, electrons_copyaux, false, m_baseElPtCut)==EL::StatusCode::FAILURE){
    MyError("selectObject()","Failing to retrieve ElectronContainer.");
    rtrvFail = true;
  }
  // preparing signal flag
  xAOD::ElectronContainer::iterator el_itr = (electrons_copy)->begin();
  xAOD::ElectronContainer::iterator el_end = (electrons_copy)->end();
  for( ; el_itr!=el_end; ++el_itr){
    m_susyObjTool->IsSignalElectron( **el_itr, m_sigElPtCut); //Signal flag are set here.
  }

  ///////////////////////////////////////////////////
  // Get the Photons from the event
  ///////////////////////////////////////////////////
  MyDebug("selectObject()", "Retrieve photon container");
  xAOD::PhotonContainer     *photons_copy(0);
  xAOD::ShallowAuxContainer *photons_copyaux(0);
  if(m_susyObjTool->GetPhotons(photons_copy,photons_copyaux)==EL::StatusCode::FAILURE){
    MyError("selectObject()","Failing to retrieve PhotonContainer.");
    rtrvFail = true;
  }

  // Print their properties, using the tools:
  for(const auto& ph : *photons_copy){
    MyDebug("selectObject()",Form("Post-fill Ph pt %f eta %f phi %f", ph->pt(), ph->eta(), ph->phi()));
    m_susyObjTool->IsSignalPhoton(*ph, 25e3);
  }

  ///////////////////////////////////////////////////
  // Get the Muons from the event:
  ///////////////////////////////////////////////////
  MyDebug("selectObject()", "Retrieve muon container");
  xAOD::MuonContainer       *muons_copy(0);
  xAOD::ShallowAuxContainer *muons_copyaux(0);
  if(m_susyObjTool->GetMuons(muons_copy,muons_copyaux, false, m_baseMuPtCut)==EL::StatusCode::FAILURE){
    MyError("selectObject()","Failing to retrieve MuonContainer.");
    rtrvFail = true;
  }
  // preparing signal flag
  xAOD::MuonContainer::iterator mu_itr = (muons_copy)->begin();
  xAOD::MuonContainer::iterator mu_end = (muons_copy)->end();
  for( ; mu_itr!=mu_end; ++mu_itr){
    m_susyObjTool->IsSignalMuon( **mu_itr, m_sigMuPtCut); //Signal flag are set here.
    m_susyObjTool->IsCosmicMuon( **mu_itr ); //Cosmic flag are set here.
    if(m_sel=="ac")passACCut_badMuon();
  }

  ///////////////////////////////////////////////////
  // Get the Jets from the event:
  ///////////////////////////////////////////////////
  MyDebug("selectObject()", "Retrieve jet container");
  //Checking jet containers status
  xAOD::JetContainer        *jets_copy(0);
  xAOD::ShallowAuxContainer *jets_copyaux(0);
  // if(m_susyObjTool->GetJets(jets_copy, jets_copyaux, false, m_baseJetPtCut, m_baseJetEtaCut)==EL::StatusCode::FAILURE){
  if(m_susyObjTool->GetJets(jets_copy, jets_copyaux, false)==EL::StatusCode::FAILURE){
    MyError("selectObject()","Failing to retrieve JetContainer.");
    rtrvFail = true;
  }
  // Check if there are b-tagged jets
  xAOD::JetContainer::iterator jet_itr = (jets_copy)->begin();
  xAOD::JetContainer::iterator jet_end = (jets_copy)->end();
  for( ; jet_itr!=jet_end; ++jet_itr){
    MyDebug("selectObject()", "");
    m_vec_preJet->push_back(**jet_itr);
  }

  // // Print their properties, using the tools:
  // xAOD::TauJetContainer::iterator tau_itr = (taus_copy)->begin();
  // xAOD::TauJetContainer::iterator tau_end = (taus_copy)->end();
  // for( ; tau_itr != tau_end; ++tau_itr ){
  //   MyInfo(APP_NAME, "  Tau passing IsBaseline? %i  pt=%g sf=%g",
  //          (int)(*tau_itr)->auxdata<bool>("baseline"), (*tau_itr)->pt(), m_susyObjTool->GetSignalTauSF(**tau_itr) );
  //   // if(m_isMC){
  //   //   T2MT.applyTruthMatch(*(*tau_itr));
  //   //   if((*tau_itr)->auxdata<bool>("IsTruthMatched")){
  //   //     Info(APP_NAME, "Tau was matched to a truth tau, which has %i prongs and a charge of %i",
  //   //          int((*tau_itr)->auxdata<size_t>("TruthProng")),
  //   //          (*tau_itr)->auxdata<int>("TruthCharge"));
  //   //   }else{
  //   //     Info(APP_NAME, "Tau was not matched to truth");
  //   //   }
  //   // }
  // }

  ///////////////////////////////////////////////////
  // Get the Taus from the event:
  ///////////////////////////////////////////////////
  MyDebug("selectObject()", "Retrieve tau container");
  xAOD::TauJetContainer     *taus_copy(0);
  xAOD::ShallowAuxContainer *taus_copyaux(0);
  if(m_susyObjTool->GetTaus(taus_copy,taus_copyaux)==EL::StatusCode::FAILURE){
    MyError("selectObject()","Failing to retrieve TauContainer.");
    rtrvFail = true;
  }

  // // Print their properties, using the tools
  // for(const auto& tau : *taus_copy){
  //   if(m_isMC){
  //     const xAOD::TruthParticle* truthTau = m_tauTruthMatchingTool->getTruth(*tau);
  //     if(tau->auxdata<char>("IsTruthMatched") || !truthTau) {
  //       if(m_dbg<=MSG::DEBUG) MyInfo( APP_NAME,
  //                                     Form("Tau was matched to a truth tau, which has %i prongs and a charge of %i",
  //                                          int(tau->auxdata<size_t>("TruthProng")),
  //                                          tau->auxdata<int>("TruthCharge")));
  //     } else {
  //       if(m_dbg<=MSG::DEBUG) MyInfo( APP_NAME, "Tau was not matched to truth" );
  //     }
  //   }
  // }

  ///////////////////////////////////////////////////
  // do overlap removal
  ///////////////////////////////////////////////////
  MyDebug("selectObject()", "Do overlap removal");
  xAOD::JetContainer* goodJets = new xAOD::JetContainer(SG::VIEW_ELEMENTS);
  MyDebug("selectObject()",Form("m_sys=%s",m_sys.c_str()));
  if(m_store->record(goodJets, Form("MySelJets%s",m_sys.c_str()))==EL::StatusCode::FAILURE){
    MyError("selectObject()",Form("Failing to recode MySelJets%s to TStore.",m_sys.c_str()));
    rtrvFail = true;
  }
  if(m_susyObjTool->OverlapRemoval(electrons_copy, muons_copy, jets_copy)==EL::StatusCode::FAILURE){
    MyError("selectObject()","Failing overlap removal process.");
    rtrvFail = true;
  }
  //////////////////////////////////////////////////////////////////////
  // preparing the goodJets container
  //////////////////////////////////////////////////////////////////////
  MyDebug("selectObject()","Preparing GoodJets Container");
  jet_itr = (jets_copy)->begin();
  jet_end = (jets_copy)->end();
  for( ; jet_itr != jet_end; ++jet_itr ){
    m_susyObjTool->IsBadJet(**jet_itr);
    m_susyObjTool->IsSignalJet(**jet_itr,m_baseJetPtCut,m_baseJetEtaCut);
    m_susyObjTool->IsBJet(**jet_itr); //Making b-tagged jet flag. Should be called "after" IsSignalJet().
    MyDebug("selectObject()",Form("jet: bad=%d"     ,(int)(*jet_itr)->auxdata<char>("bad"     )));
    MyDebug("selectObject()",Form("jet: baseline=%d",(int)(*jet_itr)->auxdata<char>("baseline")));
    MyDebug("selectObject()",Form("jet: signal=%d"  ,(int)(*jet_itr)->auxdata<char>("signal"  )));
    MyDebug("selectObject()",Form("jet: passOR=%d"  ,(int)(*jet_itr)->auxdata<char>("passOR"  )));
    MyDebug("selectObject()",Form("jet: bjet=%d"    ,(int)(*jet_itr)->auxdata<char>("bjet"    )));
    if(m_sel=="ac")passACCut_jetClean(**jet_itr);
    if( (*jet_itr)->auxdata<char>("baseline")==1 &&
        (*jet_itr)->auxdata<char>("passOR"  )==1 &&
        (*jet_itr)->auxdata<char>("bad"     )==0 ){
      m_vec_baseJet->push_back(**jet_itr);
      goodJets->push_back (*jet_itr);
    }
  }

  ///////////////////////////////////////////////////
  // This has to be called after passACCut_jetClean() and before passACCut_cosmic().
  if(m_sel=="ac")passACCut_primVtx();
  ///////////////////////////////////////////////////

  ///////////////////////////////////////////////////
  // Retrieving Missing Et
  ///////////////////////////////////////////////////
  MyDebug("selectObject()","Preparing MissingEt");
  //MET CST
  xAOD::MissingETContainer* metcst = new xAOD::MissingETContainer;
  xAOD::MissingETAuxContainer* metcst_aux = new xAOD::MissingETAuxContainer;
  metcst->setStore(metcst_aux);
  if( m_susyObjTool->GetMET(*metcst, jets_copy, electrons_copy, muons_copy,
                            photons_copy, taus_copy)==EL::StatusCode::FAILURE ){
    MyError("selectObject()","Failing to retrieve METCST.");
    rtrvFail = true;
  }
  xAOD::MissingETContainer::const_iterator metcst_it = metcst->find("Final");
  if(metcst_it == metcst->end()){
    MyError("selectObject()","No RefFinal inside MET container");
    rtrvFail = true;
  }
  
  //MET TST
  xAOD::MissingETContainer* mettst = new xAOD::MissingETContainer;
  xAOD::MissingETAuxContainer* mettst_aux = new xAOD::MissingETAuxContainer;
  mettst->setStore(mettst_aux);
  if( m_susyObjTool->GetMET(*mettst, jets_copy, electrons_copy, muons_copy,
                            photons_copy, taus_copy, true)==EL::StatusCode::FAILURE ){
    MyError("selectObject()","Failing to retrieve METTST.");
    rtrvFail = true;
  }

  m_met.SetMagPhi(0.,0.);
  for(const auto& metterm : *mettst){
    if(metterm->name()=="Final") m_met.SetMagPhi(metterm->met(),metterm->phi());
  }
  MyDebug("selectObject()",Form("MyPx=%12.6f, MyPy=%12.6f",m_met.Px(),m_met.Py()));

  //////////////////////////////////////////////////////////////////////
  // preparing signal and baseline electrons (this should be done after overlap removal)
  el_itr = (electrons_copy)->begin();
  el_end = (electrons_copy)->end();
  for( ; el_itr!=el_end; ++el_itr){
    MyDebug("selectObject()",Form("el: baseline=%d",(int)(*el_itr)->auxdata<char>("baseline")));
    MyDebug("selectObject()",Form("el: passOR=%d"  ,(int)(*el_itr)->auxdata<char>("passOR"  )));
    MyDebug("selectObject()",Form("el: signal=%d"  ,(int)(*el_itr)->auxdata<char>("signal"  )));
    if((*el_itr)->auxdata<char>("passOR")!=1){
      MyDebug("selectObject()","Electron was rejected by the overlap-removal process!!");
      continue;
    }
    if(IsMyBaselineElectron(**el_itr)) m_vec_baseElectron  ->push_back(**el_itr);
    if(IsMySignalElectron  (**el_itr)) m_vec_signalElectron->push_back(**el_itr);
  }
  //////////////////////////////////////////////////////////////////////
  // preparing signal and baseline muons (this should be done after overlap removal)
  mu_itr = (muons_copy)->begin();
  mu_end = (muons_copy)->end();
  Int_t nMu = mu_end-mu_itr;
  m_vec_baseMuon->reserve(nMu);
  m_vec_signalMuon->reserve(nMu);
  for(Int_t id=0; id<nMu; id++){
    xAOD::Muon mu;
    m_vec_baseMuon->push_back(mu);
    m_vec_signalMuon->push_back(mu);
    // Create private auxstore for the object later, copying all values
    // from the old object instead of just call push_back().
  }
  Int_t nBaselineMuon = 0;
  Int_t nSignalMuon = 0;
  for( ; mu_itr!=mu_end; ++mu_itr){
    MyDebug("selectObject()",Form("mu: baseline=%d",(int)(*mu_itr)->auxdata<char>("baseline")));
    MyDebug("selectObject()",Form("mu: passOR=%d"  ,(int)(*mu_itr)->auxdata<char>("passOR"  )));
    MyDebug("selectObject()",Form("mu: cosmic=%d"  ,(int)(*mu_itr)->auxdata<char>("cosmic"  )));
    MyDebug("selectObject()",Form("mu: signal=%d"  ,(int)(*mu_itr)->auxdata<char>("signal"  )));
    if(m_sel=="ac")passACCut_cosmic(**mu_itr);//Don't move this. This should be called after passACCut_jetClean();
    if((*mu_itr)->auxdata<char>("passOR")!=1){
      MyDebug("selectObject()","Muon was rejected by the overlap-removal process!!");
      m_vec_baseMuon  ->erase(m_vec_baseMuon  ->begin()+nBaselineMuon);
      m_vec_signalMuon->erase(m_vec_signalMuon->begin()+nSignalMuon  );
      continue;
    }
    if((*mu_itr)->auxdata<char>("cosmic")==1){
      MyDebug("selectObject()","Muon was rejected as cosmic muon candidate!!");
      m_vec_baseMuon  ->erase(m_vec_baseMuon  ->begin()+nBaselineMuon);
      m_vec_signalMuon->erase(m_vec_signalMuon->begin()+nSignalMuon  );
      continue;
    }
    if(IsMyBaselineMuon(**mu_itr)){
      m_vec_baseMuon->at(nBaselineMuon).makePrivateStore(**mu_itr);
      nBaselineMuon++;
    }else{
      m_vec_baseMuon->erase(m_vec_baseMuon->begin()+nBaselineMuon);
    }
    if(IsMySignalMuon(**mu_itr)){
      m_vec_signalMuon->at(nSignalMuon).makePrivateStore(**mu_itr);
      nSignalMuon++;
    }else{
      m_vec_signalMuon->erase(m_vec_signalMuon->begin()+nSignalMuon);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  // preparing signal, baseline and pre jets (this should be done after overlap removal)
  jet_itr = goodJets->begin();
  jet_end = goodJets->end();
  for( ; jet_itr!=jet_end; ++jet_itr){
    if((*jet_itr)->pt()>m_sigJetPtCut && fabs((*jet_itr)->eta())<m_sigJetEtaCut){
      if((*jet_itr)->auxdata<char>("signal")==1){
          m_vec_signalJet->push_back(**jet_itr);
      }
    }
  }
  MyDebug("selectObject()",
          Form("#signalJets=%d, #baseJets=%d, #preJets=%d",
               (Int_t)m_vec_signalJet->size(), (Int_t)m_vec_baseJet->size(),(Int_t)m_vec_preJet->size()) );

  //Deleting object containers
  if(electrons_copy   !=0) delete electrons_copy;
  if(electrons_copyaux!=0) delete electrons_copyaux;
  if(photons_copy     !=0) delete photons_copy;
  if(photons_copyaux  !=0) delete photons_copyaux;
  if(muons_copy       !=0) delete muons_copy;
  if(muons_copyaux    !=0) delete muons_copyaux;
  if(jets_copy        !=0) delete jets_copy;
  if(jets_copyaux     !=0) delete jets_copyaux;
  if(taus_copy        !=0) delete taus_copy;
  if(taus_copyaux     !=0) delete taus_copyaux;
  //  if(goodJets         !=0) delete goodJets; //deleted by m_store.clear() in MyxAODAnalysis.
  if(metcst           !=0) delete metcst;
  if(metcst_aux       !=0) delete metcst_aux;
  if(mettst           !=0) delete mettst;
  if(mettst_aux       !=0) delete mettst_aux;

  if(rtrvFail==true) return false;

  //Find candidate lepton indices ===============================================================
  for(Int_t id=0; id<nAnaLep; id++){
    m_baseLepIndex [id] = -1;
    m_baseLepFlavor[id] = -1;
    m_baseLeps     [id].SetPxPyPzE(0.,0.,0.,0.);
    m_baseLepIsSig [id] = -1;
    m_leadLepIndex [id] = -1;
    m_leadLepFlavor[id] = -1;
    m_leadLeps     [id].SetPxPyPzE(0.,0.,0.,0.);
  }
  //For baseline leptons
  MyDebug("selectObject()",Form("Baseline : elSize=%d, muSize=%d",(int)m_vec_baseElectron->size(),(int)m_vec_baseMuon->size()));
  for(UInt_t elIndex=0; elIndex<m_vec_baseElectron->size(); elIndex++){
    Double_t elPt = m_vec_baseElectron->at(elIndex).pt();
    Double_t elIsSig = IsMySignalElectron(m_vec_baseElectron->at(elIndex));
    for(Int_t id=0; id<nAnaLep; id++){
      if(elPt>m_baseLeps[id].Pt()){
        if(id!=nAnaLep-1){
          for(Int_t index=nAnaLep-1; id<index; index--){
            m_baseLepIndex [index] = m_baseLepIndex [index-1];
            m_baseLepFlavor[index] = m_baseLepFlavor[index-1];
            m_baseLeps     [index] = m_baseLeps     [index-1];
            m_baseLepIsSig [index] = m_baseLepIsSig [index-1];
          }
        }
        m_baseLepIndex [id] = elIndex;
        m_baseLepFlavor[id] = 0;
        m_baseLeps     [id] = m_vec_baseElectron->at(elIndex).p4();
        m_baseLepIsSig [id] = elIsSig;
        break;
      }else{
      }
    }
  }
  for(UInt_t muIndex=0; muIndex<m_vec_baseMuon->size(); muIndex++){
    Double_t muPt = m_vec_baseMuon->at(muIndex).pt();
    Double_t muIsSig = IsMySignalMuon(m_vec_baseMuon->at(muIndex));
    for(Int_t id=0; id<nAnaLep; id++){
      if(muPt>m_baseLeps[id].Pt()){
        if(id!=nAnaLep-1){
          for(Int_t index=nAnaLep-1; id<index; index--){
            m_baseLepIndex [index] = m_baseLepIndex [index-1];
            m_baseLepFlavor[index] = m_baseLepFlavor[index-1];
            m_baseLeps     [index] = m_baseLeps     [index-1];
            m_baseLepIsSig [index] = m_baseLepIsSig [index-1];
          }
        }
        m_baseLepIndex [id] = muIndex;
        m_baseLepFlavor[id] = 1;
        m_baseLeps     [id] = m_vec_baseMuon->at(muIndex).p4();
        m_baseLepIsSig [id] = muIsSig;
        break;
      }else{
      }
    }
  }
  //Check if well ordered by Pt
  if(m_dbg<=MSG::DEBUG){
    Double_t prevPt=9999999.;
    for(Int_t id=0; id<nAnaLep; id++){
      MyDebug("selectObject()",Form("Baseline : LeptonPt=%f, Flavor=%d, Index=%d (prevPt=%f)",m_baseLeps[id].Pt(),m_baseLepFlavor[id],m_baseLepIndex[id],prevPt));
      if(prevPt<=m_baseLeps[id].Pt() && m_baseLepFlavor[id]!=-1){
        MyError("selectObject()","Lepton Pt is not well ordered!!");
      }
      prevPt=m_baseLeps[id].Pt();
    }
  }

  //For signal leptons
  MyDebug("selectObject()",Form("Signal : elSize=%d, muSize=%d",(int)m_vec_signalElectron->size(),(int)m_vec_signalMuon->size()));
  for(UInt_t elIndex=0; elIndex<m_vec_signalElectron->size(); elIndex++){
    Double_t elPt = m_vec_signalElectron->at(elIndex).pt();
    for(Int_t id=0; id<nAnaLep; id++){
      if(elPt>m_leadLeps[id].Pt()){
        if(id!=nAnaLep-1){
          for(Int_t index=nAnaLep-1; id<index; index--){
            m_leadLepIndex [index] = m_leadLepIndex [index-1];
            m_leadLepFlavor[index] = m_leadLepFlavor[index-1];
            m_leadLeps     [index] = m_leadLeps     [index-1];
          }
        }
        m_leadLepIndex [id] = elIndex;
        m_leadLepFlavor[id] = 0;
        m_leadLeps     [id] = m_vec_signalElectron->at(elIndex).p4();
        break;
      }else{
      }
    }
  }
  for(UInt_t muIndex=0; muIndex<m_vec_signalMuon->size(); muIndex++){
    Double_t muPt = m_vec_signalMuon->at(muIndex).pt();
    for(Int_t id=0; id<nAnaLep; id++){
      if(muPt>m_leadLeps[id].Pt()){
        if(id!=nAnaLep-1){
          for(Int_t index=nAnaLep-1; id<index; index--){
            m_leadLepIndex [index] = m_leadLepIndex [index-1];
            m_leadLepFlavor[index] = m_leadLepFlavor[index-1];
            m_leadLeps     [index] = m_leadLeps     [index-1];
          }
        }
        m_leadLepIndex [id] = muIndex;
        m_leadLepFlavor[id] = 1;
        m_leadLeps     [id] = m_vec_signalMuon->at(muIndex).p4();
        break;
      }else{
      }
    }
  }
  //Check if well ordered by Pt
  if(m_dbg<=MSG::DEBUG){
    Double_t prevPt=9999999.;
    for(Int_t id=0; id<nAnaLep; id++){
      MyDebug("selectObject()",Form("Signal : LeptonPt=%f, Flavor=%d, Index=%d (prevPt=%f)",m_leadLeps[id].Pt(),m_leadLepFlavor[id],m_leadLepIndex[id],prevPt));
      if(prevPt<=m_leadLeps[id].Pt() && m_leadLepFlavor[id]!=-1){
        MyError("selectObject()","Lepton Pt is not well ordered!!");
      }
      prevPt=m_leadLeps[id].Pt();
    }
  }

  //Find candidate jet indices ===============================================================
  for(Int_t id=0; id<nAnaJet; id++){
    m_leadJetIndex [id] = -1;
    m_leadJets     [id].SetPxPyPzE(0.,0.,0.,0.);
  }
  //For signal jets
  MyDebug("selectObject()",Form("Signal Jet: Size=%d",(int)m_vec_signalJet->size()));
  for(UInt_t jetIndex=0; jetIndex<m_vec_signalJet->size(); jetIndex++){
    Double_t jetPt = m_vec_signalJet->at(jetIndex).pt();
    for(Int_t id=0; id<nAnaJet; id++){
      if(jetPt>m_leadJets[id].Pt()){
        if(id!=nAnaJet-1){
          for(Int_t index=nAnaJet-1; id<index; index--){
            m_leadJetIndex [index] = m_leadJetIndex [index-1];
            m_leadJets     [index] = m_leadJets     [index-1];
          }
        }
        m_leadJetIndex [id] = jetIndex;
        m_leadJets     [id] = m_vec_signalJet->at(jetIndex).p4();
        break;
      }else{
      }
    }
  }

  m_nSignalLeps = m_vec_signalElectron->size()+m_vec_signalMuon->size();
  m_nBaselineLeps = m_vec_baseElectron->size()+m_vec_baseMuon->size();
  m_nSignalJets = m_vec_signalJet->size();
  m_nBaselineJets = m_vec_baseJet->size();
  m_objReady = true;

  //For part of acceptance challenge
  if(m_sel=="ac"){
    if(b_passAC_badMuon && b_passAC_jetClean && b_passAC_primVtx && b_passAC_cosmic){
      passACCut_oneBaseLep();
      passACCut_oneSigLep ();
      passACCut_oneBaseJet();
      passACCut_oneSigJet ();
      passACCut_twoBaseLep();
      passACCut_twoSigLep ();
      passACCut_oneBaseEl ();
      passACCut_oneSigEl  ();
      passACCut_oneBaseMu ();
      passACCut_oneSigMu  ();
      passACCut_oneBaseTau();
      passACCut_oneSigTau ();
      passACCut_oneBjet   ();
    }else{
      b_passAC_oneBaseLep = false;
      b_passAC_oneSigLep = false;
      b_passAC_oneBaseJet = false;
      b_passAC_oneSigJet = false;
      b_passAC_twoBaseLep = false;
      b_passAC_twoSigLep = false;
      b_passAC_oneBaseEl = false;
      b_passAC_oneSigEl = false;
      b_passAC_oneBaseMu = false;
      b_passAC_oneSigMu = false;
      b_passAC_oneBaseTau = false;
      b_passAC_oneSigTau = false;
      b_passAC_oneBjet = false;
    }
  }

  return true;

}

/*--------------------------------------------------------------------------------*/
// Full event selection
/*--------------------------------------------------------------------------------*/
bool EventSelector::selectEvent()
{
#define PRINT_STEP(val) n_ ## val++; MyDebug("selectEvent()","n_"#val"++;");
  if(m_objReady==false){
    MyError("selectEvents()","EventSelector::selectEvent() was called before selectObject().");
    exit(1);
  }
  
  PRINT_STEP(initial);
  //   if(!passHfor()) return false;
  //   if(!passPeriod()) return false;
  //   // Event cleaning
  //   if(!passEventCleaning(preMuons, baseMuons, preJets, baseJets, met)) return false;
  PRINT_STEP(pass_clean);
  if(!passNBaseLepCut()) return false;
  PRINT_STEP(pass_nBLep);
  if(!passMllBaseCut()) return false;
  PRINT_STEP(pass_mllBase);
  if(!passNLepCut()) return false;
  PRINT_STEP(pass_nLep);
  //   if(!passNTauCut(signalTaus, tightTaus)) return false;
  PRINT_STEP(pass_nTau);

  //   // Optionally treat base leptons as signal leptons 
  //   const LeptonVector& mySigLeptons = m_useBaseLeps? baseLeps : signalLeptons;

  //   if(!passTrigger(mySigLeptons, signalTaus, met)) return false;
  PRINT_STEP(pass_trig);
  if(!passLepTruthCut()) return false;
  PRINT_STEP(pass_truth);
  if(!passFlavChargeCut()) return false;
  PRINT_STEP(pass_sfos);
  //if(!passLooseZCut(mySigLeptons)) return false; // not used
  //n_pass_lz++;
  if(!passNJetCut()) return false;
  PRINT_STEP(pass_jet);

  if(!passVbfCut()) return false;
  PRINT_STEP(pass_vbf);

  if(!passLepTauPtCuts()) return false;
  PRINT_STEP(pass_lepPt);
  if(!passBaseLepEtaCut()) return false;
  if(!pass1stBaseIsSignal()) return false;

  if(!passUpsilonCut()) return false;
  if(!passZCut()) return false;
  if(!passZeeSSCut()) return false;
  if(!passMllCut()) return false;
  if(!passMsfosCut()) return false;
  if(!passMinMsfosCut()) return false;
  if(!passMlllCut()) return false;
  //   if(!passMltCut(mySigLeptons, signalTaus)) return false;
  //   if(!passMttCut(signalTaus)) return false;
  PRINT_STEP(pass_z);

  if(!passLepDEtaCut()) return false;
  if(!passMinDPhiOSCut()) return false;
  PRINT_STEP(pass_lepDR);

  if(!passMetCut()) return false;
  if(!passMetRelCut()) return false;
  PRINT_STEP(pass_met);

  if(!passMtCut()) return false;
  if(!passMaxMT2Cut()) return false;
  PRINT_STEP(pass_mt);

  if(!passMeffCut()) return false;
  PRINT_STEP(pass_meff);

  //if(!passMljjCut(mySigLeptons, signalJets)) return false;
  // PRINT_STEP(pass_mljj);
  //if(!passMtllCut(mySigLeptons, met)) return false;
  // PRINT_STEP(pass_other);

#undef PRINT_STEP

  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_badMuon   ()
{
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_jetClean  (xAOD::Jet& jet){
  if(not (b_passAC_badMuon)){
    b_passAC_jetClean = false;
    return false;
  }
  if(jet.auxdata<char>("baseline")==1 && jet.auxdata<char>("passOR")==1){
    if(jet.pt()>m_baseJetPtCut && fabs(jet.eta())<m_baseJetEtaCut){
      //This "if" is needed since "false" will be returned when we input a jet with pt<m_baseJetPtCut to IsGoodJet()...
      if(m_susyObjTool->IsBadJet(jet)){ // bool IsBadJet(const xAOD::Jet& input, float jvtcut=0.64);
        b_passAC_jetClean = false;
        return false;
      }
    }
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_primVtx   (){
  if(not (b_passAC_badMuon && b_passAC_jetClean)){
    b_passAC_primVtx = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_cosmic    (xAOD::Muon& mu){
  if(not (b_passAC_badMuon && b_passAC_jetClean && b_passAC_primVtx)){
    b_passAC_cosmic = false;
    return false;
  }
  if(mu.auxdata<char>("baseline")==1 && mu.auxdata<char>("passOR")==1 && mu.auxdata<char>("cosmic")==1){
    b_passAC_cosmic = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneBaseLep(){
  if(nBaselineLeps()!=1){
    b_passAC_oneBaseLep = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneSigLep (){
  if(nSignalLeps()!=1){
    b_passAC_oneSigLep = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneBaseJet(){
  if(nBaselineJets()!=1){
    b_passAC_oneBaseJet = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneSigJet (){
  if(nSignalJets()!=1){
    b_passAC_oneSigJet = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_twoBaseLep(){
  if(nBaselineLeps()!=2){
    b_passAC_twoBaseLep = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_twoSigLep (){
  if(nSignalLeps()!=2){
    b_passAC_twoSigLep = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneBaseEl (){
  if(m_vec_baseElectron->size()!=1){
    b_passAC_oneBaseEl = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneSigEl  (){
  if(m_vec_signalElectron->size()!=1){
    b_passAC_oneSigEl = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneBaseMu (){
  if(m_vec_baseMuon->size()!=1){
    b_passAC_oneBaseMu = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneSigMu  (){
  if(m_vec_signalMuon->size()!=1){
    b_passAC_oneSigMu = false;
    return false;  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneBaseTau(){
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneSigTau (){
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passACCut_oneBjet   (){
  Int_t numBJets = 0;
  for(UInt_t cnt=0; cnt<m_vec_signalJet->size(); cnt++){
    if((m_vec_signalJet->at(cnt)).auxdata<char>("bjet")){
      numBJets++;
    }
  }
  if(numBJets!=1){
    b_passAC_oneBjet = false;
    return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/

// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passHfor()
// {
//   if(m_nt->evt()->hfor==4) return false;
//   return true;
// }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passPeriod()
// {
//   if(!m_nt->evt()->isMC){
//     uint run = m_nt->evt()->run;
//     if(m_doAB3 && !isPeriodAB3(run)) return false;
//   }
//   return true;
// }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passEventCleaning(const MuonVector& preMuons, const MuonVector& baseMuons,
//                                       const JetVector& preJets, const JetVector& baseJets,
//                                       const Met* met)
// {
//   const Event* evt = m_nt->evt();
//   //int cutFlag = m_nt->evt()->cutFlags[NtSys_NOM];
//   int cutFlag = cleaningCutFlags(evt->cutFlags[NtSys_NOM], preMuons, baseMuons, preJets, baseJets);
//   if(!passLAr(cutFlag)) return false;
//   if(!passHotSpot(cutFlag)) return false;
//   n_pass_hotSpot++;
//   if(!passBadJet(cutFlag)) return false;
//   n_pass_badJet++;
//   if(!passDeadRegions(preJets, met, evt->run, evt->isMC)) return false;
//   n_pass_feb++;
//   if(!passBadMuon(cutFlag)) return false;
//   n_pass_badMu++;
//   if(!passCosmic (cutFlag)) return false;
//   n_pass_cosmic++;
//   return true;
// }
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
// Lepton/jet multiplicity
/*--------------------------------------------------------------------------------*/
bool EventSelector::passNBaseLepCut()
{
  if(m_nBaseLepMin>=0 || m_nBaseLepMax>=0){
    if(m_nBaseLepMin>=0 && nBaselineLeps() < m_nBaseLepMin) return false;
    if(m_nBaseLepMax>=0 && nBaselineLeps() > m_nBaseLepMax) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passNSignalLepCut()
{
  if(m_nLepMin>=0 && nSignalLeps() < m_nLepMin) return false;
  if(m_nLepMax>=0 && nSignalLeps() > m_nLepMax) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passLepChan()
{
  if(m_nEleMin>=0 || m_nEleMax>=0){
    int nEle = m_vec_signalElectron->size();
    if(m_nEleMin>=0 && nEle < m_nEleMin) return false;
    if(m_nEleMax>=0 && nEle > m_nEleMax) return false;
  }
  //why only for electron?
  return true;
}
// /*--------------------------------------------------------------------------------*/
bool EventSelector::passNLepCut()
{
  //  if(!passNBaseLepCut()) return false; //called before in this function
  if(!passNSignalLepCut()) return false;
  if(!passLepChan()) return false;
  /*
  // signal leptons
  int nLep = signalLeptons.size();
  if(m_nLepMin>=0 && nLep < m_nLepMin) return false;
  if(m_nLepMax>=0 && nLep > m_nLepMax) return false;

  // optional cut on baseline leptons as well
  if(m_nBaseLepMin>=0 || m_nBaseLepMax>=0){
  int nBaseLep = baseLeptons.size();
  if(m_nBaseLepMin>=0 && nBaseLep < m_nBaseLepMin) return false;
  if(m_nBaseLepMax>=0 && nBaseLep > m_nBaseLepMax) return false;
  }

  // optional cut on number electrons, a way to select lepton channel
  if(m_nEleMin>=0 || m_nEleMax>=0){
  int nEle = 0;
  for(int iL=0; iL<nLep; iL++){
  if(signalLeptons[iL]->isEle()) nEle++;
  }
  if(m_nEleMin>=0 && nEle < m_nEleMin) return false;
  if(m_nEleMax>=0 && nEle > m_nEleMax) return false;
  }
  */
  return true;
}
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passNTauCut(const TauVector& signalTaus, const TauVector& tightTaus)
// {
//   int nTau = signalTaus.size();
//   if(m_nTauMin>=0 && nTau < m_nTauMin) return false;
//   if(m_nTauMax>=0 && nTau > m_nTauMax) return false;
//   int nTightTau = tightTaus.size();
//   if(m_nTightTauMin>=0 && nTightTau < m_nTightTauMin) return false;
//   if(m_nTightTauMax>=0 && nTightTau > m_nTightTauMax) return false;
//   //int nBaseTau = baseTaus.size();
//   //if(m_nBaseTauMin>=0 && nBaseTau < m_nBaseTauMin) return false;
//   //if(m_nBaseTauMax>=0 && nBaseTau > m_nBaseTauMax) return false;
//   return true;
// }
/*--------------------------------------------------------------------------------*/
bool EventSelector::passNJetCut()
{
  if(m_nJetMin>=0 && nSignalJets() < m_nJetMin) return false;
  if(m_nJetMax>=0 && nSignalJets() > m_nJetMax) return false;

  if(nSignalJets()>0){
    if(m_jetPtMin>0 && m_vec_signalJet->at(0).pt()/1000. < m_jetPtMin) return false;
    if(m_jetPtMax>0 && m_vec_signalJet->at(0).pt()/1000. > m_jetPtMax) return false;
  }

  // 3-lepton b-jet
  // if(m_vetoB || m_selB){
  //   bool hasB = hasBJet();
  //   if(m_vetoB && hasB) return false;
  //   if(m_selB && !hasB) return false;
  // }
  if(m_nBJetMin >= 0 || m_nBJetMax >= 0){
    int nBJet = numBJets();
    if(m_nBJetMin>=0 && nBJet < m_nBJetMin) return false;
    if(m_nBJetMax>=0 && nBJet > m_nBJetMax) return false;
  }

  // // 2-lepton forward jets
  // if(m_nFJetMax >= 0){
  //   int nFJets = numberOfFJets(jets);
  //   if(nFJets > m_nFJetMax) return false;
  // }
  // if(m_nCBJetMax >= 0){
  //   int nCBJets = numberOfCBJets(jets);
  //   if(nCBJets > m_nCBJetMax) return false;
  // }
  // if(m_nCLJetMin >= 0 || m_nCLJetMax >= 0){
  //   int nCLJets = numberOfCLJets(jets);
  //   if(m_nCLJetMin >= 0 && nCLJets < m_nCLJetMin) return false;
  //   if(m_nCLJetMax >= 0 && nCLJets > m_nCLJetMax) return false;
  // }
  return true;
}
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passNBJetCut(const JetVector& jets)
// {
//   /*
//   if(m_vetoB || m_selB){
//     bool hasB = hasBJet(jets);
//     if(m_vetoB && hasB) return false;
//     if(m_selB && !hasB) return false;
//   }
//   if(m_nBJetMin >= 0 || m_nBJetMax >= 0){
//     int nBJet = numBJets(jets);
//     if(m_nBJetMin>=0 && nBJet < m_nBJetMin) return false;
//     if(m_nBJetMax>=0 && nBJet > m_nBJetMax) return false;
//   }
//   //std::cout << "EVENT " << m_nt->evt()->event << std::endl;
//   */
//   return true;
// }

/*--------------------------------------------------------------------------------*/
// Flavor and charge requirements
/*--------------------------------------------------------------------------------*/
bool EventSelector::passSFOSCut()
{
  if(m_selSFOS || m_vetoSFOS){
    bool sfos = hasSFOS();
    if(m_vetoSFOS && sfos) return false;
    if(m_selSFOS && !sfos) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passSFSSCut()
{
  if(m_selSFSS || m_vetoSFSS){
    bool sfss = hasSFSS();
    if(m_vetoSFSS && sfss) return false;
    if(m_selSFSS && !sfss) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passOFOSCut()
{
  if(m_selOFOS || m_vetoOFOS){
    bool ofos = hasOFOS();
    if(m_vetoOFOS && ofos) return false;
    if(m_selOFOS && !ofos) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passDFSSCut()
{
  //Select events with different flavor or same sign leptons.
  if(m_selDForSS4SigLep || m_vetoDForSS4SigLep){
    bool df = false;
    if((m_leadLepFlavor[0]==0 && m_leadLepFlavor[1]==1) ||
       (m_leadLepFlavor[0]==1 && m_leadLepFlavor[1]==0) ) df = true;
    Int_t charge1(0), charge2(0);
    if     (m_leadLepFlavor[0]==0) charge1 = (m_vec_signalElectron->at(m_leadLepIndex[0])).charge();
    else if(m_leadLepFlavor[0]==1) charge1 = (m_vec_signalMuon    ->at(m_leadLepIndex[0])).charge();
    if     (m_leadLepFlavor[1]==0) charge2 = (m_vec_signalElectron->at(m_leadLepIndex[1])).charge();
    else if(m_leadLepFlavor[1]==1) charge2 = (m_vec_signalMuon    ->at(m_leadLepIndex[1])).charge();
    bool ss = isSS(charge1,charge2);
    if(m_selDForSS4SigLep && not (df || ss)) return false;
    if(m_vetoDForSS4SigLep && (df || ss)) return false;
  }
  //Select events with different flavor and same sign leptons.
  if(m_selDFandSS4SigLep || m_vetoDFandSS4SigLep){
    bool dfss = false;
    Int_t charge1(0), charge2(0);
    if     (m_leadLepFlavor[0]==0) charge1 = (m_vec_signalElectron->at(m_leadLepIndex[0])).charge();
    else if(m_leadLepFlavor[0]==1) charge1 = (m_vec_signalMuon    ->at(m_leadLepIndex[0])).charge();
    if     (m_leadLepFlavor[1]==0) charge2 = (m_vec_signalElectron->at(m_leadLepIndex[1])).charge();
    else if(m_leadLepFlavor[1]==1) charge2 = (m_vec_signalMuon    ->at(m_leadLepIndex[1])).charge();
    if( ( (m_leadLepFlavor[0]==0 && m_leadLepFlavor[1]==1) && (charge1==-1 && charge2==-1) ) ||
        ( (m_leadLepFlavor[0]==0 && m_leadLepFlavor[1]==1) && (charge1== 1 && charge2== 1) ) ||
        ( (m_leadLepFlavor[0]==1 && m_leadLepFlavor[1]==0) && (charge1==-1 && charge2==-1) ) ||
        ( (m_leadLepFlavor[0]==1 && m_leadLepFlavor[1]==0) && (charge1== 1 && charge2== 1) ) ) dfss = true;
    if(m_selDFandSS4SigLep && !dfss) return false;
    if(m_vetoDFandSS4SigLep && dfss) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passSSCut()
{
  if(m_selSS){
    Int_t charge1(0), charge2(0);
    if     (m_leadLepFlavor[0]==0) charge1 = (m_vec_signalElectron->at(m_leadLepIndex[0])).charge();
    else if(m_leadLepFlavor[0]==1) charge1 = (m_vec_signalMuon    ->at(m_leadLepIndex[0])).charge();
    if     (m_leadLepFlavor[1]==0) charge2 = (m_vec_signalElectron->at(m_leadLepIndex[1])).charge();
    else if(m_leadLepFlavor[1]==1) charge2 = (m_vec_signalMuon    ->at(m_leadLepIndex[1])).charge();
    bool ss = isSS(charge1,charge2);
    if(m_selSS && !ss) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passOSCut()
{
  if(m_selOS && nSignalLeps() >= 2){
    Int_t charge1(0), charge2(0);
    if     (m_leadLepFlavor[0]==0) charge1 = (m_vec_signalElectron->at(m_leadLepIndex[0])).charge();
    else if(m_leadLepFlavor[0]==1) charge1 = (m_vec_signalMuon    ->at(m_leadLepIndex[0])).charge();
    if     (m_leadLepFlavor[1]==0) charge2 = (m_vec_signalElectron->at(m_leadLepIndex[1])).charge();
    else if(m_leadLepFlavor[1]==1) charge2 = (m_vec_signalMuon    ->at(m_leadLepIndex[1])).charge();
    bool os = isOS(charge1, charge2);
    if(m_selOS && !os) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passSFCut()
{
  if(m_selSF && nSignalLeps() >= 2){
    bool sf = isSF(m_leadLepFlavor[0],m_leadLepFlavor[1]);
    if(m_selSF && !sf) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passDFCut()
{
  if(m_selDF && nSignalLeps() >= 2){
    bool sf = isDF(m_leadLepFlavor[0],m_leadLepFlavor[1]);
    if(m_selDF && !sf) return false;
  }
  return true;
}
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passOSLepTauCut(const LeptonVector& leptons, const TauVector& taus)
// {
//   if(m_selOSTau){
//     bool os = hasOS(taus);
//     if(m_selOSTau && !os) return false;
//   }
//   if(m_selOSLepTau){
//     bool hasOSLepTau = false;
//     for(uint iTau=0; iTau<taus.size(); iTau++){
//       const Tau* tau = taus[iTau];
//       for(uint iL=0; iL<leptons.size(); iL++){
//         const Lepton* lep = leptons[iL];
//         if(lep->q*tau->q < 0){
//           hasOSLepTau = true;
//           break;
//         }
//       }
//     }
//     if(!hasOSLepTau) return false;
//   }
//   return true;
// }
/*--------------------------------------------------------------------------------*/
bool EventSelector::passFlavChargeCut()
{
  // TODO: split these into sub-methods, but keep this method as well.
  if(!passSFOSCut()) return false;
  if(!passSFSSCut()) return false;
  if(!passOFOSCut()) return false;
  if(!passDFSSCut()) return false;
  if(!passSSCut()) return false;
  if(!passOSCut()) return false;
  if(!passSFCut()) return false;
  if(!passDFCut()) return false;
  //  if(!passOSLepTauCut(leptons, taus)) return false;
  return true;
}

// /*--------------------------------------------------------------------------------*/
// // Trigger cut
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passEventTrigger()
// {
//   if(m_applyTrig){
//     if(m_use2LTrig){
//       // Not currently applying an event-level flag cut for 2LepWH
//     }
//     else{
//       if(!m_3lTrigLogic->passEventTrigger(m_nt->evt())) return false;
//     }
//   }
//   return true;
// }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passTrigger(const LeptonVector& leptons, const TauVector& taus, const Met* met)
// {
//   if(m_applyTrig){
//     if(m_use2LTrig){
//       if(!m_2lTrigLogic->passDilTrig(leptons, met->Et, m_nt->evt())) return false;
//     }
//     else{
//       if(!m_3lTrigLogic->passTriggerMatching(leptons, taus, m_nt->evt(), m_useDilepTrigs)) return false;
//     }
//   }
//   return true;
// }

/*--------------------------------------------------------------------------------*/
// MET requirements
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMetCut()
{
  if(m_metMin >= 0 && m_met.Mod()/1000. < m_metMin) return false;
  if(m_metMax >= 0 && m_met.Mod()/1000. > m_metMax) return false;
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMetRelCut()
{
  if(m_metRelMin > 0){
    float metRel = getMetRel();
    if(m_metRelMin > 0 && metRel < m_metRelMin) return false;
  }
  return true;
}

/*--------------------------------------------------------------------------------*/
// VBF requirements
/*--------------------------------------------------------------------------------*/
bool EventSelector::passVbfCut()
{
  //tentatively using baseline jets
  if(nBaselineJets() >= 2){
    TLorentzVector jets[2];
    for(Int_t cnt=0; cnt<2; cnt++){
      jets[cnt].SetPtEtaPhiE(m_vec_baseJet->at(cnt).pt(), m_vec_baseJet->at(cnt).eta(),
                             m_vec_baseJet->at(cnt).phi(), m_vec_baseJet->at(cnt).e() );
    }
    float dijetM        = (jets[0]+jets[1]).M();
    float dEta          = fabs(jets[0].Eta()-jets[1].Eta());
    float productEtaEta = jets[0].Eta()*jets[1].Eta();
    // std::cout << "dijetM=" << dijetM << ", dEta=" << dEta << ", Eta1*Eta2=" << productEtaEta << std::endl;
    // std::cout << m_mjjMin << ", " << m_mjjMax << std::endl;
    // std::cout << m_dEtaJJMin << ", " << m_dEtaJJMax << std::endl;
    // std::cout << m_productEtaEtaMin << ", " << m_productEtaEtaMax << std::endl;
    if(m_mjjMin >= 0 && dijetM < m_mjjMin) return false;
    if(m_mjjMax >= 0 && dijetM > m_mjjMax) return false;
    if(m_dEtaJJMin >= 0 && dEta < m_dEtaJJMin) return false;
    if(m_dEtaJJMax >= 0 && dEta > m_dEtaJJMax) return false;
    if(m_productEtaEtaMin >= 0 && productEtaEta < m_productEtaEtaMin) return false;
    if(m_productEtaEtaMax >= 0 && productEtaEta > m_productEtaEtaMax) return false;
  }else{
    if(m_mjjMin > 0 || m_mjjMax > 0 || m_dEtaJJMin > 0 || m_dEtaJJMax > 0 ||
       m_productEtaEtaMin > 0 || m_productEtaEtaMax > 0) return false;
  }
  return true;
}
//end adding

/*--------------------------------------------------------------------------------*/
// Lepton/tau invariant mass requirements
/*--------------------------------------------------------------------------------*/
bool EventSelector::passZCut()
{
  if(m_vetoZ || m_selZ){
    bool z = hasZ();
    if(m_vetoZ && z) return false;
    if(m_selZ && !z) return false;
  }
  if(m_vetoExtZ || m_selExtZ){
    bool z = hasZlll() || hasZllll();
    if(m_vetoExtZ && z) return false;
    if(m_selExtZ && !z) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passUpsilonCut()
{
  if(m_vetoUpsilon){
    bool upsi = hasUpsilon();
    if(m_vetoUpsilon && upsi) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::isLooseSoftEle(unsigned int id, bool isSignal)
{
  // if(e->Pt() < 6.) return false;
  // if(fabs(e->d0Sig(true)) > ELECTRON_D0SIG_CUT_WH) return false;
  // if(fabs(e->z0SinTheta(true)) > ELECTRON_Z0_SINTHETA_CUT) return false;
  // // require lepton to be separated from all standard same-flav leptons
  // for(uint iL=0; iL < stdLeptons.size(); iL++){
  //   const Lepton* stdLep = stdLeptons[iL];
  //   if(e == stdLep) return false;
  //   //if(isSameFlav(e, stdLep) && e->DeltaR(*stdLep) < 0.05) return false;
  //   if(e->DeltaR(*stdLep) < 0.05) return false;
  // }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::isLooseSoftMu(unsigned int id, bool isSignal)
{
  std::vector< xAOD::Muon > *vec_muon = (isSignal ? m_vec_signalMuon : m_vec_baseMuon);
  if(vec_muon->size() <= id) return false;
  if((vec_muon->at(id).pt()/1000.) < 6.) return false;
  const xAOD::TrackParticle* track = vec_muon->at(id).primaryTrackParticle();
  Double_t mu_d0sig      = 0.;
  Double_t mu_z0sinTheta = 0.;
  if(track){
    Double_t mu_d0 = track->d0();
    Double_t vard0 = track->definingParametersCovMatrix()(0,0);
    if(vard0 > 0){
      Double_t mu_d0error = 0.;
      mu_d0error=TMath::Sqrt(vard0);
      mu_d0sig  = mu_d0/mu_d0error;
    }
    Double_t mu_z0 = track->z0();
    Double_t mu_theta = track->theta();
    mu_z0sinTheta = mu_z0*TMath::Sin(mu_theta);
  }
  if(TMath::Abs(mu_d0sig)      > MUON_D0SIG_CUT      ) return false;
  if(TMath::Abs(mu_z0sinTheta) > MUON_Z0_SINTHETA_CUT) return false;
  // require lepton to be separated from all standard same-flav leptons
  // for(uint iL=0; iL < stdLeptons.size(); iL++){
  //   const Lepton* stdLep = stdLeptons[iL];
  //   if(mu == stdLep) return false;
  //   //if(isSameFlav(mu, stdLep) && mu->DeltaR(*stdLep) < 0.05) return false;
  //   if(mu->DeltaR(*stdLep) < 0.05) return false;
  // }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passLooseZCut()
{
  if(m_vetoLooseZ){
    // for(uint iL=0; iL < leptons.size(); iL++){
    //   const Lepton* lep = leptons[iL];
    //   if(lep->isEle()){
    //     // Loop over all SusyNt electrons
    //     for(uint jEl=0; jEl < m_nt->ele()->size(); jEl++){
    //       const Electron* e2 = & m_nt->ele()->at(jEl);
    //       if(lep != e2 && isLooseSoftEle(e2, leptons) && isZ(e2, lep, 20.)){
    //         return false;
    //       }
    //     }
    //   }
    //   else{
    //     // Loop over all SusyNt muons
    //     for(uint jMu=0; jMu < m_nt->muo()->size(); jMu++){
    //       const Muon* mu2 = & m_nt->muo()->at(jMu);
    //       if(lep != mu2 && isLooseSoftMu(mu2, leptons) && isZ(mu2, lep, 20.)){
    //         return false;
    //       }
    //     }
    //   }
    // } // loop over leptons
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passZeeSSCut()
{
  if(m_vetoZeeSS){
    Double_t massWindow = m_windowZ;
    TLorentzVector leps[2];
    Int_t          charge[2];
    std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
    for(uint iL=0; iL<vec_electron->size(); iL++){
      leps[0] = vec_electron->at(iL).p4();
      charge[0] = vec_electron->at(iL).charge();
      for(uint iL2=0; iL2<vec_electron->size(); iL2++){
        if(iL2<=iL) continue;
        charge[1] = vec_electron->at(iL2).charge();
        if(isSS(charge[0], charge[1])){
          leps[1] = vec_electron->at(iL2).p4();
          if(isInMassWindow( (leps[0]+leps[1]).M()/1000., MZ, massWindow)) return false;
        }
      }
    }
  }

  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMllBaseCut()
{
  // Use the two leading leptons, regardless of flav or charge
  if((m_mllBaseMin > 0) && nBaselineLeps() >= 2){
    float mll = (m_baseLeps[0]+m_baseLeps[1]).M()/1000.;
    if(m_mllBaseMin > 0 && mll < m_mllBaseMin) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMllCut()
{
  // Use the two leading leptons, regardless of flav or charge
  if( (m_mllMin > 0 || m_mllMax > 0 || m_vetoMllMin > 0) && nSignalLeps() >= 2){
    float mll = (m_leadLeps[0]+m_leadLeps[1]).M()/1000.;
    if(m_mllMin > 0 && mll < m_mllMin) return false;
    if(m_mllMax > 0 && mll > m_mllMax) return false;
    if(m_vetoMllMin > 0 && (m_vetoMllMin < mll && mll < m_vetoMllMax)) 
      return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMsfosCut()
{
  // Use the sfos pair closest to the Z mass for the msfos cut.
  // This is a little arbitrary when there are 2 sfos pairs, but
  // apparently the sensitivity is better than considering all sfos pairs
  if(m_msfosMin >= 0 || m_msfosMax >= 0){
    int index[2]={-1,-1};
    int flav=-1;
    Double_t msfos = findBestMSFOS(index[0],index[1],flav)/1000.;
    if( ((m_msfosMin >= 0) || (m_msfosMax >= 0)) && msfos<0) return false; //cannot find sfos pair
    if(m_msfosMin >= 0 && msfos < m_msfosMin) return false;
    if(m_msfosMax >= 0 && msfos > m_msfosMax) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMinMsfosCut()
{
  // Use the sfos pair with minimum mass for this cut.
  // This is a little arbitrary when there are 2 sfos pairs, but
  // apparently the sensitivity is better than considering all sfos pairs
  if(m_msfosMin >= 0 || m_msfosMax >= 0){
    int index[2]={-1,-1};
    int flav=-1;
    Double_t msfos = findBestMSFOS(index[0],index[1],flav,0.)/1000.;//Finding min. M_SFOS
    if( ((m_minMllMin >= 0) || (m_minMllMax >= 0)) && msfos<0) return false; //cannot find sfos pair
    if(m_minMllMin >= 0 && msfos < m_minMllMin) return false;
    if(m_minMllMax >= 0 && msfos > m_minMllMax) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMlllCut()
{
  if( nSignalLeps()>=3 && (m_mlllMin >= 0 || m_mlllMax >= 0) ){
    TLorentzVector lep[3];
    Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
    Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
    for(uint id=0; id<3; id++) lep[id] = getFourVector(lepIndex[id],lepFlavor[id]);
    float mlll = (lep[0]+lep[1]+lep[2]).M()/1000.;
    if(m_mlllMin >= 0 && mlll < m_mlllMin) return false;
    if(m_mlllMax >= 0 && mlll > m_mlllMax) return false;
  }
  return true;
}
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passMltCut()
// {
//   // For now, this cut only makes sense for the 2Lep 1Tau channel
//   // So I will restrict myself to that
//   if(m_mltMax > 0){
//     uint nLep = leptons.size();
//     uint nTau = taus.size();
//     if(nLep == 2 && nTau == 1){
//       // Use invariant mass between 2nd lepton and tau
//       float mlt = Mll(leptons[1], taus[0]);
//       if(mlt > m_mltMax) return false;
//     }
//   }
//   return true;
// }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passMttCut(const TauVector& taus)
// {
//   if(m_mttMin > 0 || m_mttMax > 0){
//     uint nTau = taus.size();
//     for(uint iT=0; iT<nTau; iT++){
//       for(uint jT=iT+1; jT<nTau; jT++){
//         float mtt = Mll(taus[iT], taus[jT]);
//         if(m_mttMin > 0 && mtt < m_mttMin) return false;
//         if(m_mttMax > 0 && mtt > m_mttMax) return false;
//       }
//     }
//   }
//   return true;
// }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passMassCuts(const LeptonVector& leptons, const TauVector& taus)
// {
//   if(!passMllCut(leptons)) return false;
//   if(!passZCut(leptons)) return false;
//   if(!passZeeSSCut(leptons)) return false;
//   if(!passMsfosCut(leptons)) return false;
//   if(!passMlllCut(leptons)) return false;
//   if(!passMltCut(leptons, taus)) return false;
//   if(!passMttCut(taus)) return false;
//   return true;
// }
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMljjCut()
{
  // if(m_mljjMax > 0 || m_mljjMin > 0){
  //   if(nSignalLeps() >= 2 && nSignalJets() >= 2){
  //     float m = getMljj();
  //     if(m_mljjMax > 0 && m > m_mljjMax) return false;
  //     if(m_mljjMin > 0 && m < m_mljjMin) return false;
  //   }
  // }
  return true;
}

/*--------------------------------------------------------------------------------*/
// Transverse mass, etc.
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMtCut()
{
  if(m_mtMin > 0 || m_mtMax > 0){
    uint nLep = nSignalLeps();
    // Find the best Z candidate pair, use remaining lepton to form Mt
    Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
    Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
    if(nLep==3){
      int index[2]={-1,-1};
      int flav=-1;
      findBestMSFOS(index[0],index[1],flav);
      if(flav==-1) return false;
      else{
        Int_t lepWIndex=-1;
        if     (index[0]==0 && index[1]==1) lepWIndex=2;
        else if(index[0]==0 && index[1]==2) lepWIndex=1;
        else if(index[0]==1 && index[1]==2) lepWIndex=0;
        TLorentzVector lep = getFourVector(lepIndex[lepWIndex], lepFlavor[lepWIndex]);
        float mt = getMt(lep,m_met);
        if(m_mtMin > 0 && mt < m_mtMin) return false;
        if(m_mtMax > 0 && mt > m_mtMax) return false;
      }
    }
    // For 2 leptons, use maximum mt
    else if(nLep==2){
      TLorentzVector lep[2];
      for(uint id=0; id<2; id++) lep[id] = getFourVector(lepIndex[id], lepFlavor[id]);
      float mt = TMath::Max(getMt(lep[0], m_met), getMt(lep[1], m_met));
      if(m_mtMin > 0 && mt < m_mtMin) return false;
      if(m_mtMax > 0 && mt > m_mtMax) return false;
    }
    // General case, use all leptons
    else{
      std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
      std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
      for(uint iL=0; iL<vec_electron->size(); iL++){
        float mt = getMt(vec_electron->at(iL).p4(),m_met);
        if(m_mtMin > 0 && mt < m_mtMin) return false;
        if(m_mtMax > 0 && mt > m_mtMax) return false;
      }
      for(uint iL=0; iL<vec_muon    ->size(); iL++){
        float mt = getMt(vec_muon->at(iL).p4(),m_met);
        if(m_mtMin > 0 && mt < m_mtMin) return false;
        if(m_mtMax > 0 && mt > m_mtMax) return false;
      }
    }
  }
  return true;
}
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passMtllCut(const LeptonVector& leptons, const Met* met)
// {
//   if(m_mtllMin > 0 && leptons.size()>=2){
//     TLorentzVector lv = *leptons[0] + *leptons[1];
//     float mt = Mt(&lv, met);
//     if(mt < m_mtllMin) return false;
//   }
//   return true;
// }
/*--------------------------------------------------------------------------------*/
float EventSelector::getMaxMT2()
{
  TLorentzVector metp = TLorentzVector( m_met.X(), m_met.Y(), 0., 0.);
  Double_t invismassA = 0.;
  Double_t invismassB = 0.;

  uint nLep = nSignalLeps();
  // uint nTau = taus.size();

  // ** for 2 tau
  // double mT2TT=-100;
  // double mT2L1T2=-100;
  // double mT2L1T1=-100;
  double mT2LL=-100;  // initial dummy value
  // double mT2L2T1 = -100;
 
  // if(nTau >=2) 
  //   mT2TT = getMT2(taus[0], taus[1], met); // for 2tau exploration
  // if(nLep >=1 && nTau >=2)
  //   mT2L1T2 = getMT2(leptons[0], taus[1], met);
  // if(nLep >=1 && nTau >=1)
  //   mT2L1T1 = getMT2(leptons[0], taus[0], met);
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  if(nLep >=2){
    ComputeMT2 mt2class = ComputeMT2(getFourVector(lepIndex[0], lepFlavor[0]),
                                     getFourVector(lepIndex[1], lepFlavor[1]),
                                     metp,invismassA,invismassB);// for ss exploration and os
    mT2LL = mt2class.Compute();
  }
  // if(nLep >=2 && nTau>=1)
  //   mT2L2T1 = getMT2(leptons[1], taus[0], met);
 
  // ** finding the maximum combinations
  // double mT2LT = mT2L1T1;  // the maximum out of the L&T combinations
  // if( mT2L1T2 > mT2LT ) mT2LT = mT2L1T2; // 2tau case
  // if( mT2L2T1 > mT2LT ) mT2LT = mT2L2T1; // 1tau case

  double mT2max = mT2LL; // the maximum case out of L&L or T&T or any!
  // if( mT2TT > mT2max ) mT2max = mT2TT; // in case of 2taus
  // if( mT2LT > mT2max ) mT2max = mT2LT; // check for abs max

  return mT2max;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMaxMT2Cut()
{
  if(m_maxMT2Min > 0 || m_maxMT2Max >= 0){
    float maxMT2 = getMaxMT2();
    if(m_maxMT2Min > 0 && maxMT2 < m_maxMT2Min) return false;
    if(m_maxMT2Max >= 0 && maxMT2 > m_maxMT2Max) return false;
  }
  return true;
}

/*--------------------------------------------------------------------------------*/
// Meff
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMeffCut()
{
  if(m_meffMin >= 0 || m_meffMax >= 0){
    float meff = getMeff();
    if(m_meffMin > 0 && meff < m_meffMin) return false;
    if(m_meffMax > 0 && meff > m_meffMax) return false;
  }
  return true;
}
// /*--------------------------------------------------------------------------------*/
// /*bool EventSelector::passMetOrMeffCut(const LeptonVector& leptons, const TauVector& taus, 
//                                      const JetVector& jets, const Met* met)
// {
//   if(m_momMetMin >= 0 || m_momMeffMin >= 0){
//     float meff = Meff(leptons, taus, jets, met);
//     if(met->Et < m_momMetMin && meff < m_momMeffMin) return false;
//   }
//   return true;
// }*/

/*--------------------------------------------------------------------------------*/
// Lepton dR, dPhi
/*--------------------------------------------------------------------------------*/
// bool EventSelector::passMinDRBaseCut()
// {
//   if(m_minDRBaseMin > 0){
//     float minDR = 999.;
//     for(uint iL=0; iL<baseLeptons.size(); iL++){
//       const Lepton* lep1 = baseLeptons[iL];
//       for(uint jL=iL+1; jL<baseLeptons.size(); jL++){
//         const Lepton* lep2 = baseLeptons[jL];
//         float dR = lep1->DeltaR(*lep2);
//         if(dR < minDR) minDR = dR;
//       }
//     }
//     //std::cout << "minDR " << minDR << " minDRMin " << m_minDRBaseMin << std::endl;
//     if(minDR < m_minDRBaseMin) return false;
//   }
//   return true;
// }
/*--------------------------------------------------------------------------------*/
bool EventSelector::passMinDPhiOSCut()
{
  if(m_minDPhiOSMax > 0 || m_minDPhiOSMin > 0){
    float minDPhi = -1.;
    TLorentzVector leps[2];
    Int_t    charge[2];
    std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
    std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
    for(uint iL=0; iL<vec_electron->size(); iL++){
      leps[0] = vec_electron->at(iL).p4();
      charge[0] = vec_electron->at(iL).charge();
      //For electron-electron
      for(uint iL2=0; iL2<vec_electron->size(); iL2++){
        if(iL2<=iL) continue;
        charge[1] = vec_electron->at(iL2).charge();
        if(isOS(charge[0], charge[1])){
          leps[1] = vec_electron->at(iL2).p4();
          Double_t dphi = fabs(leps[0].DeltaPhi(leps[1]));
          if(minDPhi<0 || dphi<minDPhi) minDPhi = dphi;
        }
      }
      //For electron-muon
      for(uint iL2=0; iL2<vec_muon->size(); iL2++){
        charge[1] = vec_muon->at(iL2).charge();
        if(isOS(charge[0], charge[1])){
          leps[1] = vec_muon->at(iL2).p4();
          Double_t dphi = fabs(leps[0].DeltaPhi(leps[1]));
          if(minDPhi<0 || dphi<minDPhi) minDPhi = dphi;
        }
      }
    }
    // for muon-muon
    for(uint iL=0; iL<vec_muon->size(); iL++){
      leps[0] = vec_muon->at(iL).p4();
      charge[0] = vec_muon->at(iL).charge();
      for(uint iL2=0; iL2<vec_muon->size(); iL2++){
        if(iL2<=iL) continue;
        charge[1] = vec_muon->at(iL2).charge();
        if(isOS(charge[0], charge[1])){
          leps[1] = vec_muon->at(iL2).p4();
          Double_t dphi = fabs(leps[0].DeltaPhi(leps[1]));
          if(minDPhi<0 || dphi<minDPhi) minDPhi = dphi;
        }
      }
    }
    if(minDPhi >= 0){
      if(m_minDPhiOSMax > 0 && minDPhi > m_minDPhiOSMax) return false;
      if(m_minDPhiOSMin > 0 && minDPhi < m_minDPhiOSMin) return false;
    }
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passLepDEtaCut()
{
  // Just use the leading two leptons for now
  if(m_lepDEtaMax > 0 && nSignalLeps()>=2){
    Double_t eta[2];
    Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
    Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
    for(uint id=0; id<2; id++) eta[id] = getFourVector(lepIndex[id], lepFlavor[id]).Eta();
    float dEta = fabs(eta[0]-eta[1]);
    if(dEta > m_lepDEtaMax) return false;
  }
  return true;
}
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passLepAngularCuts(const LeptonVector& signalLeptons, const LeptonVector& baseLeptons)
// {
//   if(!passMinDRBaseCut(baseLeptons)) return false;
//   if(!passMinDPhiOSCut(signalLeptons)) return false;
//   if(!passLepDEtaCut(signalLeptons)) return false;
//   return true;
// }

/*--------------------------------------------------------------------------------*/
// Lepton and tau pt requirements
/*--------------------------------------------------------------------------------*/
bool EventSelector::passLepPtCut()
{
  Double_t lepPt[2] = {-1.,-1.};//speciall treatment for 1st/2nd leptons.
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  if(lepIndex[0]!=-1) lepPt[0] = getFourVector(lepIndex[0], lepFlavor[0]).Pt();
  if(lepIndex[1]!=-1) lepPt[1] = getFourVector(lepIndex[1], lepFlavor[1]).Pt();
  if(m_lep1PtMin > 0 && lepPt[0]/1000. < m_lep1PtMin) return false;
  if(m_lep2PtMin > 0 && lepPt[1]/1000. < m_lep2PtMin) return false;
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  for(uint iL=0; iL<vec_electron->size(); iL++){
    Double_t lepPtInGeV = vec_electron->at(iL).pt()/1000.;
    if(m_lepPtMin > 0 && lepPtInGeV<m_lepPtMin) return false;
    if(m_lepPtMax > 0 && lepPtInGeV>m_lepPtMax) return false;
  }
  for(uint iL=0; iL<vec_muon    ->size(); iL++){
    Double_t lepPtInGeV = vec_muon    ->at(iL).pt()/1000.;
    if(m_lepPtMin > 0 && lepPtInGeV<m_lepPtMin) return false;
    if(m_lepPtMax > 0 && lepPtInGeV>m_lepPtMax) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::passSumLepPtCut()
{
  if(m_sumLepPtMin > 0){
    float sumPt = 0;
    std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
    std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
    for(uint iL=0; iL<vec_electron->size(); iL++){
      Double_t lepPtInGeV = vec_electron->at(iL).pt()/1000.;
      sumPt += lepPtInGeV;
    }
    for(uint iL=0; iL<vec_muon    ->size(); iL++){
      Double_t lepPtInGeV = vec_muon    ->at(iL).pt()/1000.;
      sumPt += lepPtInGeV;
    }
    if(sumPt < m_sumLepPtMin) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
// Lepton and tau eta requirements
/*--------------------------------------------------------------------------------*/
bool EventSelector::passBaseLepEtaCut()
{
  Double_t lepEta[3] = {-3.,-3.,-3.};
  Int_t* lepIndex  = m_baseLepIndex ;
  Int_t* lepFlavor = m_baseLepFlavor;
  for (int i_l=0;i_l<3;i_l++){
    if(lepIndex[i_l]!=-1) lepEta[i_l] = getFourVector(lepIndex[i_l], lepFlavor[i_l]).Eta();
    if(m_baseLepEtaMax > 0 and fabs(lepEta[i_l]) > m_baseLepEtaMax) return false;
    if(m_baseLepEtaMin > 0 and fabs(lepEta[i_l]) < m_baseLepEtaMin) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
// bool EventSelector::passTauPtCut(const TauVector& taus)
// {
//   uint nTau = taus.size();
//   if(m_tau1PtMin > 0 && nTau > 0 && taus[0]->Pt() < m_tau1PtMin) return false;
//   if(m_tau2PtMin > 0 && nTau > 1 && taus[1]->Pt() < m_tau2PtMin) return false;
//   return true;
// }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passSumTauPtCut(const TauVector& taus)
// {
//   if(m_sumTauPtMin > 0){
//     float sumPt = 0;
//     for(uint iT=0; iT<taus.size(); iT++){
//       sumPt += taus[iT]->Pt();
//     }
//     if(sumPt < m_sumTauPtMin) return false;
//   }
//   return true;
// }
/*--------------------------------------------------------------------------------*/
bool EventSelector::passLepTauPtCuts()
{
  if(!passLepPtCut()) return false;
  if(!passSumLepPtCut()) return false;
  //  if(!passTauPtCut(taus)) return false;
  //  if(!passSumTauPtCut(taus)) return false;
  return true;
}

/*--------------------------------------------------------------------------------*/
// Lepton truth requirements
/*--------------------------------------------------------------------------------*/
bool EventSelector::passLepTruthCut()
{
  if(m_isMC && m_selRealLeps){
    for(uint i=0; i<3; i++){
      if(isFakeLepton(i)) return false;
      if(m_is3SigLepSel ? m_leadLepFlavor[i]==0 : m_baseLepFlavor[i]==0){
        if(isChargeFlipElectron(i)) return false;
      }
    }
  }
  if(m_isMC && (m_is3SigLepSel ? nSignalLeps() > 0 : nBaselineLeps() > 0)){
    bool lep1Real = isRealLepton(0);
    if(m_vetoRealLep1 && lep1Real) return false;
  }
  return true;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::pass1stBaseIsSignal(){
  if(m_1stBaseIsSignal){
    if(m_baseLepIsSig[0]==0) return false;
    // if(m_baseLepFlavor[0]==0){
    //   if(not IsMySignalElectron( m_vec_baseElectron->at(m_baseLepIndex[0]) )) return false;
    // } else if (m_baseLepFlavor[0]==1){
    //   if(not IsMySignalMuon    ( m_vec_baseMuon    ->at(m_baseLepIndex[0]) )) return false;
    // }
  }
  return true;
}

// /*--------------------------------------------------------------------------------*/
// // Subtract lepton from MET
// /*--------------------------------------------------------------------------------*/
// void EventSelector::subLepFromMet(const Met* inMet, Met& outMet, const Lepton* lep)
// {
//   outMet = *inMet;
//   // TEST no subtraction
//   //TVector2 metVec;
//   //metVec.SetMagPhi(inMet->Et, inMet->phi);
//   //metVec += lep->Vect().XYvector();
//   //outMet.Et = metVec.Mod();
//   //outMet.phi = TVector2::Phi_mpi_pi(metVec.Phi());
// }
// /*--------------------------------------------------------------------------------*/
// // Emulate SS event with 3L, and correct the MET
// /*--------------------------------------------------------------------------------*/
// /*void EventSelector::getSSEmulation(const LeptonVector& inLeps, LeptonVector& outLeps,
//                                    const Met* inMet, Met& outMet)
// {
//   outLeps.clear();
//   if(m_emulateSS){
//     // Find leading SS pair
//     for(uint i = 0; i < inLeps.size(); i++){
//       for(uint j = i+1; j < inLeps.size(); j++){
//         if(!isOppSign(inLeps[i], inLeps[j])){
//           outLeps.push_back(inLeps[i]);
//           outLeps.push_back(inLeps[j]);
//           // Trick to get k != i or j:
//           // i + j + k = 3, so
//           uint k = 3 - i - j;
//           // Force crash if not 3L
//           const Lepton* subLep = inLeps.at(k);
//           subLepFromMet(inMet, outMet, subLep);
//           return;
//         }
//       }
//     }
//   }
// }*/

// /*--------------------------------------------------------------------------------*/
// // Get SS emulation leptons as well as the dropped lepton
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::getSSEmulLeps(const LeptonVector& inLeps, LeptonVector& outLeps,
//                                   const Lepton*& dropLep)
// {
//   // SS emulation requires 3L and a Z boson cand
//   if(inLeps.size() != 3) return false;
//   if(!hasZ(inLeps)) return false;

//   outLeps.clear();

//   // Find leading SS pair
//   for(uint i = 0; i < inLeps.size(); i++){
//     for(uint j = i+1; j < inLeps.size(); j++){
//       if(!isOppSign(inLeps[i], inLeps[j])){
//         outLeps.push_back(inLeps[i]);
//         outLeps.push_back(inLeps[j]);
//         // Trick to get k != i or j:
//         // i + j + k = 3, so
//         uint k = 3 - i - j;
//         dropLep = inLeps.at(k);
//         return true;
//       }
//     }
//   }
//   return false;
// }

// /*--------------------------------------------------------------------------------*/
// // Get SS emulation jets
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isSSEmulORJet(const Jet* jet, const JetVector& inJets, const Lepton* dropLep)
// {
//   // Jet must be within dR of 0.2 of the electron
//   float dR = jet->DeltaR(*dropLep);
//   if(dR > 0.2) return false;
//   SusyNtSys ntSys = ObjSys::toSusyNtSys(m_objSys);
//   if(!isSignalJet2Lep(jet, ntSys)) return false;
//   // Make sure this jet is not already in inJets
//   if(std::find(inJets.begin(), inJets.end(), jet) != inJets.end()) return false;
//   // Throw away some fraction of jets
//   static TRandom3 rand;
//   if(rand.Uniform() < 0.85) return false;
//   return true;
// } 
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passSSEmulJetCuts(const Jet* jet, const LeptonVector& outLeps)
// {
//   if(jet->isBadVeryLoose) return false;
//   // lep-jet overlap removal - if either lepton overlaps with this jet, kill the event
//   for(uint i = 0; i < outLeps.size(); i++){
//     const Lepton* lep = outLeps[i];
//     if(lep->isEle() && jet->DeltaR(*lep) < E_J_DR) return false;
//     if(lep->isMu()  && jet->DeltaR(*lep) < M_J_DR) return false;
//   }
//   return true;
// }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::getSSEmulJets(const JetVector& inJets, JetVector& outJets,
//                                   const LeptonVector& outLeps, const Lepton* dropLep)
// {
//   outJets = inJets;
//   // Only modify the jet collection if the dropped lepton is an electron
//   if(dropLep->isEle()){
//     // Loop over ntuple jets
//     vector<Jet>& ntJets = *m_nt->jet();
//     for(uint i = 0; i < ntJets.size(); i++){
//       Jet* jet = & ntJets[i];
//       // Find an overlapping jet
//       if(isSSEmulORJet(jet, inJets, dropLep)){
//         // Now apply the necessary cleaning/OR cuts
//         if(!passSSEmulJetCuts(jet, outLeps)) return false;
//         outJets.push_back(jet);
//         //std::cout << "REPLACING LEPTON WITH A JET" << std::endl;
//         //dropLep->print();
//         //jet->print();
//       }
//     }
//   }
//   // Now we need to re-sort the output jets
//   std::sort(outJets.begin(), outJets.end(), comparePt);
//   return true;
// }

// /*--------------------------------------------------------------------------------*/
// // Pass 3L->SS emulation requirements
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::passSSEmulation(const LeptonVector& inLeps, LeptonVector& outLeps,
//                                     const JetVector& inJets, JetVector& outJets,
//                                     const Met* inMet, Met& outMet)
// {
//   if(m_emulateSS){
//     // Get the SS emulation leptons (if possible)
//     const Lepton* dropLep = NULL;
//     if(!getSSEmulLeps(inLeps, outLeps, dropLep)) return false;
//     // Get the SS emulation jets (if possible)
//     if(!getSSEmulJets(inJets, outJets, outLeps, dropLep)) return false;
//     // Correct the MET
//     subLepFromMet(inMet, outMet, dropLep);
//   }
//   return true;
// }

// /*--------------------------------------------------------------------------------*/
// // Trigger reweighting
// /*--------------------------------------------------------------------------------*/
// float EventSelector::getTriggerWeight(const LeptonVector& leptons, 
//                                       const JetVector& jets, const Met* met, 
//                                       WeightSys::WSys sys)
// {
//   const Event* evt = m_nt->evt();
//   if(m_use2LTrig){
//     SusyNtSys trigSys = WeightSys::toTrigSys(sys);
//     return m_2lTrigLogic->getTriggerWeight(leptons, evt->isMC, met->Et, 
//                                            jets.size(), evt->nVtx, trigSys);
//   }
//   else{
//     return m_3lTrigLogic->getTriggerWeight(leptons, evt);
//   }
// }

// /*--------------------------------------------------------------------------------*/
// // Lepton efficiency scale factor
// /*--------------------------------------------------------------------------------*/
// float EventSelector::getLeptonSF(const LeptonVector& leptons, uint anaSys)
// {
//   float sf = 1.;
//   for(uint i=0; i<leptons.size(); i++){
//     const Lepton* lep = leptons[i];
//     if(lep->isEle() && anaSys==WeightSys::el_eff_p)
//       sf *= (lep->effSF + lep->errEffSF);
//     else if(lep->isEle() && anaSys==WeightSys::el_eff_n)
//       sf *= (lep->effSF - lep->errEffSF);
//     else if(lep->isMu() && anaSys==WeightSys::mu_eff_p)
//       sf *= (lep->effSF + lep->errEffSF);
//     else if(lep->isMu() && anaSys==WeightSys::mu_eff_n)
//       sf *= (lep->effSF - lep->errEffSF);
//     else sf *= lep->effSF;
//   }
//   return sf;
// }

// /*--------------------------------------------------------------------------------*/
// // Electron charge flip, smears leps and met, returns q-flip factor
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isOkForQFlip(const LeptonVector& leptons)
// {
//   /*if(leptons.size() == 2 && m_nt->evt()->isMC){
//     if(isOppSign(leptons[0], leptons[1])){
//       if(leptons[0]->isEle() || leptons[1]->isEle()) return true;
//     }
//   }*/
//   return false;
// }
// /*--------------------------------------------------------------------------------*/
// LeptonVector EventSelector::getQFlipLeptons(const LeptonVector& inLeps)
// {
//   // Electrons get copied, muons do not
//   m_qFlipElectrons.clear();
//   m_qFlipElectrons.reserve(inLeps.size());
//   LeptonVector outLeps;
//   /*for(uint i=0; i<inLeps.size(); i++){
//     const Lepton* lep = inLeps[i];
//     if(lep->isEle()){
//       const Electron* ele = (const Electron*) lep;
//       m_qFlipElectrons.push_back(Electron(*ele));
//       outLeps.push_back(&m_qFlipElectrons.back());
//     }
//     else {
//       // It's a muon, but need tricky const workaround
//       const Muon* mu = (const Muon*) lep;
//       vector<Muon>* ntMuons = m_nt->muo();
//       vector<Muon>::iterator muItr = std::find(ntMuons->begin(), ntMuons->end(), *mu);
//       assert(muItr != ntMuons->end());
//       outLeps.push_back(&(*muItr));
//     }
//   }*/
//   return outLeps;
// }

// /*--------------------------------------------------------------------------------*/
// float EventSelector::getChargeFlip(const LeptonVector& inLeps, LeptonVector& outLeps,
//                                    const Met* inMet, Met& outMet)
// {
//   float qFlipSF = 1.;
//   /*
//   // This method only really makes sense for 2L SS selection
//   if(inLeps.size() != 2){
//     std::cout << "EventSelector::getChargeFlip - ERROR - Requres 2 leptons, you have " 
//          << inLeps.size() << std::endl;
//     abort();
//   }
  
//   // Copy the MET
//   outMet = *inMet;

//   // First step: determine if we need to copy and smear
//   if(isOkForQFlip(inLeps)){
//     outLeps = getQFlipLeptons(inLeps);

//     int pdg0 = outLeps[0]->isEle()? 11 : 13;
//     int pdg1 = outLeps[1]->isEle()? 11 : 13;
//     TVector2 outMetVec2(outMet.lv().Vect().XYvector());

//     // Now to do the smearing and get the SF using the chargeFlip tool
//     m_chargeFlip->setSeed(m_nt->evt()->event);
//     float sf = m_chargeFlip->OS2SS(pdg0, outLeps[0], pdg1, outLeps[1], &outMetVec2);
//     float overlap = m_chargeFlip->overlapFrac().first;
//     qFlipSF = sf * overlap;

//     outMet.Et = outMetVec2.Mod();
//     outMet.phi = TVector2::Phi_mpi_pi(outMetVec2.Phi());
//   } 
//   else{
//     // Not an event for charge-flip estimation, so no need to smear.
//     // Just return the inputs as outputs
//     outLeps = inLeps;
//   }
//   */
//   return qFlipSF;
// }

// /*--------------------------------------------------------------------------------*/
// // Pileup systematic
// /*--------------------------------------------------------------------------------*/
// float EventSelector::getPileupSys(const Event* evt, WeightSys::WSys sys)
// {
//   // Pileup weight is already applied upstream, 
//   // so we need to correct it for the pileup systematics
//   if(evt->wPileup == 0) return 0; // not exactly kosher
//   if(sys==WeightSys::pileup_p) return evt->wPileup_up / evt->wPileup;
//   else if(sys==WeightSys::pileup_n) return evt->wPileup_dn / evt->wPileup;
//   else return 1.;
// }

/*--------------------------------------------------------------------------------*/
// Event counters
/*--------------------------------------------------------------------------------*/
void EventSelector::dumpEventCounters()
{
  std::cout<<std::endl;
  MyVerbose("dumpEventCounters()",Form("Event selection counters for (%s, %s).",m_sel.c_str(),m_sys.c_str()) );

  MyVerbose("dumpEventCounters()",Form("  initial:      %d", n_initial     ));
  MyVerbose("dumpEventCounters()",Form("  pass hotSpot: %d", n_pass_hotSpot));
  MyVerbose("dumpEventCounters()",Form("  pass badJet:  %d", n_pass_badJet ));
  MyVerbose("dumpEventCounters()",Form("  pass FEB:     %d", n_pass_feb    ));
  MyVerbose("dumpEventCounters()",Form("  pass badMu:   %d", n_pass_badMu  ));
  MyVerbose("dumpEventCounters()",Form("  pass cosmic:  %d", n_pass_cosmic ));
  MyVerbose("dumpEventCounters()",Form("  pass clean:   %d", n_pass_clean  ));
  MyVerbose("dumpEventCounters()",Form("  pass nBLep:   %d", n_pass_nBLep  ));
  MyVerbose("dumpEventCounters()",Form("  pass mllBase: %d", n_pass_mllBase));
  MyVerbose("dumpEventCounters()",Form("  pass nLep:    %d", n_pass_nLep   ));
  MyVerbose("dumpEventCounters()",Form("  pass nTau:    %d", n_pass_nTau   ));
  MyVerbose("dumpEventCounters()",Form("  pass trig:    %d", n_pass_trig   ));
  MyVerbose("dumpEventCounters()",Form("  pass truth:   %d", n_pass_truth  ));
  MyVerbose("dumpEventCounters()",Form("  pass flavQ:   %d", n_pass_sfos   ));
  //MyVerbose("dumpEventCounters()",Form("  pass loose Z: %d", n_pass_lz     ));
  MyVerbose("dumpEventCounters()",Form("  pass jet:     %d", n_pass_jet    ));
  MyVerbose("dumpEventCounters()",Form("  pass vbf:     %d", n_pass_vbf    ));
  MyVerbose("dumpEventCounters()",Form("  pass lepPt:   %d", n_pass_lepPt  ));
  MyVerbose("dumpEventCounters()",Form("  pass z:       %d", n_pass_z      ));
  MyVerbose("dumpEventCounters()",Form("  pass lep dR:  %d", n_pass_lepDR  ));
  MyVerbose("dumpEventCounters()",Form("  pass met:     %d", n_pass_met    ));
  MyVerbose("dumpEventCounters()",Form("  pass mt:      %d", n_pass_mt     ));
  MyVerbose("dumpEventCounters()",Form("  pass meff:    %d", n_pass_meff   ));
  //MyVerbose("dumpEventCounters()",Form("  pass mljj:    %d", n_pass_mljj   ));
  //MyVerbose("dumpEventCounters()",Form("  pass other:   %d", n_pass_other  ));
}

/*--------------------------------------------------------------------------------*/
// Truth selection
/*--------------------------------------------------------------------------------*/
bool EventSelector::isRealLepton(unsigned int id)
{
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  if(lepIndex[id]==-1) return false;
  else{
    if(lepFlavor[id]==0){
      Int_t type   = xAOD::TruthHelpers::getParticleTruthType  (vec_electron->at(lepIndex[id]));
      if(type==MCTruthPartClassifier::IsoElectron) return true;
    }else{
      int muonTruthType   = 0;
      //      int muonTruthOrigin = 0;
      const xAOD::TrackParticle* trackParticle = (&(vec_muon->at(lepIndex[id])))->primaryTrackParticle();
      if(trackParticle){
        static SG::AuxElement::Accessor<int> acc_truthType  ("truthType"  );
        //        static SG::AuxElement::Accessor<int> acc_truthOrigin("truthOrigin");
        if(acc_truthType  .isAvailable(*trackParticle)) muonTruthType   = acc_truthType  (*trackParticle);
        //        if(acc_truthOrigin.isAvailable(*trackParticle)) muonTruthOrigin = acc_truthOrigin(*trackParticle);
        //        std::cout<<Form("Muon type %d origin %d", muonTruthType, muonTruthOrigin)<<std::endl;
        if(muonTruthType==MCTruthPartClassifier::IsoMuon) return true;
      }
    }
  }
  return false;
}
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isConvLepton(const Lepton* lep)
// { return lep->truthType == RecoTruthMatch::CONV; }
/*--------------------------------------------------------------------------------*/
bool EventSelector::isFakeLepton(unsigned int id)
{ return !isRealLepton(id); }
/*--------------------------------------------------------------------------------*/
bool EventSelector::isChargeFlipElectron(unsigned int id)
{
  //Assuming lepton[id] is electron and true isolated electron.
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  const xAOD::TruthParticle* truthEle = xAOD::TruthHelpers::getTruthParticle(vec_electron->at(lepIndex[id]));
  Int_t trueCharge = truthEle->charge();
  Int_t recoCharge = (vec_electron->at(lepIndex[id])).charge();
  if(isSS(trueCharge, recoCharge)) return false;
  else return false;
}
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isHFLepton(const Lepton* lep)
// { return lep->truthType == RecoTruthMatch::HF; }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isLFLepton(const Lepton* lep)
// { return lep->truthType == RecoTruthMatch::LF; }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isJFLepton(const Lepton* lep)
// { return isHFLepton(lep) || isLFLepton(lep); }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isUKLepton(const Lepton* lep)
// { return lep->truthType == RecoTruthMatch::UK; }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isQuarkJetTau(const Tau* tau)
// {
//   return tau->detailedTruthType == RecoTauMatch::TAU_ELEC_BJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_ELEC_CJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_ELEC_QJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_MUON_BJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_MUON_CJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_MUON_QJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_BJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_CJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_QJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_HFJET;
// }
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::isGluonJetTau(const Tau* tau)
// {
//   return tau->detailedTruthType == RecoTauMatch::TAU_ELEC_GJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_MUON_GJET ||
//          tau->detailedTruthType == RecoTauMatch::TAU_GJET;
// }

// /*--------------------------------------------------------------------------------*/
// // Lepton truth particle matching
// /*--------------------------------------------------------------------------------*/
// TruthParticle* EventSelector::getTruthLepton(const Lepton* lep)
// {
//   // I can refine this, but for a first pass let's just find the closest lepton
//   std::vector<TruthParticle>& truthParticles = *m_nt->tpr();
//   float minDR = 0.1;
//   TruthParticle* bestTruthPar = NULL;
//   for(uint i = 0; i < truthParticles.size(); i++){
//     TruthParticle* truthPar = & truthParticles[i];
//     float dR = lep->DeltaR(*truthPar);
//     if(dR < minDR){
//       minDR = dR;
//       bestTruthPar = truthPar;
//     }
//   }
//   return bestTruthPar;
// }

/*--------------------------------------------------------------------------------*/
int EventSelector::getChan()
{
  Int_t chan = -1;
  Int_t nEl=0;
  Int_t nMu=0;
  for(Int_t id=0; id<3; id++){
    if(m_is3SigLepSel){
      if     (m_leadLepFlavor[id]==0) nEl++;
      else if(m_leadLepFlavor[id]==1) nMu++;
    }else{
      if     (m_baseLepFlavor[id]==0) nEl++;
      else if(m_baseLepFlavor[id]==1) nMu++;
    }
  }
  if(nEl+nMu!=3) return -1; //Do not plot anything.
  else if(nEl==3 && nMu==0) chan = Ch_eee;
  else if(nEl==2 && nMu==1) chan = Ch_eem;
  else if(nEl==1 && nMu==2) chan = Ch_emm;
  else if(nEl==0 && nMu==3) chan = Ch_mmm;

  return chan;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::hasSFOS()
{
  Int_t recoCharge[3]={0,0,0};
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  for(Int_t id=0; id<3; id++) recoCharge[id] = getCharge(lepIndex[id], lepFlavor[id]);
  Bool_t hasSFOS = kFALSE;
  for(Int_t id1=0; id1<3; id1++){
    if(lepIndex[id1]==-1) continue;
    for(Int_t id2=0; id2<3; id2++){
      if(lepIndex[id2]==-1) continue;
      if(lepFlavor[id1]==lepFlavor[id2] && 
         recoCharge     [id1]!=recoCharge     [id2] ){
        hasSFOS = kTRUE;
        break;
      }
    }
  }
  return hasSFOS;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::hasSFSS()
{
  Int_t recoCharge[3]={0,0,0};
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  for(Int_t id=0; id<3; id++) recoCharge[id] = getCharge(lepIndex[id], lepFlavor[id]);
  Bool_t hasSFSS = kFALSE;
  for(Int_t id1=0; id1<3; id1++){
    if(lepIndex[id1]==-1) continue;
    for(Int_t id2=0; id2<3; id2++){
      if(lepIndex[id2]==-1) continue;
      if(lepFlavor[id1]==lepFlavor[id2] && 
         recoCharge     [id1]==recoCharge     [id2] ){
        hasSFSS = kTRUE;
        break;
      }
    }
  }
  return hasSFSS;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::hasOFOS()
{
  Int_t recoCharge[3]={0,0,0};
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  for(Int_t id=0; id<3; id++) recoCharge[id] = getCharge(lepIndex[id], lepFlavor[id]);
  Bool_t hasOFOS = kFALSE;
  for(Int_t id1=0; id1<3; id1++){
    if(lepIndex[id1]==-1) continue;
    for(Int_t id2=0; id2<3; id2++){
      if(lepIndex[id2]==-1) continue;
      if(lepFlavor[id1]!=lepFlavor[id2] && 
         recoCharge     [id1]!=recoCharge     [id2] ){
        hasOFOS = kTRUE;
        break;
      }
    }
  }
  return hasOFOS;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::hasOS()
{
  Int_t recoCharge[3]={0,0,0};
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  for(Int_t id=0; id<3; id++) recoCharge[id] = getCharge(lepIndex[id], lepFlavor[id]);
  Bool_t hasOS = kFALSE;
  for(Int_t id1=0; id1<3; id1++){
    if(lepIndex[id1]==-1) continue;
    for(Int_t id2=0; id2<3; id2++){
      if(lepIndex[id2]==-1) continue;
      if(recoCharge[id1]!=recoCharge[id2]){
        hasOS = kTRUE;
        break;
      }
    }
  }
  return hasOS;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::hasSS()
{
  Int_t recoCharge[3]={0,0,0};
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  for(Int_t id=0; id<3; id++) recoCharge[id] = getCharge(lepIndex[id], lepFlavor[id]);
  Bool_t hasSS = kFALSE;
  for(Int_t id1=0; id1<3; id1++){
    if(lepIndex[id1]==-1) continue;
    for(Int_t id2=0; id2<3; id2++){
      if(lepIndex[id2]==-1) continue;
      if(recoCharge[id1]==recoCharge[id2]){
        hasSS = kTRUE;
        break;
      }
    }
  }
  return hasSS;
}

// /*--------------------------------------------------------------------------------*/
// bool EventSelector::hasBJet()
// {
//   Bool_t hasBJet = kFALSE;
//   for(UInt_t cnt=0; cnt<m_vec_signalJet->size(); cnt++){
//     if((m_vec_signalJet->at(cnt)).auxdata<char>("bjet")){
//       hasBJet = kTRUE;
//       break;
//     }
//   }
//   return hasBJet;
// }
/*--------------------------------------------------------------------------------*/
int EventSelector::numBJets()
{
  Int_t numBJets = 0;
  for(UInt_t cnt=0; cnt<m_vec_signalJet->size(); cnt++){
    if((m_vec_signalJet->at(cnt)).auxdata<char>("bjet")){
      numBJets++;
    }
  }
  return numBJets;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::isInMassWindow(Double_t mass, Double_t targetMass, Double_t window)
{
  Bool_t inWindow = kFALSE;
  if(fabs(mass-targetMass)<window) inWindow = kTRUE;
  return inWindow;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::hasUpsilon()
{
  //To find Upsilon candidate (same flavor, oppsite sign, close to Zmass)
  Double_t massWindow = 1.;
  int index[2]={-1,-1};
  int flav=-1;
  Double_t msfos = findBestMSFOS(index[0],index[1],flav,MUPSILON)/1000.;
  if(isInMassWindow(msfos, MUPSILON, massWindow)) return true;
  return false;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::hasZ()
{
  //To find Z candidate (same flavor, oppsite sign, close to Zmass)
  Double_t massWindow = m_windowZ;
  int index[2]={-1,-1};
  int flav=-1;
  Double_t msfos = findBestMSFOS(index[0],index[1],flav)/1000.;
  if(isInMassWindow(msfos, MZ, massWindow)) return true;
  return false;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::hasZlll()
{
  //To find Z candidate with 3leptons (including same flavor, oppsite sign) and close to Zmass
  Double_t massWindow = m_windowZ;
  TLorentzVector leps[3];
  Int_t          charge[3];
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  //Electron pair search
  for(uint iL=0; iL<vec_electron->size(); iL++){
    leps[0] = vec_electron->at(iL).p4();
    charge[0] = vec_electron->at(iL).charge();
    for(uint iL2=0; iL2<vec_electron->size(); iL2++){
      if(iL2<=iL) continue;
      charge[1] = vec_electron->at(iL2).charge();
      if(isOS(charge[0],charge[1])){
        leps[1] = vec_electron->at(iL2).p4();
        //Trying to add one more electron
        for(uint iL3=0; iL3<vec_electron->size(); iL3++){
          if((iL3==iL) || (iL3==iL2)) continue;
          leps[2] = vec_electron->at(iL3).p4();
          if(isInMassWindow( (leps[0]+leps[1]+leps[2]).M()/1000., MZ, massWindow)) return true;
        }
        //Trying to add a muon
        for(uint iL3=0; iL3<vec_muon->size(); iL3++){
          leps[2] = vec_muon->at(iL3).p4();
          if(isInMassWindow( (leps[0]+leps[1]+leps[2]).M()/1000., MZ, massWindow)) return true;
        }
      }
    }
  }
  //Muon pair search
  for(uint iL=0; iL<vec_muon->size(); iL++){
    leps[0] = vec_muon->at(iL).p4();
    charge[0] = vec_muon->at(iL).charge();
    for(uint iL2=0; iL2<vec_muon->size(); iL2++){
      if(iL2<=iL) continue;
      charge[1] = vec_muon->at(iL2).charge();
      if(isOS(charge[0],charge[1])){
        leps[1] = vec_muon->at(iL2).p4();
        //Trying to add one more muon
        for(uint iL3=0; iL3<vec_muon->size(); iL3++){
          if((iL3==iL) || (iL3==iL2)) continue;
          leps[2] = vec_muon->at(iL3).p4();
          if(isInMassWindow( (leps[0]+leps[1]+leps[2]).M()/1000., MZ, massWindow)) return true;
        }
        //Trying to add a electron
        for(uint iL3=0; iL3<vec_electron->size(); iL3++){
          leps[2] = vec_electron->at(iL3).p4();
          if(isInMassWindow( (leps[0]+leps[1]+leps[2]).M()/1000., MZ, massWindow)) return true;
        }
      }
    }
  }

  return false;
}

/*--------------------------------------------------------------------------------*/
bool EventSelector::hasZllll()
{
  //To find Z candidate with 4leptons (including "two" same flavor, oppsite sign lepton pairs) and close to Zmass
  Double_t massWindow = m_windowZ;
  TLorentzVector leps[4];
  Int_t          charge[4];
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  //Electron pair search
  for(uint iL=0; iL<vec_electron->size(); iL++){
    leps[0] = vec_electron->at(iL).p4();
    charge[0] = vec_electron->at(iL).charge();
    for(uint iL2=0; iL2<vec_electron->size(); iL2++){
      if(iL2<=iL) continue;
      charge[1] = vec_electron->at(iL2).charge();
      if(isOS(charge[0],charge[1])){
        leps[1] = vec_electron->at(iL2).p4();
        //Trying to add two more electrons
        for(uint iL3=0; iL3<vec_electron->size(); iL3++){
          if((iL3==iL) || (iL3==iL2)) continue;
          leps[2] = vec_electron->at(iL3).p4();
          charge[2] = vec_electron->at(iL3).charge();
          for(uint iL4=0; iL4<vec_electron->size(); iL4++){
            if((iL4==iL) || (iL4==iL2) || (iL4==iL3)) continue;
            charge[3] = vec_electron->at(iL4).charge();
            if(isOS(charge[2],charge[3])){
              leps[3] = vec_electron->at(iL4).p4();
              if(isInMassWindow( (leps[0]+leps[1]+leps[2]+leps[3]).M()/1000., MZ, massWindow)) return true;
            }
          }
        }
        //Trying to add two more muons
        for(uint iL3=0; iL3<vec_muon->size(); iL3++){
          leps[2] = vec_muon->at(iL3).p4();
          charge[2] = vec_muon->at(iL3).charge();
          for(uint iL4=0; iL4<vec_muon->size(); iL4++){
            if(iL4<=iL3) continue;
            charge[3] = vec_muon->at(iL4).charge();
            if(isOS(charge[2],charge[3])){
              leps[3] = vec_muon->at(iL4).p4();
              if(isInMassWindow( (leps[0]+leps[1]+leps[2]+leps[3]).M()/1000., MZ, massWindow)) return true;
            }
          }
        }
      }
    }
  }
  //Muon pair search
  for(uint iL=0; iL<vec_muon->size(); iL++){
    leps[0] = vec_muon->at(iL).p4();
    charge[0] = vec_muon->at(iL).charge();
    for(uint iL2=0; iL2<vec_muon->size(); iL2++){
      if(iL2<=iL) continue;
      charge[1] = vec_muon->at(iL2).charge();
      if(isOS(charge[0],charge[1])){
        leps[1] = vec_muon->at(iL2).p4();
        //Trying to add two more muons
        for(uint iL3=0; iL3<vec_muon->size(); iL3++){
          if((iL3==iL) || (iL3==iL2)) continue;
          leps[2] = vec_muon->at(iL3).p4();
          charge[2] = vec_muon->at(iL3).charge();
          for(uint iL4=0; iL4<vec_muon->size(); iL4++){
            if((iL4==iL) || (iL4==iL2) || (iL4==iL3)) continue;
            charge[3] = vec_muon->at(iL4).charge();
            if(isOS(charge[2],charge[3])){
              leps[3] = vec_muon->at(iL4).p4();
              if(isInMassWindow( (leps[0]+leps[1]+leps[2]+leps[3]).M()/1000., MZ, massWindow)) return true;
            }
          }
        }
        //Trying to add two more electrons
        for(uint iL3=0; iL3<vec_electron->size(); iL3++){
          leps[2] = vec_electron->at(iL3).p4();
          charge[2] = vec_electron->at(iL3).charge();
          for(uint iL4=0; iL4<vec_electron->size(); iL4++){
            if(iL4<=iL3) continue;
            charge[3] = vec_electron->at(iL4).charge();
            if(isOS(charge[2],charge[3])){
              leps[3] = vec_electron->at(iL4).p4();
              if(isInMassWindow( (leps[0]+leps[1]+leps[2]+leps[3]).M()/1000., MZ, massWindow)) return true;
            }
          }
        }
      }
    }
  }

  return false;
}

float EventSelector::findBestMSFOS(int& index1, int& index2, int& flav, float mass)
{
  //Finding the best SFOS lepton pair with a mass closest to the target mass, and return the resulting mass
  Float_t bestMsfosMass = -1.;
  TLorentzVector leps[2];
  Int_t          charge[2];
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  //Electron pair search
  for(uint iL=0; iL<vec_electron->size(); iL++){
    leps[0] = vec_electron->at(iL).p4();
    charge[0] = vec_electron->at(iL).charge();
    for(uint iL2=0; iL2<vec_electron->size(); iL2++){
      if(iL2<=iL) continue;
      charge[1] = vec_electron->at(iL2).charge();
      if(isOS(charge[0],charge[1])){
        leps[1] = vec_electron->at(iL2).p4();
        Double_t mll = (leps[0]+leps[1]).M();
        if(bestMsfosMass<0. || fabs(bestMsfosMass-mass)>fabs(mll-mass)){
          bestMsfosMass = mll;
          index1 = iL;
          index2 = iL2;
          flav = 0; //means electron.
        }
      }
    }
  }
  //Muon pair search
  for(uint iL=0; iL<vec_muon->size(); iL++){
    leps[0] = vec_muon->at(iL).p4();
    charge[0] = vec_muon->at(iL).charge();
    for(uint iL2=0; iL2<vec_muon->size(); iL2++){
      if(iL2<=iL) continue;
      charge[1] = vec_muon->at(iL2).charge();
      if(isOS(charge[0],charge[1])){
        leps[1] = vec_muon->at(iL2).p4();
        Double_t mll = (leps[0]+leps[1]).M();
        if(bestMsfosMass<0. || fabs(bestMsfosMass-mass)>fabs(mll-mass)){
          bestMsfosMass = mll;
          index1 = iL;
          index2 = iL2;
          flav = 1; //means muon.
        }
      }
    }
  }

  return bestMsfosMass;
}
/*--------------------------------------------------------------------------------*/
float EventSelector::getMetRel(bool useForward)
{
  TLorentzVector metLV; metLV.SetPxPyPzE(m_met.X(),m_met.Y(),0.,0.);
  float dPhi = TMath::Pi()/2.;

  for(uint il=0; il<m_vec_baseElectron->size(); ++il)
    if( fabs(metLV.DeltaPhi( m_vec_baseElectron->at(il).p4() )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( m_vec_baseElectron->at(il).p4() ));
  for(uint il=0; il<m_vec_baseMuon->size(); ++il)
    if( fabs(metLV.DeltaPhi( m_vec_baseMuon->at(il).p4() )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( m_vec_baseMuon->at(il).p4() ));
  for(uint ij=0; ij<m_vec_signalJet->size(); ++ij){
    if( !useForward && fabs(m_vec_signalJet->at(ij).eta())>2.5 ) continue; // Use only central jets
    if( fabs(metLV.DeltaPhi( m_vec_signalJet->at(ij).p4() )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( m_vec_signalJet->at(ij).p4() ));
  }// end loop over jets
  return metLV.Pt() * sin(dPhi);
}
/*--------------------------------------------------------------------------------*/
float EventSelector::getMt(TLorentzVector lep, TVector2 met)
{
  TLorentzVector metLV; metLV.SetPxPyPzE(m_met.X(),m_met.Y(),0.,0.);
  Double_t mt = sqrt(2.*lep.Pt()*m_met.Mod()*(1 - cos(lep.DeltaPhi(metLV))));
  return mt;
}
/*--------------------------------------------------------------------------------*/
float EventSelector::getMeff(float jetPtCut, bool useMet)
{
  float meff = 0;
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  for(uint cnt=0; cnt<vec_electron->size(); ++cnt) meff += vec_electron->at(cnt).pt();
  for(uint cnt=0; cnt<vec_muon    ->size(); ++cnt) meff += vec_muon    ->at(cnt).pt();
  for(uint cnt=0; cnt<m_vec_signalJet     ->size(); ++cnt){
    if(m_vec_signalJet->at(cnt).pt()/1000. > jetPtCut) meff += m_vec_signalJet->at(cnt).pt();
  }
  if(useMet) meff += m_met.Mod();
  return meff;
}
/*--------------------------------------------------------------------------------*/
float EventSelector::getMljj()
{
  if(nSignalJets() < 2 || nSignalLeps() < 2) return -999;

  TLorentzVector jet[2];
  TLorentzVector lep[2];
  for(Int_t id=0; id<2; id++) jet[id] = m_vec_signalJet->at(id).p4();
  Int_t* lepIndex  = m_is3SigLepSel ? m_leadLepIndex  : m_baseLepIndex ;
  Int_t* lepFlavor = m_is3SigLepSel ? m_leadLepFlavor : m_baseLepFlavor;
  for(Int_t id=0; id<2; id++) lep[id] = getFourVector(lepIndex[id],lepFlavor[id]);

  TLorentzVector jj = jet[0]+jet[1];
  float dR1 = jj.DeltaR(lep[0]);
  float dR2 = jj.DeltaR(lep[1]);

  return (dR1<dR2) ? (jj+lep[0]).M() : (jj+lep[1]).M();
}
/*--------------------------------------------------------------------------------*/
TLorentzVector EventSelector::getFourVector(int index, int flav)
{
  //method to retrieve four vector by m_leadLepIndex;
  TLorentzVector lep(0.,0.,0.,0.);
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  if(index!=-1){
    if     (flav==0) lep = (vec_electron->at(index)).p4();
    else if(flav==1) lep = (vec_muon    ->at(index)).p4();
  }
  return lep;
}
/*--------------------------------------------------------------------------------*/
int EventSelector::getCharge(int index, int flav)
{
  //method to retrieve charge by m_lead/baseLepIndex;
  Int_t charge = 0;
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  if(index!=-1){
    if     (flav==0) charge = (vec_electron->at(index)).charge();
    else if(flav==1) charge = (vec_muon    ->at(index)).charge();
  }
  return charge;
}
/*--------------------------------------------------------------------------------*/
float EventSelector::getIsolationValue(int index, int flav, xAOD::Iso::IsolationType type)
{
  //method to retrieve isolation value by m_leadLepIndex;
  float iso = 0.;
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  if(index!=-1){
    if     (flav==0) (vec_electron->at(index)).isolationValue(iso,type);
    else if(flav==1) (vec_muon    ->at(index)).isolation     (iso,type);
  }
  return iso;
}
/*--------------------------------------------------------------------------------*/
xAOD::TrackParticle* EventSelector::getTrack(int index, int flav)
{
  //method to retrieve track object by m_leadLepIndex;
  xAOD::TrackParticle* track = 0;
  std::vector< xAOD::Electron >* vec_electron = m_is3SigLepSel ? m_vec_signalElectron : m_vec_baseElectron;
  std::vector< xAOD::Muon >*     vec_muon     = m_is3SigLepSel ? m_vec_signalMuon     : m_vec_baseMuon;
  if(index!=-1){
    if     (flav==0) track = const_cast<xAOD::TrackParticle*>((vec_electron->at(index)).trackParticle());
    else if(flav==1) track = const_cast<xAOD::TrackParticle*>((vec_muon    ->at(index)).primaryTrackParticle());
  }
  return track;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::isOS(int charge1, int charge2)
{
  Bool_t isOS = kFALSE;
  if( (charge1==1  && charge2==-1) ||
      (charge1==-1 && charge2==1 ) ) isOS = kTRUE;
  return isOS;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::isSS(int charge1, int charge2)
{
  Bool_t isSS = kFALSE;
  if( (charge1==1  && charge2==1 ) ||
      (charge1==-1 && charge2==-1) ) isSS = kTRUE;
  return isSS;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::isSF(int flavor1, int flavor2)
{
  Bool_t isSF = kFALSE;
  if( (flavor1==0 && flavor2==0 ) ||
      (flavor1==1 && flavor2==1)) isSF = kTRUE;
  return isSF;
}
/*--------------------------------------------------------------------------------*/
bool EventSelector::isDF(int flavor1, int flavor2)
{
  Bool_t isDF = kFALSE;
  if( (flavor1==0 && flavor2==1 ) ||
      (flavor1==1 && flavor2==0)) isDF = kTRUE;
  return isDF;
}

/*--------------------------------------------------------------------------------*/
// Typical selection for 3L 
/*--------------------------------------------------------------------------------*/
void EventSelector::TypSel(int nLep, int nTau, int nBaseLep, int nBaseTau, int nBjetMin, int nBjetMax)
{
  m_nLepMin = m_nLepMax = nLep;
  m_nTauMin = m_nTauMax = nTau;
  m_nBaseLepMin = m_nBaseLepMax = nBaseLep;
  //  m_nBaseTauMin = m_nBaseTauMax = nBaseTau;
  m_nBJetMin = nBjetMin;
  m_nBJetMax = nBjetMax;
  //  if(m_nBJetMax==0) m_selB = false;
  //  if(m_nBJetMin>=1) m_selB = true;
}

// /*--------------------------------------------------------------------------------*/
// // Debug event
// /*--------------------------------------------------------------------------------*/
// bool EventSelector::debugEvent()
// {
//   //uint run = m_nt->evt()->run;
//   //uint evt = m_nt->evt()->event;
//   //if(evt==89871) return true;
//   return false;
// }

