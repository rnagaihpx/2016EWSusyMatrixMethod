//////////////////////////////////////////////////////
// Scripts to make various plots.
// Usage : 
// root -q -b 'mkPlots.C+O("h0001","none")'
// root -q -b 'mkPlots.C+O("h0002","3lep")'
// etc...
//////////////////////////////////////////////////////
#define IntLumi 10000. //Integrated luminosity to normalize MC in pico-barn

#include<iostream>
#include<vector>
#include<string>

#include"TH2F.h"
#include"TFile.h"
#include"TString.h"
#include"THStack.h"
#include"TLegend.h"
//#include"Math/ProbFuncMathCore.h"
#include"RooStats/NumberCountingUtils.h"

#include"TUtil.cc"
#include"AtlasUtils.cc"

std::vector<TString> *DistTypeNames;
void SetDistType(void){
  DistTypeNames = new std::vector<TString>();
  //  DistTypeNames->push_back("lepChan"); //cannot draw this with this macro.
  DistTypeNames->push_back("lep1Pt");
  DistTypeNames->push_back("lep2Pt");
  DistTypeNames->push_back("lep3Pt");
  DistTypeNames->push_back("el1Pt");
  DistTypeNames->push_back("el2Pt");
  DistTypeNames->push_back("el3Pt");
  DistTypeNames->push_back("mu1Pt");
  DistTypeNames->push_back("mu2Pt");
  DistTypeNames->push_back("mu3Pt");
  DistTypeNames->push_back("lep1Eta");
  DistTypeNames->push_back("lep2Eta");
  DistTypeNames->push_back("lep3Eta");
  // DistTypeNames->push_back("PRlep1Pt");
  // DistTypeNames->push_back("PRlep2Pt");
  // DistTypeNames->push_back("PRlep3Pt");
  // DistTypeNames->push_back("COlep1Pt");
  // DistTypeNames->push_back("COlep2Pt");
  // DistTypeNames->push_back("COlep3Pt");
  // DistTypeNames->push_back("HFlep1Pt");
  // DistTypeNames->push_back("HFlep2Pt");
  // DistTypeNames->push_back("HFlep3Pt");
  // DistTypeNames->push_back("LFlep1Pt");
  // DistTypeNames->push_back("LFlep2Pt");
  // DistTypeNames->push_back("LFlep3Pt");
  // DistTypeNames->push_back("UKlep1Pt");
  // DistTypeNames->push_back("UKlep2Pt");
  // DistTypeNames->push_back("UKlep3Pt");
  // DistTypeNames->push_back("baselep1Pt");
  // DistTypeNames->push_back("baselep2Pt");
  // DistTypeNames->push_back("baselep3Pt");
  // DistTypeNames->push_back("baseel1Pt");
  // DistTypeNames->push_back("baseel2Pt");
  // DistTypeNames->push_back("baseel3Pt");
  // DistTypeNames->push_back("basemu1Pt");
  // DistTypeNames->push_back("basemu2Pt");
  // DistTypeNames->push_back("basemu3Pt");
  // DistTypeNames->push_back("baselep1Eta");
  // DistTypeNames->push_back("baselep2Eta");
  // DistTypeNames->push_back("baselep3Eta");
  DistTypeNames->push_back("el1Eta");
  DistTypeNames->push_back("el2Eta");
  DistTypeNames->push_back("el3Eta");
  DistTypeNames->push_back("mu1Eta");
  DistTypeNames->push_back("mu2Eta");
  DistTypeNames->push_back("mu3Eta");
  DistTypeNames->push_back("llPt");
  DistTypeNames->push_back("sumLepPt");
  DistTypeNames->push_back("sumLepPtMet");
  //  DistTypeNames->push_back("dPhiWZ");
  //  DistTypeNames->push_back("nMuComb");
  DistTypeNames->push_back("elTrackIso");
  DistTypeNames->push_back("elCaloIso");
  DistTypeNames->push_back("muTrackIso");
  DistTypeNames->push_back("muCaloIso");
  DistTypeNames->push_back("el1TrackIso");
  DistTypeNames->push_back("el1CaloIso");
  DistTypeNames->push_back("mu1TrackIso");
  DistTypeNames->push_back("mu1CaloIso");
  DistTypeNames->push_back("el2TrackIso");
  DistTypeNames->push_back("el2CaloIso");
  DistTypeNames->push_back("mu2TrackIso");
  DistTypeNames->push_back("mu2CaloIso");
  DistTypeNames->push_back("el3TrackIso");
  DistTypeNames->push_back("el3CaloIso");
  DistTypeNames->push_back("mu3TrackIso");
  DistTypeNames->push_back("mu3CaloIso");
  DistTypeNames->push_back("lepD0");
  DistTypeNames->push_back("lep1D0");
  DistTypeNames->push_back("lep2D0");
  DistTypeNames->push_back("lep3D0");
  DistTypeNames->push_back("lepZ0");
  DistTypeNames->push_back("lep1Z0");
  DistTypeNames->push_back("lep2Z0");
  DistTypeNames->push_back("lep3Z0");
  DistTypeNames->push_back("lepD0Sig");
  DistTypeNames->push_back("lep1D0Sig");
  DistTypeNames->push_back("lep2D0Sig");
  DistTypeNames->push_back("lep3D0Sig");
  DistTypeNames->push_back("lepZ0SinTheta");
  DistTypeNames->push_back("lep1Z0SinTheta");
  DistTypeNames->push_back("lep2Z0SinTheta");
  DistTypeNames->push_back("lep3Z0SinTheta");
  // DistTypeNames->push_back("lepOrigin");
  // DistTypeNames->push_back("lep1Origin");
  // DistTypeNames->push_back("lep2Origin");
  // DistTypeNames->push_back("lep3Origin");
  DistTypeNames->push_back("lepClass");
  DistTypeNames->push_back("lep1Class");
  DistTypeNames->push_back("lep2Class");
  DistTypeNames->push_back("lep3Class");
  // DistTypeNames->push_back("baselepOrigin");
  // DistTypeNames->push_back("baselep1Origin");
  // DistTypeNames->push_back("baselep2Origin");
  // DistTypeNames->push_back("baselep3Origin");
  // DistTypeNames->push_back("baselepClass");
  // DistTypeNames->push_back("baselep1Class");
  // DistTypeNames->push_back("baselep2Class");
  // DistTypeNames->push_back("baselep3Class");
  // DistTypeNames->push_back("hasSS");
  // DistTypeNames->push_back("nTau");
  // DistTypeNames->push_back("tauPt");
  // DistTypeNames->push_back("tau1Pt");
  // DistTypeNames->push_back("tau2Pt");
  // DistTypeNames->push_back("tau1Eta");
  // DistTypeNames->push_back("tau2Eta");
  // DistTypeNames->push_back("tauProng");
  // DistTypeNames->push_back("dPhiTauTau");
  // DistTypeNames->push_back("tauClass");
  // DistTypeNames->push_back("tau1Class");
  // DistTypeNames->push_back("tau2Class");
  // DistTypeNames->push_back("minDRLepLep");
  // DistTypeNames->push_back("dPhiLep1Met");
  // DistTypeNames->push_back("dPhiLep2Met");
  // DistTypeNames->push_back("dPhiLep3Met");
  // DistTypeNames->push_back("dPhiLLMet");
  // DistTypeNames->push_back("dPhiJet1Met");
  // DistTypeNames->push_back("dPhiJet2Met");
  // DistTypeNames->push_back("dPhiJJMet");
  // DistTypeNames->push_back("minDPhiLepLep");
  // DistTypeNames->push_back("minDPhiLepMet");
  // DistTypeNames->push_back("minDPhiJetJet");
  // DistTypeNames->push_back("minDPhiJetMet");
  // DistTypeNames->push_back("minDPhiTauMet");
  // DistTypeNames->push_back("minDRLepJet");
  // DistTypeNames->push_back("maxDPhiLepLep");
  // DistTypeNames->push_back("maxDPhiLepMet");
  // DistTypeNames->push_back("maxDPhiJetJet");
  // DistTypeNames->push_back("maxDPhiJetMet");
  // DistTypeNames->push_back("maxDRLepJet");
  // DistTypeNames->push_back("dEtaLL");
  // DistTypeNames->push_back("dEtaJetJet");
  // DistTypeNames->push_back("maxDEtaJetJet");
  // DistTypeNames->push_back("minDEtaJetJet");
  DistTypeNames->push_back("met");
  DistTypeNames->push_back("meff");
  DistTypeNames->push_back("meffNoMet");
  // DistTypeNames->push_back("metEle");
  // DistTypeNames->push_back("metMuo");
  // DistTypeNames->push_back("metJet");
  // DistTypeNames->push_back("metCell");
  DistTypeNames->push_back("metRel");
  DistTypeNames->push_back("nJet");
  DistTypeNames->push_back("nBJet");
  DistTypeNames->push_back("jetPt");
  // DistTypeNames->push_back("jet1Pt");
  // DistTypeNames->push_back("jet2Pt");
  // DistTypeNames->push_back("jet3Pt");
  DistTypeNames->push_back("bJetPt");
  // DistTypeNames->push_back("bJet1Pt");
  // DistTypeNames->push_back("bJet2Pt");
  // DistTypeNames->push_back("bJet3Pt");
  DistTypeNames->push_back("jetEta");
  DistTypeNames->push_back("bJetEta");
  // DistTypeNames->push_back("bJet1Eta");
  // DistTypeNames->push_back("bJet2Eta");
  // DistTypeNames->push_back("bJet3Eta");
  DistTypeNames->push_back("jetPhi");
  //  DistTypeNames->push_back("jetMV1");
  //  DistTypeNames->push_back("jetJVF");
  //  DistTypeNames->push_back("bJetJVF");
  //  DistTypeNames->push_back("dijetM");
  //  DistTypeNames->push_back("productEta1Eta2");
  DistTypeNames->push_back("mll");
  DistTypeNames->push_back("msfos");
  DistTypeNames->push_back("minMsfos");
  //  DistTypeNames->push_back("msfss");
  //  DistTypeNames->push_back("mlll");
  //  DistTypeNames->push_back("mt");
  //  DistTypeNames->push_back("mtL1");
  //  DistTypeNames->push_back("mtL2");
  DistTypeNames->push_back("mt2");
  // DistTypeNames->push_back("mjj");
  // DistTypeNames->push_back("mlljj");
  // DistTypeNames->push_back("mlt");
  // DistTypeNames->push_back("mtt");
  // DistTypeNames->push_back("mbb");
  DistTypeNames->push_back("mljj");
  //  DistTypeNames->push_back("mljjFine");
  // DistTypeNames->push_back("mtll");
  // DistTypeNames->push_back("nVtx");
  // DistTypeNames->push_back("mu");
}

