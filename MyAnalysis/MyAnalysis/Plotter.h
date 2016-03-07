#ifndef SusyAna_Plotter_h
#define SusyAna_Plotter_h

// Plotter - filling histogram for each selection region and systematics

#include<string>
#include<TObject.h>
#include"AsgTools/MsgLevel.h"

#include"MyAnalysis/EventSelector.h"

namespace MM {
  class MMTool;
}
class TFile;
class TH1F;
class TH2F;

#define nChan 5

class Plotter : public TObject
{
 public:
  Plotter(const std::string& sel="none", const std::string& sys="none", const MSG::Level& dbg=MSG::ERROR);
  virtual ~Plotter(){};

  virtual void initialize(const char* path, int dsid, double XS=0., TFile* file=NULL);
  virtual void finalize();
  void SetRunMM(Bool_t runMM){m_runMM = runMM;};
  void SetEffFile(const char* effFile){m_effFile = effFile;};

  bool FillHistoPreSelec(EventSelector *EveSelec, double weight);
  bool FillHistograms(EventSelector *EveSelec, double weight);
  bool FillNEvent(double weight);

  bool isRealLepton(Int_t type, Int_t origin, Int_t pdgid);

  ClassDef(Plotter, 1);

 protected:
  MSG::Level m_dbg; // debug level
  std::string m_sel;
  std::string m_sys;
  Bool_t m_runMM;
  std::string m_effFile;
  MM::MMTool *m_MMTool;
  std::string m_filename;
  double m_crossSection;
  Bool_t m_isMC;
  TFile *m_rootfile;

  bool BookHistograms();

  // Declaration of histograms
  TH1F *h_xsec; //cross section for the processed sample (for data, 0. is filled.)
  TH1F *h_nEve; //#processed events to make histograms (not #events for whole dataset)
  TH2F *h_nSigBaseLep; // #signal vs #baseline leptons
  TH1F *h_lepChan[nChan]; //count entries for each final state
  TH1F *h_mmWeight[nChan];//MM weight
  //Lepton kinematics
  TH1F *h_lep1Pt [nChan]; //1st lepton Pt
  TH1F *h_lep2Pt [nChan]; //2nd lepton Pt
  TH1F *h_lep3Pt [nChan]; //3rd lepton Pt
  TH1F *h_el1Pt  [nChan]; //1st electron Pt
  TH1F *h_el2Pt  [nChan]; //2nd electron Pt
  TH1F *h_el3Pt  [nChan]; //3rd electron Pt
  TH1F *h_mu1Pt  [nChan]; //1st muon Pt
  TH1F *h_mu2Pt  [nChan]; //2nd muon Pt
  TH1F *h_mu3Pt  [nChan]; //3rd muon Pt
  TH1F *h_lep1Eta[nChan]; //1st lepton Eta
  TH1F *h_lep2Eta[nChan]; //2nd lepton Eta
  TH1F *h_lep3Eta[nChan]; //3rd lepton Eta
  TH1F *h_el1Eta [nChan]; //1st electron Eta
  TH1F *h_el2Eta [nChan]; //2nd electron Eta
  TH1F *h_el3Eta [nChan]; //3rd electron Eta
  TH1F *h_mu1Eta [nChan]; //1st muon Eta
  TH1F *h_mu2Eta [nChan]; //2nd muon Eta
  TH1F *h_mu3Eta [nChan]; //3rd muon Eta

