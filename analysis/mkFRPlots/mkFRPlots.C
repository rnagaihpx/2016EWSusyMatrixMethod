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
std::vector<TString> *DistBaseTypeNames;
std::vector<TString> *DistRatioTypeNames;
void SetDistType(void){
  DistTypeNames = new std::vector<TString>();       DistBaseTypeNames = new std::vector<TString>();         DistRatioTypeNames = new std::vector<TString>();
  DistTypeNames->push_back("PREl2and3Pt");          DistBaseTypeNames->push_back("PRbaseEl2and3Pt");        DistRatioTypeNames->push_back("PRRatioEl2and3Pt");
  DistTypeNames->push_back("PREl2and3Eta");         DistBaseTypeNames->push_back("PRbaseEl2and3Eta");       DistRatioTypeNames->push_back("PRRatioEl2and3Eta");
  DistTypeNames->push_back("COEl2and3Pt");          DistBaseTypeNames->push_back("CObaseEl2and3Pt") ;       DistRatioTypeNames->push_back("CORatioEl2and3Pt");
  DistTypeNames->push_back("COEl2and3Eta");         DistBaseTypeNames->push_back("CObaseEl2and3Eta");       DistRatioTypeNames->push_back("CORatioEl2and3Eta");
  DistTypeNames->push_back("HFEl2and3Pt");          DistBaseTypeNames->push_back("HFbaseEl2and3Pt") ;       DistRatioTypeNames->push_back("HFRatioEl2and3Pt");
  DistTypeNames->push_back("HFEl2and3Eta");         DistBaseTypeNames->push_back("HFbaseEl2and3Eta");       DistRatioTypeNames->push_back("HFRatioEl2and3Eta");
  DistTypeNames->push_back("LFEl2and3Pt");          DistBaseTypeNames->push_back("LFbaseEl2and3Pt") ;       DistRatioTypeNames->push_back("LFRatioEl2and3Pt");
  DistTypeNames->push_back("LFEl2and3Eta");         DistBaseTypeNames->push_back("LFbaseEl2and3Eta");       DistRatioTypeNames->push_back("LFRatioEl2and3Eta");
  DistTypeNames->push_back("UKEl2and3Pt");          DistBaseTypeNames->push_back("UKbaseEl2and3Pt") ;       DistRatioTypeNames->push_back("UKRatioEl2and3Pt");
  DistTypeNames->push_back("UKEl2and3Eta");         DistBaseTypeNames->push_back("UKbaseEl2and3Eta");       DistRatioTypeNames->push_back("UKRatioEl2and3Eta");
  DistTypeNames->push_back("fakeEl2and3Pt");        DistBaseTypeNames->push_back("fakeBaseEl2and3Pt") ;     DistRatioTypeNames->push_back("fakeRatioEl2and3Pt");
  DistTypeNames->push_back("fakeEl2and3Eta");       DistBaseTypeNames->push_back("fakeBaseEl2and3Eta");     DistRatioTypeNames->push_back("fakeRatioEl2and3Eta");
  DistTypeNames->push_back("PRMu2and3Pt");          DistBaseTypeNames->push_back("PRbaseMu2and3Pt") ;       DistRatioTypeNames->push_back("PRRatioMu2and3Pt");
  DistTypeNames->push_back("PRMu2and3Eta");         DistBaseTypeNames->push_back("PRbaseMu2and3Eta");       DistRatioTypeNames->push_back("PRRatioMu2and3Eta");
  DistTypeNames->push_back("COMu2and3Pt");          DistBaseTypeNames->push_back("CObaseMu2and3Pt") ;       DistRatioTypeNames->push_back("CORatioMu2and3Pt");
  DistTypeNames->push_back("COMu2and3Eta");         DistBaseTypeNames->push_back("CObaseMu2and3Eta");       DistRatioTypeNames->push_back("CORatioMu2and3Eta");
  DistTypeNames->push_back("HFMu2and3Pt");          DistBaseTypeNames->push_back("HFbaseMu2and3Pt") ;       DistRatioTypeNames->push_back("HFRatioMu2and3Pt");
  DistTypeNames->push_back("HFMu2and3Eta");         DistBaseTypeNames->push_back("HFbaseMu2and3Eta");       DistRatioTypeNames->push_back("HFRatioMu2and3Eta");
  DistTypeNames->push_back("LFMu2and3Pt");          DistBaseTypeNames->push_back("LFbaseMu2and3Pt") ;       DistRatioTypeNames->push_back("LFRatioMu2and3Pt");
  DistTypeNames->push_back("LFMu2and3Eta");         DistBaseTypeNames->push_back("LFbaseMu2and3Eta");       DistRatioTypeNames->push_back("LFRatioMu2and3Eta");
  DistTypeNames->push_back("UKMu2and3Pt");          DistBaseTypeNames->push_back("UKbaseMu2and3Pt") ;       DistRatioTypeNames->push_back("UKRatioMu2and3Pt");
  DistTypeNames->push_back("UKMu2and3Eta");         DistBaseTypeNames->push_back("UKbaseMu2and3Eta");       DistRatioTypeNames->push_back("UKRatioMu2and3Eta");
  DistTypeNames->push_back("fakeMu2and3Pt");        DistBaseTypeNames->push_back("fakeBaseMu2and3Pt");      DistRatioTypeNames->push_back("fakeRatioMu2and3Pt");
  DistTypeNames->push_back("fakeMu2and3Eta");       DistBaseTypeNames->push_back("fakeBaseMu2and3Eta");     DistRatioTypeNames->push_back("fakeRatioMu2and3Eta");

  DistTypeNames->push_back("fakeEl2and3Class");     DistBaseTypeNames->push_back("fakebaseEl2and3Class");   DistRatioTypeNames->push_back("fakeRatioEl2and3Class");
  DistTypeNames->push_back("fakeMu2and3Class");     DistBaseTypeNames->push_back("fakebaseMu2and3Class");   DistRatioTypeNames->push_back("fakeRatioMu2and3Class");
  DistTypeNames->push_back("sgnlfakelepClass");     DistBaseTypeNames->push_back("basefakelepClass");       DistRatioTypeNames->push_back("fakeRatioLep2and3Class");
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
  //  BGFileNames[ttbar]->push_back("110401"); BGIncludeFlag[ttbar]->push_back("01111");//no all-had
  BGFileNames[ttbar]->push_back("410000"); BGIncludeFlag[ttbar]->push_back("01111");//no all-had

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

Int_t mkFRPlots(TString Tag, TString SelecReg){

  std::cout<<"Making plots for the region \""<<SelecReg.Data()<<"\"."<<std::endl;

  std::vector<TFile*> *vec_datafiles   = new std::vector<TFile*>();
  std::vector<TFile*> *vec_mcfiles     = new std::vector<TFile*>();
  vec_datafiles  ->clear();
  vec_mcfiles    ->clear();

  SetDataType();
  SetBGType();
  SetDistType();

  const Int_t nDistType      = DistTypeNames->size();

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

  ////////////////////////////////////////////////////
  //Defining histograms
  ////////////////////////////////////////////////////
  TH1::SetDefaultSumw2(); //No need to call this for ROOT6?

  Bool_t debugPlot = kTRUE;
  TUtil *u  = new TUtil(("plots/"+Tag+"."+SelecReg+".ps").Data(),("plots/"+Tag+"."+SelecReg+".root").Data(),debugPlot);
  gStyle->SetPalette(1);
  
  TH1F* dist_ratio[nDistType];
  for(Int_t disttype=0; disttype<nDistType; disttype++){
    std::string distname     = DistTypeNames    ->at(disttype).Data();
    std::string distbasename = DistBaseTypeNames->at(disttype).Data();
    std::string distrationame = DistRatioTypeNames->at(disttype).Data();
    TFile *f_tmp = vec_mcfiles->at(0);
    TH1F *s_tmp = (TH1F*)(f_tmp->Get(Form("all_%s;1",distname.c_str(),disttype)));
    TH1F *b_tmp = (TH1F*)(f_tmp->Get(Form("all_%s;1",distbasename.c_str(),disttype)));
    dist_ratio[disttype] = (TH1F*)(s_tmp->Clone(Form("%s",distrationame.c_str())));
    dist_ratio[disttype]->Divide(s_tmp,b_tmp,1,1,"B");
    dist_ratio[disttype]->GetYaxis()->SetTitle("Ratio");
    u->Draw(dist_ratio[disttype],"text");
    s_tmp->Reset();
    s_tmp->Sumw2();
    b_tmp->Reset();
    b_tmp->Sumw2();
  }

  u->cdPad();

  delete vec_datafiles;
  delete vec_mcfiles;
  delete u;

  return 0;
}