enum FSType{allchan,eee,eem,emm,mmm,nFSType};
TString FSTypeNames[nFSType] = {"all","eee","eem","emm","mmm"};

/////////////////////////////////////////////////////////////////
enum SignalType{
  SmplViaWZ,
  nSignalType};
TString SignalTypeNames[nSignalType] = {"SmplViaWZ"};
enum SimplifiedModelType{
  SmplViaWZ_1 , SmplViaWZ_2 , SmplViaWZ_3 , SmplViaWZ_4 , SmplViaWZ_5 ,
  SmplViaWZ_6 , SmplViaWZ_7 , SmplViaWZ_8 , SmplViaWZ_9 , SmplViaWZ_10,
  SmplViaWZ_11, SmplViaWZ_12, SmplViaWZ_13, nSimplifiedModelType};
Int_t SmplViaWZHistColors[nSimplifiedModelType] = {kAzure,kAzure-1,kAzure-2,kAzure-3,kAzure-4,
                                                   kCyan-2,kCyan-3,kCyan-4,kCyan-5,
                                                   kRed-2,kRed-1,kRed,kMagenta};
Int_t SmplViaWZLineStyle [nSimplifiedModelType] = {kDotted,kDotted,kDotted,kDotted,kDotted,
                                                   kDotted,kDotted,kDotted,kDotted,kDotted,
                                                   kDotted,kDotted,kDotted};
std::vector<TString> *SignalFileNames[nSignalType];
std::vector<TString> *SignalMN2[nSignalType];
std::vector<TString> *SignalMN1[nSignalType];
std::vector<Int_t> *DrawnSignalType;
std::vector<Int_t> *DrawnSignalFile;
std::vector<Int_t> *DrawnSignalColor;
std::vector<Int_t> *DrawnSignalStyle;
std::vector<std::string> *DrawnSignalLegend;

void SetSignalType(void){
  DrawnSignalType  = new std::vector<Int_t>();
  DrawnSignalFile  = new std::vector<Int_t>();
  DrawnSignalColor = new std::vector<Int_t>();
  DrawnSignalStyle = new std::vector<Int_t>();
  DrawnSignalLegend= new std::vector<std::string>();
  //Signal reference sample #1
  DrawnSignalType->push_back(SmplViaWZ);
  DrawnSignalFile->push_back(SmplViaWZ_6);//M_N2=150, M_N1=100
  DrawnSignalColor->push_back(kRed);
  DrawnSignalStyle->push_back(kDashDotted+2);
  DrawnSignalLegend->push_back(Form("M_{#tilde{#chi}_{2}^{0}}=%dGeV, M_{#tilde{#chi}_{1}^{0}}=%dGeV",150,100));
  //Signal reference sample #2
  DrawnSignalType->push_back(SmplViaWZ);
  DrawnSignalFile->push_back(SmplViaWZ_8);//M_N2=200, M_N1=150
  DrawnSignalColor->push_back(kBlue);
  DrawnSignalStyle->push_back(kDashDotted+2);
  DrawnSignalLegend->push_back(Form("M_{#tilde{#chi}_{2}^{0}}=%dGeV, M_{#tilde{#chi}_{1}^{0}}=%dGeV",200,150));
  //Signal reference sample #3
  DrawnSignalType->push_back(SmplViaWZ);
  DrawnSignalFile->push_back(SmplViaWZ_10);//M_N2=125, M_N1=100
  DrawnSignalColor->push_back(kRed);
  DrawnSignalStyle->push_back(kDashed);
  DrawnSignalLegend->push_back(Form("M_{#tilde{#chi}_{2}^{0}}=%dGeV, M_{#tilde{#chi}_{1}^{0}}=%dGeV",125,100));
  //Signal reference sample #4
  DrawnSignalType->push_back(SmplViaWZ);
  DrawnSignalFile->push_back(SmplViaWZ_13);//M_N2=125, M_N1=112.5
  DrawnSignalColor->push_back(kBlue);
  DrawnSignalStyle->push_back(kDashed);
  DrawnSignalLegend->push_back(Form("M_{#tilde{#chi}_{2}^{0}}=%dGeV, M_{#tilde{#chi}_{1}^{0}}=%4.1fGeV",125,112.5));

  for(Int_t signaltype=0; signaltype<nSignalType; signaltype++){
    SignalFileNames[signaltype] = new std::vector<TString>();
    SignalMN2[signaltype] = new std::vector<TString>();
    SignalMN1[signaltype] = new std::vector<TString>();
  }
  //Do not change the push_back order!!
  //Simplified Model with WZ
  
  SignalFileNames[SmplViaWZ]->push_back("205048"); SignalMN2[SmplViaWZ]->push_back("100"); SignalMN1[SmplViaWZ]->push_back("0");
  SignalFileNames[SmplViaWZ]->push_back("205049"); SignalMN2[SmplViaWZ]->push_back("200"); SignalMN1[SmplViaWZ]->push_back("0");
  SignalFileNames[SmplViaWZ]->push_back("205050"); SignalMN2[SmplViaWZ]->push_back("300"); SignalMN1[SmplViaWZ]->push_back("0");
  SignalFileNames[SmplViaWZ]->push_back("205051"); SignalMN2[SmplViaWZ]->push_back("400"); SignalMN1[SmplViaWZ]->push_back("0");
  SignalFileNames[SmplViaWZ]->push_back("205052"); SignalMN2[SmplViaWZ]->push_back("500"); SignalMN1[SmplViaWZ]->push_back("0");
  SignalFileNames[SmplViaWZ]->push_back("205053"); SignalMN2[SmplViaWZ]->push_back("150"); SignalMN1[SmplViaWZ]->push_back("100");
  SignalFileNames[SmplViaWZ]->push_back("205054"); SignalMN2[SmplViaWZ]->push_back("200"); SignalMN1[SmplViaWZ]->push_back("100");
  SignalFileNames[SmplViaWZ]->push_back("205055"); SignalMN2[SmplViaWZ]->push_back("200"); SignalMN1[SmplViaWZ]->push_back("150");
  SignalFileNames[SmplViaWZ]->push_back("205056"); SignalMN2[SmplViaWZ]->push_back("300"); SignalMN1[SmplViaWZ]->push_back("200");
  SignalFileNames[SmplViaWZ]->push_back("205128"); SignalMN2[SmplViaWZ]->push_back("125"); SignalMN1[SmplViaWZ]->push_back("100");
  SignalFileNames[SmplViaWZ]->push_back("205129"); SignalMN2[SmplViaWZ]->push_back("150"); SignalMN1[SmplViaWZ]->push_back("125");
  SignalFileNames[SmplViaWZ]->push_back("205130"); SignalMN2[SmplViaWZ]->push_back("175"); SignalMN1[SmplViaWZ]->push_back("150");
  SignalFileNames[SmplViaWZ]->push_back("205131"); SignalMN2[SmplViaWZ]->push_back("125"); SignalMN1[SmplViaWZ]->push_back("112.5");

}