  // Fake baseline lepton
  TH1F *h_PRbaselep1Pt [nChan]; // 1st PR base lepton Pt
  TH1F *h_PRbaselep2Pt [nChan]; // 2nd PR base lepton Pt
  TH1F *h_PRbaselep3Pt [nChan]; // 3nd PR base lepton Pt
  TH1F *h_CObaselep1Pt [nChan]; // 1st CO base lepton Pt
  TH1F *h_CObaselep2Pt [nChan]; // 2nd CO base lepton Pt
  TH1F *h_CObaselep3Pt [nChan]; // 3nd CO base lepton Pt
  TH1F *h_HFbaselep1Pt [nChan]; // 1st HF base lepton Pt
  TH1F *h_HFbaselep2Pt [nChan]; // 2nd HF base lepton Pt
  TH1F *h_HFbaselep3Pt [nChan]; // 3nd HF base lepton Pt
  TH1F *h_LFbaselep1Pt [nChan]; // 1st LF base lepton Pt
  TH1F *h_LFbaselep2Pt [nChan]; // 2nd LF base lepton Pt
  TH1F *h_LFbaselep3Pt [nChan]; // 3nd LF base lepton Pt
  TH1F *h_UKbaselep1Pt [nChan]; // 1st UK base lepton Pt
  TH1F *h_UKbaselep2Pt [nChan]; // 2nd UK base lepton Pt
  TH1F *h_UKbaselep3Pt [nChan]; // 3nd UK base lepton Pt
  TH1F *h_PRbaselep1Eta[nChan]; // 1st PR base lepton Eta
  TH1F *h_PRbaselep2Eta[nChan]; // 2nd PR base lepton Eta
  TH1F *h_PRbaselep3Eta[nChan]; // 3nd PR base lepton Eta
  TH1F *h_CObaselep1Eta[nChan]; // 1st CO base lepton Eta
  TH1F *h_CObaselep2Eta[nChan]; // 2nd CO base lepton Eta
  TH1F *h_CObaselep3Eta[nChan]; // 3nd CO base lepton Eta
  TH1F *h_HFbaselep1Eta[nChan]; // 1st HF base lepton Eta
  TH1F *h_HFbaselep2Eta[nChan]; // 2nd HF base lepton Eta
  TH1F *h_HFbaselep3Eta[nChan]; // 3nd HF base lepton Eta
  TH1F *h_LFbaselep1Eta[nChan]; // 1st LF base lepton Eta
  TH1F *h_LFbaselep2Eta[nChan]; // 2nd LF base lepton Eta
  TH1F *h_LFbaselep3Eta[nChan]; // 3nd LF base lepton Eta
  TH1F *h_UKbaselep1Eta[nChan]; // 1st UK base lepton Eta
  TH1F *h_UKbaselep2Eta[nChan]; // 2nd UK base lepton Eta
  TH1F *h_UKbaselep3Eta[nChan]; // 3nd UK base lepton Eta

  // Fake signal lepton for matrix method
  TH1F *h_PRbaselep2and3Pt [nChan]; // 2nd and 3rd PR base lepton Pt
  TH1F *h_CObaselep2and3Pt [nChan]; // 2nd and 3rd CO base lepton Pt
  TH1F *h_HFbaselep2and3Pt [nChan]; // 2nd and 3rd HF base lepton Pt
  TH1F *h_LFbaselep2and3Pt [nChan]; // 2nd and 3rd LF base lepton Pt
  TH1F *h_UKbaselep2and3Pt [nChan]; // 2nd and 3rd UK base lepton Pt
  TH1F *h_PRbaselep2and3Eta[nChan]; // 2nd and 3rd PR base lepton Eta
  TH1F *h_CObaselep2and3Eta[nChan]; // 2nd and 3rd CO base lepton Eta
  TH1F *h_HFbaselep2and3Eta[nChan]; // 2nd and 3rd HF base lepton Eta
  TH1F *h_LFbaselep2and3Eta[nChan]; // 2nd and 3rd LF base lepton Eta
  TH1F *h_UKbaselep2and3Eta[nChan]; // 2nd and 3rd UK base lepton Eta

