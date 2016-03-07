#define APP_NAME "Plotter"
#define MyInfo(a,b)    if(m_dbg<=MSG::INFO  ) std::cout<<"Info    in <Plotter::"<<(a)<<">: "<<(b)<<std::endl;
#define MyError(a,b)   if(m_dbg<=MSG::ERROR ) std::cout<<"Error   in <Plotter::"<<(a)<<">: "<<(b)<<std::endl;
#define MyDebug(a,b)   if(m_dbg<=MSG::DEBUG ) std::cout<<"Debug   in <Plotter::"<<(a)<<">: "<<(b)<<std::endl;
#define MyVerbose(a,b) if(m_dbg<=MSG::DEBUG ) std::cout<<"Verbose in <Plotter::"<<(a)<<">: "<<(b)<<std::endl;
#define MyAlways(a,b)  if(m_dbg<=MSG::ALWAYS) std::cout<<"In         <Plotter::"<<(a)<<">: "<<(b)<<std::endl;

#include<iostream>
#include"MyAnalysis/Plotter.h"
#include"SUSYTools/SUSYObjDef_xAOD.h"
#include"xAODTruth/xAODTruthHelpers.h"
#include"MyAnalysis/MCTruthClassifierDefs.h"
#include"EventPrimitives/EventPrimitivesHelpers.h"

#include"MMTool/MMTool.h"

#include"TSystem.h"
#include"TFile.h"
#include"TH1F.h"
#include"TH2F.h"

/////////////////////////////
// Histogram bins
/////////////////////////////
// My binning choices
double lep1PtBins[] = {0, 10, 20, 30, 50, 75, 100, 125, 150, 200, 300, 400, 500};
uint nLep1PtBins = 12;
double lep2PtBins[] = {0, 10, 20, 30, 40, 50, 60, 80, 100, 120, 150, 200};
uint nLep2PtBins = 11;
double mmEffPtBins[] = {0, 5, 7, 10, 15, 25, 50, 100, 200};
uint nMmEffPtBins = 8;
double lep3PtBins[] = {0, 10, 20, 30, 40, 50, 60, 80, 100, 120, 150};
uint nLep3PtBins = 10;
double massBins[] = {0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 225, 250, 275, 300, 325, 350, 375, 400, 425, 450, 475, 500};
uint nMassBins = 22;
double meffBins[] = {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400};
uint nMeffBins = 20; 
double mljjFineBins[] = {10, 30, 50, 70, 90, 110, 130, 150, 170, 200, 230, 260, 300, 340, 380, 420, 460, 500};
uint nMljjFineBins = 17;
double jetPtBins[] = {0, 20, 30, 40, 50, 65, 80, 100, 120, 140, 165, 190, 220, 250, 285, 320, 360, 400, 450, 500};
uint nJetPtBins = 19;

// New Mt binnings for the SRs
double mtCoarseBins[] = {110, 150, 200, 300};
uint nMtCoarseBins = 3;
double mtLowBins[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110};
uint nMtLowBins = 10;

// dR bins
double dRBins[] = {0, 0.1, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0, 1.3, 1.6, 2.0, 2.5, 3., 3.5, 4., 5., 6.};
uint nDRBins = 16;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------*/
// Plotter Constructor
/*--------------------------------------------------------------------------------*/
Plotter::Plotter(const std::string& sel, const std::string& sys, const MSG::Level& dbg) : 
  m_dbg(dbg),
  m_sel(sel),
  m_sys(sys),
  m_runMM(false),
  m_effFile(""),
  m_MMTool(NULL),
  m_filename(""),
  m_crossSection(0.),
  m_isMC(kFALSE)
{}

/*--------------------------------------------------------------------------------*/
// Initialize the plotter
/*--------------------------------------------------------------------------------*/
void Plotter::initialize(const char* path, int dsid, double XS, TFile* file)
{
  MyDebug("initialize()","Plotter::initialize()");

  m_crossSection = XS;
  if(m_crossSection>0.) m_isMC = kTRUE;

  // Preparing TFile
  if(file==NULL){
    m_filename = Form("%s/%d.%s.%s.AnaHists.root",path,dsid,m_sel.c_str(),m_sys.c_str());
    m_rootfile = new TFile(m_filename.c_str(),"RECREATE");
  }else{
    m_filename = file->GetName();
    m_rootfile = file;
  }
  m_rootfile->cd();
  MyInfo("initialize()",Form("Output TFile for \"%s\", \"%s\" was created at %s.",
                             m_sel.c_str(), m_sys.c_str(), m_filename.c_str()) );

  // Booking histograms
  BookHistograms();

  if(m_runMM){
    m_MMTool = new MM::MMTool(MM::TwoLepMM,m_dbg);
    std::string prefix = gSystem->ExpandPathName("$ROOTCOREBIN/data/MMTool/");
    Int_t nDimForEff = 1;
    if     (nDimForEff==0) m_MMTool->prepEffHists(prefix+"h0026.GT1S3B.root",nDimForEff);
    else if(nDimForEff==1) m_MMTool->prepEffHists(prefix+m_effFile,nDimForEff);
  }

}

/*--------------------------------------------------------------------------------*/
// Finalize event selection
/*--------------------------------------------------------------------------------*/
void Plotter::finalize()
{
  MyDebug("finalize()","Plotter::finalize()");

  if(m_sys!=""){
    m_rootfile->cd();
    h_xsec->Write();
    h_nEve->Write();
    h_nSigBaseLep->Write();
    h_lepChan[Ch_all]->Write();
    h_baselepChan[Ch_all]->Write();
    for(uint iCh=0; iCh<nChan; iCh++){
      h_mmWeight[iCh]->Write();
      h_lep1Pt[iCh]->Write();
      h_lep2Pt[iCh]->Write();
      h_lep3Pt[iCh]->Write();
      h_el1Pt[iCh]->Write();
      h_el2Pt[iCh]->Write();
      h_el3Pt[iCh]->Write();
      h_mu1Pt[iCh]->Write();
      h_mu2Pt[iCh]->Write();
      h_mu3Pt[iCh]->Write();
      h_lep1Eta[iCh]->Write();
      h_lep2Eta[iCh]->Write();
      h_lep3Eta[iCh]->Write();
      h_el1Eta[iCh]->Write();
      h_el2Eta[iCh]->Write();
      h_el3Eta[iCh]->Write();
      h_mu1Eta[iCh]->Write();
      h_mu2Eta[iCh]->Write();
      h_mu3Eta[iCh]->Write();
      h_PRbaselep1Pt [iCh]->Write();
      h_PRbaselep2Pt [iCh]->Write();
      h_PRbaselep3Pt [iCh]->Write();
      h_CObaselep1Pt [iCh]->Write();
      h_CObaselep2Pt [iCh]->Write();
      h_CObaselep3Pt [iCh]->Write();
      h_HFbaselep1Pt [iCh]->Write();
      h_HFbaselep2Pt [iCh]->Write();
      h_HFbaselep3Pt [iCh]->Write();
      h_LFbaselep1Pt [iCh]->Write();
      h_LFbaselep2Pt [iCh]->Write();
      h_LFbaselep3Pt [iCh]->Write();
      h_UKbaselep1Pt [iCh]->Write();
      h_UKbaselep2Pt [iCh]->Write();
      h_UKbaselep3Pt [iCh]->Write();
      h_PRbaselep1Eta[iCh]->Write();
      h_PRbaselep2Eta[iCh]->Write();
      h_PRbaselep3Eta[iCh]->Write();
      h_CObaselep1Eta[iCh]->Write();
      h_CObaselep2Eta[iCh]->Write();
      h_CObaselep3Eta[iCh]->Write();
      h_HFbaselep1Eta[iCh]->Write();
      h_HFbaselep2Eta[iCh]->Write();
      h_HFbaselep3Eta[iCh]->Write();
      h_LFbaselep1Eta[iCh]->Write();
      h_LFbaselep2Eta[iCh]->Write();
      h_LFbaselep3Eta[iCh]->Write();
      h_UKbaselep1Eta[iCh]->Write();
      h_UKbaselep2Eta[iCh]->Write();
      h_UKbaselep3Eta[iCh]->Write();
      h_CObaselep2and3Pt [iCh]->Write();
      h_LFbaselep2and3Pt [iCh]->Write();
      h_PRbaselep2and3Eta[iCh]->Write();
      h_HFbaselep2and3Eta[iCh]->Write();
      h_LFbaselep2and3Eta[iCh]->Write();
      h_UKbaselep2and3Eta[iCh]->Write();
      h_PRbaseEl2Pt     [iCh]->Write();
      h_PRbaseEl3Pt     [iCh]->Write();
      h_PRbaseEl2and3Pt [iCh]->Write();
      h_PRbaseEl2Eta    [iCh]->Write();
      h_PRbaseEl3Eta    [iCh]->Write();
      h_PRbaseEl2and3Eta[iCh]->Write();
      h_CObaseEl2Pt     [iCh]->Write();
      h_CObaseEl3Pt     [iCh]->Write();
      h_CObaseEl2and3Pt [iCh]->Write();
      h_CObaseEl2Eta    [iCh]->Write();
      h_CObaseEl3Eta    [iCh]->Write();
      h_CObaseEl2and3Eta[iCh]->Write();
      h_HFbaseEl2Pt     [iCh]->Write();
      h_HFbaseEl3Pt     [iCh]->Write();
      h_HFbaseEl2and3Pt [iCh]->Write();
      h_HFbaseEl2Eta    [iCh]->Write();
      h_HFbaseEl3Eta    [iCh]->Write();
      h_HFbaseEl2and3Eta[iCh]->Write();
      h_LFbaseEl2Pt     [iCh]->Write();
      h_LFbaseEl3Pt     [iCh]->Write();
      h_LFbaseEl2and3Pt [iCh]->Write();
      h_LFbaseEl2Eta    [iCh]->Write();
      h_LFbaseEl3Eta    [iCh]->Write();
      h_LFbaseEl2and3Eta[iCh]->Write();
      h_UKbaseEl2Pt     [iCh]->Write();
      h_UKbaseEl3Pt     [iCh]->Write();
      h_UKbaseEl2and3Pt [iCh]->Write();
      h_UKbaseEl2Eta    [iCh]->Write();
      h_UKbaseEl3Eta    [iCh]->Write();
      h_UKbaseEl2and3Eta[iCh]->Write();
      h_PRbaseMu2Pt     [iCh]->Write();
      h_PRbaseMu3Pt     [iCh]->Write();
      h_PRbaseMu2and3Pt [iCh]->Write();
      h_PRbaseMu2Eta    [iCh]->Write();
      h_PRbaseMu3Eta    [iCh]->Write();
      h_PRbaseMu2and3Eta[iCh]->Write();
      h_CObaseMu2Pt     [iCh]->Write();
      h_CObaseMu3Pt     [iCh]->Write();
      h_CObaseMu2and3Pt [iCh]->Write();
      h_CObaseMu2Eta    [iCh]->Write();
      h_CObaseMu3Eta    [iCh]->Write();
      h_CObaseMu2and3Eta[iCh]->Write();
      h_HFbaseMu2Pt     [iCh]->Write();
      h_HFbaseMu3Pt     [iCh]->Write();
      h_HFbaseMu2and3Pt [iCh]->Write();
      h_HFbaseMu2Eta    [iCh]->Write();
      h_HFbaseMu3Eta    [iCh]->Write();
      h_HFbaseMu2and3Eta[iCh]->Write();
      h_LFbaseMu2Pt     [iCh]->Write();
      h_LFbaseMu3Pt     [iCh]->Write();
      h_LFbaseMu2and3Pt [iCh]->Write();
      h_LFbaseMu2Eta    [iCh]->Write();
      h_LFbaseMu3Eta    [iCh]->Write();
      h_LFbaseMu2and3Eta[iCh]->Write();
      h_UKbaseMu2Pt     [iCh]->Write();
      h_UKbaseMu3Pt     [iCh]->Write();
      h_UKbaseMu2and3Pt [iCh]->Write();
      h_UKbaseMu2Eta    [iCh]->Write();
      h_UKbaseMu3Eta    [iCh]->Write();
      h_UKbaseMu2and3Eta[iCh]->Write();
      h_PREl2Pt     [iCh]->Write();
      h_PREl3Pt     [iCh]->Write();
      h_PREl2and3Pt [iCh]->Write();
      h_PREl2Eta    [iCh]->Write();
      h_PREl3Eta    [iCh]->Write();
      h_PREl2and3Eta[iCh]->Write();
      h_COEl2Pt     [iCh]->Write();
      h_COEl3Pt     [iCh]->Write();
      h_COEl2and3Pt [iCh]->Write();
      h_COEl2Eta    [iCh]->Write();
      h_COEl3Eta    [iCh]->Write();
      h_COEl2and3Eta[iCh]->Write();
      h_HFEl2Pt     [iCh]->Write();
      h_HFEl3Pt     [iCh]->Write();
      h_HFEl2and3Pt [iCh]->Write();
      h_HFEl2Eta    [iCh]->Write();
      h_HFEl3Eta    [iCh]->Write();
      h_HFEl2and3Eta[iCh]->Write();
      h_LFEl2Pt     [iCh]->Write();
      h_LFEl3Pt     [iCh]->Write();
      h_LFEl2and3Pt [iCh]->Write();
      h_LFEl2Eta    [iCh]->Write();
      h_LFEl3Eta    [iCh]->Write();
      h_LFEl2and3Eta[iCh]->Write();
      h_UKEl2Pt     [iCh]->Write();
      h_UKEl3Pt     [iCh]->Write();
      h_UKEl2and3Pt [iCh]->Write();
      h_UKEl2Eta    [iCh]->Write();
      h_UKEl3Eta    [iCh]->Write();
      h_UKEl2and3Eta[iCh]->Write();
      h_PRMu2Pt     [iCh]->Write();
      h_PRMu3Pt     [iCh]->Write();
      h_PRMu2and3Pt [iCh]->Write();
      h_PRMu2Eta    [iCh]->Write();
      h_PRMu3Eta    [iCh]->Write();
      h_PRMu2and3Eta[iCh]->Write();
      h_COMu2Pt     [iCh]->Write();
      h_COMu3Pt     [iCh]->Write();
      h_COMu2and3Pt [iCh]->Write();
      h_COMu2Eta    [iCh]->Write();
      h_COMu3Eta    [iCh]->Write();
      h_COMu2and3Eta[iCh]->Write();
      h_HFMu2Pt     [iCh]->Write();
      h_HFMu3Pt     [iCh]->Write();
      h_HFMu2and3Pt [iCh]->Write();
      h_HFMu2Eta    [iCh]->Write();
      h_HFMu3Eta    [iCh]->Write();
      h_HFMu2and3Eta[iCh]->Write();
      h_LFMu2Pt     [iCh]->Write();
      h_LFMu3Pt     [iCh]->Write();
      h_LFMu2and3Pt [iCh]->Write();
      h_LFMu2Eta    [iCh]->Write();
      h_LFMu3Eta    [iCh]->Write();
      h_LFMu2and3Eta[iCh]->Write();
      h_UKMu2Pt     [iCh]->Write();
      h_UKMu3Pt     [iCh]->Write();
      h_UKMu2and3Pt [iCh]->Write();
      h_UKMu2Eta    [iCh]->Write();
      h_UKMu3Eta    [iCh]->Write();
      h_UKMu2and3Eta[iCh]->Write();
      h_fakeEl2Pt     [iCh]->Write();
      h_fakeEl3Pt     [iCh]->Write();
      h_fakeEl2and3Pt [iCh]->Write();
      h_fakeEl2Eta    [iCh]->Write();
      h_fakeEl3Eta    [iCh]->Write();
      h_fakeEl2and3Eta[iCh]->Write();
      h_fakeMu2Pt     [iCh]->Write();
      h_fakeMu3Pt     [iCh]->Write();
      h_fakeMu2and3Pt [iCh]->Write();
      h_fakeMu2Eta    [iCh]->Write();
      h_fakeMu3Eta    [iCh]->Write();
      h_fakeMu2and3Eta[iCh]->Write();
      h_sgnlfakelepClass    [iCh]->Write();
      h_fakeEl2and3Class    [iCh]->Write();
      h_fakeMu2and3Class    [iCh]->Write();
      h_basefakelepClass    [iCh]->Write();
      h_fakebaseEl2and3Class[iCh]->Write();
      h_fakebaseMu2and3Class[iCh]->Write(); 
      h_fakebaseEl2and3Class[iCh]->Write();
      h_fakebaseMu2and3Class[iCh]->Write(); 
      h_baselepOrigin       [iCh]->Write();
      h_baselep1Origin      [iCh]->Write();
      h_baselep2Origin      [iCh]->Write();
      h_baselep3Origin      [iCh]->Write();
      h_baselepClass        [iCh]->Write();
      h_baselep1Class       [iCh]->Write();
      h_baselep2Class       [iCh]->Write();
      h_baselep3Class       [iCh]->Write();
      h_llPt[iCh]->Write();
      h_sumLepPt[iCh]->Write();
      h_sumLepPtMet[iCh]->Write();
      h_dPhiWZ[iCh]->Write();
      h_nMuComb[iCh]->Write();
      h_elTrackIso[iCh]->Write();
      h_elCaloIso[iCh]->Write();
      h_muTrackIso[iCh]->Write();
      h_muCaloIso[iCh]->Write();
      h_el1TrackIso[iCh]->Write();
      h_el1CaloIso[iCh]->Write();
      h_mu1TrackIso[iCh]->Write();
      h_mu1CaloIso[iCh]->Write();
      h_el2TrackIso[iCh]->Write();
      h_el2CaloIso[iCh]->Write();
      h_mu2TrackIso[iCh]->Write();
      h_mu2CaloIso[iCh]->Write();
      h_el3TrackIso[iCh]->Write();
      h_el3CaloIso[iCh]->Write();
      h_mu3TrackIso[iCh]->Write();
      h_mu3CaloIso[iCh]->Write();
      h_lepD0[iCh]->Write();
      h_lep1D0[iCh]->Write();
      h_lep2D0[iCh]->Write();
      h_lep3D0[iCh]->Write();
      h_lepZ0[iCh]->Write();
      h_lep1Z0[iCh]->Write();
      h_lep2Z0[iCh]->Write();
      h_lep3Z0[iCh]->Write();
      h_lepD0Sig[iCh]->Write();
      h_lep1D0Sig[iCh]->Write();
      h_lep2D0Sig[iCh]->Write();
      h_lep3D0Sig[iCh]->Write();
      h_lepZ0SinTheta[iCh]->Write();
      h_lep1Z0SinTheta[iCh]->Write();
      h_lep2Z0SinTheta[iCh]->Write();
      h_lep3Z0SinTheta[iCh]->Write();
      h_lepOrigin[iCh]->Write();
      h_lep1Origin[iCh]->Write();
      h_lep2Origin[iCh]->Write();
      h_lep3Origin[iCh]->Write();
      h_lepClass[iCh]->Write();
      h_lep1Class[iCh]->Write();
      h_lep2Class[iCh]->Write();
      h_lep3Class[iCh]->Write();
      h_hasSS[iCh]->Write();
      h_nTau[iCh]->Write();
      h_tauPt[iCh]->Write();
      h_tau1Pt[iCh]->Write();
      h_tau2Pt[iCh]->Write();
      h_tau1Eta[iCh]->Write();
      h_tau2Eta[iCh]->Write();
      h_tauProng[iCh]->Write();
      h_dPhiTauTau[iCh]->Write();
      h_tauClass[iCh]->Write();
      h_tau1Class[iCh]->Write();
      h_tau2Class[iCh]->Write();
      h_minDRLepLep[iCh]->Write();
      h_dPhiLep1Met[iCh]->Write();
      h_dPhiLep2Met[iCh]->Write();
      h_dPhiLep3Met[iCh]->Write();
      h_dPhiLLMet[iCh]->Write();
      h_dPhiLLJet[iCh]->Write();
      h_dPhiJet1Met[iCh]->Write();
      h_dPhiJet2Met[iCh]->Write();
      h_dPhiJJMet[iCh]->Write();
      h_minDPhiLepLep[iCh]->Write();
      h_minDPhiLepMet[iCh]->Write();
      h_minDPhiJetJet[iCh]->Write();
      h_minDPhiJetMet[iCh]->Write();
      h_minDPhiTauMet[iCh]->Write();
      h_minDRLepJet[iCh]->Write();
      h_maxDPhiLepLep[iCh]->Write();
      h_maxDPhiLepMet[iCh]->Write();
      h_maxDPhiJetJet[iCh]->Write();
      h_maxDPhiJetMet[iCh]->Write();
      h_maxDRLepJet[iCh]->Write();
      h_dEtaLL[iCh]->Write();
      h_dEtaJetJet[iCh]->Write();
      h_maxDEtaJetJet[iCh]->Write();
      h_minDEtaJetJet[iCh]->Write();
      h_met[iCh]->Write();
      h_meff[iCh]->Write();
      h_meffNoMet[iCh]->Write();
      h_metEle[iCh]->Write();
      h_metMuo[iCh]->Write();
      h_metJet[iCh]->Write();
      h_metCell[iCh]->Write();
      h_metRel[iCh]->Write();
      h_nJet[iCh]->Write();
      h_nBJet[iCh]->Write();
      h_jetPt[iCh]->Write();
      h_jet1Pt[iCh]->Write();
      h_jet2Pt[iCh]->Write();
      h_jet3Pt[iCh]->Write();
      h_bJetPt[iCh]->Write();
      h_bJet1Pt[iCh]->Write();
      h_bJet2Pt[iCh]->Write();
      h_bJet3Pt[iCh]->Write();
      h_jetEta[iCh]->Write();
      h_bJetEta[iCh]->Write();
      h_bJet1Eta[iCh]->Write();
      h_bJet2Eta[iCh]->Write();
      h_bJet3Eta[iCh]->Write();
      h_jetPhi[iCh]->Write();
      h_jetMV1[iCh]->Write();
      h_jetJVF[iCh]->Write();
      h_bJetJVF[iCh]->Write();
      h_dijetM[iCh]->Write();
      h_productEta1Eta2[iCh]->Write();
      h_mll[iCh]->Write();
      h_msfos[iCh]->Write();
      h_minMsfos[iCh]->Write();
      h_msfss[iCh]->Write();
      h_mlll[iCh]->Write();
      h_mt[iCh]->Write();
      h_mtL1[iCh]->Write();
      h_mtL2[iCh]->Write();
      h_mt2[iCh]->Write();
      h_mjj[iCh]->Write();
      h_mlljj[iCh]->Write();
      h_mlt[iCh]->Write();
      h_mtt[iCh]->Write();
      h_mbb[iCh]->Write();
      h_mljj[iCh]->Write();
      h_mljjFine[iCh]->Write();
      h_mtll[iCh]->Write();
      h_mtll_mt[iCh]->Write();
      h_nVtx[iCh]->Write();
      h_mu[iCh]->Write();
      h_elParentPdgId[iCh]->Write();
      h_muParentPdgId[iCh]->Write();
    }
  }

  delete m_MMTool;

  MyAlways("finalize()",Form("Finalizing TFile: %s",m_rootfile->GetName()));
  if(m_sys!=""){
    m_rootfile->Close();
    delete m_rootfile;
  }
}