/////////////////////////////////////////////////////////////////////////////////////////
enum DataType{periodA,nDataType};
TString DataTypeNames[nDataType] = {"periodA"};

std::vector<TString> *DataFileNames;
void SetDataType(void){
  DataFileNames = new std::vector<TString>();
  for(Int_t dataType=0; dataType<nDataType; dataType++){
    DataFileNames->push_back(DataTypeNames[dataType]);
  }
}


/////////////////////////////////////////////////////////////////////////////////////////
enum BGType{WW,WZ,ZZ,ttV,Higgs,ttbar,SingleTop,Zjets,Wjets,nBGType};
TString BGTypeNames[nBGType] = {"WW","WZ","ZZ","ttV","Higgs","ttbar","SingleTop","Zjets","Wjets"};
Int_t BGTypeHistColors[nBGType] = {kTeal-8,kAzure-3,kAzure-4,kCyan-1,kBlack,kOrange-2,kYellow-9,kViolet-9,kGray};
Int_t BGTypeFillColors[nBGType] = {kTeal-8,kAzure-3,kAzure-4,kCyan-1,kWhite,kOrange-2,kYellow-9,kViolet-9,kGray};
Int_t BGStackOrder1[nBGType] = {WZ,ZZ,ttV,Higgs,SingleTop,WW,ttbar,Zjets,Wjets};
Int_t BGStackOrder2[nBGType] = {Higgs,ttV,ZZ,SingleTop,WZ,WW,Wjets,Zjets,ttbar};
std::vector<TString> *BGFileNames  [nBGType];
std::vector<TString> *BGIncludeFlag[nBGType]; //e.g. "00110" means the sample is included to the "eem" and "emm" channels.