  // Fake baseline lepton
  TH1F *h_PRbaseEl2Pt     [nChan];
  TH1F *h_PRbaseEl3Pt     [nChan];
  TH1F *h_PRbaseEl2and3Pt [nChan];
  TH1F *h_PRbaseEl2Eta    [nChan];
  TH1F *h_PRbaseEl3Eta    [nChan];
  TH1F *h_PRbaseEl2and3Eta[nChan];
  TH1F *h_CObaseEl2Pt     [nChan];
  TH1F *h_CObaseEl3Pt     [nChan];
  TH1F *h_CObaseEl2and3Pt [nChan];
  TH1F *h_CObaseEl2Eta    [nChan];
  TH1F *h_CObaseEl3Eta    [nChan];
  TH1F *h_CObaseEl2and3Eta[nChan];
  TH1F *h_HFbaseEl2Pt     [nChan];
  TH1F *h_HFbaseEl3Pt     [nChan];
  TH1F *h_HFbaseEl2and3Pt [nChan];
  TH1F *h_HFbaseEl2Eta    [nChan];
  TH1F *h_HFbaseEl3Eta    [nChan];
  TH1F *h_HFbaseEl2and3Eta[nChan];
  TH1F *h_LFbaseEl2Pt     [nChan];
  TH1F *h_LFbaseEl3Pt     [nChan];
  TH1F *h_LFbaseEl2and3Pt [nChan];
  TH1F *h_LFbaseEl2Eta    [nChan];
  TH1F *h_LFbaseEl3Eta    [nChan];
  TH1F *h_LFbaseEl2and3Eta[nChan];
  TH1F *h_UKbaseEl2Pt     [nChan];
  TH1F *h_UKbaseEl3Pt     [nChan];
  TH1F *h_UKbaseEl2and3Pt [nChan];
  TH1F *h_UKbaseEl2Eta    [nChan];
  TH1F *h_UKbaseEl3Eta    [nChan];
  TH1F *h_UKbaseEl2and3Eta[nChan];
  TH1F *h_PRbaseMu2Pt     [nChan];
  TH1F *h_PRbaseMu3Pt     [nChan];
  TH1F *h_PRbaseMu2and3Pt [nChan];
  TH1F *h_PRbaseMu2Eta    [nChan];
  TH1F *h_PRbaseMu3Eta    [nChan];
  TH1F *h_PRbaseMu2and3Eta[nChan];
  TH1F *h_CObaseMu2Pt     [nChan];
  TH1F *h_CObaseMu3Pt     [nChan];
  TH1F *h_CObaseMu2and3Pt [nChan];
  TH1F *h_CObaseMu2Eta    [nChan];
  TH1F *h_CObaseMu3Eta    [nChan];
  TH1F *h_CObaseMu2and3Eta[nChan];
  TH1F *h_HFbaseMu2Pt     [nChan];
  TH1F *h_HFbaseMu3Pt     [nChan];
  TH1F *h_HFbaseMu2and3Pt [nChan];
  TH1F *h_HFbaseMu2Eta    [nChan];
  TH1F *h_HFbaseMu3Eta    [nChan];
  TH1F *h_HFbaseMu2and3Eta[nChan];
  TH1F *h_LFbaseMu2Pt     [nChan];
  TH1F *h_LFbaseMu3Pt     [nChan];
  TH1F *h_LFbaseMu2and3Pt [nChan];
  TH1F *h_LFbaseMu2Eta    [nChan];
  TH1F *h_LFbaseMu3Eta    [nChan];
  TH1F *h_LFbaseMu2and3Eta[nChan];
  TH1F *h_UKbaseMu2Pt     [nChan];
  TH1F *h_UKbaseMu3Pt     [nChan];
  TH1F *h_UKbaseMu2and3Pt [nChan];
  TH1F *h_UKbaseMu2Eta    [nChan];
  TH1F *h_UKbaseMu3Eta    [nChan];
  TH1F *h_UKbaseMu2and3Eta[nChan];
  TH1F *h_fakeBaseEl2Pt     [nChan];
  TH1F *h_fakeBaseEl3Pt     [nChan];
  TH1F *h_fakeBaseEl2and3Pt [nChan];
  TH1F *h_fakeBaseEl2Eta    [nChan];
  TH1F *h_fakeBaseEl3Eta    [nChan];
  TH1F *h_fakeBaseEl2and3Eta[nChan];
  TH1F *h_fakeBaseMu2Pt     [nChan];
  TH1F *h_fakeBaseMu3Pt     [nChan];
  TH1F *h_fakeBaseMu2and3Pt [nChan];
  TH1F *h_fakeBaseMu2Eta    [nChan];
  TH1F *h_fakeBaseMu3Eta    [nChan];
  TH1F *h_fakeBaseMu2and3Eta[nChan];