/*--------------------------------------------------------------------------------*/
bool Plotter::BookHistograms()
{
  MyDebug("BookHistograms()","Plotter::BookHistograms()");

  std::vector<std::string> vec_chan; vec_chan.clear();
  vec_chan.push_back("all");
  vec_chan.push_back("eee");
  vec_chan.push_back("eem");
  vec_chan.push_back("emm");
  vec_chan.push_back("mmm");

  // Preprocessor convenience                                                                                 
  // make a histogram by name (leave off the "h_") and binning
#define NEWHIST(name, xLbl, nbin, min, max)                             \
  h_ ## name[iCh] = new TH1F((chanName+"_"+#name).c_str(), #name ";" xLbl, nbin, min, max);
#define NEWVARHIST(name, xLbl, nbin, bins)                              \
  h_ ## name[iCh] = new TH1F((chanName+"_"+#name).c_str(), #name ";" xLbl, nbin, bins);
#define NEWVARHIST2D(name, lbl, nXbin, xBins, nYBin, yBins) \
  h_ ## name[iCh] = new TH2F((chanName+"_"+#name).c_str(), #name ";" lbl, nXbin, xBins, nYBin, yBins);
  // shorthand way to set bin labels, since it is kind of gross                                               
#define SETBINLABEL(name, bin, label)                         \
  h_ ## name[iCh]->GetXaxis()->SetBinLabel(bin, label)
  // Variable binning for lep1Pt
#define PT1HIST(name, xLbl) NEWVARHIST(name, xLbl, nLep1PtBins, lep1PtBins)
  // MY choice of binnings                                                                                    
#define ETAHIST(name, xLbl) NEWHIST(name, xLbl, 10, -2.5, 2.5)
#define PTHIST(name, xLbl) NEWHIST(name, xLbl, 25, 0., 500.)
#define DRHIST(name, xLbl) NEWVARHIST(name, xLbl, nDRBins, dRBins)
#define DPHIHIST(name, xLbl) NEWHIST(name, xLbl, 10, 0., 3.1416)
#define MASSHIST(name, xLbl) NEWHIST(name, xLbl, 25, 0., 500.)
#define ZMASSHIST(name, xLbl) NEWHIST(name, xLbl, 25, 1.2, 501.2) // shifted for Z window 

  ///////////////////////////////////////////////////////////////
  //Defining histograms
  ///////////////////////////////////////////////////////////////
  TH1F::SetDefaultSumw2();

  // Histogram to store the MC cross section
  h_xsec = new TH1F("h_xsec","h_xsec;;MC cross-section[pb]",1,0.,1.);
  h_nEve = new TH1F("h_nEve","h_nEve;;Events"              ,1,0.,1.);
  h_xsec->SetBinContent(h_xsec->FindBin(0.),m_crossSection); //set cross-section
  h_nSigBaseLep = new TH2F("h_nSigBaseLep","h_nSigBaseLep;#Baseline Lep.;#Signal Lep.;Events",8,0.-0.5,8.-0.5,8,0.-0.5,8.-0.5);

  // Lepton channel histo, only defined for the 'all' channel
  h_lepChan    [Ch_all] = new TH1F("all_lepChan"    ,"all_lepChan;Unordered lepton channel;Events", nChan, 0, nChan);
  h_baselepChan[Ch_all] = new TH1F("all_baselepChan","all_baselepChan;Unordered lepton channel;Events", nChan, 0, nChan);
  // lepton channel loop
  for(uint iCh=0; iCh<nChan; iCh++){
    std::string chanName = vec_chan.at(iCh);
    h_lepChan[Ch_all]->GetXaxis()->SetBinLabel(iCh+1, chanName.c_str());
    h_baselepChan[Ch_all]->GetXaxis()->SetBinLabel(iCh+1, chanName.c_str());
    // MM weight
    NEWHIST( mmWeight, "MM Weight;Events", 100, -3., 3.);
    // lep pt - my choice of binning                                                                            
    NEWVARHIST( lep1Pt, "Leading lepton_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( lep2Pt, "Second lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( lep3Pt, "Third lepton_{} P_{T} [GeV];Events", nLep3PtBins, lep3PtBins );
    NEWVARHIST( el1Pt, "Leading electron_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( el2Pt, "Second electron_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( el3Pt, "Third electron_{} P_{T} [GeV];Events", nLep3PtBins, lep3PtBins );
    NEWVARHIST( mu1Pt, "Leading muon_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( mu2Pt, "Second muon_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( mu3Pt, "Third muon_{} P_{T} [GeV];Events", nLep3PtBins, lep3PtBins );
    NEWVARHIST( PREl2Pt,               "Second PR electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PREl3Pt,                "Third PR electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PREl2and3Pt, "Second and Third PR electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( COEl2Pt,               "Second CO electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( COEl3Pt,                "Third CO electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( COEl2and3Pt, "Second and Third CO electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFEl2Pt,               "Second HF electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFEl3Pt,                "Third HF electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFEl2and3Pt, "Second and Third HF electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFEl2Pt,               "Second LF electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFEl3Pt,                "Third LF electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFEl2and3Pt, "Second and Third LF electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKEl2Pt,               "Second UK electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKEl3Pt,                "Third UK electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKEl2and3Pt, "Second and Third UK electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PRMu2Pt,               "Second PR muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PRMu3Pt,                "Third PR muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PRMu2and3Pt, "Second and Third PR muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( COMu2Pt,               "Second CO muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( COMu3Pt,                "Third CO muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( COMu2and3Pt, "Second and Third CO muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFMu2Pt,               "Second HF muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFMu3Pt,                "Third HF muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFMu2and3Pt, "Second and Third HF muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFMu2Pt,               "Second LF muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFMu3Pt,                "Third LF muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFMu2and3Pt, "Second and Third LF muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKMu2Pt,               "Second UK muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKMu3Pt,                "Third UK muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKMu2and3Pt, "Second and Third UK muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeEl2Pt,               "Second fake electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeEl3Pt,                "Third fake electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeEl2and3Pt, "Second and Third fake electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeMu2Pt,               "Second fake muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeMu3Pt,                "Third fake muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeMu2and3Pt, "Second and Third fake muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );


    // lep eta
    ETAHIST( lep1Eta, "Leading lepton #eta;Events" );
    ETAHIST( lep2Eta, "Second lepton #eta;Events" );
    ETAHIST( lep3Eta, "Third lepton #eta;Events" );
    ETAHIST( el1Eta, "Leading electron #eta;Events" );
    ETAHIST( el2Eta, "Second electron #eta;Events" );
    ETAHIST( el3Eta, "Third electron #eta;Events" );
    ETAHIST( mu1Eta, "Leading muon #eta;Events" );
    ETAHIST( mu2Eta, "Second muon #eta;Events" );
    ETAHIST( mu3Eta, "Third muon #eta;Events" );
    ETAHIST( PREl2Eta,               "Second PR electron_{} #eta;Events" );
    ETAHIST( PREl3Eta,                "Third PR electron_{} #eta;Events" );
    ETAHIST( PREl2and3Eta, "Second and Third PR electron_{} #eta;Events" );
    ETAHIST( COEl2Eta,               "Second CO electron_{} #eta;Events" );
    ETAHIST( COEl3Eta,                "Third CO electron_{} #eta;Events" );
    ETAHIST( COEl2and3Eta, "Second and Third CO electron_{} #eta;Events" );
    ETAHIST( HFEl2Eta,               "Second HF electron_{} #eta;Events" );
    ETAHIST( HFEl3Eta,                "Third HF electron_{} #eta;Events" );
    ETAHIST( HFEl2and3Eta, "Second and Third HF electron_{} #eta;Events" );
    ETAHIST( LFEl2Eta,               "Second LF electron_{} #eta;Events" );
    ETAHIST( LFEl3Eta,                "Third LF electron_{} #eta;Events" );
    ETAHIST( LFEl2and3Eta, "Second and Third LF electron_{} #eta;Events" );
    ETAHIST( UKEl2Eta,               "Second UK electron_{} #eta;Events" );
    ETAHIST( UKEl3Eta,                "Third UK electron_{} #eta;Events" );
    ETAHIST( UKEl2and3Eta, "Second and Third UK electron_{} #eta;Events" );
    ETAHIST( PRMu2Eta,               "Second PR muon_{} #eta;Events" );
    ETAHIST( PRMu3Eta,                "Third PR muon_{} #eta;Events" );
    ETAHIST( PRMu2and3Eta, "Second and Third PR muon_{} #eta;Events" );
    ETAHIST( COMu2Eta,               "Second CO muon_{} #eta;Events" );
    ETAHIST( COMu3Eta,                "Third CO muon_{} #eta;Events" );
    ETAHIST( COMu2and3Eta, "Second and Third CO muon_{} #eta;Events" );
    ETAHIST( HFMu2Eta,               "Second HF muon_{} #eta;Events" );
    ETAHIST( HFMu3Eta,                "Third HF muon_{} #eta;Events" );
    ETAHIST( HFMu2and3Eta, "Second and Third HF muon_{} #eta;Events" );
    ETAHIST( LFMu2Eta,               "Second LF muon_{} #eta;Events" );
    ETAHIST( LFMu3Eta,                "Third LF muon_{} #eta;Events" );
    ETAHIST( LFMu2and3Eta, "Second and Third LF muon_{} #eta;Events" );
    ETAHIST( UKMu2Eta,               "Second UK muon_{} #eta;Events" );
    ETAHIST( UKMu3Eta,                "Third UK muon_{} #eta;Events" );
    ETAHIST( UKMu2and3Eta, "Second and Third UK muon_{} #eta;Events" );
    ETAHIST( fakeEl2Eta,               "Second fake electron_{} #eta;Events" );
    ETAHIST( fakeEl3Eta,                "Third fake electron_{} #eta;Events" );
    ETAHIST( fakeEl2and3Eta, "Second and Third fake electron_{} #eta;Events" );
    ETAHIST( fakeMu2Eta,               "Second fake muon_{} #eta;Events" );
    ETAHIST( fakeMu3Eta,                "Third fake muon_{} #eta;Events" );
    ETAHIST( fakeMu2and3Eta, "Second and Third fake muon_{} #eta;Events" );

    // baseline lep pt
    NEWVARHIST( PRbaseEl2Pt,               "Second PR baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PRbaseEl3Pt,                "Third PR baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PRbaseEl2and3Pt, "Second and Third PR baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( CObaseEl2Pt,               "Second CO baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( CObaseEl3Pt,                "Third CO baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( CObaseEl2and3Pt, "Second and Third CO baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFbaseEl2Pt,               "Second HF baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFbaseEl3Pt,                "Third HF baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFbaseEl2and3Pt, "Second and Third HF baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFbaseEl2Pt,               "Second LF baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFbaseEl3Pt,                "Third LF baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFbaseEl2and3Pt, "Second and Third LF baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKbaseEl2Pt,               "Second UK baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKbaseEl3Pt,                "Third UK baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKbaseEl2and3Pt, "Second and Third UK baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PRbaseMu2Pt,               "Second PR baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PRbaseMu3Pt,                "Third PR baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( PRbaseMu2and3Pt, "Second and Third PR baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( CObaseMu2Pt,               "Second CO baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( CObaseMu3Pt,                "Third CO baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( CObaseMu2and3Pt, "Second and Third CO baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFbaseMu2Pt,               "Second HF baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFbaseMu3Pt,                "Third HF baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( HFbaseMu2and3Pt, "Second and Third HF baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFbaseMu2Pt,               "Second LF baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFbaseMu3Pt,                "Third LF baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( LFbaseMu2and3Pt, "Second and Third LF baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKbaseMu2Pt,               "Second UK baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKbaseMu3Pt,                "Third UK baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( UKbaseMu2and3Pt, "Second and Third UK baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeBaseEl2Pt,               "Second fake baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeBaseEl3Pt,                "Third fake baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeBaseEl2and3Pt, "Second and Third fake baseline electron_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeBaseMu2Pt,               "Second fake baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeBaseMu3Pt,                "Third fake baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );
    NEWVARHIST( fakeBaseMu2and3Pt, "Second and Third fake baseline muon_{} P_{T} [Gev];Events", nMmEffPtBins, mmEffPtBins );

    NEWVARHIST( PRbaselep1Pt, "Leading PR base lepton_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( PRbaselep2Pt,  "Second PR base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( PRbaselep3Pt,   "Third PR base lepton_{} P_{T} [GeV];Events", nLep3PtBins, lep3PtBins );
    NEWVARHIST( CObaselep1Pt, "Leading CO base lepton_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( CObaselep2Pt,  "Second CO base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( CObaselep3Pt,   "Third CO base lepton_{} P_{T} [GeV];Events", nLep3PtBins, lep3PtBins );
    NEWVARHIST( HFbaselep1Pt, "Leading HF base lepton_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( HFbaselep2Pt,  "Second HF base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( HFbaselep3Pt,   "Third HF base lepton_{} P_{T} [GeV];Events", nLep3PtBins, lep3PtBins );
    NEWVARHIST( LFbaselep1Pt, "Leading LF base lepton_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( LFbaselep2Pt,  "Second LF base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( LFbaselep3Pt,   "Third LF base lepton_{} P_{T} [GeV];Events", nLep3PtBins, lep3PtBins );
    NEWVARHIST( UKbaselep1Pt, "Leading UK base lepton_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( UKbaselep2Pt,  "Second UK base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( UKbaselep3Pt,   "Third UK base lepton_{} P_{T} [GeV];Events", nLep3PtBins, lep3PtBins );

    NEWVARHIST( PRbaselep2and3Pt, "Second and third PR base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( CObaselep2and3Pt, "Second and third CO base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( HFbaselep2and3Pt, "Second and third HF base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( LFbaselep2and3Pt, "Second and third LF base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    NEWVARHIST( UKbaselep2and3Pt, "Second and third UK base lepton_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    // baseline lep eta
    ETAHIST( PRbaseEl2Eta,               "Second PR baseline electron_{} #eta;Events" );
    ETAHIST( PRbaseEl3Eta,                "Third PR baseline electron_{} #eta;Events" );
    ETAHIST( PRbaseEl2and3Eta, "Second and Third PR baseline electron_{} #eta;Events" );
    ETAHIST( CObaseEl2Eta,               "Second CO baseline electron_{} #eta;Events" );
    ETAHIST( CObaseEl3Eta,                "Third CO baseline electron_{} #eta;Events" );
    ETAHIST( CObaseEl2and3Eta, "Second and Third CO baseline electron_{} #eta;Events" );
    ETAHIST( HFbaseEl2Eta,               "Second HF baseline electron_{} #eta;Events" );
    ETAHIST( HFbaseEl3Eta,                "Third HF baseline electron_{} #eta;Events" );
    ETAHIST( HFbaseEl2and3Eta, "Second and Third HF baseline electron_{} #eta;Events" );
    ETAHIST( LFbaseEl2Eta,               "Second LF baseline electron_{} #eta;Events" );
    ETAHIST( LFbaseEl3Eta,                "Third LF baseline electron_{} #eta;Events" );
    ETAHIST( LFbaseEl2and3Eta, "Second and Third LF baseline electron_{} #eta;Events" );
    ETAHIST( UKbaseEl2Eta,               "Second UK baseline electron_{} #eta;Events" );
    ETAHIST( UKbaseEl3Eta,                "Third UK baseline electron_{} #eta;Events" );
    ETAHIST( UKbaseEl2and3Eta, "Second and Third UK baseline electron_{} #eta;Events" );
    ETAHIST( PRbaseMu2Eta,               "Second PR baseline muon_{} #eta;Events" );
    ETAHIST( PRbaseMu3Eta,                "Third PR baseline muon_{} #eta;Events" );
    ETAHIST( PRbaseMu2and3Eta, "Second and Third PR baseline muon_{} #eta;Events" );
    ETAHIST( CObaseMu2Eta,               "Second CO baseline muon_{} #eta;Events" );
    ETAHIST( CObaseMu3Eta,                "Third CO baseline muon_{} #eta;Events" );
    ETAHIST( CObaseMu2and3Eta, "Second and Third CO baseline muon_{} #eta;Events" );
    ETAHIST( HFbaseMu2Eta,               "Second HF baseline muon_{} #eta;Events" );
    ETAHIST( HFbaseMu3Eta,                "Third HF baseline muon_{} #eta;Events" );
    ETAHIST( HFbaseMu2and3Eta, "Second and Third HF baseline muon_{} #eta;Events" );
    ETAHIST( LFbaseMu2Eta,               "Second LF baseline muon_{} #eta;Events" );
    ETAHIST( LFbaseMu3Eta,                "Third LF baseline muon_{} #eta;Events" );
    ETAHIST( LFbaseMu2and3Eta, "Second and Third LF baseline muon_{} #eta;Events" );
    ETAHIST( UKbaseMu2Eta,               "Second UK baseline muon_{} #eta;Events" );
    ETAHIST( UKbaseMu3Eta,                "Third UK baseline muon_{} #eta;Events" );
    ETAHIST( UKbaseMu2and3Eta, "Second and Third UK baseline muon_{} #eta;Events" );
    ETAHIST( fakeBaseEl2Eta,               "Second fake baseline electron_{} #eta;Events" );
    ETAHIST( fakeBaseEl3Eta,                "Third fake baseline electron_{} #eta;Events" );
    ETAHIST( fakeBaseEl2and3Eta, "Second and Third fake baseline electron_{} #eta;Events" );
    ETAHIST( fakeBaseMu2Eta,               "Second fake baseline muon_{} #eta;Events" );
    ETAHIST( fakeBaseMu3Eta,                "Third fake baseline muon_{} #eta;Events" );
    ETAHIST( fakeBaseMu2and3Eta, "Second and Third fake baseline muon_{} #eta;Events" );

    ETAHIST( PRbaselep1Eta, "Leading PR base lepton #eta;Events" );
    ETAHIST( PRbaselep2Eta,  "Second PR base lepton #eta;Events" );
    ETAHIST( PRbaselep3Eta,   "Third PR base lepton #eta;Events" );
    ETAHIST( CObaselep1Eta, "Leading CO base lepton #eta;Events" );
    ETAHIST( CObaselep2Eta,  "Second CO base lepton #eta;Events" );
    ETAHIST( CObaselep3Eta,   "Third CO base lepton #eta;Events" );
    ETAHIST( HFbaselep1Eta, "Leading HF base lepton #eta;Events" );
    ETAHIST( HFbaselep2Eta,  "Second HF base lepton #eta;Events" );
    ETAHIST( HFbaselep3Eta,   "Third HF base lepton #eta;Events" );
    ETAHIST( LFbaselep1Eta, "Leading LF base lepton #eta;Events" );
    ETAHIST( LFbaselep2Eta,  "Second LF base lepton #eta;Events" );
    ETAHIST( LFbaselep3Eta,   "Third LF base lepton #eta;Events" );
    ETAHIST( UKbaselep1Eta, "Leading UK base lepton #eta;Events" );
    ETAHIST( UKbaselep2Eta,  "Second UK base lepton #eta;Events" );
    ETAHIST( UKbaselep3Eta,   "Third UK base lepton #eta;Events" );

    ETAHIST( PRbaselep2and3Eta, "Second and third PR base lepton #eta;Events" );
    ETAHIST( CObaselep2and3Eta, "Second and third CO base lepton #eta;Events" );
    ETAHIST( HFbaselep2and3Eta, "Second and third HF base lepton #eta;Events" );
    ETAHIST( LFbaselep2and3Eta, "Second and third LF base lepton #eta;Events" );
    ETAHIST( UKbaselep2and3Eta, "Second and third UK base lepton #eta;Events" );


    // signal/base lepton for real efficiency/fake rate
    NEWHIST( sgnlfakelepClass,      "signal lepton classcation", 6, 0, 6 );
    NEWHIST( basefakelepClass, "baseline lepton classification", 6, 0, 6 );
    SETBINLABEL( sgnlfakelepClass, 1, "El+Mu Real" ); 
    SETBINLABEL( sgnlfakelepClass, 2, "El+Mu Fake" ); 
    SETBINLABEL( sgnlfakelepClass, 3,    "El Real" ); 
    SETBINLABEL( sgnlfakelepClass, 4,    "El Fake" ); 
    SETBINLABEL( sgnlfakelepClass, 5,    "Mu Real" ); 
    SETBINLABEL( sgnlfakelepClass, 6,    "Mu Fake" ); 
    SETBINLABEL( basefakelepClass, 1, "El+Mu Real" ); 
    SETBINLABEL( basefakelepClass, 2, "El+Mu Fake" ); 
    SETBINLABEL( basefakelepClass, 3,    "El Real" ); 
    SETBINLABEL( basefakelepClass, 4,    "El Fake" ); 
    SETBINLABEL( basefakelepClass, 5,    "Mu Real" ); 
    SETBINLABEL( basefakelepClass, 6,    "Mu Fake" ); 

    // lepton system pt
    NEWVARHIST( llPt, "P_{T}(lep,lep) [GeV];Events", nLep1PtBins, lep1PtBins );

    // sum of lepton pt and MET
    NEWVARHIST( sumLepPt, "#Sigma |lepton P_{T}| [GeV];Events", nMeffBins, meffBins );
    NEWVARHIST( sumLepPtMet, "#Sigma |lepton_{} P_{T}| + |MET| [GeV];Events", nMeffBins, meffBins );

    // dPhi WZ
    DPHIHIST( dPhiWZ, "dPhi(SFOS,l);Events" );

    // combined muons
    NEWHIST( nMuComb, "Number of combined muons;Events", 4, -0.5, 3.5 );

    // isolation vars
    NEWHIST( elTrackIso, "Electron TrackIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( elCaloIso, "Electron CaloIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( muTrackIso, "Muon TrackIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( muCaloIso, "Muon CaloIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( el1TrackIso, "Leading electron TrackIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( el1CaloIso, "Leading electron CaloIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( mu1TrackIso, "Leading muon TrackIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( mu1CaloIso, "Leading muon CaloIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( el2TrackIso, "Second electron TrackIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( el2CaloIso, "Second electron CaloIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( mu2TrackIso, "Second muon TrackIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( mu2CaloIso, "Second muon CaloIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( el3TrackIso, "Third electron TrackIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( el3CaloIso, "Third electron CaloIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( mu3TrackIso, "Third muon TrackIso/P_{T};Events", 50, 0, 1 );
    NEWHIST( mu3CaloIso, "Third muon CaloIso/P_{T};Events", 50, 0, 1 );

    // impact parameters
    NEWHIST( lepD0, "Lepton |d0| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep1D0, "Leading lepton |d0| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep2D0, "Second lepton |d0| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep3D0, "Third lepton |d0| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lepZ0, "Lepton |z0| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep1Z0, "Leading lepton |z0| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep2Z0, "Second lepton |z0| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep3Z0, "Third lepton |z0| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lepD0Sig, "Lepton |d0/#sigma_{d0}|;Events", 25, 0, 6.0 );
    NEWHIST( lep1D0Sig, "Leading lepton |d0/#sigma_{d0}|;Events", 25, 0, 6.0 );
    NEWHIST( lep2D0Sig, "Second lepton |d0/#sigma_{d0}|;Events", 25, 0, 6.0 );
    NEWHIST( lep3D0Sig, "Third lepton |d0/#sigma_{d0}|;Events", 25, 0, 6.0 );
    NEWHIST( lepZ0SinTheta, "Lepton |z0sin#theta| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep1Z0SinTheta, "Leading lepton |z0sin#theta| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep2Z0SinTheta, "Second lepton |z0sin#theta| [mm];Events", 25, 0, 0.5 );
    NEWHIST( lep3Z0SinTheta, "Third lepton |z0sin#theta| [mm];Events", 25, 0, 0.5 );

    // lepton truth matching
    NEWHIST( lepOrigin, "Lepton truth origin;Leptons", PARTICLETYPES, 0, PARTICLETYPES );
    NEWHIST( lep1Origin, "Leading lepton truth origin;Events", PARTICLETYPES, 0, PARTICLETYPES );
    NEWHIST( lep2Origin, "Second lepton truth origin;Events", PARTICLETYPES, 0, PARTICLETYPES );
    NEWHIST( lep3Origin, "Third lepton truth origin;Events", PARTICLETYPES, 0, PARTICLETYPES );
    NEWHIST( lepClass, "Lepton truth classification;Leptons", 5, 0, 5 );
    NEWHIST( lep1Class, "Leading lepton truth classification;Events", 5, 0, 5 );
    NEWHIST( lep2Class, "Second lepton truth classification;Events", 5, 0, 5 );
    NEWHIST( lep3Class, "Third lepton truth classification;Events", 5, 0, 5 );
    NEWHIST( fakeEl2and3Class, "Second and third fake electron truth classification;Events", 5, 0, 5 );
    NEWHIST( fakeMu2and3Class, "Second and third fake muon truth classification;Events", 5, 0, 5 );
    SETBINLABEL( lepClass, 1, "PR" ); 
    SETBINLABEL( lep1Class, 1, "PR" ); 
    SETBINLABEL( lep2Class, 1, "PR" );
    SETBINLABEL( lep3Class, 1, "PR" );
    SETBINLABEL( fakeEl2and3Class, 1, "PR" );
    SETBINLABEL( fakeMu2and3Class, 1, "PR" );
    SETBINLABEL( lepClass, 2, "CO" ); 
    SETBINLABEL( lep1Class, 2, "CO" ); 
    SETBINLABEL( lep2Class, 2, "CO" );
    SETBINLABEL( lep3Class, 2, "CO" );
    SETBINLABEL( fakeEl2and3Class, 2, "CO" );
    SETBINLABEL( fakeMu2and3Class, 2, "CO" );
    SETBINLABEL( lepClass, 3, "HF" ); 
    SETBINLABEL( lep1Class, 3, "HF" ); 
    SETBINLABEL( lep2Class, 3, "HF" );
    SETBINLABEL( lep3Class, 3, "HF" );
    SETBINLABEL( fakeEl2and3Class, 3, "HF" );
    SETBINLABEL( fakeMu2and3Class, 3, "HF" );
    SETBINLABEL( lepClass, 4, "LF" ); 
    SETBINLABEL( lep1Class, 4, "LF" ); 
    SETBINLABEL( lep2Class, 4, "LF" );
    SETBINLABEL( lep3Class, 4, "LF" );
    SETBINLABEL( fakeEl2and3Class, 4, "LF" );
    SETBINLABEL( fakeMu2and3Class, 4, "LF" );
    SETBINLABEL( lepClass, 5, "UK" ); 
    SETBINLABEL( lep1Class, 5, "UK" ); 
    SETBINLABEL( lep2Class, 5, "UK" );
    SETBINLABEL( lep3Class, 5, "UK" );
    SETBINLABEL( fakeEl2and3Class, 5, "UK" );
    SETBINLABEL( fakeMu2and3Class, 5, "UK" );

    NEWHIST( hasSS, "has SS light leptons;Events", 2, 0, 2 );

    // lepton truth matching
    NEWHIST( baselepOrigin, "Base lepton truth origin;Leptons", PARTICLETYPES, 0, PARTICLETYPES );
    NEWHIST( baselep1Origin, "Leading base lepton truth origin;Events", PARTICLETYPES, 0, PARTICLETYPES );
    NEWHIST( baselep2Origin, "Second base lepton truth origin;Events", PARTICLETYPES, 0, PARTICLETYPES );
    NEWHIST( baselep3Origin, "Third base lepton truth origin;Events", PARTICLETYPES, 0, PARTICLETYPES );
    NEWHIST( baselepClass, "Base lepton truth classification;Leptons", 5, 0, 5 );
    NEWHIST( baselep1Class, "Leading base lepton truth classification;Events", 5, 0, 5 );
    NEWHIST( baselep2Class, "Second base lepton truth classification;Events", 5, 0, 5 );
    NEWHIST( baselep3Class, "Third base lepton truth classification;Events", 5, 0, 5 );
    SETBINLABEL( baselepClass, 1, "PR" ); 
    SETBINLABEL( baselep1Class, 1, "PR" ); 
    SETBINLABEL( baselep2Class, 1, "PR" );
    SETBINLABEL( baselep3Class, 1, "PR" );
    SETBINLABEL( baselepClass, 2, "CO" ); 
    SETBINLABEL( baselep1Class, 2, "CO" ); 
    SETBINLABEL( baselep2Class, 2, "CO" );
    SETBINLABEL( baselep3Class, 2, "CO" );
    SETBINLABEL( baselepClass, 3, "HF" ); 
    SETBINLABEL( baselep1Class, 3, "HF" ); 
    SETBINLABEL( baselep2Class, 3, "HF" );
    SETBINLABEL( baselep3Class, 3, "HF" );
    SETBINLABEL( baselepClass, 4, "LF" ); 
    SETBINLABEL( baselep1Class, 4, "LF" ); 
    SETBINLABEL( baselep2Class, 4, "LF" );
    SETBINLABEL( baselep3Class, 4, "LF" );
    SETBINLABEL( baselepClass, 5, "UK" ); 
    SETBINLABEL( baselep1Class, 5, "UK" ); 
    SETBINLABEL( baselep2Class, 5, "UK" );
    SETBINLABEL( baselep3Class, 5, "UK" );
    NEWHIST( fakebaseEl2and3Class, "Second and third fake base electron truth classification;Events", 5, 0, 5 );
    NEWHIST( fakebaseMu2and3Class, "Second and third fake base muon truth classification;Events", 5, 0, 5 );
    SETBINLABEL( fakebaseEl2and3Class, 1, "PR" );
    SETBINLABEL( fakebaseMu2and3Class, 1, "PR" );
    SETBINLABEL( fakebaseEl2and3Class, 2, "CO" );
    SETBINLABEL( fakebaseMu2and3Class, 2, "CO" );
    SETBINLABEL( fakebaseEl2and3Class, 3, "HF" );
    SETBINLABEL( fakebaseMu2and3Class, 3, "HF" );
    SETBINLABEL( fakebaseEl2and3Class, 4, "LF" );
    SETBINLABEL( fakebaseMu2and3Class, 4, "LF" );
    SETBINLABEL( fakebaseEl2and3Class, 5, "UK" );
    SETBINLABEL( fakebaseMu2and3Class, 5, "UK" );

    // tau kinematics
    NEWHIST( nTau, "Number of taus;Events", 6, -0.5, 5.5 );
    NEWVARHIST( tauPt, "Tau P_{T} [GeV];Taus", nLep1PtBins, lep1PtBins );
    NEWVARHIST( tau1Pt, "Leading tau_{} P_{T} [GeV];Events", nLep1PtBins, lep1PtBins );
    NEWVARHIST( tau2Pt, "Second tau_{} P_{T} [GeV];Events", nLep2PtBins, lep2PtBins );
    ETAHIST( tau1Eta, "Leading tau #eta;Events" );
    ETAHIST( tau2Eta, "Second tau #eta;Events" );
    DPHIHIST( dPhiTauTau, "dPhi(tau1, tau2);Events" );
    NEWHIST( tauProng, "Tau prong;Taus", 2, 0, 2 );
    SETBINLABEL( tauProng, 1, "1" );
    SETBINLABEL( tauProng, 2, "3" );
    NEWHIST( tauClass, "Tau truth classification;Taus", 5, 0, 5 );
    NEWHIST( tau1Class, "Leading tau truth classification;Taus", 5, 0, 5 );
    NEWHIST( tau2Class, "Second tau truth classification;Taus", 5, 0, 5 );
    SETBINLABEL( tauClass, 1, "PR" ); 
    SETBINLABEL( tau1Class, 1, "PR" ); 
    SETBINLABEL( tau2Class, 1, "PR" );
    SETBINLABEL( tauClass, 2, "CO" ); 
    SETBINLABEL( tau1Class, 2, "CO" ); 
    SETBINLABEL( tau2Class, 2, "CO" );
    SETBINLABEL( tauClass, 3, "HF" ); 
    SETBINLABEL( tau1Class, 3, "HF" ); 
    SETBINLABEL( tau2Class, 3, "HF" );
    SETBINLABEL( tauClass, 4, "LF" ); 
    SETBINLABEL( tau1Class, 4, "LF" ); 
    SETBINLABEL( tau2Class, 4, "LF" );
    SETBINLABEL( tauClass, 5, "UK" ); 
    SETBINLABEL( tau1Class, 5, "UK" ); 
    SETBINLABEL( tau2Class, 5, "UK" );

    // dR, dPhi plots
    DRHIST( minDRLepLep, "min dR(lep, lep);Events" );
    DPHIHIST( dPhiLep1Met, "dPhi(lep1, MET);Events" );
    DPHIHIST( dPhiLep2Met, "dPhi(lep2, MET);Events" );
    DPHIHIST( dPhiLep3Met, "dPhi(lep3, MET);Events" );
    DPHIHIST( dPhiLLMet, "dPhi(lep1+lep2, MET);Events" );
    DPHIHIST( dPhiLLJet, "dPhi(lep1+lep2, jet1);Events" );
    DPHIHIST( dPhiJet1Met, "dPhi(jet1, MET);Events" );
    DPHIHIST( dPhiJet2Met, "dPhi(jet2, MET);Events" );
    DPHIHIST( dPhiJJMet, "dPhi(jet1+jet2, MET);Events" );
    DPHIHIST( minDPhiLepLep, "min dPhi(lep, lep);Events" );
    DPHIHIST( minDPhiLepMet, "min dPhi(lep, MET);Events" );
    DPHIHIST( minDPhiJetJet, "min dPhi(jet, jet);Events" );
    DPHIHIST( minDPhiJetMet, "min dPhi(jet, MET);Events" );
    DPHIHIST( minDPhiTauMet, "min dPhi(tau, MET);Events" );
    DRHIST( minDRLepJet, "min dR(lep, Jet);Events" );
    DPHIHIST( maxDPhiLepLep, "max dPhi(lep, lep);Events" );
    DPHIHIST( maxDPhiLepMet, "max dPhi(lep, MET);Events" );
    DPHIHIST( maxDPhiJetJet, "max dPhi(jet, jet);Events" );
    DPHIHIST( maxDPhiJetMet, "max dPhi(jet, MET);Events" );
    DRHIST( maxDRLepJet, "max dR(lep, Jet);Events" );
    NEWHIST( dEtaLL, "#Delta#eta(lep, lep);Events", 30, 0, 3 );
    NEWHIST( dEtaJetJet, "#Delta#eta(jet, jet);Events", 20, 0, 8 );
    NEWHIST( maxDEtaJetJet, "max #Delta#eta(jet, jet);Events", 20, 0, 8 );
    NEWHIST( minDEtaJetJet, "min #Delta#eta(jet, jet);Events", 20, 0, 8 );

    // mass plots
    NEWHIST( mll, "M_{ll} [GeV];Events", 50, 50, 150 );
    //    ZMASSHIST( msfos, "M_{SFOS} [GeV];Events" );
    NEWHIST( msfos, "M_{SFOS} [GeV];Events", 50, 0., 150 );
    NEWHIST( minMsfos, "min M_{SFOS} [GeV];Events", 50, 0., 150 );
    NEWHIST( msfss, "M_{SFSS} [GeV];SFSS lepton pairs", 50, 0, 500 );
    NEWVARHIST( mlll, "M_{lll} [GeV];Events", nMassBins, massBins );
    //NEWHIST( mlll, "M_{lll} [GeV];Events", 50, 0, 1000 );
    NEWVARHIST( mt, "M_{T} [GeV];Events", nMassBins, massBins );
    NEWVARHIST( mtL1, "M_{T}(lep1, MET) [GeV];Events", nMassBins, massBins );
    NEWVARHIST( mtL2, "M_{T}(lep2, MET) [GeV];Events", nMassBins, massBins );
    NEWVARHIST( mt2, "M_{T2} [GeV];Events", nMassBins, massBins );
    NEWVARHIST( meff, "M_{eff} [GeV];Events", nMeffBins, meffBins );
    NEWVARHIST( meffNoMet, "M_{eff} (No MET) [GeV];Events", nMeffBins, meffBins );
    NEWVARHIST( mjj, "M_{jj} [GeV];Events", nMassBins, massBins );
    NEWHIST( mlljj, "M_{lljj} [GeV];Events", 40, 0, 1000 );
    NEWVARHIST( mlt, "M_{l#tau} [GeV];Lepton-tau pairs", nMassBins, massBins );
    NEWVARHIST( mtt, "M_{#tau#tau} [GeV];Events", nMassBins, massBins );
    NEWVARHIST( mbb, "M_{bb} [GeV];Events", nMassBins, massBins );
    NEWVARHIST( mljj, "M_{ljj} [GeV];Events", nMeffBins, meffBins );
    NEWVARHIST( mljjFine, "M_{ljj} [GeV];Events", nMljjFineBins, mljjFineBins );
    NEWVARHIST( mtll, "M_{T}^{WW} [GeV];Events", nMassBins, massBins );
    NEWVARHIST2D( mtll_mt, "M_{T}^{WW} [GeV];M_{T} [GeV];Events", nMassBins, massBins, nMassBins, massBins );

    // MET
    //NEWVARHIST( met, "MET [GeV];Events", nMetBins, metBins );
    NEWHIST( met, "MET [GeV];Events", 30, 0, 450 );
    NEWHIST( metEle, "MET RefElectron [GeV];Events", 20, 0, 200 );
    NEWHIST( metMuo, "MET Muon [GeV];Events", 20, 0, 200 );
    NEWHIST( metJet, "MET RefJet [GeV];Events", 20, 0, 200 );
    NEWHIST( metCell, "MET CellOut [GeV];Events", 20, 0, 200 );
    NEWHIST( metRel, "MET rel [GeV];Events", 30, 0, 450 );

    // Jets
    NEWHIST( nJet, "Number of jets;Events", 17, -0.5, 16.5 );
    NEWHIST( nBJet, "Number of b jets;Events", 17, -0.5, 16.5 );

    PTHIST( jet1Pt, "Leading jet_{} P_{T} [GeV];Events" );
    PTHIST( jet2Pt, "Second jet_{} P_{T} [GeV];Events" );
    PTHIST( jet3Pt, "Third jet_{} P_{T} [GeV];Events" );
    NEWVARHIST( jetPt, "Jet P_{T} [GeV];Jets", nJetPtBins, jetPtBins );
    NEWVARHIST( bJetPt, "B Jet P_{T} [GeV];Jets", nJetPtBins, jetPtBins );
    NEWVARHIST( bJet1Pt, "Leading B Jet P_{T} [GeV];Events", nJetPtBins, jetPtBins );
    NEWVARHIST( bJet2Pt, "Second B Jet P_{T} [GeV];Events", nJetPtBins, jetPtBins );
    NEWVARHIST( bJet3Pt, "Third B Jet P_{T} [GeV];Events", nJetPtBins, jetPtBins );
    ETAHIST( jetEta, "Jet #eta;Jets" );
    ETAHIST( bJetEta, "B Jet #eta;Jets" );
    ETAHIST( bJet1Eta, "Leading B Jet #eta;Events" );
    ETAHIST( bJet2Eta, "Second B Jet #eta;Events" );
    ETAHIST( bJet3Eta, "Third B Jet #eta;Events" );
    NEWHIST( jetPhi, "Jet phi;Jets", 10, -3.1416, 3.1416 );
    NEWHIST( jetMV1, "Jet MV1;Jets", 20, 0., 1. );
    NEWHIST( jetJVF, "Jet JVF;Jets", 20, -1., 1. );
    NEWHIST( bJetJVF, "B Jet JVF;B Jets", 20, -1., 1. );
    NEWHIST( dijetM, "Mjj [GeV];Events", 20, 0, 1200 );
    NEWHIST( productEta1Eta2, "Eta1*Eta2;Events", 30, -10, 5. );

    // Pileup hists
    NEWHIST( nVtx, "Number of good vertices;Events", 40, 0, 40 );
    NEWHIST( mu, "Avg interactions per bunch crossing;Events", 40, 0, 40 );

    // To look into pdgId of parents
    NEWHIST( elParentPdgId, "PDG ID of parents of electron;Events", 100, -50, 50);
    NEWHIST( muParentPdgId, "PDG ID of parents of muon;Events",     100, -50, 50);

  }

#undef PT1HIST
#undef ETAHIST
#undef PTHIST
#undef DRHIST
#undef DPHIHIST
#undef MASSHIST
#undef ZMASSHIST
#undef NEWHIST
#undef NEWVARHIST
#undef SETBINLABEL
  
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Plotter::FillHistoPreSelec(EventSelector *EveSelec, double weight)
{
  MyDebug("FillHistoPreSelec()","Plotter::FillHistoPreSelec()");

  Double_t w = weight*EveSelec->getTotalSF();

  h_nSigBaseLep->Fill(EveSelec->nBaselineLeps(),EveSelec->nSignalLeps(),w);

  return true;
}
/*--------------------------------------------------------------------------------*/
bool Plotter::FillHistograms(EventSelector *EveSelec, double weight)
{
  MyDebug("FillHistograms()","Plotter::FillHistograms()");

  //Retrieving objects via EventSelector
  std::vector< xAOD::Electron >* vec_signalElectron = EveSelec->GetSignalElectron();
  std::vector< xAOD::Electron >* vec_baseElectron   = EveSelec->GetBaseElectron  ();
  std::vector< xAOD::Muon >*     vec_signalMuon     = EveSelec->GetSignalMuon    ();
  std::vector< xAOD::Muon >*     vec_baseMuon       = EveSelec->GetBaseMuon      ();
  std::vector< xAOD::Jet >*      vec_signalJet      = EveSelec->GetSignalJet     ();
  // std::vector< xAOD::Jet >*      vec_baseJet        = EveSelec->GetBaseJet       ();
  // std::vector< xAOD::Jet >*      vec_preJet         = EveSelec->GetPreJet        ();
  TVector2                       met                = EveSelec->GetMEt           ();

  // std::cout<<"elSize="<<vec_signalElectron->size()<<", muSize="<<vec_signalMuon->size()<<std::endl;
  // for(UInt_t el=0; el<vec_signalElectron->size(); el++){
  //   std::cout<<"el="<<el<<", address ="<<&(vec_signalElectron->at(el))<<std::endl;
  //   std::cout<<"pt ="<<vec_signalElectron->at(el).pt()<<std::endl;
  //   std::cout<<"eta="<<vec_signalElectron->at(el).eta()<<std::endl;
  //   std::cout<<"phi="<<vec_signalElectron->at(el).phi()<<std::endl;
  //   const xAOD::TrackParticle* track = vec_signalElectron->at(el).trackParticle();
  //   double el_d0 = track->d0();
  //   std::cout<<"d0="<<el_d0<<std::endl;
  // }
  // for(UInt_t mu=0; mu<vec_signalMuon->size(); mu++){
  //   std::cout<<"mu="<<mu<<", address ="<<&(vec_signalMuon->at(mu))<<std::endl;
  //   std::cout<<"pt ="<<vec_signalMuon->at(mu).pt()<<std::endl;
  //   std::cout<<"eta="<<vec_signalMuon->at(mu).eta()<<std::endl;
  //   std::cout<<"phi="<<vec_signalMuon->at(mu).phi()<<std::endl;
  //   const xAOD::TrackParticle* track = vec_signalMuon->at(mu).primaryTrackParticle();
  //   double mu_d0 = track->d0();
  //   std::cout<<"d0="<<mu_d0<<std::endl;
  // }

  Int_t chan = EveSelec->getChan();
  MyDebug("FillHistograms()",Form("Analyzed channel : %d",chan));
  if(chan<0) return false;

  //Prepare 1st-3rd leading signal lepton's four-vector
  const Int_t nSigLeps = EveSelec->nSignalLeps();
  std::vector<Int_t> leadLepIndex(nSigLeps);
  std::vector<Int_t> leadLepFlavor(nSigLeps);
  std::vector<TLorentzVector> leadLep(nSigLeps);
  for(Int_t id=0; id<nSigLeps; id++){
    leadLepIndex [id] = EveSelec->getLeadLepIndex (id);
    leadLepFlavor[id] = EveSelec->getLeadLepFlavor(id);
    leadLep      [id] = EveSelec->getLeadLep      (id);
  }

  //Prepare 1st-3rd leading baseline lepton's four-vector
  Int_t baseLepIndex[3];
  Int_t baseLepFlavor[3];
  TLorentzVector baseLep[3];
  Int_t baseLepIsSig[3];
  for(Int_t id=0; id<3; id++){
    baseLepIndex [id] = EveSelec->getBaseLepIndex (id);
    baseLepFlavor[id] = EveSelec->getBaseLepFlavor(id);
    baseLep      [id] = EveSelec->getBaseLep      (id);
    baseLepIsSig [id] = EveSelec->getBaseLepIsSig (id);
  }

  //Prepare 1st-3rd lepton's used for the analysis (can treat events with less than three signal lepton region)
  Int_t lepIndex[3];
  Int_t lepFlavor[3];
  TLorentzVector lep[3];
  for(Int_t id=0; id<3; id++){
    lepIndex [id] = EveSelec->is3SigLepSel() ? EveSelec->getLeadLepIndex (id) : EveSelec->getBaseLepIndex (id);
    lepFlavor[id] = EveSelec->is3SigLepSel() ? EveSelec->getLeadLepFlavor(id) : EveSelec->getBaseLepFlavor(id);
    lep      [id] = EveSelec->is3SigLepSel() ? EveSelec->getLeadLep      (id) : EveSelec->getBaseLep      (id);
  }

  //Prepare 1st-3rd leading signal jet's four-vector
  //  Int_t leadJetIndex[3];
  TLorentzVector leadJet[3];
  for(Int_t id=0; id<3; id++){
    //    leadJetIndex[id] = EveSelec->getLeadJetIndex (id);
    leadJet     [id] = EveSelec->getLeadJet      (id);
  }

  //===========================================================================
  Double_t w = weight*EveSelec->getTotalSF();
  Double_t mmWeight = 0.;
  if(m_runMM){
    MM::LepProp lepProp[2];
    for(Int_t lep=0; lep<2; lep++){
      lepProp[lep].pt       = baseLep      [lep+1].Pt(); //in GeV
      lepProp[lep].eta      = baseLep      [lep+1].Eta();
      lepProp[lep].flav     = (MM::LepFlav)baseLepFlavor[lep+1];
      lepProp[lep].isSignal = baseLepIsSig [lep+1];
      m_MMTool->setLepProp(lep,lepProp[lep]);
    }
    mmWeight = m_MMTool->getMMWeight();
    w *= mmWeight;
  }
  MyDebug("FillHistograms()",Form("Event weight : %f",w));

  // Preprocessor convenience
  // All this does is append the corrent indices to the histo for sys and channel
  // It also fills the all-channel histo 
#define FillChanHist( h, val, w )               \
  do{                                           \
    h[Ch_all]->Fill(val,w);                     \
    h[chan]->Fill(val,w);                       \
  } while(0)
#define FillChanHist2D( h, xVal, yVal, w )      \
  do{                                           \
    h[Ch_all]->Fill(xVal, yVal, w);             \
    h[chan]->Fill(xVal, yVal, w);               \
  } while(0)
#define FillElHist( index, h, val, w )                  \
  do{                                                   \
    if(leadLepFlavor[index]==0) h[Ch_all]->Fill(val,w); \
    if(leadLepFlavor[index]==0) h[chan]->Fill(val,w);   \
  } while(0)
#define FillMuHist( index, h, val, w )                  \
  do{                                                   \
    if(leadLepFlavor[index]==1) h[Ch_all]->Fill(val,w); \
    if(leadLepFlavor[index]==1) h[chan]->Fill(val,w);   \
  } while(0)
#define FillBaseElHist( index, h, val, w )                  \
  do{                                                   \
    if(baseLepFlavor[index]==0) h[Ch_all]->Fill(val,w); \
    if(baseLepFlavor[index]==0) h[chan]->Fill(val,w);   \
  } while(0)
#define FillBaseMuHist( index, h, val, w )                  \
  do{                                                   \
    if(baseLepFlavor[index]==1) h[Ch_all]->Fill(val,w); \
    if(baseLepFlavor[index]==1) h[chan]->Fill(val,w);   \
  } while(0)
  //Fill lepChan histograms
  h_lepChan    [Ch_all]->Fill(Ch_all,w);
  h_lepChan    [Ch_all]->Fill(chan,w);
  h_baselepChan[Ch_all]->Fill(Ch_all,w);
  h_baselepChan[Ch_all]->Fill(chan,w);

  //MM Weight
  FillChanHist( h_mmWeight, mmWeight, 1.);

  //Fill lepton
  for(Int_t id=0; id<3; id++){
    if(lepIndex[id]==-1) continue;
    if(id==0){
      FillChanHist( h_lep1Pt , lep[id].Pt()/1000., w );
      FillChanHist( h_lep1Eta, lep[id].Eta(), w );
      if(lepFlavor[id]==0){
        if(EveSelec->is3SigLepSel()){
          FillElHist( id, h_el1Pt,  vec_signalElectron->at(leadLepIndex[id]).pt()/1000., w );
          FillElHist( id, h_el1Eta, vec_signalElectron->at(leadLepIndex[id]).eta(), w );
        }else{
          FillBaseElHist( id, h_el1Pt,  vec_baseElectron->at(baseLepIndex[id]).pt()/1000., w );
          FillBaseElHist( id, h_el1Eta, vec_baseElectron->at(baseLepIndex[id]).eta(), w );
        }
      }else if(lepFlavor[id]==1){
        if(EveSelec->is3SigLepSel()){
          FillMuHist( id, h_mu1Pt,  vec_signalMuon->at(leadLepIndex[id]).pt()/1000., w );
          FillMuHist( id, h_mu1Eta, vec_signalMuon->at(leadLepIndex[id]).eta(), w );
        }else{
          FillBaseMuHist( id, h_mu1Pt,  vec_baseMuon->at(baseLepIndex[id]).pt()/1000., w );
          FillBaseMuHist( id, h_mu1Eta, vec_baseMuon->at(baseLepIndex[id]).eta(), w );
        }
      }
    }else if(id==1){
      FillChanHist( h_lep2Pt , lep[id].Pt()/1000., w );
      FillChanHist( h_lep2Eta, lep[id].Eta(), w );
      if(lepFlavor[id]==0){
        if(EveSelec->is3SigLepSel()){
          FillElHist( id, h_el2Pt,  vec_signalElectron->at(leadLepIndex[id]).pt()/1000., w );
          FillElHist( id, h_el2Eta, vec_signalElectron->at(leadLepIndex[id]).eta(), w );
        }else{
          FillBaseElHist( id, h_el2Pt,  vec_baseElectron->at(baseLepIndex[id]).pt()/1000., w );
          FillBaseElHist( id, h_el2Eta, vec_baseElectron->at(baseLepIndex[id]).eta(), w );
        }
      }else if(lepFlavor[id]==1){
        if(EveSelec->is3SigLepSel()){
          FillMuHist( id, h_mu2Pt,  vec_signalMuon->at(leadLepIndex[id]).pt()/1000., w );
          FillMuHist( id, h_mu2Eta, vec_signalMuon->at(leadLepIndex[id]).eta(), w );
        }else{
          FillBaseMuHist( id, h_mu2Pt,  vec_baseMuon->at(baseLepIndex[id]).pt()/1000., w );
          FillBaseMuHist( id, h_mu2Eta, vec_baseMuon->at(baseLepIndex[id]).eta(), w );
        }
      }
    }else if(id==2){
      FillChanHist( h_lep3Pt , lep[id].Pt()/1000., w );
      FillChanHist( h_lep3Eta, lep[id].Eta(), w );
      if(lepFlavor[id]==0){
        if(EveSelec->is3SigLepSel()){
          FillElHist( id, h_el3Pt,  vec_signalElectron->at(leadLepIndex[id]).pt()/1000., w );
          FillElHist( id, h_el3Eta, vec_signalElectron->at(leadLepIndex[id]).eta(), w );
        }else{
          FillBaseElHist( id, h_el3Pt,  vec_baseElectron->at(baseLepIndex[id]).pt()/1000., w );
          FillBaseElHist( id, h_el3Eta, vec_baseElectron->at(baseLepIndex[id]).eta(), w );
        }
      }else if(lepFlavor[id]==1){
        if(EveSelec->is3SigLepSel()){
          FillMuHist( id, h_mu3Pt,  vec_signalMuon->at(leadLepIndex[id]).pt()/1000., w );
          FillMuHist( id, h_mu3Eta, vec_signalMuon->at(leadLepIndex[id]).eta(), w );
        }else{
          FillBaseMuHist( id, h_mu3Pt,  vec_baseMuon->at(baseLepIndex[id]).pt()/1000., w );
          FillBaseMuHist( id, h_mu3Eta, vec_baseMuon->at(baseLepIndex[id]).eta(), w );
        }
      }
    }

  }

  //Fill lepton system, sum Pt
  FillChanHist( h_llPt, (lep[0]+lep[1]).Pt()/1000., w );
  FillChanHist( h_sumLepPt, (lep[0].Pt()+lep[1].Pt()+lep[2].Pt())/1000., w );
  FillChanHist( h_sumLepPtMet, (lep[0].Pt()+lep[1].Pt()+lep[2].Pt()+met.Mod())/1000., w );

  // FillChanHist( h_dPhiWZ, , w);
  // FillChanHist( h_nMuComb, , w);

  //Fill isolation values
  MyDebug("FillHistograms()","Filling isolation values");
  for(Int_t id=0; id<3; id++){
    if(lepIndex[id]==-1) continue;
    if(lepFlavor[id]==0){
      float TrackIso = EveSelec->getIsolationValue(lepIndex[id], lepFlavor[id], xAOD::Iso::ptvarcone20 );
      float CaloIso  = EveSelec->getIsolationValue(lepIndex[id], lepFlavor[id], xAOD::Iso::topoetcone20);
      FillChanHist( h_elTrackIso, TrackIso/lep[id].Pt(), w);
      FillChanHist( h_elCaloIso , CaloIso /lep[id].Pt(), w);
      if(id==0){
        FillChanHist( h_el1TrackIso, TrackIso/lep[id].Pt(), w);
        FillChanHist( h_el1CaloIso , CaloIso /lep[id].Pt(), w);
      }else if(id==1){
        FillChanHist( h_el2TrackIso, TrackIso/lep[id].Pt(), w);
        FillChanHist( h_el2CaloIso , CaloIso /lep[id].Pt(), w);
      }else if(id==2){
        FillChanHist( h_el3TrackIso, TrackIso/lep[id].Pt(), w);
        FillChanHist( h_el3CaloIso , CaloIso /lep[id].Pt(), w);
      }
    }else{
      float TrackIso = EveSelec->getIsolationValue(lepIndex[id], lepFlavor[id], xAOD::Iso::ptvarcone30 );
      float CaloIso  = EveSelec->getIsolationValue(lepIndex[id], lepFlavor[id], xAOD::Iso::topoetcone20);
      FillChanHist( h_muTrackIso, TrackIso/lep[id].Pt(), w);
      FillChanHist( h_muCaloIso , CaloIso /lep[id].Pt(), w);
      if(id==0){
        FillChanHist( h_mu1TrackIso, TrackIso/lep[id].Pt(), w);
        FillChanHist( h_mu1CaloIso , CaloIso /lep[id].Pt(), w);
      }else if(id==1){
        FillChanHist( h_mu2TrackIso, TrackIso/lep[id].Pt(), w);
        FillChanHist( h_mu2CaloIso , CaloIso /lep[id].Pt(), w);
      }else if(id==2){
        FillChanHist( h_mu3TrackIso, TrackIso/lep[id].Pt(), w);
        FillChanHist( h_mu3CaloIso , CaloIso /lep[id].Pt(), w);
      }
    }
  }

  //Fill lepton track variables
  MyDebug("FillHistograms()","Filling lepton track variables");
  for(Int_t id=0; id<3; id++){
    if(lepIndex[id]==-1) continue;
    const xAOD::TrackParticle* track = EveSelec->getTrack(lepIndex[id], lepFlavor[id]);
    Double_t d0         = 0.;
    Double_t d0sig      = 0.;
    Double_t z0         = 0.;
    Double_t z0sinTheta = 0.;
    if(track){
      //track d0
      d0 = TMath::Abs(track->d0());
      FillChanHist( h_lepD0, d0, w);
      if     (lepIndex[id]==0) FillChanHist( h_lep1D0, d0, w);
      else if(lepIndex[id]==1) FillChanHist( h_lep2D0, d0, w);
      else if(lepIndex[id]==2) FillChanHist( h_lep3D0, d0, w);
      //track z0
      const xAOD::Vertex* pv = EveSelec->getSUSYTools()->GetPrimVtx();
      double primvertex_z = pv ? pv->z() : 0;
      z0 = TMath::Abs(track->z0() + track->vz() - primvertex_z);
      FillChanHist( h_lepZ0, z0, w);
      if     (lepIndex[id]==0) FillChanHist( h_lep1Z0, z0, w);
      else if(lepIndex[id]==1) FillChanHist( h_lep2Z0, z0, w);
      else if(lepIndex[id]==2) FillChanHist( h_lep3Z0, z0, w);
      //track d0/sigma(d0)
      if(Amg::error(track->definingParametersCovMatrix(),0)!=0){
        d0sig = fabs(track->d0())/Amg::error(track->definingParametersCovMatrix(),0);
        FillChanHist( h_lepD0Sig, d0sig, w);
        if     (lepIndex[id]==0) FillChanHist( h_lep1D0Sig, d0sig, w);
        else if(lepIndex[id]==1) FillChanHist( h_lep2D0Sig, d0sig, w);
        else if(lepIndex[id]==2) FillChanHist( h_lep3D0Sig, d0sig, w);
      }
      //track z0*sinTheta
      Double_t theta = track->theta();
      z0sinTheta = z0*TMath::Sin(theta);
      FillChanHist( h_lepZ0SinTheta, z0sinTheta, w);
      if     (lepIndex[id]==0) FillChanHist( h_lep1Z0SinTheta, z0sinTheta, w);
      else if(lepIndex[id]==1) FillChanHist( h_lep2Z0SinTheta, z0sinTheta, w);
      else if(lepIndex[id]==2) FillChanHist( h_lep3Z0SinTheta, z0sinTheta, w);
    }
  }

  //Fill lepton truth infomation
  if(m_isMC){
    MyDebug("FillHistograms()","Filling lepton truth information");
    for(Int_t id=0; id<3; id++){
      if(lepIndex[id]==-1) continue;
      Int_t type   = -1;
      Int_t origin = -1;
      Int_t pdgid  = 0;
      std::vector< xAOD::Electron >* vec_electron = EveSelec->is3SigLepSel() ? vec_signalElectron : vec_baseElectron;
      std::vector< xAOD::Muon >*     vec_muon     = EveSelec->is3SigLepSel() ? vec_signalMuon     : vec_baseMuon;
      if(lepFlavor[id]==0){
        type   = xAOD::TruthHelpers::getParticleTruthType  (vec_electron->at(lepIndex[id]));
        origin = xAOD::TruthHelpers::getParticleTruthOrigin(vec_electron->at(lepIndex[id]));
        const xAOD::TruthParticle* etrue = xAOD::TruthHelpers::getTruthParticle(vec_electron->at(lepIndex[id]));
        if(etrue) pdgid = etrue->pdgId();
      }else{
        const xAOD::TrackParticle* trackParticle = (&(vec_muon->at(lepIndex[id])))->primaryTrackParticle();
        if(trackParticle){
          static SG::AuxElement::Accessor<int> acc_truthType  ("truthType"  );
          static SG::AuxElement::Accessor<int> acc_truthOrigin("truthOrigin");
          static SG::AuxElement::Accessor<int> acc_truthPdgId ("PdgId"      );
          if(acc_truthType  .isAvailable(*trackParticle)) type   = acc_truthType  (*trackParticle);
          if(acc_truthOrigin.isAvailable(*trackParticle)) origin = acc_truthOrigin(*trackParticle);
          if(acc_truthPdgId .isAvailable(*trackParticle)) pdgid  = acc_truthPdgId (*trackParticle);
        }
      }
      //For origin
      FillChanHist( h_lepOrigin, type, w);
      if       (id==0) { FillChanHist( h_lep1Origin, type, w);
      } else if(id==1) { 
        FillChanHist( h_lep2Origin, type, w);
      } else if(id==2) {
        FillChanHist( h_lep3Origin, type, w);
      }
      // Classification of Primary/Conversion/HeavyFlavor/LightFlavor/Unknown
      // Primary
      if(isRealLepton(type, origin, pdgid)){
        FillChanHist( h_lepClass, 0., w);
        if       (id==0){ 
          FillChanHist( h_lep1Class, 0., w);
        } else if(id==1){ 
          FillChanHist( h_lep2Class, 0., w);
        } else if(id==2){ 
          FillChanHist( h_lep3Class, 0., w);
        }
      }
      // Conversion
      else if(origin==5 || origin==6 || origin==7){
        FillChanHist( h_lepClass, 1., w);
        if       (id==0){ 
          FillChanHist( h_lep1Class, 1., w);
        } else if(id==1){ 
          FillChanHist( h_lep2Class, 1., w);
        } else if(id==2){ 
          FillChanHist( h_lep3Class, 1., w);
        }
      }
      // Heavy Flavor
      else if(origin==25 || origin==26 || origin==27 || origin==28 ||
              origin==29 || origin==32 || origin==33 ){
        FillChanHist( h_lepClass, 2., w);
        if       (id==0){ 
          FillChanHist( h_lep1Class, 2., w);
        } else if(id==1){ 
          FillChanHist( h_lep2Class, 2., w);
        } else if(id==2){ 
          FillChanHist( h_lep3Class, 2., w);
        }
      }
      // LightFlavor
      else if( !(type==0 && origin==0)) {
        FillChanHist( h_lepClass, 3., w);
        if       (id==0){ 
          FillChanHist( h_lep1Class, 3., w);
        } else if(id==1){ 
          FillChanHist( h_lep2Class, 3., w);
        } else if(id==2){ 
          FillChanHist( h_lep3Class, 3., w);
        }
      // Unknown
      }else{
        FillChanHist( h_lepClass, 4., w);
        if       (id==0){ 
          FillChanHist( h_lep1Class, 4., w);
        } else if(id==1){ 
          FillChanHist( h_lep2Class, 4., w);
        } else if(id==2){ 
          FillChanHist( h_lep3Class, 4., w);
        }
      }
    } // for loop (lead lepton)

    // Fill base lepton truth information
    MyDebug("FillHistograms()","Filling baseline lepton truth information");
    for(Int_t id=0; id<3; id++){
      MyDebug("FillHistograms()",Form("For baselep[%d]...",id));
      if(baseLepIndex[id]==-1) continue;
      Int_t type   = -1;
      Int_t origin = -1;
      Int_t pdgid  = 0;
      Int_t pdgid_parent = 0;
      if(baseLepFlavor[id]==0){
        MyDebug("FillHistograms()","flav electron");
        type   = xAOD::TruthHelpers::getParticleTruthType  (vec_baseElectron->at(baseLepIndex[id]));
        origin = xAOD::TruthHelpers::getParticleTruthOrigin(vec_baseElectron->at(baseLepIndex[id]));
        const xAOD::TruthParticle* etrue = xAOD::TruthHelpers::getTruthParticle(vec_baseElectron->at(baseLepIndex[id]));
        if(etrue) pdgid = etrue->pdgId();
        if(etrue){
          const xAOD::TruthParticle* etrue_parent = etrue->parent();
          if (etrue_parent!=0){
            pdgid_parent = etrue_parent->pdgId();
          }
          FillChanHist( h_elParentPdgId, pdgid_parent, w);
        }
      }else{
        MyDebug("FillHistograms()","flav muon");
        const xAOD::TrackParticle* trackParticle = (&(vec_baseMuon->at(baseLepIndex[id])))->primaryTrackParticle();
        if(trackParticle){
          static SG::AuxElement::Accessor<int> acc_truthType  ("truthType"  );
          static SG::AuxElement::Accessor<int> acc_truthOrigin("truthOrigin");
          static SG::AuxElement::Accessor<int> acc_truthPdgId ("PdgId"      );
          // static SG::AuxElement::Accessor< ElementLink< xAOD::TruthParticleContainer > > acc_truthParticle( "truthParticleLink" );
          if(acc_truthType  .isAvailable(*trackParticle)) type   = acc_truthType  (*trackParticle);
          if(acc_truthOrigin.isAvailable(*trackParticle)) origin = acc_truthOrigin(*trackParticle);
          if(acc_truthPdgId .isAvailable(*trackParticle)) pdgid  = acc_truthPdgId (*trackParticle);
          // if(acc_truthParticle.isAvailable(*trackParticle)) {
          if(trackParticle->isAvailable< ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
            // try {
            //            const xAOD::TruthParticle* mtrue = *trackParticle->auxdata< ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
            ElementLink<xAOD::TruthParticleContainer> truthLink = trackParticle->auxdata< ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
            if(truthLink.isValid()){
              const xAOD::TruthParticle* mtrue_parent = (*truthLink)->parent();
              if(mtrue_parent!=0){
                pdgid_parent = mtrue_parent->pdgId();
              }
              FillChanHist( h_muParentPdgId, pdgid_parent, w);
            }
            // MyDebug("FillHistograms()","hogehoge1");
            // if(mtrue!=0){
            //   MyDebug("FillHistograms()","hogehoge2");
            //   const xAOD::TruthParticle* mtrue_parent = mtrue->parent();
            //   if(mtrue_parent!=0){
            //     MyDebug("FillHistograms()","hogehoge3");
            //     pdgid_parent = mtrue_parent->pdgId();
            //   }
            //   MyDebug("FillHistograms()","hogehoge4");
            //   FillChanHist( h_muParentPdgId, pdgid_parent, w);
            // }
            // FillChanHist( h_muParentPdgId, -1, w); // debug
            // // } catch (...) {}
          }
          FillChanHist( h_muParentPdgId, -2, w); // debug
        }
      }
      MyDebug("FillHistograms()","Filling baseline lepton origin");
      //For origin
      FillChanHist( h_baselepOrigin, type, w);
      if     (id==0) {
        FillChanHist( h_baselep1Origin, type, w);
      } else if(id==1) {
        FillChanHist( h_baselep2Origin, type, w);
        // FillBaseElHist( h_fakebaseEl2and3Origin, type, w);
        // FillBaseMuHist( h_fakebaseMu2and3Origin, type, w);
      } else if(id==2) {
        FillChanHist( h_baselep3Origin, type, w);
        // FillBaseElHist( h_fakebaseEl2and3Origin, type, w);
        // FillBaseMuHist( h_fakebaseMu2and3Origin, type, w);
      }
      MyDebug("FillHistograms()","Filling histogram from each origin (PR/CO/HF/LF/UK)");
      // Classification of Primary/Conversion/HeavyFlavor/LightFlavor/Unknown
      // Primary
      if(isRealLepton(type,origin,pdgid)){
        FillChanHist( h_baselepClass, 0., w);
        if       (id==0) {
          FillChanHist( h_baselep1Class, 0., w);
          FillChanHist( h_PRbaselep1Pt,  baseLep[0].Pt()/1000., w );
          FillChanHist( h_PRbaselep1Eta, baseLep[0].Eta()     , w );
        } else if(id==1) {
          FillChanHist( h_baselep2Class, 0., w);
          FillChanHist( h_PRbaselep2Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_PRbaselep2Eta, baseLep[1].Eta()     , w );
          FillChanHist( h_PRbaselep2and3Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_PRbaselep2and3Eta, baseLep[1].Eta(),w );
          FillBaseElHist( id, h_basefakelepClass,     0., w);
          FillBaseElHist( id, h_basefakelepClass,     2., w);
          FillBaseElHist( id, h_fakebaseEl2and3Class, 0., w);
          FillBaseMuHist( id, h_basefakelepClass,     0., w);
          FillBaseMuHist( id, h_basefakelepClass,     4., w);
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 0., w);
          FillBaseElHist( id, h_PRbaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_PRbaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_PRbaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_PRbaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_PRbaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_PRbaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_PRbaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_PRbaseMu2and3Eta, baseLep[id].Eta(),      w );
          // To estimate fake rate, looking into whether the base lepton passes tight criteria or not
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 0., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 2., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 0., w );
              FillBaseElHist( id, h_PREl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_PREl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_PREl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_PREl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 0., w);
              FillBaseMuHist( id, h_sgnlfakelepClass, 4., w);
              FillBaseMuHist( id, h_fakeMu2and3Class, 0., w);
              FillBaseMuHist( id, h_PRMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_PRMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_PRMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_PRMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        } else if(id==2) {
          FillChanHist( h_baselep3Class, 0., w);
          FillChanHist( h_PRbaselep3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_PRbaselep3Eta, baseLep[2].Eta()     , w );
          FillChanHist( h_PRbaselep2and3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_PRbaselep2and3Eta, baseLep[2].Eta()     , w );
          FillBaseElHist( id, h_basefakelepClass,     0., w);
          FillBaseElHist( id, h_basefakelepClass,     2., w);
          FillBaseElHist( id, h_fakebaseEl2and3Class, 0., w);
          FillBaseMuHist( id, h_basefakelepClass,     0., w);
          FillBaseMuHist( id, h_basefakelepClass,     4., w);
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 0., w);
          FillBaseElHist( id, h_PRbaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_PRbaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_PRbaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_PRbaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_PRbaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_PRbaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_PRbaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_PRbaseMu2and3Eta, baseLep[id].Eta(),      w );
          // To estimate fake rate, looking into whether the base lepton passes tight criteria or not
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 0., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 2., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 0., w );
              FillBaseElHist( id, h_PREl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_PREl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_PREl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_PREl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 0., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 4., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 0., w );
              FillBaseMuHist( id, h_PRMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_PRMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_PRMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_PRMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        }
      }
      // Conversion
      else if(origin==5 || origin==6 || origin==7){
        FillChanHist( h_baselepClass, 1., w );
        if       (id==0) {
          FillChanHist( h_baselep1Class, 1., w );
          FillChanHist( h_CObaselep1Pt,  baseLep[0].Pt()/1000., w );
          FillChanHist( h_CObaselep1Eta, baseLep[0].Eta()     , w );
        } else if(id==1) {
          FillChanHist( h_baselep2Class, 1., w );
          FillChanHist( h_CObaselep2Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_CObaselep2Eta, baseLep[1].Eta()     , w );
          FillChanHist( h_CObaselep2and3Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_CObaselep2and3Eta, baseLep[1].Eta()     , w );
          FillBaseElHist( id, h_basefakelepClass,     1., w );
          FillBaseElHist( id, h_basefakelepClass,     3., w );
          FillBaseElHist( id, h_fakebaseEl2and3Class, 1., w );
          FillBaseMuHist( id, h_basefakelepClass,     1., w );
          FillBaseMuHist( id, h_basefakelepClass,     5., w );
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 1., w );
          FillBaseElHist( id, h_CObaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_CObaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_CObaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_CObaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_CObaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_CObaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_CObaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_CObaseMu2and3Eta, baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Eta, baseLep[id].Eta(),      w );
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 3., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 1., w );
              FillBaseElHist( id, h_COEl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_COEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_COEl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_COEl2and3Eta, baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 5., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 1., w );
              FillBaseMuHist( id, h_COMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_COMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_COMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_COMu2and3Eta, baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        } else if(id==2) {
          FillChanHist( h_baselep3Class, 1., w );
          FillChanHist( h_CObaselep3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_CObaselep3Eta, baseLep[2].Eta()     , w );
          FillChanHist( h_CObaselep2and3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_CObaselep2and3Eta, baseLep[2].Eta()     , w );
          FillBaseElHist( id, h_basefakelepClass,     1., w );
          FillBaseElHist( id, h_basefakelepClass,     3., w );
          FillBaseElHist( id, h_fakebaseEl2and3Class, 1., w );
          FillBaseMuHist( id, h_basefakelepClass,     1., w );
          FillBaseMuHist( id, h_basefakelepClass,     5., w );
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 1., w );
          FillBaseElHist( id, h_CObaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_CObaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_CObaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_CObaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_CObaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_CObaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_CObaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_CObaseMu2and3Eta, baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Eta, baseLep[id].Eta(),      w );
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 3., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 1., w );
              FillBaseElHist( id, h_COEl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_COEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_COEl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_COEl2and3Eta, baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 5., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 1., w );
              FillBaseMuHist( id, h_COMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_COMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_COMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_COMu2and3Eta, baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        }
      }
      // Heavy Flavor
      else if(origin==25 || origin==26 || origin==27 || origin==28 ||
              origin==29 || origin==32 || origin==33 ){
        FillChanHist( h_baselepClass, 2., w);
        if     (id==0)   { 
          FillChanHist( h_baselep1Class, 2., w);
          FillChanHist( h_HFbaselep1Pt,  baseLep[0].Pt()/1000., w );
          FillChanHist( h_HFbaselep1Eta, baseLep[0].Eta()     , w );
        } else if(id==1) {
          FillChanHist( h_baselep2Class, 2., w);
          FillChanHist( h_HFbaselep2Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_HFbaselep2Eta, baseLep[1].Eta()     , w );
          FillChanHist( h_HFbaselep2and3Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_HFbaselep2and3Eta, baseLep[1].Eta()     , w );
          FillBaseElHist( id, h_basefakelepClass,     1., w);
          FillBaseElHist( id, h_basefakelepClass,     3., w);
          FillBaseElHist( id, h_fakebaseEl2and3Class, 2., w);
          FillBaseMuHist( id, h_basefakelepClass,     1., w);
          FillBaseMuHist( id, h_basefakelepClass,     5., w);
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 2., w);
          FillBaseElHist( id, h_HFbaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_HFbaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_HFbaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_HFbaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_HFbaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_HFbaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_HFbaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_HFbaseMu2and3Eta, baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Eta, baseLep[id].Eta(),      w );
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 3., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 2., w );
              FillBaseElHist( id, h_HFEl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_HFEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_HFEl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_HFEl2and3Eta, baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 5., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 2., w );
              FillBaseMuHist( id, h_HFMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_HFMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_HFMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_HFMu2and3Eta, baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        } else if(id==2) {
          FillChanHist( h_baselep3Class, 2., w);
          FillChanHist( h_HFbaselep3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_HFbaselep3Eta, baseLep[2].Eta()     , w );
          FillChanHist( h_HFbaselep2and3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_HFbaselep2and3Eta, baseLep[2].Eta()     , w );
          FillBaseElHist( id, h_basefakelepClass,     1., w);
          FillBaseElHist( id, h_basefakelepClass,     3., w);
          FillBaseElHist( id, h_fakebaseEl2and3Class, 2., w);
          FillBaseMuHist( id, h_basefakelepClass,     1., w);
          FillBaseMuHist( id, h_basefakelepClass,     5., w);
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 2., w);
          FillBaseElHist( id, h_HFbaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_HFbaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_HFbaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_HFbaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_HFbaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_HFbaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_HFbaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_HFbaseMu2and3Eta, baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Eta, baseLep[id].Eta(),      w );
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 3., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 2., w );
              FillBaseElHist( id, h_HFEl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_HFEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_HFEl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_HFEl2and3Eta, baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 5., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 2., w );
              FillBaseMuHist( id, h_HFMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_HFMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_HFMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_HFMu2and3Eta, baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        }
      }
      // Light Flavor
      else if( !(type==0 && origin==0)) {
        FillChanHist( h_baselepClass, 3., w);
        if     (id==0)   { 
          FillChanHist( h_baselep1Class, 3., w);
          FillChanHist( h_LFbaselep1Pt,  baseLep[0].Pt()/1000., w );
          FillChanHist( h_LFbaselep1Eta, baseLep[0].Eta()     , w );
        } else if(id==1) {
          FillChanHist( h_baselep2Class, 3., w);
          FillChanHist( h_LFbaselep2Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_LFbaselep2Eta, baseLep[1].Eta()     , w );
          FillChanHist( h_LFbaselep2and3Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_LFbaselep2and3Eta, baseLep[1].Eta()     , w );
          FillBaseElHist( id, h_basefakelepClass,     1., w );
          FillBaseElHist( id, h_basefakelepClass,     3., w );
          FillBaseElHist( id, h_fakebaseEl2and3Class, 3., w );
          FillBaseMuHist( id, h_basefakelepClass,     1., w );
          FillBaseMuHist( id, h_basefakelepClass,     5., w );
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 3., w );
          FillBaseElHist( id, h_LFbaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_LFbaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_LFbaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_LFbaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_LFbaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_LFbaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_LFbaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_LFbaseMu2and3Eta, baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Eta, baseLep[id].Eta(),      w );
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 3., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 3., w );
              FillBaseElHist( id, h_LFEl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_LFEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_LFEl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_LFEl2and3Eta, baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 5., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 3., w );
              FillBaseMuHist( id, h_LFMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_LFMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_LFMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_LFMu2and3Eta, baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        } else if(id==2) {
          FillChanHist( h_baselep3Class, 3., w);
          FillChanHist( h_LFbaselep3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_LFbaselep3Eta, baseLep[2].Eta()     , w );
          FillChanHist( h_LFbaselep2and3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_LFbaselep2and3Eta, baseLep[2].Eta()     , w );
          FillBaseElHist( id, h_basefakelepClass,     1., w );
          FillBaseElHist( id, h_basefakelepClass,     3., w );
          FillBaseElHist( id, h_fakebaseEl2and3Class, 3., w );
          FillBaseMuHist( id, h_basefakelepClass,     1., w );
          FillBaseMuHist( id, h_basefakelepClass,     5., w );
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 3., w );
          FillBaseElHist( id, h_LFbaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_LFbaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_LFbaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_LFbaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_LFbaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_LFbaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_LFbaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_LFbaseMu2and3Eta, baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Eta, baseLep[id].Eta(),      w );
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 3., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 3., w );
              FillBaseElHist( id, h_LFEl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_LFEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_LFEl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_LFEl2and3Eta, baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 5., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 3., w );
              FillBaseMuHist( id, h_LFMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_LFMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_LFMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_LFMu2and3Eta, baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        }
      // UnKnown
      }else{
        FillChanHist( h_baselepClass, 4., w);
        if     (id==0)   { 
          FillChanHist( h_baselep1Class, 4., w);
          FillChanHist( h_UKbaselep1Pt,  baseLep[0].Pt()/1000., w );
          FillChanHist( h_UKbaselep1Eta, baseLep[0].Eta()     , w );
        } else if(id==1) {
          FillChanHist( h_baselep2Class, 4., w);
          FillChanHist( h_UKbaselep2Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_UKbaselep2Eta, baseLep[1].Eta()     , w );
          FillChanHist( h_UKbaselep2and3Pt,  baseLep[1].Pt()/1000., w );
          FillChanHist( h_UKbaselep2and3Eta, baseLep[1].Eta(),w );
          FillBaseElHist( id, h_basefakelepClass,     1., w );
          FillBaseElHist( id, h_basefakelepClass,     3., w );
          FillBaseElHist( id, h_fakebaseEl2and3Class, 4., w );
          FillBaseMuHist( id, h_basefakelepClass,     1., w );
          FillBaseMuHist( id, h_basefakelepClass,     5., w );
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 4., w );
          FillBaseElHist( id, h_UKbaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_UKbaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_UKbaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_UKbaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_UKbaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_UKbaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_UKbaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_UKbaseMu2and3Eta, baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Eta, baseLep[id].Eta(),      w );
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 3., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 4., w );
              FillBaseElHist( id, h_UKEl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_UKEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_UKEl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_UKEl2and3Eta, baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 5., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 4., w );
              FillBaseMuHist( id, h_UKMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_UKMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_UKMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_UKMu2and3Eta, baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        } else if(id==2) {
          FillChanHist( h_baselep3Class, 4., w);
          FillChanHist( h_UKbaselep3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_UKbaselep3Eta, baseLep[2].Eta()     , w );
          FillChanHist( h_UKbaselep2and3Pt,  baseLep[2].Pt()/1000., w );
          FillChanHist( h_UKbaselep2and3Eta, baseLep[2].Eta(),w );
          FillBaseElHist( id, h_basefakelepClass,     1., w );
          FillBaseElHist( id, h_basefakelepClass,     3., w );
          FillBaseElHist( id, h_fakebaseEl2and3Class, 4., w );
          FillBaseMuHist( id, h_basefakelepClass,     1., w );
          FillBaseMuHist( id, h_basefakelepClass,     5., w );
          FillBaseMuHist( id, h_fakebaseMu2and3Class, 4., w );
          FillBaseElHist( id, h_UKbaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_UKbaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_UKbaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_UKbaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_UKbaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_UKbaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_UKbaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_UKbaseMu2and3Eta, baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseElHist( id, h_fakeBaseEl3Eta,     baseLep[id].Eta(),      w );
          FillBaseElHist( id, h_fakeBaseEl2and3Eta, baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu3Pt,      baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Pt,  baseLep[id].Pt()/1000., w );
          FillBaseMuHist( id, h_fakeBaseMu3Eta,     baseLep[id].Eta(),      w );
          FillBaseMuHist( id, h_fakeBaseMu2and3Eta, baseLep[id].Eta(),      w );
          if(baseLepFlavor[id]==0){
            if(EveSelec->IsMySignalElectron(vec_baseElectron->at(baseLepIndex[id]))){
              FillBaseElHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseElHist( id, h_sgnlfakelepClass, 3., w );
              FillBaseElHist( id, h_fakeEl2and3Class, 4., w );
              FillBaseElHist( id, h_UKEl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_UKEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_UKEl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_UKEl2and3Eta, baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseElHist( id, h_fakeEl3Eta,     baseLep[id].Eta(),      w );
              FillBaseElHist( id, h_fakeEl2and3Eta, baseLep[id].Eta(),      w );
            }
          } else {
            if(EveSelec->IsMySignalMuon(vec_baseMuon->at(baseLepIndex[id]))){
              FillBaseMuHist( id, h_sgnlfakelepClass, 1., w );
              FillBaseMuHist( id, h_sgnlfakelepClass, 5., w );
              FillBaseMuHist( id, h_fakeMu2and3Class, 4., w );
              FillBaseMuHist( id, h_UKMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_UKMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_UKMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_UKMu2and3Eta, baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu3Pt,      baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu2and3Pt,  baseLep[id].Pt()/1000., w );
              FillBaseMuHist( id, h_fakeMu3Eta,     baseLep[id].Eta(),      w );
              FillBaseMuHist( id, h_fakeMu2and3Eta, baseLep[id].Eta(),      w );
            }
          }
        }
      }
    }
  }

  MyDebug("FillHistograms()","Filling event topology variables");
  FillChanHist( h_hasSS, (EveSelec->hasSS()?1.:0.), w);
  // FillChanHist( h_nTau, , w);
  // FillChanHist( h_tauPt, , w);
  // FillChanHist( h_tau1Pt, , w);
  // FillChanHist( h_tau2Pt, , w);
  // FillChanHist( h_tau1Eta, , w);
  // FillChanHist( h_tau2Eta, , w);
  // FillChanHist( h_tauProng, , w);
  // FillChanHist( h_dPhiTauTau, , w);
  // FillChanHist( h_tauClass, , w);
  // FillChanHist( h_tau1Class, , w);
  // FillChanHist( h_tau2Class, , w);
  // FillChanHist( h_minDRLepLep, , w);
  FillChanHist( h_dPhiLep1Met, lep[0].Vect().XYvector().DeltaPhi(met), w);
  FillChanHist( h_dPhiLep2Met, lep[1].Vect().XYvector().DeltaPhi(met), w);
  FillChanHist( h_dPhiLep3Met, lep[2].Vect().XYvector().DeltaPhi(met), w);
  if(nSigLeps>1){
    FillChanHist( h_dPhiLLMet, (leadLep[0]+leadLep[1]).Vect().XYvector().DeltaPhi(met), w);
    FillChanHist( h_dPhiLLJet, (leadLep[0]+leadLep[1]).DeltaPhi(leadJet[0]), w);
  }
  FillChanHist( h_dPhiJet1Met, leadJet[0].Vect().XYvector().DeltaPhi(met), w);
  FillChanHist( h_dPhiJet2Met, leadJet[1].Vect().XYvector().DeltaPhi(met), w);
  FillChanHist( h_dPhiJJMet, (leadJet[0]+leadJet[1]).Vect().XYvector().DeltaPhi(met), w);
  // FillChanHist( h_minDPhiLepLep, , w);
  // FillChanHist( h_minDPhiLepMet, , w);
  // FillChanHist( h_minDPhiJetJet, , w);
  // FillChanHist( h_minDPhiJetMet, , w);
  // FillChanHist( h_minDPhiTauMet, , w);
  // FillChanHist( h_minDRLepJet, , w);
  // FillChanHist( h_maxDPhiLepLep, , w);
  // FillChanHist( h_maxDPhiLepMet, , w);
  // FillChanHist( h_maxDPhiJetJet, , w);
  // FillChanHist( h_maxDPhiJetMet, , w);
  // FillChanHist( h_maxDRLepJet, , w);
  // FillChanHist( h_dEtaLL, , w);
  // FillChanHist( h_dEtaJetJet, , w);
  // FillChanHist( h_maxDEtaJetJet, , w);
  // FillChanHist( h_minDEtaJetJet, , w);
  FillChanHist( h_met      , met.Mod()/1000., w);
  FillChanHist( h_meff     , EveSelec->getMeff(40.,true )/1000., w);
  FillChanHist( h_meffNoMet, EveSelec->getMeff(40.,false)/1000., w);
  // FillChanHist( h_metEle, , w);
  // FillChanHist( h_metMuo, , w);
  // FillChanHist( h_metJet, , w);
  // FillChanHist( h_metCell, , w);
  FillChanHist( h_metRel, EveSelec->getMetRel()/1000., w);
  FillChanHist( h_nJet , vec_signalJet->size(), w);
  FillChanHist( h_nBJet, EveSelec->numBJets(), w);
  for(uint ij=0; ij<vec_signalJet->size(); ++ij){
    FillChanHist( h_jetPt , vec_signalJet->at(ij).p4().Pt()/1000., w);
    FillChanHist( h_jetEta, vec_signalJet->at(ij).p4().Eta()     , w);
    FillChanHist( h_jetPhi, vec_signalJet->at(ij).p4().Phi()     , w);
    if((vec_signalJet->at(ij)).auxdata<char>("bjet")){
      FillChanHist( h_bJetPt , vec_signalJet->at(ij).p4().Pt ()/1000., w);
      FillChanHist( h_bJetEta, vec_signalJet->at(ij).p4().Eta()      , w);
    }
  }
  FillChanHist( h_jet1Pt, leadJet[0].Pt()/1000., w);
  FillChanHist( h_jet2Pt, leadJet[1].Pt()/1000., w);
  FillChanHist( h_jet3Pt, leadJet[2].Pt()/1000., w);
  // FillChanHist( h_bJet1Pt, , w);
  // FillChanHist( h_bJet2Pt, , w);
  // FillChanHist( h_bJet3Pt, , w);
  // FillChanHist( h_bJet1Eta, , w);
  // FillChanHist( h_bJet2Eta, , w);
  // FillChanHist( h_bJet3Eta, , w);
  // FillChanHist( h_jetMV1, , w);
  // FillChanHist( h_jetJVF, , w);
  // FillChanHist( h_bJetJVF, , w);
  FillChanHist( h_dijetM, (leadJet[0]+leadJet[1]).M()/1000., w);
  // FillChanHist( h_productEta1Eta2, , w);
  FillChanHist( h_mll, (lep[0]+lep[1]).M()/1000., w);
  Int_t sfosIndex[2]={-1,-1};
  Int_t sfosFlav=-1;
  Double_t msfos = EveSelec->findBestMSFOS(sfosIndex[0],sfosIndex[1],sfosFlav,MZ);
  FillChanHist( h_msfos, msfos/1000., w);
  Int_t minSfosIndex[2]={-1,-1};
  Int_t minSfosFlav=-1;
  Double_t minMsfos = EveSelec->findBestMSFOS(minSfosIndex[0],minSfosIndex[1],minSfosFlav,-1.);
  FillChanHist( h_minMsfos, minMsfos/1000., w);
  // FillChanHist( h_msfss, , w);
  // FillChanHist( h_mlll, , w);
  // FillChanHist( h_mt, , w);
  // FillChanHist( h_mtL1, , w);
  // FillChanHist( h_mtL2, , w);
  FillChanHist( h_mt2, EveSelec->getMaxMT2()/1000., w);
  // FillChanHist( h_mjj, , w);
  // FillChanHist( h_mlljj, , w);
  // FillChanHist( h_mlt, , w);
  // FillChanHist( h_mtt, , w);
  // FillChanHist( h_mbb, , w);
  FillChanHist( h_mljj, EveSelec->getMljj()/1000., w);
  // FillChanHist( h_mljjFine, , w);
  // FillChanHist( h_mtll, , w);
  // FillChanHist( h_mtll_mt, , w);
  // FillChanHist( h_nVtx, , w);
  // FillChanHist( h_mu, , w);

#undef FillChanHist
#undef FillChanHist2D
#undef FillElHist
#undef FillMuHist

  MyDebug("FillHistograms()","Filling finished");
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Plotter::FillNEvent(double weight)
{
  MyDebug("FillNEvent()","Plotter::FillNEvent()");
  h_nEve->Fill(0., weight);
  return true;
}
/*--------------------------------------------------------------------------------*/
bool Plotter::isRealLepton(Int_t type, Int_t origin, Int_t pdgid) {
  /// when reco ele from conversion of photon radiated from a truth muon, conversions are conversions always, never prompt electrons
  if(fabs(pdgid)==13 && type==4 && origin==5) return false;
  // avoid returning true if type is not electron or muon (ex: type 17 by chance close to prompt lep)
  return ((type==21||type==1||type==2||type==3||type==4||type==5||type==6||type==7||type==8) && (origin==12 or origin==13 or origin==22));
}