void SetBGType(void){
  for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
    BGFileNames  [bgtype] = new std::vector<TString>();
    BGIncludeFlag[bgtype] = new std::vector<TString>();
  }

  //WZ files
  BGFileNames[WZ]->push_back("187160"); BGIncludeFlag[WZ]->push_back("01000");
  BGFileNames[WZ]->push_back("187161"); BGIncludeFlag[WZ]->push_back("00010");
  BGFileNames[WZ]->push_back("187162"); BGIncludeFlag[WZ]->push_back("01110");
  BGFileNames[WZ]->push_back("187163"); BGIncludeFlag[WZ]->push_back("00100");
  BGFileNames[WZ]->push_back("187164"); BGIncludeFlag[WZ]->push_back("00001");
  BGFileNames[WZ]->push_back("187165"); BGIncludeFlag[WZ]->push_back("00111");
  BGFileNames[WZ]->push_back("187166"); BGIncludeFlag[WZ]->push_back("01100");
  BGFileNames[WZ]->push_back("187167"); BGIncludeFlag[WZ]->push_back("00011");
  BGFileNames[WZ]->push_back("187168"); BGIncludeFlag[WZ]->push_back("01111");
  BGFileNames[WZ]->push_back("187170"); BGIncludeFlag[WZ]->push_back("01000");
  BGFileNames[WZ]->push_back("187171"); BGIncludeFlag[WZ]->push_back("00010");
  BGFileNames[WZ]->push_back("187172"); BGIncludeFlag[WZ]->push_back("01110");
  BGFileNames[WZ]->push_back("187173"); BGIncludeFlag[WZ]->push_back("00100");
  BGFileNames[WZ]->push_back("187174"); BGIncludeFlag[WZ]->push_back("00001");
  BGFileNames[WZ]->push_back("187175"); BGIncludeFlag[WZ]->push_back("00111");
  BGFileNames[WZ]->push_back("187176"); BGIncludeFlag[WZ]->push_back("01100");
  BGFileNames[WZ]->push_back("187177"); BGIncludeFlag[WZ]->push_back("00011");
  BGFileNames[WZ]->push_back("187178"); BGIncludeFlag[WZ]->push_back("01111");

  //ZZ files
  BGFileNames[ZZ]->push_back("187180"); BGIncludeFlag[ZZ]->push_back("01000");
  BGFileNames[ZZ]->push_back("187181"); BGIncludeFlag[ZZ]->push_back("00110");
  BGFileNames[ZZ]->push_back("187182"); BGIncludeFlag[ZZ]->push_back("01110");
  BGFileNames[ZZ]->push_back("187183"); BGIncludeFlag[ZZ]->push_back("00001");
  BGFileNames[ZZ]->push_back("187184"); BGIncludeFlag[ZZ]->push_back("00111");
  BGFileNames[ZZ]->push_back("187185"); BGIncludeFlag[ZZ]->push_back("01111");
  BGFileNames[ZZ]->push_back("187186"); BGIncludeFlag[ZZ]->push_back("01111");
  BGFileNames[ZZ]->push_back("187187"); BGIncludeFlag[ZZ]->push_back("01111");
  BGFileNames[ZZ]->push_back("187188"); BGIncludeFlag[ZZ]->push_back("01111");

  //WW files
  BGFileNames[WW]->push_back("187150"); BGIncludeFlag[WW]->push_back("01000");
  BGFileNames[WW]->push_back("187151"); BGIncludeFlag[WW]->push_back("00110");
  BGFileNames[WW]->push_back("187152"); BGIncludeFlag[WW]->push_back("01110");
  BGFileNames[WW]->push_back("187153"); BGIncludeFlag[WW]->push_back("00001");
  BGFileNames[WW]->push_back("187154"); BGIncludeFlag[WW]->push_back("00111");
  BGFileNames[WW]->push_back("187155"); BGIncludeFlag[WW]->push_back("00111");
  BGFileNames[WW]->push_back("187156"); BGIncludeFlag[WW]->push_back("01111");
  BGFileNames[WW]->push_back("187157"); BGIncludeFlag[WW]->push_back("01111");
  BGFileNames[WW]->push_back("187158"); BGIncludeFlag[WW]->push_back("01111");
  BGFileNames[WW]->push_back("187401"); BGIncludeFlag[WW]->push_back("01000");
  BGFileNames[WW]->push_back("187402"); BGIncludeFlag[WW]->push_back("00110");
  BGFileNames[WW]->push_back("187403"); BGIncludeFlag[WW]->push_back("01110");
  BGFileNames[WW]->push_back("187404"); BGIncludeFlag[WW]->push_back("00001");
  BGFileNames[WW]->push_back("187405"); BGIncludeFlag[WW]->push_back("00111");
  BGFileNames[WW]->push_back("187406"); BGIncludeFlag[WW]->push_back("00111");
  BGFileNames[WW]->push_back("187407"); BGIncludeFlag[WW]->push_back("01111");
  BGFileNames[WW]->push_back("187408"); BGIncludeFlag[WW]->push_back("01111");
  BGFileNames[WW]->push_back("187409"); BGIncludeFlag[WW]->push_back("01111");

  //ttV files
  BGFileNames[ttV]->push_back("119353"); BGIncludeFlag[ttV]->push_back("01111");//ttW
  BGFileNames[ttV]->push_back("119355"); BGIncludeFlag[ttV]->push_back("01111");//ttZ
  BGFileNames[ttV]->push_back("119583"); BGIncludeFlag[ttV]->push_back("01111");//ttWW
  BGFileNames[ttV]->push_back("174830"); BGIncludeFlag[ttV]->push_back("01111");//ttWjExcl
  BGFileNames[ttV]->push_back("174831"); BGIncludeFlag[ttV]->push_back("01111");//ttWjjIncl
  BGFileNames[ttV]->push_back("174832"); BGIncludeFlag[ttV]->push_back("01111");//ttZjExcl
  BGFileNames[ttV]->push_back("174833"); BGIncludeFlag[ttV]->push_back("01111");//ttZjjIncl

  //Higgs
  BGFileNames[Higgs]->push_back("161105"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->WW(lnulnu)
  BGFileNames[Higgs]->push_back("161155"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->WW(lnulnu)
  BGFileNames[Higgs]->push_back("161305"); BGIncludeFlag[Higgs]->push_back("01111");//ttH:H->WW(incl.)
  BGFileNames[Higgs]->push_back("161555"); BGIncludeFlag[Higgs]->push_back("01111");//ggH tautau(ll)
  BGFileNames[Higgs]->push_back("161566"); BGIncludeFlag[Higgs]->push_back("01111");//ggH tautau(lh)
  BGFileNames[Higgs]->push_back("161577"); BGIncludeFlag[Higgs]->push_back("01111");//ggH tautau(hh)
  BGFileNames[Higgs]->push_back("161595"); BGIncludeFlag[Higgs]->push_back("01111");//VBF tautau(ll)
  BGFileNames[Higgs]->push_back("161606"); BGIncludeFlag[Higgs]->push_back("01111");//VBF tautau(lh)
  BGFileNames[Higgs]->push_back("161617"); BGIncludeFlag[Higgs]->push_back("01111");//VBF tautau(hh)
  BGFileNames[Higgs]->push_back("161635"); BGIncludeFlag[Higgs]->push_back("01111");//WH tautau(ll)
  BGFileNames[Higgs]->push_back("161646"); BGIncludeFlag[Higgs]->push_back("01111");//WH tautau(lh)
  BGFileNames[Higgs]->push_back("161657"); BGIncludeFlag[Higgs]->push_back("01111");//WH tautau(hh)
  BGFileNames[Higgs]->push_back("161675"); BGIncludeFlag[Higgs]->push_back("01111");//ZH tautau(ll)
  //  BGFileNames[Higgs]->push_back("161686"); BGIncludeFlag[Higgs]->push_back("01111");//ZH tautau(lh)
  BGFileNames[Higgs]->push_back("161697"); BGIncludeFlag[Higgs]->push_back("01111");//ZH tautau(hh)
  BGFileNames[Higgs]->push_back("161827"); BGIncludeFlag[Higgs]->push_back("01111");//ZH llbb
  BGFileNames[Higgs]->push_back("161849"); BGIncludeFlag[Higgs]->push_back("01111");//ZH nunubb
  //  BGFileNames[Higgs]->push_back("160755"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->ZZ(llnunu)
  //  BGFileNames[Higgs]->push_back("160805"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->ZZ(llnunu)
  //  BGFileNames[Higgs]->push_back("169072"); BGIncludeFlag[Higgs]->push_back("01111");//ttH:H->ZZ(incl.)
  //  BGFileNames[Higgs]->push_back("160155"); BGIncludeFlag[Higgs]->push_back("01111");//ggF:H->ZZ(4l)
  //  BGFileNames[Higgs]->push_back("160205"); BGIncludeFlag[Higgs]->push_back("01111");//VBF:H->ZZ(4l)
  //  BGFileNames[Higgs]->push_back("160255"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->ZZ(4l)
  //  BGFileNames[Higgs]->push_back("160305"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->ZZ(4l)
  //  BGFileNames[Higgs]->push_back("160505"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->ZZ(llqq)
  //  BGFileNames[Higgs]->push_back("160555"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->ZZ(llqq)
  //  BGFileNames[Higgs]->push_back("161719"); BGIncludeFlag[Higgs]->push_back("01111");//ttH:H->tautau(lhad)
  //  BGFileNames[Higgs]->push_back("XXXXXX"); BGIncludeFlag[Higgs]->push_back("00011");//WH:H->mumu ???
  //  BGFileNames[Higgs]->push_back("167418"); BGIncludeFlag[Higgs]->push_back("00111");//ZH:H->mumu

  //ttbar
  BGFileNames[ttbar]->push_back("110401"); BGIncludeFlag[ttbar]->push_back("01111");//no all-had

  //SingleTop
  BGFileNames[SingleTop]->push_back("110070"); BGIncludeFlag[SingleTop]->push_back("01111");//t t-chan
  BGFileNames[SingleTop]->push_back("110071"); BGIncludeFlag[SingleTop]->push_back("01111");//tbar t-chan
  BGFileNames[SingleTop]->push_back("110302"); BGIncludeFlag[SingleTop]->push_back("01111");//s-chan
  BGFileNames[SingleTop]->push_back("110305"); BGIncludeFlag[SingleTop]->push_back("01111");//Wt

  //Zjets
  BGFileNames[Zjets]->push_back("147406"); BGIncludeFlag[Zjets]->push_back("01100");//Zee
  BGFileNames[Zjets]->push_back("147407"); BGIncludeFlag[Zjets]->push_back("00011");//Zmumu
  //  BGFileNames[Zjets]->push_back("147408"); BGIncludeFlag[Zjets]->push_back("01111");//Ztautau

  //Wjets
  BGFileNames[Wjets]->push_back("147400"); BGIncludeFlag[Wjets]->push_back("01100");//Wp->enu
  BGFileNames[Wjets]->push_back("147401"); BGIncludeFlag[Wjets]->push_back("00011");//Wp->munu
  BGFileNames[Wjets]->push_back("147402"); BGIncludeFlag[Wjets]->push_back("01111");//Wp->taunu
  BGFileNames[Wjets]->push_back("147403"); BGIncludeFlag[Wjets]->push_back("01100");//Wm->enu
  //  BGFileNames[Wjets]->push_back("147404"); BGIncludeFlag[Wjets]->push_back("00011");//Wm->munu
  BGFileNames[Wjets]->push_back("147405"); BGIncludeFlag[Wjets]->push_back("01111");//Wm->taunu

}

TString getHistFileName(std::string fileKeyWord){
  TString strFileName = gSystem->GetFromPipe(("ls "+fileKeyWord).c_str());
  //  std::cout << "-- result" << std::endl;
  //  std::cout << strFileName.Data() << std::endl;
  if(strFileName==""){
    std::cout<<"Cannnot find output histogram in "<<fileKeyWord.c_str()<<std::endl;
    exit(1);
  }
  return strFileName;
}

Double_t getMCScale(TFile *file){
  //Calculate MC scaling factor from the given file
  Double_t xsec = ((TH1F*)file->Get("h_xsec"))->GetBinContent(1);
  Double_t neve = ((TH1F*)file->Get("h_nEve"))->GetBinContent(1);
  Double_t scale = IntLumi/(neve/xsec);
  std::cout<<"MC CrossSection="<<xsec<<", #Eve="<<neve<<", MCScale="<<scale<<std::endl;
  return scale;
}

Int_t mkPlots(TString Tag, TString SelecReg){

  std::cout<<"Making plots for the region \""<<SelecReg.Data()<<"\"."<<std::endl;

  std::vector<TFile*> *vec_datafiles   = new std::vector<TFile*>();
  std::vector<TFile*> *vec_mcfiles     = new std::vector<TFile*>();
  std::vector<TFile*> *vec_signalfiles = new std::vector<TFile*>();
  vec_datafiles  ->clear();
  vec_mcfiles    ->clear();
  vec_signalfiles->clear();

  SetDataType();
  SetBGType();
  SetSignalType();
  SetDistType();

  const Int_t nDistType = DistTypeNames->size();

  TString filepath_prefix = "$ROOTCOREBIN/../result/";

  // //DataFiles
  // for(Int_t datatype=0; datatype<nDataType; datatype++){
  //   std::cout<<"Now processing for the real data : "<<DataTypeNames[datatype].Data()<<std::endl;
  //   TString dsid = DataFileNames->at(datatype).Data();
  //   TString filename = filepath_prefix+dsid+"."+SelecReg+".AnaHists.root";
  //   std::cout<<"**** DatasetID : "<<dsid.Data()<<", filename="<<filename.Data()<<std::endl;
  //   TFile *f_tmp = new TFile(filename.Data());
  //   f_tmp->cd("nom");
  //   vec_datafiles->push_back(f_tmp);
  // }

  //Background Files
  for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
    UInt_t nsamples = BGFileNames[bgtype]->size();
    std::cout<<"Now processing for the background : "<<BGTypeNames[bgtype].Data()<<" ("<<nsamples<<" files)"<<std::endl;
    for(UInt_t bgfile=0; bgfile<nsamples; bgfile++){
      TString dsid = BGFileNames[bgtype]->at(bgfile).Data();
      TString includeflag = BGIncludeFlag[bgtype]->at(bgfile).Data();
      TString filename = getHistFileName((filepath_prefix+"/"+Tag+"/"+dsid+"."+SelecReg+".AnaHists.root").Data());
      std::cout<<"**** DatasetID : "<<dsid.Data()<<", filename=\""<<filename.Data()<<"\", IncludeFlag="<<includeflag.Data()<<std::endl;
      TFile *f_tmp = new TFile(filename.Data());
      vec_mcfiles->push_back(f_tmp);
    }
  }

  //Signal Files
  for(Int_t signaltype=0; signaltype<nSignalType; signaltype++){
    UInt_t nsamples = SignalFileNames[signaltype]->size();
    std::cout<<"Now processing for the signal : "<<SignalTypeNames[signaltype].Data()<<" ("<<nsamples<<" files)"<<std::endl;
    for(UInt_t signalfile=0; signalfile<nsamples; signalfile++){
      TString dsid = SignalFileNames[signaltype]->at(signalfile).Data();
      TString filename = getHistFileName((filepath_prefix+"/"+Tag+"/"+dsid+"."+SelecReg+".AnaHists.root").Data());
      std::cout<<"**** DatasetID : "<<dsid.Data()<<", filename=\""<<filename.Data()<<"\""<<std::endl;
      TFile *f_tmp = new TFile(filename.Data());
      vec_signalfiles->push_back(f_tmp);
    }
  }

  ////////////////////////////////////////////////////
  //Defining histograms
  ////////////////////////////////////////////////////
  //counting the number of signal samples
  TH1::SetDefaultSumw2(); //No need to call this for ROOT6?
  TH1F* dist_data[nFSType][nDistType];
  TH1F* dist_bg[nFSType][nBGType][nDistType];
  TH1F* dist_totalbg[nFSType][nDistType];
  TH1F* dist_totalbgErr[nFSType][nDistType];
  TH1F* dist_ratio[nFSType][nDistType];
  TH1F* dist_ratioErr[nFSType][nDistType];
  std::vector<TH1F*> *vec_dist_signal[nFSType][nSignalType][nDistType];
  //  std::cout<<vec_mcfiles->at(0)->GetName()<<std::endl;
  for(Int_t fstype=0; fstype<nFSType; fstype++){
    for(Int_t disttype=0; disttype<nDistType; disttype++){
      std::string distname = DistTypeNames->at(disttype).Data();
      TFile *f_tmp = vec_signalfiles->at(0);
      TH1F *h_tmp = (TH1F*)(f_tmp->Get(Form("all_%s;1",distname.c_str())));
      //     //Real data
      //     std::cout<<"Preparing final histogram of data for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
      //     dist_data[fstype][disttype] = (TH1F*)(h_tmp->Clone(Form("dist_data_%d_%d",fstype,disttype)));
      //     dist_data[fstype][disttype]->Reset();
      //Total BG
      std::cout<<"Preparing final histogram of total BG for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
      dist_totalbg[fstype][disttype] = (TH1F*)(h_tmp->Clone(Form("dist_totalbg_%d_%d",fstype,disttype)));
      dist_totalbg[fstype][disttype]->Reset();
      dist_totalbg[fstype][disttype]->Sumw2();
      //     //Ratio plot
      //     std::cout<<"Preparing final histogram of Data/SM ratio and its error for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
      //     dist_ratio[fstype][disttype] = (TH1F*)(h_tmp->Clone(Form("dist_ratio_%d_%d",fstype,disttype)));
      //     dist_ratio[fstype][disttype]->Reset();
      //     dist_ratioErr[fstype][disttype] = (TH1F*)(h_tmp->Clone(Form("dist_ratioErr_%d_%d",fstype,disttype)));
      //     dist_ratioErr[fstype][disttype]->Reset();
      //Backgrounds
      for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
        std::cout<<"Preparing final histogram of "<<BGTypeNames[bgtype]<<" for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
        dist_bg[fstype][bgtype][disttype] = (TH1F*)(h_tmp->Clone(Form("dist_bg_%d_%d_%d",fstype,bgtype,disttype)));
        dist_bg[fstype][bgtype][disttype]->Reset();
        dist_bg[fstype][bgtype][disttype]->Sumw2();
      }
      //Signal sample
      for(Int_t signaltype=0; signaltype<nSignalType; signaltype++){
        vec_dist_signal[fstype][signaltype][disttype] = new std::vector<TH1F*>();
        std::cout<<"Preparing histograms of "<<SignalTypeNames[signaltype]<<" for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
        for(UInt_t signalfile=0; signalfile<SignalFileNames[signaltype]->size(); signalfile++){
          vec_dist_signal[fstype][signaltype][disttype]->push_back((TH1F*)(h_tmp->Clone(Form("dist_signal_%d_%d_%d",fstype,signaltype,disttype))));
          vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->Reset();
          vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->Sumw2();
        }
      }
    }
  }

  // //Adding histograms of data for eee,eem,emm,mmm.
  // for(UInt_t datafile=0; datafile<nDataType; datafile++){
  //   for(Int_t fstype=0; fstype<nFSType; fstype++){
  //     std::string fsname = FSTypeNames[fstype].Data();
  //     std::cout<<"Adding histograms in "<<DataFileNames->at(datafile)<<" to "<<FSTypeNames[fstype]<<" histograms..."<<std::endl;
  //     for(Int_t disttype=0; disttype<nDistType; disttype++){
  //       //Getting the histogram to add.
  //       std::string distname = DistTypeNames->at(disttype).Data();
  //       std::string histname = Form("%s_%s;1",fsname.c_str(),distname.c_str());
  //       std::cout<<"Accessing "<<histname<<" : "<<distname<<std::endl;
  //       TFile *f_tmp = vec_datafiles->at(datafile);
  //       TH1F *h_tmp = (TH1F*)(f_tmp->Get(histname.c_str()));
  //       dist_data[fstype][disttype]->Add(h_tmp);
  //       dist_data[fstype][disttype]->SetTitle(FSTypeNames[fstype]+":Data");
  //       dist_data[fstype][disttype]->SetMarkerStyle(kFullCircle);
  //       dist_data[fstype][disttype]->SetMarkerSize(1.2);
  //       std::cout<<"#Entries : "<<h_tmp->GetName()<<" : "<<h_tmp->GetEntries()<<std::endl;
  //     }
  //   }
  // }

  // //Adding histograms of BG for eee,eem,emm,mmm.
  UInt_t fileidx=0;
  for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
    UInt_t nsamples = BGFileNames[bgtype]->size();
    for(UInt_t bgfile=0; bgfile<nsamples; bgfile++){
      for(Int_t fstype=0; fstype<nFSType; fstype++){
        std::string fsname = FSTypeNames[fstype].Data();
        TString includeflag = BGIncludeFlag[bgtype]->at(bgfile);
        TString inclFlag4fs = includeflag[fstype];
        std::cout<<"FileIndex:"<<fileidx<<", DSID="<<BGFileNames[bgtype]->at(bgfile)<<std::endl;
        if(inclFlag4fs=="0") continue;
        else{
          std::cout<<"Adding histograms in "<<BGFileNames[bgtype]->at(bgfile)<<" to "<<FSTypeNames[fstype]<<" histograms..."<<std::endl;
          // Obtaining XS and N_MC
          TFile *f_tmp = vec_mcfiles->at(fileidx);
          Double_t scale = getMCScale(f_tmp);
          for(Int_t disttype=0; disttype<nDistType; disttype++){
            //Getting the histogram to add.
            std::string distname = DistTypeNames->at(disttype).Data();
            std::string histname = Form("%s_%s;1",fsname.c_str(),distname.c_str());
            std::cout<<"Accessing "<<histname<<" : "<<distname<<std::endl;
            TH1F *h_tmp = (TH1F*)(f_tmp->Get(histname.c_str()));
            if(h_tmp->Integral()<0.){
              std::cout<<"FileIdx : "<<fileidx<<", "<<h_tmp->Integral()<<std::endl;
              //              getchar();
            }
            dist_bg[fstype][bgtype][disttype]->Add(h_tmp,scale);
            dist_bg[fstype][bgtype][disttype]->SetTitle(FSTypeNames[fstype]+":"+BGTypeNames[bgtype]);
            dist_bg[fstype][bgtype][disttype]->SetMarkerColor(BGTypeHistColors[bgtype]);
            dist_bg[fstype][bgtype][disttype]->SetFillColor(BGTypeFillColors[bgtype]);
            dist_bg[fstype][bgtype][disttype]->SetLineColor(BGTypeFillColors[bgtype]);
            std::cout<<"#Entries : "<<h_tmp->GetName()<<" : "<<h_tmp->GetEntries()<<std::endl;
          }
        }
      }
      fileidx++;
    }
  }

  //Adding histograms of Signal for eee,eem,emm,mmm.
  fileidx=0;
  for(Int_t signaltype=0; signaltype<nSignalType; signaltype++){
    UInt_t nsamples = SignalFileNames[signaltype]->size();
    for(UInt_t signalfile=0; signalfile<nsamples; signalfile++){
      for(Int_t fstype=0; fstype<nFSType; fstype++){
        std::string fsname = FSTypeNames[fstype].Data();
        std::cout<<"Adding histograms in "<<SignalFileNames[signaltype]->at(signalfile)<<" to "<<FSTypeNames[fstype]<<" histograms..."<<std::endl;
        // Obtaining XS and N_MC
        TFile *f_tmp = vec_signalfiles->at(fileidx);
        Double_t scale = getMCScale(f_tmp);
        for(Int_t disttype=0; disttype<nDistType; disttype++){
          //Getting the histogram to add.
          std::string distname = DistTypeNames->at(disttype).Data();
          std::string histname = Form("%s_%s;1",fsname.c_str(),distname.c_str());
          TH1F *h_tmp = (TH1F*)(f_tmp->Get(histname.c_str()));
          std::cout<<"Accessing "<<histname<<" : "<<distname<<std::endl;
          vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->Add(h_tmp,scale);
          vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->SetTitle(FSTypeNames[fstype]+":"+SignalTypeNames[signaltype]);
          Int_t color = kBlack;
          Int_t style = kSolid;
          if(SignalTypeNames[signaltype]=="SmplViaWZ"){
            color = SmplViaWZHistColors[signalfile];
            style = SmplViaWZLineStyle [signalfile];
          }
          vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->SetMarkerColor(color);
          vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->SetLineColor(color);
          vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->SetLineStyle(style);
          std::cout<<"#Entries : "<<h_tmp->GetName()<<" : "<<h_tmp->GetEntries()<<std::endl;
        }
      }
      fileidx++;
    }
  }

  //Making histograms for all channels (eee,eem,emm,mmm).
  for(Int_t disttype=0; disttype<nDistType; disttype++){
    for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
      dist_bg[allchan][bgtype][disttype]->Add(dist_bg[eee][bgtype][disttype]);
      dist_bg[allchan][bgtype][disttype]->Add(dist_bg[eem][bgtype][disttype]);
      dist_bg[allchan][bgtype][disttype]->Add(dist_bg[emm][bgtype][disttype]);
      dist_bg[allchan][bgtype][disttype]->Add(dist_bg[mmm][bgtype][disttype]);
      dist_bg[allchan][bgtype][disttype]->SetTitle(FSTypeNames[allchan]+":"+BGTypeNames[bgtype]);
      dist_bg[allchan][bgtype][disttype]->SetMarkerColor(BGTypeHistColors[bgtype]);
      dist_bg[allchan][bgtype][disttype]->SetLineColor(BGTypeFillColors[bgtype]);
      dist_bg[allchan][bgtype][disttype]->SetFillColor(BGTypeFillColors[bgtype]);
    }
  }

  //Making plots for total BG
  THStack *hs_bg[nFSType][nDistType];
  for(Int_t fstype=0; fstype<nFSType; fstype++){
    for(Int_t disttype=0; disttype<nDistType; disttype++){
      //for the shape
      for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
        dist_totalbg[fstype][disttype]->Add(dist_bg[fstype][bgtype][disttype]);
      }
      dist_totalbg[fstype][disttype]->SetTitle(FSTypeNames[fstype]+":Total");
      dist_totalbg[fstype][disttype]->SetLineColor(kRed);
      //for the error band
      dist_totalbgErr[fstype][disttype] = (TH1F*)dist_totalbg[fstype][disttype]->Clone(Form("dist_totalbgErr_%d_%d",fstype,disttype));
      dist_totalbgErr[fstype][disttype]->SetTitle(FSTypeNames[fstype]+":Total");
      dist_totalbgErr[fstype][disttype]->SetLineColor(kBlack);
      dist_totalbgErr[fstype][disttype]->SetFillColor(kGray+3);
      dist_totalbgErr[fstype][disttype]->SetMarkerStyle(0);
      dist_totalbgErr[fstype][disttype]->SetFillStyle(3454);
      //Creating SM stuck histogram
      std::string distname = DistTypeNames->at(disttype).Data();
      std::cout<<"Preparing THStack for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
      hs_bg[fstype][disttype] = new THStack(Form("hs_bg_%d_%d",fstype,disttype),FSTypeNames[fstype]+":Total");
      for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
        std::cout<<"Adding "<<BGTypeNames[BGStackOrder2[bgtype]]<<" to THStack... "
                 <<"#Eve="<<dist_bg[fstype][BGStackOrder2[bgtype]][disttype]->Integral()<<std::endl;
        hs_bg[fstype][disttype]->Add(dist_bg[fstype][BGStackOrder2[bgtype]][disttype]);
        //hs_bg[fstype][disttype]->Add(dist_bg[fstype][bgtype][disttype]);
      }
    }
  }

  // //Making plots for Data/MC ratio
  // for(Int_t fstype=0; fstype<nFSType; fstype++){
  //   for(Int_t disttype=0; disttype<nDistType; disttype++){
  //     dist_ratio[fstype][disttype]->SetTitle(FSTypeNames[fstype]+":Total");
  //     dist_ratio[fstype][disttype]->GetYaxis()->SetTitle("Data/SM");
  //     dist_ratio[fstype][disttype]->SetLineColor(kBlack);
  //     dist_ratio[fstype][disttype]->SetMarkerColor(kBlack);
  //     dist_ratio[fstype][disttype]->SetMarkerStyle(kFullCircle);
  //     dist_ratio[fstype][disttype]->SetMarkerSize(1.2);
  //     dist_ratioErr[fstype][disttype]->SetTitle(FSTypeNames[fstype]+":Total");
  //     dist_ratioErr[fstype][disttype]->GetYaxis()->SetTitle("Data/SM");
  //     dist_ratioErr[fstype][disttype]->SetLineColor(kBlack);
  //     dist_ratioErr[fstype][disttype]->SetFillColor(kGray+3);
  //     dist_ratioErr[fstype][disttype]->SetMarkerStyle(0);
  //     dist_ratioErr[fstype][disttype]->SetFillStyle(3454);

  //     Int_t nbins = dist_ratioErr[fstype][disttype]->GetNbinsX();
  //     for(Int_t bin=0; bin<nbins; bin++){
  //       Double_t dataCont = dist_data   [fstype][disttype]->GetBinContent(bin+1);
  //       Double_t bgCont   = dist_totalbg[fstype][disttype]->GetBinContent(bin+1);
  //       Double_t dataErr  = dist_data   [fstype][disttype]->GetBinError(bin+1);
  //       Double_t bgErr    = dist_totalbg[fstype][disttype]->GetBinError(bin+1);
  //       Double_t ratioData    = bgCont!=0.?dataCont/bgCont:0.;
  //       Double_t ratioDataErr = bgCont!=0.?dataErr/bgCont:0.;
  //       Double_t ratioMC      = 1.;
  //       //Double_t ratioMCErr   = bgCont!=0.?bgErr*dataCont/bgCont/bgCont:0.;
  //       Double_t ratioMCErr   = bgCont!=0.?bgErr/bgCont:0.;
  //       dist_ratio[fstype][disttype]->SetBinContent(bin+1,ratioData);
  //       dist_ratio[fstype][disttype]->SetBinError(bin+1,ratioDataErr);
  //       dist_ratioErr[fstype][disttype]->SetBinContent(bin+1,ratioMC);
  //       dist_ratioErr[fstype][disttype]->SetBinError(bin+1,ratioMCErr);
  //     }

  //   }
  // }

  //Preparing the legend
  TLegend *leg = new TLegend(0.1,0.1,0.9,0.9);
  leg->SetNColumns(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextFont(72);
  leg->SetTextSize(0.05);
  // leg->AddEntry(dist_data[0][0],"Data","lp");
  leg->AddEntry(dist_bg[0][WW][0],"WW","f");
  leg->AddEntry(dist_bg[0][WZ][0],"WZ","f");
  leg->AddEntry(dist_bg[0][ZZ][0],"ZZ","f");
  leg->AddEntry(dist_bg[0][ttV][0],"t#bar{t}V","f");
  // leg->AddEntry(dist_bg[0][Tribosons][0],"Tribosons","f");
  //For Higgs histograms, special treatment is needed to make it visible.
  TH1F* h_higgs4leg = (TH1F*)dist_bg[0][Higgs][0]->Clone("h_higgs4leg");
  h_higgs4leg->SetLineColor(kBlack);
  leg->AddEntry(h_higgs4leg,"Higgs","f");
  leg->AddEntry(dist_bg[0][Wjets][0],"W+jets","f");
  leg->AddEntry(dist_bg[0][ttbar][0],"t#bar{t}","f");
  leg->AddEntry(dist_bg[0][SingleTop][0],"Single top","f");
  leg->AddEntry(dist_bg[0][Zjets][0],"Z/#gamma*+jets","f");
  // leg->AddEntry(dist_bg[0][OtherFake][0],"Other BG with fakes","f");
  leg->AddEntry(dist_totalbg[0][0],"Total SM","l");
  leg->AddEntry(dist_totalbgErr[0][0],"Unct. for SM ","f");
  const Int_t nDrawnSignal = DrawnSignalType->size();

  ////////////////////////////////////////////////////
  // histograms definition end.
  ////////////////////////////////////////////////////

  Bool_t debugPlot = kTRUE;
  TUtil *u = new TUtil(("plots/"+Tag+"."+SelecReg+".ps").Data(),("plots/"+Tag+"."+SelecReg+".root").Data(),debugPlot);
  gStyle->SetPalette(1);
  
  u->SetPadNumXY(1,1);
  u->cdPad();
  leg->Draw();

  // for(Int_t disttype=0; disttype<nDistType; disttype++){
  //   for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
  //     u->Draw(dist_bg[allchan][bgtype][disttype],"E");
  //     u->cdPad();
  //     u->Draw(dist_bg[eee][bgtype][disttype],"E");
  //     u->Draw(dist_bg[eem][bgtype][disttype],"E");
  //     u->Draw(dist_bg[emm][bgtype][disttype],"E");
  //     u->Draw(dist_bg[mmm][bgtype][disttype],"E");
  //   }
  // }

  u->SetPadNumXY(3,4);
  for(Int_t disttype=0; disttype<nDistType; disttype++){
    u->cdPad();
    for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
      dist_bg[allchan][bgtype][disttype]->DrawNormalized(bgtype==0?"":"same");
      dist_bg[allchan][bgtype][disttype]->Write();
    }
  }

  TH1F* dist_zNminusOneAbove[nFSType][nDistType][nDrawnSignal];
  TH1F* dist_zNminusOneBelow[nFSType][nDistType][nDrawnSignal];
  //  u->SetPadNumXY(1,3);
  u->SetPadNumXY(1,5);
  std::vector<std::string> vec_yieldStr;
  for(Int_t fstype=0; fstype<nFSType; fstype++){
    for(Int_t disttype=0; disttype<nDistType; disttype++){
      //      u->Draw(dist_totalbgErr[fstype][disttype],"E2",010);
      //      u->Draw(dist_totalbg   [fstype][disttype],"samehist"); //should be drawn after the error histogram.
      //      dist_data[fstype][disttype]->Draw("sameE");
      //      Double_t logMax = TMath::Max(dist_totalbg[fstype][disttype]->GetMaximum(),dist_data[fstype][disttype]->GetMaximum());
      //      Double_t linMax = TMath::Max(dist_totalbg[fstype][disttype]->GetMaximum(),dist_data[fstype][disttype]->GetMaximum());
      Double_t logMax = TMath::Max(dist_totalbg[fstype][disttype]->GetMaximum(),-1.);
      Double_t linMax = TMath::Max(dist_totalbg[fstype][disttype]->GetMaximum(),-1.);
      logMax *= 1.3E0*TMath::Log10(logMax);
      linMax *= 1.3E0;
      if(logMax<=1.) logMax=2.;

      Double_t entry = dist_totalbg[fstype][disttype]->GetEntries();
      Double_t integral = dist_totalbg[fstype][disttype]->Integral();
      if(fstype==0 && DistTypeNames->at(disttype)=="nJet"){
        vec_yieldStr.push_back("*************************************************");
        vec_yieldStr.push_back(Form("Total BG : %f",integral));
        for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
          vec_yieldStr.push_back(Form("--------- %9s: #Eve=%9.3f, (%5.1f%%)",BGTypeNames[BGStackOrder2[bgtype]].Data(),
                                      dist_bg[fstype][BGStackOrder2[bgtype]][disttype]->Integral(),
                                      dist_bg[fstype][BGStackOrder2[bgtype]][disttype]->Integral()/integral*100)
                                 );
        }
        vec_yieldStr.push_back("=================================================");
        for(Int_t signaltype=0; signaltype<nSignalType; signaltype++){
          UInt_t nsamples = SignalFileNames[signaltype]->size();
          for(UInt_t signalfile=0; signalfile<SignalFileNames[signaltype]->size(); signalfile++){
            Double_t sigyields = vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->Integral();
            Double_t p0 = RooStats::NumberCountingUtils::BinomialExpP(sigyields, integral, 0.30);
            Double_t Zn = RooStats::NumberCountingUtils::BinomialExpZ(sigyields, integral, 0.30);
            vec_yieldStr.push_back(Form("(%s,%5s): %7.1f, p0=%9.7f, Zn=%5.3f",SignalMN2[signaltype]->at(signalfile).Data(),
                                        SignalMN1[signaltype]->at(signalfile).Data(),sigyields,p0,Zn)
                                   );
          }
        }
        vec_yieldStr.push_back("*************************************************");
      }
      if(entry==0.){ //to avoid error in PostScript
        u->Draw(dist_totalbg[fstype][disttype],"hist");
        u->Draw(dist_totalbg[fstype][disttype],"hist",010);
        dist_totalbg[fstype][disttype]->Write();
        u->cdPad();
        u->cdPad();
      }else{
        //////////////////////////////////////////////////////////////
        // Linear version
        //////////////////////////////////////////////////////////////
        u->cdPad();
        //should be drawn in this order
        hs_bg[fstype][disttype]->SetMaximum(linMax);
        hs_bg[fstype][disttype]->Draw("hist");
        hs_bg[fstype][disttype]->GetXaxis()->SetTitle(dist_bg[fstype][0][disttype]->GetXaxis()->GetTitle());
        hs_bg[fstype][disttype]->GetYaxis()->SetTitle(dist_bg[fstype][0][disttype]->GetYaxis()->GetTitle());
        u->Draw(dist_totalbg   [fstype][disttype],"samehist"); //should be drawn after the error histogram.
        // dist_data[fstype][disttype]->Draw("sameE");
        u->Draw(dist_totalbgErr[fstype][disttype],"sameE2");
        dist_totalbg[fstype][disttype]->Write();
        TH1F *h_sig[nDrawnSignal];
        for(Int_t cnt=0; cnt<nDrawnSignal; cnt++){
          h_sig[cnt] = vec_dist_signal[fstype][DrawnSignalType->at(cnt)][disttype]->at(DrawnSignalFile->at(cnt));
          h_sig[cnt]->SetLineColor(DrawnSignalColor->at(cnt));
          h_sig[cnt]->SetMarkerColor(DrawnSignalColor->at(cnt));
          h_sig[cnt]->SetFillColor(0);
          h_sig[cnt]->SetLineStyle(DrawnSignalStyle->at(cnt));
          h_sig[cnt]->Draw("samehisttext");
          if(fstype==0&&disttype==0) leg->AddEntry(h_sig[cnt],DrawnSignalLegend->at(cnt).c_str(),"l");
        }
        //////////////////////////////////////////////////////////////
        // Log version
        //////////////////////////////////////////////////////////////
        u->cdPad();
        //should be drawn in this order
        hs_bg[fstype][disttype]->SetMaximum(logMax);
        hs_bg[fstype][disttype]->SetMinimum(1.E-2);
        hs_bg[fstype][disttype]->Draw("hist");
        u->Draw(dist_totalbg   [fstype][disttype],"samehist");
        // dist_data[fstype][disttype]->Draw("sameE");
        u->Draw(dist_totalbgErr[fstype][disttype],"sameE2");
        for(Int_t cnt=0; cnt<nDrawnSignal; cnt++) h_sig[cnt]->Draw("samehist");
        gPad->SetLogy();
        ////////////////////////////////////////////////////////////
        //Preparing Z(n-1) plots
        ////////////////////////////////////////////////////////////
        for(Int_t cnt=0; cnt<nDrawnSignal; cnt++){
          dist_zNminusOneAbove[fstype][disttype][cnt] = (TH1F*)dist_totalbg[fstype][disttype]->Clone(Form("dist_zNminusOneAbove_%d_%d_%d",fstype,disttype,cnt));
          dist_zNminusOneAbove[fstype][disttype][cnt]->Reset();
          dist_zNminusOneAbove[fstype][disttype][cnt]->GetYaxis()->SetTitle("Z_{n-1}");
          dist_zNminusOneAbove[fstype][disttype][cnt]->SetLineColor(DrawnSignalColor->at(cnt));
          dist_zNminusOneAbove[fstype][disttype][cnt]->SetMarkerColor(DrawnSignalColor->at(cnt));
          dist_zNminusOneAbove[fstype][disttype][cnt]->SetFillColor(0);
          dist_zNminusOneAbove[fstype][disttype][cnt]->SetLineStyle(DrawnSignalStyle->at(cnt));
          dist_zNminusOneBelow[fstype][disttype][cnt] = (TH1F*)dist_totalbg[fstype][disttype]->Clone(Form("dist_zNminusOneBelow_%d_%d_%d",fstype,disttype,cnt));
          dist_zNminusOneBelow[fstype][disttype][cnt]->Reset();
          dist_zNminusOneBelow[fstype][disttype][cnt]->GetYaxis()->SetTitle("Z_{n-1}");
          dist_zNminusOneBelow[fstype][disttype][cnt]->SetLineColor(DrawnSignalColor->at(cnt));
          dist_zNminusOneBelow[fstype][disttype][cnt]->SetMarkerColor(DrawnSignalColor->at(cnt));
          dist_zNminusOneBelow[fstype][disttype][cnt]->SetFillColor(0);
          dist_zNminusOneBelow[fstype][disttype][cnt]->SetLineStyle(DrawnSignalStyle->at(cnt));
          h_sig[cnt] = vec_dist_signal[fstype][DrawnSignalType->at(cnt)][disttype]->at(DrawnSignalFile->at(cnt));
          Int_t nBins = h_sig[cnt]->GetNbinsX()+2; //+2 is needed to look through all bins (with under/overflow bins).
          //Loop for Zn-1 culculation (looking all bins)
          for(Int_t cntBin=0; cntBin<nBins; cntBin++){
            //Loop for summing up bin contetnts toword bin on right(look all bins above cntBin)
            Double_t binContSumAboveSig = 0.;
            Double_t binContSumAboveBG  = 0.;
            for(Int_t tmpBin=cntBin; tmpBin<nBins; tmpBin++){ //+2 is needed to look through all bins.
              binContSumAboveSig += h_sig[cnt]                    ->GetBinContent(tmpBin);
              binContSumAboveBG  += dist_totalbg[fstype][disttype]->GetBinContent(tmpBin);
            }
            if(binContSumAboveSig>0. && binContSumAboveBG>0.){
              Double_t ZnAbove = RooStats::NumberCountingUtils::BinomialExpZ(binContSumAboveSig, binContSumAboveBG, 0.30);
              dist_zNminusOneAbove[fstype][disttype][cnt]->SetBinContent(cntBin,ZnAbove);
            }
            //Loop for summing up bin contetnts toword bin on left(look all bins below cntBin)
            Double_t binContSumBelowSig = 0.;
            Double_t binContSumBelowBG  = 1E-20;
            for(Int_t tmpBin=nBins-cntBin-1; tmpBin>=0; tmpBin--){ //+2 is needed to look through all bins.
              binContSumBelowSig += h_sig[cnt]                    ->GetBinContent(tmpBin);
              binContSumBelowBG  += dist_totalbg[fstype][disttype]->GetBinContent(tmpBin);
            }
            if(binContSumBelowSig>0. && binContSumBelowBG>0.){
              Double_t ZnBelow = RooStats::NumberCountingUtils::BinomialExpZ(binContSumBelowSig, binContSumBelowBG, 0.30);
              dist_zNminusOneBelow[fstype][disttype][cnt]->SetBinContent(nBins-cntBin-1,ZnBelow);
            }
            // std::cout<<"SigAbove="<<binContSumAboveSig<<", BGAbove="<<binContSumAboveBG<<std::endl;
            // std::cout<<"SigBelow="<<binContSumBelowSig<<", BGBelow="<<binContSumBelowBG<<std::endl;
            // std::cout<<"ZnAbove="<<ZnAbove<<", ZnBelow="<<ZnBelow<<std::endl;
            // getchar();
          }
        }
        ////////////////////////////////////////////////////////////
        //Drawing Z(n-1) plots
        ////////////////////////////////////////////////////////////
        u->cdPad();
        Double_t maxZnAbove = -10.;
        Double_t minZnAbove =  10.;
        for(Int_t cnt=0; cnt<nDrawnSignal; cnt++){
          Double_t tmpMax = dist_zNminusOneAbove[fstype][disttype][cnt]->GetMaximum();
          Double_t tmpMin = dist_zNminusOneAbove[fstype][disttype][cnt]->GetMinimum();
          maxZnAbove = (tmpMax>maxZnAbove?tmpMax:maxZnAbove);
          minZnAbove = (tmpMin>minZnAbove?minZnAbove:tmpMin);
          dist_zNminusOneAbove[fstype][disttype][cnt]->Draw(cnt==0?"text":"same");
        }
        Double_t diffAbove = maxZnAbove-minZnAbove;
        dist_zNminusOneAbove[fstype][disttype][0]->GetYaxis()->SetRangeUser(minZnAbove-0.2*diffAbove,maxZnAbove+0.2*diffAbove);
        u->cdPad();
        Double_t maxZnBelow = -10.;
        Double_t minZnBelow =  10.;
        for(Int_t cnt=0; cnt<nDrawnSignal; cnt++){
          Double_t tmpMax = dist_zNminusOneBelow[fstype][disttype][cnt]->GetMaximum();
          Double_t tmpMin = dist_zNminusOneBelow[fstype][disttype][cnt]->GetMinimum();
          maxZnBelow = (tmpMax>maxZnBelow?tmpMax:maxZnBelow);
          minZnBelow = (tmpMin>minZnBelow?minZnBelow:tmpMin);
          dist_zNminusOneBelow[fstype][disttype][cnt]->Draw(cnt==0?"text":"same");
        }        
        Double_t diffBelow = maxZnBelow-minZnBelow;
        dist_zNminusOneBelow[fstype][disttype][0]->GetYaxis()->SetRangeUser(minZnBelow-0.2*diffBelow,maxZnBelow+0.2*diffBelow);
      }
      ////////////////////////////////////////////////////////////
      //Ratio plot
      ////////////////////////////////////////////////////////////
      u->cdPad(); //not needed when we include ratio plot in the future
      // //should be drawn in this order
      // Double_t ratioWidth = 1.0;
      // dist_ratio[fstype][disttype]->SetStats(0);
      // dist_ratio[fstype][disttype]->GetYaxis()->SetRangeUser(1.-ratioWidth,1.+ratioWidth);
      // //      dist_ratio[fstype][disttype]->SetMarkerColor(kBlack);
      // u->Draw(dist_ratio[fstype][disttype],"E");
      // u->Draw(dist_ratioErr[fstype][disttype],"sameE2");
    }
  }

  //Plotting #signal vs #baseline leptons
  fileidx=0;
  u->Update();
  for(Int_t signaltype=0; signaltype<nSignalType; signaltype++){
    UInt_t nsamples = SignalFileNames[signaltype]->size();
    for(UInt_t signalfile=0; signalfile<nsamples; signalfile++){
      TFile *f_tmp = vec_signalfiles->at(fileidx);
      TH2F *h2_tmp = (TH2F*)(f_tmp->Get("h_nSigBaseLep"));
      h2_tmp->SetStats(0);
      h2_tmp->SetMarkerSize(2.0);
      h2_tmp->GetZaxis()->SetTitle("Events in 10k");
      u->cdPad();
      h2_tmp->DrawNormalized("colztext",10000)->GetZaxis()->SetRangeUser(0,1000);
      myText(0.18,0.78,kBlack,
             Form("MN2=%s,MN1=%s",
                  SignalMN2[signaltype]->at(signalfile).Data(),
                  SignalMN1[signaltype]->at(signalfile).Data()), 0.1);
      fileidx++;
    }
  }

  // //Plotting signal only distributions
  // for(Int_t fstype=0; fstype<nFSType; fstype++){
  //   for(Int_t disttype=0; disttype<nDistType; disttype++){
  //     for(Int_t signaltype=0; signaltype<nSignalType; signaltype++){
  //       UInt_t nsamples = SignalFileNames[signaltype]->size();
  //       for(UInt_t signalfile=0; signalfile<nsamples; signalfile++){
  //         std::string option = (signaltype==0&&signalfile==0) ? "" : "same";
  //         if(option=="") u->cdPad();
  //         vec_dist_signal[fstype][signaltype][disttype]->at(signalfile)->Draw(option.c_str());
  //       }
  //     }
  //   }
  // }

  //dump signal/background yields
  u->SetPadNumXY(1,1);
  u->Update();
  u->cdPad();
  for(yield=0; yield<vec_yieldStr.size(); yield++){
    std::cout<<vec_yieldStr.at(yield)<<std::endl;
    Double_t top = 0.9;
    Double_t bottom = 0.1;
    Double_t diff = top-bottom;
    myText(0.07,top-(diff/(vec_yieldStr.size()-1))*yield,kBlack,const_cast<char*>(vec_yieldStr.at(yield).c_str()),0.03,82);
  }

  //drawing legends again...
  u->Update();
  u->cdPad();
  leg->Draw();

  delete vec_datafiles;
  delete vec_signalfiles;
  delete vec_mcfiles;
  delete u;

  return 0;
}