  // Fake baseline lepton which can pass signal selection
  TH1F *h_PREl2Pt     [nChan];
  TH1F *h_PREl3Pt     [nChan];
  TH1F *h_PREl2and3Pt [nChan];
  TH1F *h_PREl2Eta    [nChan];
  TH1F *h_PREl3Eta    [nChan];
  TH1F *h_PREl2and3Eta[nChan];
  TH1F *h_COEl2Pt     [nChan];
  TH1F *h_COEl3Pt     [nChan];
  TH1F *h_COEl2and3Pt [nChan];
  TH1F *h_COEl2Eta    [nChan];
  TH1F *h_COEl3Eta    [nChan];
  TH1F *h_COEl2and3Eta[nChan];
  TH1F *h_HFEl2Pt     [nChan];
  TH1F *h_HFEl3Pt     [nChan];
  TH1F *h_HFEl2and3Pt [nChan];
  TH1F *h_HFEl2Eta    [nChan];
  TH1F *h_HFEl3Eta    [nChan];
  TH1F *h_HFEl2and3Eta[nChan];
  TH1F *h_LFEl2Pt     [nChan];
  TH1F *h_LFEl3Pt     [nChan];
  TH1F *h_LFEl2and3Pt [nChan];
  TH1F *h_LFEl2Eta    [nChan];
  TH1F *h_LFEl3Eta    [nChan];
  TH1F *h_LFEl2and3Eta[nChan];
  TH1F *h_UKEl2Pt     [nChan];
  TH1F *h_UKEl3Pt     [nChan];
  TH1F *h_UKEl2and3Pt [nChan];
  TH1F *h_UKEl2Eta    [nChan];
  TH1F *h_UKEl3Eta    [nChan];
  TH1F *h_UKEl2and3Eta[nChan];
  TH1F *h_fakeEl2Pt     [nChan];
  TH1F *h_fakeEl3Pt     [nChan];
  TH1F *h_fakeEl2and3Pt [nChan];
  TH1F *h_fakeEl2Eta    [nChan];
  TH1F *h_fakeEl3Eta    [nChan];
  TH1F *h_fakeEl2and3Eta[nChan];
  TH1F *h_PRMu2Pt     [nChan];
  TH1F *h_PRMu3Pt     [nChan];
  TH1F *h_PRMu2and3Pt [nChan];
  TH1F *h_PRMu2Eta    [nChan];
  TH1F *h_PRMu3Eta    [nChan];
  TH1F *h_PRMu2and3Eta[nChan];
  TH1F *h_COMu2Pt     [nChan];
  TH1F *h_COMu3Pt     [nChan];
  TH1F *h_COMu2and3Pt [nChan];
  TH1F *h_COMu2Eta    [nChan];
  TH1F *h_COMu3Eta    [nChan];
  TH1F *h_COMu2and3Eta[nChan];
  TH1F *h_HFMu2Pt     [nChan];
  TH1F *h_HFMu3Pt     [nChan];
  TH1F *h_HFMu2and3Pt [nChan];
  TH1F *h_HFMu2Eta    [nChan];
  TH1F *h_HFMu3Eta    [nChan];
  TH1F *h_HFMu2and3Eta[nChan];
  TH1F *h_LFMu2Pt     [nChan];
  TH1F *h_LFMu3Pt     [nChan];
  TH1F *h_LFMu2and3Pt [nChan];
  TH1F *h_LFMu2Eta    [nChan];
  TH1F *h_LFMu3Eta    [nChan];
  TH1F *h_LFMu2and3Eta[nChan];
  TH1F *h_UKMu2Pt     [nChan];
  TH1F *h_UKMu3Pt     [nChan];
  TH1F *h_UKMu2and3Pt [nChan];
  TH1F *h_UKMu2Eta    [nChan];
  TH1F *h_UKMu3Eta    [nChan];
  TH1F *h_UKMu2and3Eta[nChan];
  TH1F *h_fakeMu2Pt     [nChan];
  TH1F *h_fakeMu3Pt     [nChan];
  TH1F *h_fakeMu2and3Pt [nChan];
  TH1F *h_fakeMu2Eta    [nChan];
  TH1F *h_fakeMu3Eta    [nChan];
  TH1F *h_fakeMu2and3Eta[nChan];

