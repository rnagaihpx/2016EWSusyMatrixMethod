//////////////////////////////////////////////////////
// Scripts to make various plots.
// Usage : 
// root -q -b -n 'mkFRMeas.C+O("h0001","none")'
// root -q -b -n 'mkFRMeas.C+O("h0002","3lep")'
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

#include"TUtil.cc"
#include"AtlasUtils.cc"

std::vector<TString> *DistTypeNames;
void SetDistType(void){
  DistTypeNames = new std::vector<TString>();
  DistTypeNames->push_back("PREl2and3Pt");          DistTypeNames->push_back("PRbaseEl2and3Pt");
  DistTypeNames->push_back("PREl2and3Eta");         DistTypeNames->push_back("PRbaseEl2and3Eta");
  DistTypeNames->push_back("COEl2and3Pt");          DistTypeNames->push_back("CObaseEl2and3Pt");
  DistTypeNames->push_back("COEl2and3Eta");         DistTypeNames->push_back("CObaseEl2and3Eta");
  DistTypeNames->push_back("HFEl2and3Pt");          DistTypeNames->push_back("HFbaseEl2and3Pt");
  DistTypeNames->push_back("HFEl2and3Eta");         DistTypeNames->push_back("HFbaseEl2and3Eta");
  DistTypeNames->push_back("LFEl2and3Pt");          DistTypeNames->push_back("LFbaseEl2and3Pt");
  DistTypeNames->push_back("LFEl2and3Eta");         DistTypeNames->push_back("LFbaseEl2and3Eta");
  DistTypeNames->push_back("UKEl2and3Pt");          DistTypeNames->push_back("UKbaseEl2and3Pt");
  DistTypeNames->push_back("UKEl2and3Eta");         DistTypeNames->push_back("UKbaseEl2and3Eta");
  DistTypeNames->push_back("PRMu2and3Pt");          DistTypeNames->push_back("PRbaseMu2and3Pt");
  DistTypeNames->push_back("PRMu2and3Eta");         DistTypeNames->push_back("PRbaseMu2and3Eta");
  DistTypeNames->push_back("COMu2and3Pt");          DistTypeNames->push_back("CObaseMu2and3Pt");
  DistTypeNames->push_back("COMu2and3Eta");         DistTypeNames->push_back("CObaseMu2and3Eta");
  DistTypeNames->push_back("HFMu2and3Pt");          DistTypeNames->push_back("HFbaseMu2and3Pt");
  DistTypeNames->push_back("HFMu2and3Eta");         DistTypeNames->push_back("HFbaseMu2and3Eta");
  DistTypeNames->push_back("LFMu2and3Pt");          DistTypeNames->push_back("LFbaseMu2and3Pt");
  DistTypeNames->push_back("LFMu2and3Eta");         DistTypeNames->push_back("LFbaseMu2and3Eta");
  DistTypeNames->push_back("UKMu2and3Pt");          DistTypeNames->push_back("UKbaseMu2and3Pt");
  DistTypeNames->push_back("UKMu2and3Eta");         DistTypeNames->push_back("UKbaseMu2and3Eta");

  DistTypeNames->push_back("fakeEl2and3Class");      DistTypeNames->push_back("fakebaseEl2and3Class");
  DistTypeNames->push_back("fakeMu2and3Class");      DistTypeNames->push_back("fakebaseMu2and3Class");
  DistTypeNames->push_back("sgnlfakelepClass");      DistTypeNames->push_back("basefakelepClass");
  DistTypeNames->push_back("elParentPdgId"   );      DistTypeNames->push_back("muParentPdgId"       );

  // DistTypeNames->push_back("PREl2Pt");              DistTypeNames->push_back("PRbaseEl2Pt");
  // DistTypeNames->push_back("PREl2Eta");             DistTypeNames->push_back("PRbaseEl2Eta");
  // DistTypeNames->push_back("PREl3Pt");              DistTypeNames->push_back("PRbaseEl3Pt");
  // DistTypeNames->push_back("PREl3Eta");             DistTypeNames->push_back("PRbaseEl3Eta");
  // DistTypeNames->push_back("COEl2Pt");              DistTypeNames->push_back("CObaseEl2Pt");
  // DistTypeNames->push_back("COEl2Eta");             DistTypeNames->push_back("CObaseEl2Eta");
  // DistTypeNames->push_back("COEl3Pt");              DistTypeNames->push_back("CObaseEl3Pt");
  // DistTypeNames->push_back("COEl3Eta");             DistTypeNames->push_back("CObaseEl3Eta");
  // DistTypeNames->push_back("HFEl2Eta");             DistTypeNames->push_back("HFbaseEl2Eta");
  // DistTypeNames->push_back("HFEl3Pt");              DistTypeNames->push_back("HFbaseEl3Pt");
  // DistTypeNames->push_back("HFEl3Eta");             DistTypeNames->push_back("HFbaseEl3Eta");
  // DistTypeNames->push_back("LFEl2Pt");              DistTypeNames->push_back("LFbaseEl2Pt");
  // DistTypeNames->push_back("LFEl2Eta");             DistTypeNames->push_back("LFbaseEl2Eta");
  // DistTypeNames->push_back("LFEl3Pt");              DistTypeNames->push_back("LFbaseEl3Pt");
  // DistTypeNames->push_back("LFEl3Eta");             DistTypeNames->push_back("LFbaseEl3Eta");
  // DistTypeNames->push_back("UKEl2Pt");              DistTypeNames->push_back("UKbaseEl2Pt");
  // DistTypeNames->push_back("UKEl2Eta");             DistTypeNames->push_back("UKbaseEl2Eta");
  // DistTypeNames->push_back("UKEl3Pt");              DistTypeNames->push_back("UKbaseEl3Pt");
  // DistTypeNames->push_back("UKEl3Eta");             DistTypeNames->push_back("UKbaseEl3Eta");
  // DistTypeNames->push_back("PRMu2Pt");              DistTypeNames->push_back("PRbaseMu2Pt");
  // DistTypeNames->push_back("PRMu2Eta");             DistTypeNames->push_back("PRbaseMu2Eta");
  // DistTypeNames->push_back("PRMu3Pt");              DistTypeNames->push_back("PRbaseMu3Pt");
  // DistTypeNames->push_back("PRMu3Eta");             DistTypeNames->push_back("PRbaseMu3Eta");
  // DistTypeNames->push_back("COMu2Pt");              DistTypeNames->push_back("CObaseMu2Pt");
  // DistTypeNames->push_back("COMu2Eta");             DistTypeNames->push_back("CObaseMu2Eta");
  // DistTypeNames->push_back("COMu3Pt");              DistTypeNames->push_back("CObaseMu3Pt");
  // DistTypeNames->push_back("COMu3Eta");             DistTypeNames->push_back("CObaseMu3Eta");
  // DistTypeNames->push_back("HFMu2Eta");             DistTypeNames->push_back("HFbaseMu2Eta");
  // DistTypeNames->push_back("HFMu3Pt");              DistTypeNames->push_back("HFbaseMu3Pt");
  // DistTypeNames->push_back("HFMu3Eta");             DistTypeNames->push_back("HFbaseMu3Eta");
  // DistTypeNames->push_back("LFMu2Pt");              DistTypeNames->push_back("LFbaseMu2Pt");
  // DistTypeNames->push_back("LFMu2Eta");             DistTypeNames->push_back("LFbaseMu2Eta");
  // DistTypeNames->push_back("LFMu3Pt");              DistTypeNames->push_back("LFbaseMu3Pt");
  // DistTypeNames->push_back("LFMu3Eta");             DistTypeNames->push_back("LFbaseMu3Eta");
  // DistTypeNames->push_back("UKMu2Pt");              DistTypeNames->push_back("UKbaseMu2Pt");
  // DistTypeNames->push_back("UKMu2Eta");             DistTypeNames->push_back("UKbaseMu2Eta");
  // DistTypeNames->push_back("UKMu3Pt");              DistTypeNames->push_back("UKbaseMu3Pt");
  // DistTypeNames->push_back("UKEl3Eta");             DistTypeNames->push_back("UKbaseMu3Eta");
  // DistTypeNames->push_back("baselepOrigin");
  // DistTypeNames->push_back("baselep1Origin");
  // DistTypeNames->push_back("baselep2Origin");
  // DistTypeNames->push_back("baselep3Origin");
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
enum BGType{WZ,ZZ,ttV,Higgs,ttbar,SingleTop,Zjets,nBGType};
TString BGTypeNames[nBGType] = {"WZ","ZZ","ttV","Higgs","ttbar","SingleTop","Zjets"};
Int_t BGTypeHistColors[nBGType] = {kAzure-3,kAzure-4,kCyan-1,kBlack,kOrange-2,kYellow-9,kViolet-9};
Int_t BGTypeFillColors[nBGType] = {kAzure-3,kAzure-4,kCyan-1,kWhite,kOrange-2,kYellow-9,kViolet-9};
Int_t BGStackOrder1[nBGType] = {WZ,ZZ,ttV,Higgs,SingleTop,ttbar,Zjets};
Int_t BGStackOrder2[nBGType] = {Higgs,ttV,ZZ,SingleTop,WZ,Zjets,ttbar};
std::vector<TString> *BGFileNames  [nBGType];
std::vector<TString> *BGIncludeFlag[nBGType]; //e.g. "00110" means the sample is included to the "eem" and "emm" channels.

void SetBGType(void){
  for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
    BGFileNames  [bgtype] = new std::vector<TString>();
    BGIncludeFlag[bgtype] = new std::vector<TString>();
  }

  //WZ files
  // BGFileNames[WZ]->push_back("187160"); BGIncludeFlag[WZ]->push_back("01000");
  // BGFileNames[WZ]->push_back("187161"); BGIncludeFlag[WZ]->push_back("00010");
  // BGFileNames[WZ]->push_back("187162"); BGIncludeFlag[WZ]->push_back("01110");
  // BGFileNames[WZ]->push_back("187163"); BGIncludeFlag[WZ]->push_back("00100");
  // BGFileNames[WZ]->push_back("187164"); BGIncludeFlag[WZ]->push_back("00001");
  // BGFileNames[WZ]->push_back("187165"); BGIncludeFlag[WZ]->push_back("00111");
  // BGFileNames[WZ]->push_back("187166"); BGIncludeFlag[WZ]->push_back("01100");
  // BGFileNames[WZ]->push_back("187167"); BGIncludeFlag[WZ]->push_back("00011");
  // BGFileNames[WZ]->push_back("187168"); BGIncludeFlag[WZ]->push_back("01111");
  // BGFileNames[WZ]->push_back("187170"); BGIncludeFlag[WZ]->push_back("01000");
  // BGFileNames[WZ]->push_back("187171"); BGIncludeFlag[WZ]->push_back("00010");
  // BGFileNames[WZ]->push_back("187172"); BGIncludeFlag[WZ]->push_back("01110");
  // BGFileNames[WZ]->push_back("187173"); BGIncludeFlag[WZ]->push_back("00100");
  // BGFileNames[WZ]->push_back("187174"); BGIncludeFlag[WZ]->push_back("00001");
  // BGFileNames[WZ]->push_back("187175"); BGIncludeFlag[WZ]->push_back("00111");
  // BGFileNames[WZ]->push_back("187176"); BGIncludeFlag[WZ]->push_back("01100");
  // BGFileNames[WZ]->push_back("187177"); BGIncludeFlag[WZ]->push_back("00011");
  // BGFileNames[WZ]->push_back("187178"); BGIncludeFlag[WZ]->push_back("01111");

  // //ZZ files
  // BGFileNames[ZZ]->push_back("187180"); BGIncludeFlag[ZZ]->push_back("01000");
  // BGFileNames[ZZ]->push_back("187181"); BGIncludeFlag[ZZ]->push_back("00110");
  // BGFileNames[ZZ]->push_back("187182"); BGIncludeFlag[ZZ]->push_back("01110");
  // BGFileNames[ZZ]->push_back("187183"); BGIncludeFlag[ZZ]->push_back("00001");
  // BGFileNames[ZZ]->push_back("187184"); BGIncludeFlag[ZZ]->push_back("00111");
  // BGFileNames[ZZ]->push_back("187185"); BGIncludeFlag[ZZ]->push_back("01111");
  // BGFileNames[ZZ]->push_back("187186"); BGIncludeFlag[ZZ]->push_back("01111");
  // BGFileNames[ZZ]->push_back("187187"); BGIncludeFlag[ZZ]->push_back("01111");
  // BGFileNames[ZZ]->push_back("187188"); BGIncludeFlag[ZZ]->push_back("01111");

  // //ttV files
  // BGFileNames[ttV]->push_back("119353"); BGIncludeFlag[ttV]->push_back("01111");//ttW
  // BGFileNames[ttV]->push_back("119355"); BGIncludeFlag[ttV]->push_back("01111");//ttZ
  // BGFileNames[ttV]->push_back("119583"); BGIncludeFlag[ttV]->push_back("01111");//ttWW

  // //Higgs
  // BGFileNames[Higgs]->push_back("161105"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->WW(lnulnu)
  // BGFileNames[Higgs]->push_back("161155"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->WW(lnulnu)
  // BGFileNames[Higgs]->push_back("161305"); BGIncludeFlag[Higgs]->push_back("01111");//ttH:H->WW(incl.)
  // //  BGFileNames[Higgs]->push_back("160755"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->ZZ(llnunu)
  // //  BGFileNames[Higgs]->push_back("160805"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->ZZ(llnunu)
  // //  BGFileNames[Higgs]->push_back("169072"); BGIncludeFlag[Higgs]->push_back("01111");//ttH:H->ZZ(incl.)
  // //  BGFileNames[Higgs]->push_back("160155"); BGIncludeFlag[Higgs]->push_back("01111");//ggF:H->ZZ(4l)
  // //  BGFileNames[Higgs]->push_back("160205"); BGIncludeFlag[Higgs]->push_back("01111");//VBF:H->ZZ(4l)
  // //  BGFileNames[Higgs]->push_back("160255"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->ZZ(4l)
  // //  BGFileNames[Higgs]->push_back("160305"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->ZZ(4l)
  // //  BGFileNames[Higgs]->push_back("160505"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->ZZ(llqq)
  // //  BGFileNames[Higgs]->push_back("160555"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->ZZ(llqq)
  // //  BGFileNames[Higgs]->push_back("XXXXXX"); BGIncludeFlag[Higgs]->push_back("01111");//WH:H->tautau(ll) ???
  // //  BGFileNames[Higgs]->push_back("161675"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->tautau(ll)
  // //  BGFileNames[Higgs]->push_back("161708"); BGIncludeFlag[Higgs]->push_back("01111");//ttH:H->tautau(ll)
  // //  BGFileNames[Higgs]->push_back("161686"); BGIncludeFlag[Higgs]->push_back("01111");//ZH:H->tautau(lhad)
  // //  BGFileNames[Higgs]->push_back("161719"); BGIncludeFlag[Higgs]->push_back("01111");//ttH:H->tautau(lhad)
  // //  BGFileNames[Higgs]->push_back("XXXXXX"); BGIncludeFlag[Higgs]->push_back("00011");//WH:H->mumu ???
  // //  BGFileNames[Higgs]->push_back("167418"); BGIncludeFlag[Higgs]->push_back("00111");//ZH:H->mumu

  //ttbar
  BGFileNames[ttbar]->push_back("110401"); BGIncludeFlag[ttbar]->push_back("01111");//no all-had

  //SingleTop
  // BGFileNames[SingleTop]->push_back("110302"); BGIncludeFlag[SingleTop]->push_back("01111");//s-chan
  // BGFileNames[SingleTop]->push_back("110305"); BGIncludeFlag[SingleTop]->push_back("01111");//Wt

  //Zjets
  // BGFileNames[Zjets]->push_back("147406"); BGIncludeFlag[Zjets]->push_back("01100");//Zee
  // BGFileNames[Zjets]->push_back("147407"); BGIncludeFlag[Zjets]->push_back("00011");//Zmumu
  // BGFileNames[Zjets]->push_back("147408"); BGIncludeFlag[Zjets]->push_back("01111");//Ztautau

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

Int_t mkFRMeas(TString Tag, TString SelecReg){

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

  const Int_t nDistType = (DistTypeNames->size())/2;

  TString filepath_prefix = "$ROOTCOREBIN/../result/";

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
  TH1::SetDefaultSumw2(); //No need to call this for ROOT6?
  TH1F* dist_bg[nFSType][nBGType][nDistType][2]; // 0: signal 1: baseline 
  TH1F* dist_totalbg[nFSType][nDistType][2];
  TH1F* dist_totalbgErr[nFSType][nDistType][2];
  TH1F* dist_ratio[nFSType][nDistType][2];
  TH1F* dist_ratioErr[nFSType][nDistType][2];
  std::vector<TH1F*> *vec_dist_signal[nFSType][nSignalType][nDistType][2];
  //  std::cout<<vec_mcfiles->at(0)->GetName()<<std::endl;
  for(Int_t fstype=0; fstype<nFSType; fstype++){
    for(Int_t disttype=0; disttype<nDistType; disttype++){
      for (Int_t sorb=0; sorb<2; sorb++){
        Int_t disttype_sorb = 2*disttype+sorb;
        std::string distname = DistTypeNames->at(disttype_sorb).Data();
        TFile *f_tmp = vec_signalfiles->at(0);
        TH1F *h_tmp = (TH1F*)(f_tmp->Get(Form("all_%s;1",distname.c_str())));
        //Total BG
        std::cout<<"Preparing final histogram of total BG for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
        dist_totalbg[fstype][disttype][sorb] = (TH1F*)(h_tmp->Clone(Form("dist_totalbg_%d_%d_%d",fstype,disttype,sorb)));
        dist_totalbg[fstype][disttype][sorb]->Reset();
        dist_totalbg[fstype][disttype][sorb]->Sumw2();
        //Backgrounds
        for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
          std::cout<<"Preparing final histogram of "<<BGTypeNames[bgtype]<<" for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
          dist_bg[fstype][bgtype][disttype][sorb] = (TH1F*)(h_tmp->Clone(Form("dist_bg_%d_%d_%d_%d",fstype,bgtype,disttype,sorb)));
          dist_bg[fstype][bgtype][disttype][sorb]->Reset();
          dist_bg[fstype][bgtype][disttype][sorb]->Sumw2();
        }
        //Signal sample
        for(Int_t signaltype=0; signaltype<nSignalType; signaltype++){
          vec_dist_signal[fstype][signaltype][disttype][sorb] = new std::vector<TH1F*>();
          std::cout<<"Preparing histograms of "<<SignalTypeNames[signaltype]<<" for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
          for(UInt_t signalfile=0; signalfile<SignalFileNames[signaltype]->size(); signalfile++){
            vec_dist_signal[fstype][signaltype][disttype][sorb]->push_back((TH1F*)(h_tmp->Clone(Form("dist_signal_%d_%d_%d_%d",fstype,signaltype,disttype,sorb))));
            vec_dist_signal[fstype][signaltype][disttype][sorb]->at(signalfile)->Reset();
            vec_dist_signal[fstype][signaltype][disttype][sorb]->at(signalfile)->Sumw2();
          }
        }
      }
    }
  }

  //Adding histograms of BG for eee,eem,emm,mmm.
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
            for(Int_t sorb=0; sorb<2; sorb++){
              Int_t disttype_sorb = 2*disttype+sorb;
              //Getting the histogram to add.
              std::string distname = DistTypeNames->at(disttype_sorb).Data();
              std::string histname = Form("%s_%s;1",fsname.c_str(),distname.c_str());
              std::cout<<"Accessing "<<histname<<" : "<<distname<<std::endl;
              TH1F *h_tmp = (TH1F*)(f_tmp->Get(histname.c_str()));
              if(h_tmp->Integral()<0.){
                std::cout<<"FileIdx : "<<fileidx<<", "<<h_tmp->Integral()<<std::endl;
                //              getchar();
              }
              dist_bg[fstype][bgtype][disttype][sorb]->Add(h_tmp,scale);
              dist_bg[fstype][bgtype][disttype][sorb]->SetTitle(FSTypeNames[fstype]+":"+BGTypeNames[bgtype]);
              dist_bg[fstype][bgtype][disttype][sorb]->SetMarkerColor(BGTypeHistColors[bgtype]);
              dist_bg[fstype][bgtype][disttype][sorb]->SetFillColor(BGTypeFillColors[bgtype]);
              dist_bg[fstype][bgtype][disttype][sorb]->SetLineColor(BGTypeFillColors[bgtype]);
              std::cout<<"#Entries : "<<h_tmp->GetName()<<" : "<<h_tmp->GetEntries()<<std::endl;
            }
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
          for(Int_t sorb=0; sorb<2; sorb++){
            Int_t disttype_sorb = 2*disttype+sorb;
            //Getting the histogram to add.
            std::string distname = DistTypeNames->at(disttype_sorb).Data();;
            std::string histname = Form("%s_%s;1",fsname.c_str(),distname.c_str());
            TH1F *h_tmp = (TH1F*)(f_tmp->Get(histname.c_str()));
            std::cout<<"Accessing "<<histname<<" : "<<distname<<std::endl;
            vec_dist_signal[fstype][signaltype][disttype][sorb]->at(signalfile)->Add(h_tmp,scale);
            vec_dist_signal[fstype][signaltype][disttype][sorb]->at(signalfile)->SetTitle(FSTypeNames[fstype]+":"+SignalTypeNames[signaltype]);
            Int_t color = kBlack;
            Int_t style = kSolid;
            if(SignalTypeNames[signaltype]=="SmplViaWZ"){
              color = SmplViaWZHistColors[signalfile];
              style = SmplViaWZLineStyle [signalfile];
            }
            vec_dist_signal[fstype][signaltype][disttype][sorb]->at(signalfile)->SetMarkerColor(color);
            vec_dist_signal[fstype][signaltype][disttype][sorb]->at(signalfile)->SetLineColor(color);
            vec_dist_signal[fstype][signaltype][disttype][sorb]->at(signalfile)->SetLineStyle(style);
            std::cout<<"#Entries : "<<h_tmp->GetName()<<" : "<<h_tmp->GetEntries()<<std::endl;
          }
        }
      }
      fileidx++;
    }
  }

  //Making histograms for all channels (eee,eem,emm,mmm).
  for(Int_t disttype=0; disttype<nDistType; disttype++){
    for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
      for(Int_t sorb=0; sorb<2; sorb++){
        dist_bg[allchan][bgtype][disttype][sorb]->Add(dist_bg[eee][bgtype][disttype][sorb]);
        dist_bg[allchan][bgtype][disttype][sorb]->Add(dist_bg[eem][bgtype][disttype][sorb]);
        dist_bg[allchan][bgtype][disttype][sorb]->Add(dist_bg[emm][bgtype][disttype][sorb]);
        dist_bg[allchan][bgtype][disttype][sorb]->Add(dist_bg[mmm][bgtype][disttype][sorb]);
        dist_bg[allchan][bgtype][disttype][sorb]->SetTitle(FSTypeNames[allchan]+":"+BGTypeNames[bgtype]);
        dist_bg[allchan][bgtype][disttype][sorb]->SetMarkerColor(BGTypeHistColors[bgtype]);
        dist_bg[allchan][bgtype][disttype][sorb]->SetLineColor(BGTypeFillColors[bgtype]);
        dist_bg[allchan][bgtype][disttype][sorb]->SetFillColor(BGTypeFillColors[bgtype]);
      }
    }
  }

  //Making plots for total BG
  THStack *hs_bg[nFSType][nDistType][2];
  for(Int_t fstype=0; fstype<nFSType; fstype++){
    for(Int_t disttype=0; disttype<nDistType; disttype++){
      for(Int_t sorb=0; sorb<2; sorb++){
        Int_t disttype_sorb = 2*disttype+sorb;
        //for the shape
        for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
          dist_totalbg[fstype][disttype][sorb]->Add(dist_bg[fstype][bgtype][disttype][sorb]);
        }
        dist_totalbg[fstype][disttype][sorb]->SetTitle(FSTypeNames[fstype]+":Total");
        dist_totalbg[fstype][disttype][sorb]->SetLineColor(kRed);
        //for the error band
        dist_totalbgErr[fstype][disttype][sorb] = (TH1F*)dist_totalbg[fstype][disttype][sorb]->Clone(Form("dist_totalbgErr_%d_%d_%d",fstype,disttype,sorb));
        dist_totalbgErr[fstype][disttype][sorb]->SetTitle(FSTypeNames[fstype]+":Total");
        dist_totalbgErr[fstype][disttype][sorb]->SetLineColor(kBlack);
        dist_totalbgErr[fstype][disttype][sorb]->SetFillColor(kGray+3);
        dist_totalbgErr[fstype][disttype][sorb]->SetMarkerStyle(0);
        dist_totalbgErr[fstype][disttype][sorb]->SetFillStyle(3454);
        //Creating SM stuck histogram
        std::string distname = DistTypeNames->at(disttype_sorb).Data();
        std::cout<<"Preparing THStack for "<<FSTypeNames[fstype]<<" based on "<<Form("all_%s;1",distname.c_str())<<"."<<std::endl;
        hs_bg[fstype][disttype][sorb] = new THStack(Form("hs_bg_%d_%d_%d",fstype,disttype,sorb),FSTypeNames[fstype]+":Total");
        for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
          std::cout<<"Adding "<<BGTypeNames[BGStackOrder2[bgtype]]<<" to THStack..."<<std::endl;
          hs_bg[fstype][disttype][sorb]->Add(dist_bg[fstype][BGStackOrder2[bgtype]][disttype][sorb]);
          //std::cout<<"Adding "<<BGTypeNames[bgtype]<<" to THStack..."<<std::endl;
          //hs_bg[fstype][disttype]->Add(dist_bg[fstype][bgtype][disttype]);
        }
      }
    }
  }


  //Preparing the legend
  TLegend *leg = new TLegend(0.1,0.1,0.9,0.9);
  leg->SetNColumns(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextFont(72);
  leg->SetTextSize(0.05);
  // leg->AddEntry(dist_data[0][0],"Data","lp");
  leg->AddEntry(dist_bg[0][WZ][0][0],"WZ","f");
  leg->AddEntry(dist_bg[0][ZZ][0][0],"ZZ","f");
  leg->AddEntry(dist_bg[0][ttV][0][0],"t#bar{t}V","f");
  // leg->AddEntry(dist_bg[0][Tribosons][0],"Tribosons","f");
  //For Higgs histograms, special treatment is needed to make it visible.
  TH1F* h_higgs4leg = (TH1F*)dist_bg[0][Higgs][0][0]->Clone("h_higgs4leg");
  h_higgs4leg->SetLineColor(kBlack);
  leg->AddEntry(h_higgs4leg,"Higgs","f");
  // leg->AddEntry(dist_bg[0][Wjets][0],"W+jets","f");
  leg->AddEntry(dist_bg[0][ttbar][0][0],"t#bar{t}","f");
  leg->AddEntry(dist_bg[0][SingleTop][0][0],"Single top","f");
  leg->AddEntry(dist_bg[0][Zjets][0][0],"Z/#gamma*+jets","f");
  // leg->AddEntry(dist_bg[0][OtherFake][0],"Other BG with fakes","f");
  leg->AddEntry(dist_totalbg[0][0][0],"Total SM","l");
  leg->AddEntry(dist_totalbgErr[0][0][0],"Unct. for SM ","f");
  const Int_t nDrawnSignal = DrawnSignalType->size();

  ////////////////////////////////////////////////////
  // histograms definition end.
  ////////////////////////////////////////////////////

  Bool_t debugPlot = kTRUE;
  TUtil *u  = new TUtil(("plots/"+Tag+"."+SelecReg+".ps").Data(),("plots/"+Tag+"."+SelecReg+".root").Data(),debugPlot);
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

  // u->SetPadNumXY(3,4);
  // for(Int_t disttype=0; disttype<nDistType; disttype++){
  //   for(Int_t sorb=0; sorb<2; sorb++){
  //     u->cdPad();
  //     for(Int_t bgtype=0; bgtype<nBGType; bgtype++){
  //       dist_bg[allchan][bgtype][disttype][sorb]->DrawNormalized(bgtype==0?"":"same");
  //     }
  //   }
  // }

  u->SetPadNumXY(1,3);
  //  u->SetPadNumXY(3,4);
  for(Int_t fstype=0; fstype<nFSType; fstype++){
    for(Int_t disttype=0; disttype<nDistType; disttype++){
      for(Int_t sorb=0; sorb<2; sorb++){
        Int_t disttype_sorb = 2*disttype+sorb;
        //      u->Draw(dist_totalbgErr[fstype][disttype],"E2",010);
        //      u->Draw(dist_totalbg   [fstype][disttype],"samehist"); //should be drawn after the error histogram.
        //      dist_data[fstype][disttype]->Draw("sameE");
        //      Double_t logMax = TMath::Max(dist_totalbg[fstype][disttype]->GetMaximum(),dist_data[fstype][disttype]->GetMaximum());
        //      Double_t linMax = TMath::Max(dist_totalbg[fstype][disttype]->GetMaximum(),dist_data[fstype][disttype]->GetMaximum());
        Double_t logMax = TMath::Max(dist_totalbg[fstype][disttype][sorb]->GetMaximum(),-1.);
        Double_t linMax = TMath::Max(dist_totalbg[fstype][disttype][sorb]->GetMaximum(),-1.);
        logMax *= 1.3E0*TMath::Log10(logMax);
        linMax *= 1.3E0;
        if(logMax<=1.) logMax=2.;

        Double_t entry = dist_totalbg[fstype][disttype][sorb]->GetEntries();
        Double_t integral = dist_totalbg[fstype][disttype][sorb]->Integral();
        if(fstype==0 && DistTypeNames->at(disttype_sorb)=="nJet"){
          std::cout<<"*********************************************"<<std::endl;
          std::cout<<"Total BG : "<<integral<<std::endl;
          std::cout<<"*********************************************"<<std::endl;
        }
        if(entry==0.){ //to avoid error in PostScript
          u->Draw(dist_totalbg[fstype][disttype][sorb],"hist");
          u->Draw(dist_totalbg[fstype][disttype][sorb],"hist",010);
        }else{
          //////////////////////////////////////////////////////////////
          // Linear version
          //////////////////////////////////////////////////////////////
          u->cdPad();
          //should be drawn in this order
          hs_bg[fstype][disttype][sorb]->SetMaximum(linMax);
          hs_bg[fstype][disttype][sorb]->Draw("hist");
          hs_bg[fstype][disttype][sorb]->GetXaxis()->SetTitle(dist_bg[fstype][0][disttype][sorb]->GetXaxis()->GetTitle());
          hs_bg[fstype][disttype][sorb]->GetYaxis()->SetTitle(dist_bg[fstype][0][disttype][sorb]->GetYaxis()->GetTitle());
          u->Draw(dist_totalbg[fstype][disttype][sorb],"samehist"); //should be drawn after the error histogram.
          // dist_data[fstype][disttype]->Draw("sameE");
          u->Draw(dist_totalbgErr[fstype][disttype][sorb],"sameE2");
          TH1F *h_sig[nDrawnSignal];
          for(Int_t cnt=0; cnt<nDrawnSignal; cnt++){
            h_sig[cnt] = vec_dist_signal[fstype][DrawnSignalType->at(cnt)][disttype][sorb]->at(DrawnSignalFile->at(cnt));
            h_sig[cnt]->SetLineColor(DrawnSignalColor->at(cnt));
            h_sig[cnt]->SetMarkerColor(DrawnSignalColor->at(cnt));
            h_sig[cnt]->SetFillColor(0);
            h_sig[cnt]->SetLineStyle(DrawnSignalStyle->at(cnt));
            h_sig[cnt]->Draw("samehist");
            if(fstype==0&&disttype==0) leg->AddEntry(h_sig[cnt],DrawnSignalLegend->at(cnt).c_str(),"l");
          }
          //////////////////////////////////////////////////////////////
          // Log version
          //////////////////////////////////////////////////////////////
          u->cdPad();
          //should be drawn in this order
          hs_bg[fstype][disttype][sorb]->SetMaximum(logMax);
          hs_bg[fstype][disttype][sorb]->SetMinimum(1.E-2);
          hs_bg[fstype][disttype][sorb]->Draw("hist");
          u->Draw(dist_totalbg[fstype][disttype][sorb],"samehist");
          // dist_data[fstype][disttype]->Draw("sameE");
          u->Draw(dist_totalbgErr[fstype][disttype][sorb],"sameE2");
          for(Int_t cnt=0; cnt<nDrawnSignal; cnt++) h_sig[cnt]->Draw("samehist");
          gPad->SetLogy();
        }
        //Ratio plot
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
  }

  // Plotting  fake rate
  // 0: singal 1: baseline
  u->SetPadNumXY(2,3);
  TH1F* fakerate_bg[nFSType][nDistType];
  TH1F* fakerate_bgErr[nFSType][nDistType];
  TH1F* hist_tmp0[nFSType][nDistType];
  TH1F* hist_tmp1[nFSType][nDistType];
  TH1F* hist_tmp0Err[nFSType][nDistType];
  TH1F* hist_tmp1Err[nFSType][nDistType];
  for(Int_t fstype=0; fstype<nFSType; fstype++){
    for(Int_t disttype=0; disttype<nDistType; disttype++){
      dist_totalbg[fstype][disttype][0]->Sumw2();
      dist_totalbg[fstype][disttype][1]->Sumw2();
      fakerate_bg[fstype][disttype] = (TH1F*)dist_totalbg[fstype][disttype][0]->Clone(Form("fakerate_bg_%d_%d",fstype,disttype));
      fakerate_bgErr[fstype][disttype] = (TH1F*)dist_totalbgErr[fstype][disttype][0]->Clone(Form("fakerate_bg_error_%d_%d",fstype,disttype));
      hist_tmp0[fstype][disttype] = (TH1F*)dist_totalbg[fstype][disttype][0]->Clone(Form("hist_tmp0_%d_%d",fstype,disttype));
      hist_tmp1[fstype][disttype] = (TH1F*)dist_totalbg[fstype][disttype][1]->Clone(Form("hist_tmp1_%d_%d",fstype,disttype));
      hist_tmp0Err[fstype][disttype] = (TH1F*)dist_totalbgErr[fstype][disttype][0]->Clone(Form("hist_tmp0_error_%d_%d",fstype,disttype));
      hist_tmp1Err[fstype][disttype] = (TH1F*)dist_totalbgErr[fstype][disttype][1]->Clone(Form("hist_tmp1_error_%d_%d",fstype,disttype));
      // u->cdPad();
      fakerate_bg[fstype][disttype]->Divide(hist_tmp0[fstype][disttype],hist_tmp1[fstype][disttype],1,1,"B");
      fakerate_bg[fstype][disttype]->SetMarkerColor(kBlack);
      fakerate_bg[fstype][disttype]->SetLineColor(kBlack);
      fakerate_bg[fstype][disttype]->SetStats(1);
      fakerate_bgErr[fstype][disttype]->Divide(hist_tmp0Err[fstype][disttype],hist_tmp1Err[fstype][disttype],1,1,"B");
      fakerate_bg[fstype][disttype]->GetYaxis()->SetTitle("Ratio");
      if(disttype%2==0 ) u->Update();
      fakerate_bg[fstype][disttype]->GetYaxis()->SetRangeUser(0.0,1.0);
      u->Draw(fakerate_bg[fstype][disttype],"E");
      fakerate_bg[fstype][disttype]->GetYaxis()->UnZoom();
      u->Draw(fakerate_bg[fstype][disttype],"text");
      // u->Draw(fakerate_bgErr[fstype][disttype],"sameE2");


      // TH1F *fr_tmp = dist_totalbg[fstype][disttype][1]->Clone();
      // fr_tmp->SetName(Form("fakerate_dist_bg_%d_%d",fstype,disttype));
      // fr_tmp->Divide(dist_totalbg[fstype][disttype][0],dist_totalbg[fstype][disttype][1],1.0,1.0,"B");
      // fakerate_bg[fstype][disttype]->Draw("hist");
    }
  }

  //drawing legends again...
  u->SetPadNumXY(1,1);
  leg->Draw();

  delete vec_datafiles;
  delete vec_signalfiles;
  delete vec_mcfiles;
  delete u;

  return 0;
}