  // Summary of real efficiency/fake rate
  TH1F *h_sgnlfakelepClass[nChan]; // signal lepton, 0: all, 1:electron, 2:muon
  TH1F *h_basefakelepClass[nChan]; //   base lepton, 0: all, 1:electron, 2:muon

  // Base lepton truth classification
  TH1F *h_baselepOrigin [nChan];// Lepton origin from McTruthClassifier
  TH1F *h_baselep1Origin[nChan];// Lepton origin from McTruthClassifier
  TH1F *h_baselep2Origin[nChan];// Lepton origin from McTruthClassifier
  TH1F *h_baselep3Origin[nChan];// Lepton origin from McTruthClassifier
  TH1F *h_baselepClass [nChan]; // truth classifcation from LeptonTruthTools
  TH1F *h_baselep1Class[nChan]; // truth classifcation from LeptonTruthTools
  TH1F *h_baselep2Class[nChan]; // truth classifcation from LeptonTruthTools
  TH1F *h_baselep3Class[nChan]; // truth classifcation from LeptonTruthTools
  TH1F *h_fakebaseEl2and3Class[nChan];  // fake electron truth classification from LeptonTruthTools
  TH1F *h_fakebaseMu2and3Class[nChan];  // fake muon truth classification from LeptonTruthTools

  // Lepton truth classification
  TH1F *h_lepOrigin [nChan];// Lepton origin from McTruthClassifier
  TH1F *h_lep1Origin[nChan];// Lepton origin from McTruthClassifier
  TH1F *h_lep2Origin[nChan];// Lepton origin from McTruthClassifier
  TH1F *h_lep3Origin[nChan];// Lepton origin from McTruthClassifier
  TH1F *h_lepClass [nChan]; // truth classifcation from LeptonTruthTools
  TH1F *h_lep1Class[nChan]; // truth classifcation from LeptonTruthTools
  TH1F *h_lep2Class[nChan]; // truth classifcation from LeptonTruthTools
  TH1F *h_lep3Class[nChan]; // truth classifcation from LeptonTruthTools
  TH1F *h_fakeEl2and3Class[nChan];  // fake electron truth classification
  TH1F *h_fakeMu2and3Class[nChan];  // fake muon truth classification


  // base lepton kinematics
  TH1F *h_baselepChan[nChan];

  TH1F *h_llPt[nChan]; //Dilepton system Pt

  TH1F *h_sumLepPt   [nChan]; // Scalar sum of lepton pt
  TH1F *h_sumLepPtMet[nChan]; // Scalar sum of lepton pt and MET
  TH1F *h_dPhiWZ     [nChan]; // dPhi between W and Z leptons
  TH1F *h_nMuComb    [nChan]; // number of combined muons

  // Isolation
  TH1F *h_elTrackIso[nChan]; // electron track iso.
  TH1F *h_elCaloIso [nChan]; // electron calo iso.
  TH1F *h_muTrackIso[nChan]; // muon track iso.
  TH1F *h_muCaloIso [nChan]; // muon calo iso.
  TH1F *h_el1TrackIso[nChan]; // electron track iso.
  TH1F *h_el1CaloIso [nChan]; // electron calo iso.
  TH1F *h_mu1TrackIso[nChan]; // muon track iso.
  TH1F *h_mu1CaloIso [nChan]; // muon calo iso.
  TH1F *h_el2TrackIso[nChan]; // electron track iso.
  TH1F *h_el2CaloIso [nChan]; // electron calo iso.
  TH1F *h_mu2TrackIso[nChan]; // muon track iso.
  TH1F *h_mu2CaloIso [nChan]; // muon calo iso.
  TH1F *h_el3TrackIso[nChan]; // electron track iso.
  TH1F *h_el3CaloIso [nChan]; // electron calo iso.
  TH1F *h_mu3TrackIso[nChan]; // muon track iso.
  TH1F *h_mu3CaloIso [nChan]; // muon calo iso.

  // impact parameter histograms
  TH1F *h_lepD0 [nChan]; // all lepton d0
  TH1F *h_lep1D0[nChan]; // lepton d0
  TH1F *h_lep2D0[nChan]; // lepton d0
  TH1F *h_lep3D0[nChan]; // lepton d0
  TH1F *h_lepZ0 [nChan]; // all lepton z0
  TH1F *h_lep1Z0[nChan]; // lepton z0
  TH1F *h_lep2Z0[nChan]; // lepton z0
  TH1F *h_lep3Z0[nChan]; // lepton z0
  TH1F *h_lepD0Sig [nChan]; // all lepton d0 significance
  TH1F *h_lep1D0Sig[nChan]; // lepton d0 significance
  TH1F *h_lep2D0Sig[nChan]; // lepton d0 significance
  TH1F *h_lep3D0Sig[nChan]; // lepton d0 significance
  TH1F *h_lepZ0SinTheta [nChan]; // all lepton z0 sin theta
  TH1F *h_lep1Z0SinTheta[nChan]; // lepton z0 sin theta
  TH1F *h_lep2Z0SinTheta[nChan]; // lepton z0 sin theta
  TH1F *h_lep3Z0SinTheta[nChan]; // lepton z0 sin theta

  TH1F *h_hasSS[nChan];     // event has SS leptons


  // Tau kinematics
  TH1F *h_nTau      [nChan]; // number of signal taus
  TH1F *h_tauPt     [nChan]; // all tau pt
  TH1F *h_tau1Pt    [nChan]; // leading tau pt
  TH1F *h_tau2Pt    [nChan]; // second tau pt
  TH1F *h_tau1Eta   [nChan]; // leading tau eta
  TH1F *h_tau2Eta   [nChan]; // second tau eta
  TH1F *h_tauProng  [nChan]; // number of tracks
  TH1F *h_dPhiTauTau[nChan]; // dPhi(tau1,tau2)
  TH1F *h_tauClass  [nChan]; // truth classification from LeptonTruthTools
  TH1F *h_tau1Class [nChan]; // truth classification from LeptonTruthTools
  TH1F *h_tau2Class [nChan]; // truth classification from LeptonTruthTools

  // Angular histos
  TH1F *h_minDRLepLep  [nChan]; // min dR(lep, lep)
  TH1F *h_dPhiLep1Met  [nChan]; // dPhi(lep1, met)
  TH1F *h_dPhiLep2Met  [nChan]; // dPhi(lep2, met)
  TH1F *h_dPhiLep3Met  [nChan]; // dPhi(lep3, met)
  TH1F *h_dPhiLLMet    [nChan]; // dPhi(lep1+lep2, met)
  TH1F *h_dPhiLLJet    [nChan]; // dPhi(lep1+lep2, leading jet)
  TH1F *h_dPhiJet1Met  [nChan]; // dPhi(jet1, met)
  TH1F *h_dPhiJet2Met  [nChan]; // dPhi(jet2, met)
  TH1F *h_dPhiJJMet    [nChan]; // dPhi(jet1+jet2, met)
  TH1F *h_minDPhiLepLep[nChan]; // min dPhi(lep, lep) 
  TH1F *h_minDPhiLepMet[nChan]; // min dPhi(lep, met) 
  TH1F *h_minDPhiJetJet[nChan]; // min dPhi(jet, jet)
  TH1F *h_minDPhiJetMet[nChan]; // min dPhi(jet, met)
  TH1F *h_minDPhiTauMet[nChan]; // min dPhi(tau, met)
  TH1F *h_minDRLepJet  [nChan]; // min dR(lep, jet)
  TH1F *h_maxDPhiLepLep[nChan]; // max dPhi(lep, lep) 
  TH1F *h_maxDPhiLepMet[nChan]; // max dPhi(lep, met) 
  TH1F *h_maxDPhiJetJet[nChan]; // max dPhi(jet, jet)
  TH1F *h_maxDPhiJetMet[nChan]; // max dPhi(jet, met)
  TH1F *h_maxDRLepJet  [nChan]; // max dR(lep, jet)
  TH1F *h_dEtaLL       [nChan]; // dEta(lep, lep)
  TH1F *h_dEtaJetJet   [nChan]; // dEta(jet, jet)
  TH1F *h_maxDEtaJetJet[nChan]; // max dEta(jet, jet)
  TH1F *h_minDEtaJetJet[nChan]; // max dEta(jet, jet)

  // Missing ET hists
  TH1F *h_met      [nChan]; // Missing Et
  TH1F *h_meff     [nChan]; // M effective
  TH1F *h_meffNoMet[nChan]; // M effective without met
  TH1F *h_metEle   [nChan]; // Ref electron term
  TH1F *h_metMuo   [nChan]; // Muon total term
  TH1F *h_metJet   [nChan]; // Ref jet term
  TH1F *h_metCell  [nChan]; // Cell out term
  TH1F *h_metRel   [nChan]; // MET rel

  // Jet hists
  TH1F *h_nJet    [nChan]; // Jet multiplicity
  TH1F *h_nBJet   [nChan]; // b-jet multiplicity
  TH1F *h_jetPt   [nChan]; // all jet pt
  TH1F *h_jet1Pt  [nChan]; // leading jet pt
  TH1F *h_jet2Pt  [nChan]; // second jet pt
  TH1F *h_jet3Pt  [nChan]; // third jet pt
  TH1F *h_bJetPt  [nChan]; // all b jet pt
  TH1F *h_bJet1Pt [nChan]; // b jet pt
  TH1F *h_bJet2Pt [nChan]; // b jet pt
  TH1F *h_bJet3Pt [nChan]; // b jet pt
  TH1F *h_jetEta  [nChan]; // jet eta
  TH1F *h_bJetEta [nChan]; // all b jet eta
  TH1F *h_bJet1Eta[nChan]; // b jet eta
  TH1F *h_bJet2Eta[nChan]; // b jet eta
  TH1F *h_bJet3Eta[nChan]; // b jet eta
  TH1F *h_jetPhi  [nChan]; // jet phi
  TH1F *h_jetMV1  [nChan]; // jet mv1 weight
  TH1F *h_jetJVF  [nChan]; // jet JVF
  TH1F *h_bJetJVF [nChan]; // b jet JVF
  TH1F *h_dijetM  [nChan]; // Mass(jet1+jet2)
  TH1F *h_productEta1Eta2[nChan]; // Eta1*Eta2

  // Mass hists
  TH1F *h_mll       [nChan]; // leading dilepton mass, no charge/flav requirement
  TH1F *h_msfos     [nChan]; // sfos inv mass closest to Z
  TH1F *h_minMsfos  [nChan]; // minimum sfos inv mass
  TH1F *h_msfss     [nChan]; // sfss inv mass, all pairs
  TH1F *h_mlll      [nChan]; // trilepton mass
  TH1F *h_mt        [nChan]; // transverse mass from best non-Z lepton and MET
  TH1F *h_mtL1      [nChan]; // transverse using leading lepton
  TH1F *h_mtL2      [nChan]; // transverse using sub-leading lepton
  TH1F *h_mt2       [nChan]; // maximum MT2 of all object combinations
  TH1F *h_mjj       [nChan]; // dijet mass closest to the W
  TH1F *h_mlljj     [nChan]; // inv mass of sfos leptons and 2 jets closest to ZZ
  TH1F *h_mlt       [nChan]; // inv mass of OS lepton+tau
  TH1F *h_mtt       [nChan]; // inv mass of OS tau+tau
  TH1F *h_mbb       [nChan]; // inv mass of leading b-jets

  TH1F *h_mljj    [nChan]; // inv mass of ljj
  TH1F *h_mljjFine[nChan]; // inv mass of ljj, fine binning
  TH1F *h_mtll    [nChan]; // transverse mass of ll and MET
  TH2F *h_mtll_mt [nChan]; // mtll vs. max mt

  // Pileup  hists
  TH1F *h_nVtx[nChan]; // number of vtx with at least 5 tracks
  TH1F *h_mu  [nChan]; // average interactions per bunch crossing

  // PDGID of parents
  TH1F *h_elParentPdgId[nChan];
  TH1F *h_muParentPdgId[nChan];

};
#endif
