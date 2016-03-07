#include "TUtil.h"
#include "AtlasStyle.cc"

std::string get_file_contents(const char *filename)
{
  std::ifstream fin(filename, std::ios_base::in);
  if (fin)
    {
      std::string s_contents;
      fin.seekg(0, std::ios::end);
      s_contents.resize(fin.tellg());
      fin.seekg(0, std::ios::beg);
      fin.read(&s_contents[0], s_contents.size());
      fin.close();
      return(s_contents);
      //how to delete s_contents?
    }
  std::cout << filename <<" cannot be read!" << std::endl;
  throw(errno);
}

// constructor
TUtil::TUtil(const char* psFile, const char* rootFile, const Int_t canv_x, const Int_t canv_y, const Int_t debug):m_debugPlot(debug),m_bps(0),m_bpdf(0),m_broot(0),m_padNum(0),m_padNumX(2),m_padNumY(3),m_colorTableNum(0),m_applyColorToFirstDraw(1){
  m_canv_x = canv_x;
  m_canv_y = canv_y;
  init_style();
  init_canvas();
  init_ps(psFile);
  init_ofile(rootFile);
  init_colortable();
  
  // for the odd behavior in interactive/macro mode (CINT bug??)
#ifdef __CINT__
  m_drawObj.push_back(new TObject);
  delete m_drawObj[0];
  m_drawObj.clear();
#endif // __CINT__
  
}

// other constructor
TUtil::TUtil(const char* psFile, const char* rootFile, const Bool_t debug):m_debugPlot(debug),m_bps(0),m_bpdf(0),m_broot(0),m_padNum(0),m_padNumX(2),m_padNumY(3),m_colorTableNum(0),m_applyColorToFirstDraw(1){

  m_canv_x = 640;
  m_canv_y = 856;
  init_style();
  init_canvas();
  init_ps(psFile);
  init_ofile(rootFile);
  init_colortable();
  
  // for the odd behavior in interactive/macro mode (CINT bug??)
#ifdef __CINT__
  m_drawObj.push_back(new TObject);
  delete m_drawObj[0];
  m_drawObj.clear();
#endif // __CINT__

}

// destructor
TUtil::~TUtil(void){
  if ( m_bps ){ m_ps->Close(); }
  if ( m_bpdf ){ m_pdf->Close(); }
  for ( int i=0;i<(int)m_drawObj.size();i++ ){ delete m_drawObj[i]; }
  if ( m_broot ){ m_f->Write(); m_f->Clear(); }
  if ( m_bps ){ delete m_ps; }
  if ( m_bpdf ){ delete m_pdf; }
  if ( m_broot ){ delete m_f; }
  delete m_c;
}

void TUtil::CdPad(void){
  if ( m_padNum==m_padNumX*m_padNumY ){
    Update();
    m_padNum=1;
  }else{
    if ( m_padNum!=0 ){
      gPad->Update();
      //m_c->Update();
    }
    
    m_c->cd(++m_padNum);
  }
  m_colorTableNum=0;
}

void TUtil::Update(int padNum){
  if ( !(1<=padNum && padNum<=m_padNumX*m_padNumY) ){
    std::cerr << "[Error] TUtil::Update >> padNum is out of range, ignored." << std::endl;
    return;
  }
  gPad->Update();
  //m_c->Update();
  
  for ( int i=0;i<(int)m_drawObj.size();i++ ){ delete m_drawObj[i]; }
  m_drawObj.clear();
  
  if ( m_padNum!=0 ){
    if ( m_bps ){ m_ps->NewPage(); }
    else if ( m_bpdf ){ m_c->cd(0); m_pdf->NewPage(); }
  }
  for ( int i=0;i<m_padNumX*m_padNumY;i++ ){
    m_c->cd(i+1);
    gPad->SetLogx(0);
    gPad->SetLogy(0);
    gPad->SetLogz(0);
    gPad->Clear();
  }
  m_c->Update();
  m_c->cd(padNum);
  m_padNum=padNum-1;
}

// init
void TUtil::init_canvas(void){
  m_c=new TCanvas("c","ROOT canvas",0,0,m_canv_x,m_canv_y);
  m_c->Divide(m_padNumX, m_padNumY, 0.000001, 0.000001);
  
  int i=1;
  while( i<=m_padNumX*m_padNumY ){
    m_c->cd(i);
    //    gPad->SetGridx();
    //    gPad->SetGridy();
    i++;
  }
  m_c->cd(1);
}

void TUtil::init_ps(const char* psFile){
  m_psFileName.Append(psFile);
  if ( m_psFileName.EndsWith(".pdf",TString::kIgnoreCase) ){
    m_bpdf=kTRUE;
  }else if( m_psFileName.Length()!=0 ){
    if ( !m_psFileName.EndsWith(".ps",TString::kIgnoreCase) ){
      std::cerr << "[Warning] TUtil::init_ps >> no valid extensions found in 'psFile', assume a PostScript file." << std::endl;
      m_psFileName.Append(".ps");
    }
    m_bps=kTRUE;
  }
  
  if ( ( m_bps || m_bpdf) && !gSystem->AccessPathName(m_psFileName) ){
    gSystem->Rename(m_psFileName,m_psFileName+"~");
  }
  if ( m_bps ){
    m_ps=new TPostScript(m_psFileName,111);
    //m_ps->Range(20,40);
    m_ps->NewPage();
  }else if ( m_bpdf ){
    m_c->cd(0);
    if ( 0 ){
      m_pdf=new TPDF(m_psFileName);
    }else{ // debug
      m_pdf=new TPDF;
      m_pdf->Range(21.0,29.7); // [cm] : A4
      m_pdf->Open(m_psFileName);
    }
    m_c->cd(1);
  }
}

void TUtil::init_ofile(const char* rootFile){
  m_rootFileName.Append(rootFile);
  if ( m_rootFileName.Length()!=0 ){
    if ( !m_rootFileName.EndsWith(".root",TString::kIgnoreCase) ){
      m_rootFileName.Append(".root");
      std::cerr << "[Warning] TUtil::init_ofile >> no valid extensions found in 'rootFile', assume a ROOT file." << std::endl;
    }
    m_broot=kTRUE;
  }
  
  if ( m_broot ){
    if ( !gSystem->AccessPathName(m_rootFileName) ){
      gSystem->Rename(m_rootFileName,m_rootFileName+"~");
    }
    m_f=new TFile(m_rootFileName,"RECREATE");
  }
}

void TUtil::init_colortable(void){
  m_colorTable.push_back(kBlack);
  m_colorTable.push_back(kRed);
  m_colorTable.push_back(kBlue);
  m_colorTable.push_back(kMagenta);
  
  m_colorTable.push_back(kSpring-1);
  m_colorTable.push_back(kAzure-3);
  m_colorTable.push_back(kOrange+1);
  
  m_styleTable.push_back(1);
  m_styleTable.push_back(2);
}

void TUtil::init_style(void){

  SetAtlasStyle();
  
  //   gStyle->SetStatColor(0);
  //   gStyle->SetTitleFillColor(0);
  //   gStyle->SetTitleH(0.05);
  //   gStyle->SetTitleBorderSize(1);
  //   gStyle->SetStatX(0.94);
  //   gStyle->SetStatY(0.98);
  //   gStyle->SetStatFontSize(0.023);
  //   gStyle->SetStatBorderSize(1);
  //   //  gStyle->SetStatStyle(0);
  //   gStyle->SetFrameFillColor(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetLegendBorderSize(1);
  //   gStyle->SetCanvasColor(0);
  gStyle->SetPadTopMargin(0.12);
  //   gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  //   gStyle->SetLabelSize(0.05,"X");
  //   gStyle->SetLabelSize(0.05,"Y");
  //   gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(0.7);
  //   gStyle->SetFuncWidth(1);
  //   gStyle->SetFuncColor(2);
  //   gStyle->SetEndErrorSize(0);
  //   gStyle->SetNdivisions(505);
  //   gStyle->SetPalette(1);

  if(m_debugPlot){
    gStyle->SetOptTitle(1);
    gStyle->SetOptStat("eMinoru");
    gStyle->SetOptFit(1111);
  }
  //   gStyle->SetStatW(0.3);
  //   gStyle->SetStatFontSize(0.03);
}

// Draw
void TUtil::skeletonDraw(void* obj, const char* option, int logScale, int drawType, void* fitFunc, const char* fitOption, double xmin, double xmax){
  
  if ( obj==NULL ){
    std::cerr << "[Error] TUtil::Draw(...) >> Invalid argument. The pointer to a draw object is NULL, ignored." << std::endl;
    return;
  }
  
  TString t_opt=option;
  bool b_same=kFALSE;
  TString t_classname=((TObject*)obj)->ClassName();
  if ( t_classname.Contains("TGraph") ){
    if ( !t_opt.Contains("A",TString::kIgnoreCase) ){ b_same=kTRUE; }
    if ( b_same && gPad->GetListOfPrimitives()->GetEntries()==0 ){
      t_opt.Append("A");
      b_same=kFALSE;
      std::cerr << "[Error] TUtil::Draw(...) >> Missing option 'A', automatically added." << std::endl;
    }
  }else if ( t_classname.Contains("TH") || t_classname.Contains("TProfile") ){
    if ( t_opt.Contains("same",TString::kIgnoreCase) ){ b_same=kTRUE; }
    if ( b_same && gPad->GetListOfPrimitives()->GetEntries()==0 ){
      if ( t_opt.Contains("sames",TString::kIgnoreCase) ){
        t_opt.Remove(t_opt.Index("sames",0,TString::kIgnoreCase),5);
      }else{
        t_opt.Remove(t_opt.Index("same",0,TString::kIgnoreCase),4);
      }
      b_same=kFALSE;
      std::cerr << "[Error] TUtil::Draw(...) >> Invalid option 'SAME(S)', ignored." << std::endl;
    }
  }
  
  if ( !b_same ){
    CdPad();
    bool b_log[3];
    b_log[2]=logScale%2;
    b_log[1]=(logScale%100>2) ? 1 : 0;
    b_log[0]=(logScale>20) ? 1 : 0;
    gPad->SetLogx(b_log[0]);
    gPad->SetLogy(b_log[1]);
    gPad->SetLogz(b_log[2]);
  }
  
  std::string t_fitOption=fitOption;
  switch(drawType){
  case 0: // Draw
    break;
  case 1: // Fit(char*, ...)
    ((TH1*)obj)->Fit((char*)fitFunc,(t_fitOption+'0').c_str(),"",xmin,xmax);
    ((TH1*)obj)->GetFunction((const char*)fitFunc)->ResetBit(1<<9);
    break;
  case 2: // Fit(TF1*, ...)
    ((TH1*)obj)->Fit((TF1*)fitFunc,(t_fitOption+'0').c_str(),"",xmin,xmax);
    ((TH1*)obj)->GetFunction(((TF1*)fitFunc)->GetName())->ResetBit(1<<9);
    break;
  case 3: // DrawNormalized
    if ( ((TH1*)obj)->GetSumOfWeights()==0 ){
      std::cerr << "[Error] TUtil::skeletonDraw >> Sum of weights is null, ignored." << std::endl;
      return;
    }
    //((TH1*)obj)->Sumw2();
    ((TH1*)obj)->Scale(1./((TH1*)obj)->GetSumOfWeights());
    break;
  default:
    std::cerr << "[Error] TUtil::skeletonDraw >> Invalid drawType, ignored." << std::endl;
    return;
  }
  
  TAttLine* t_line=0;
  TAttMarker* t_marker=0;
  void* drawObj=0;
  if ( t_classname.Contains("TGraph") ){
    drawObj=((TGraph*)obj)->Clone();
    m_drawObj.push_back((TGraph*)drawObj);
    t_line=(TGraph*)drawObj;
    t_marker=(TGraph*)drawObj;
  }else if ( t_classname.Contains("TH") || t_classname.Contains("TProfile") ){
    drawObj=((TH1*)obj)->Clone();
    m_drawObj.push_back((TH1*)drawObj);
    t_line=(TH1*)drawObj;
    t_marker=(TH1*)drawObj;
  }else{
    std::cerr << "[Error] TUtil::skeletonDraw >> Unknown object type, ignored." << std::endl;
    return;
  }
  ((TNamed*)drawObj)->Draw(t_opt);
  
  if ( b_same ){
    if ( m_colorTableNum==0 && m_applyColorToFirstDraw && m_drawObj.size()>=2 ){
      TAttLine* t_firstDrawLine=0;
      TAttMarker* t_firstDrawMarker=0;
      TString t_firstDrawClassName=m_drawObj[m_drawObj.size()-2]->ClassName();
      if ( t_firstDrawClassName.Contains("TGraph") ){
        t_firstDrawLine=(TGraph*)m_drawObj[m_drawObj.size()-2];
        t_firstDrawMarker=(TGraph*)m_drawObj[m_drawObj.size()-2];
      }else if ( t_firstDrawClassName.Contains("TH") || t_firstDrawClassName.Contains("TProfile") ){
        t_firstDrawLine=(TH1*)m_drawObj[m_drawObj.size()-2];
        t_firstDrawMarker=(TH1*)m_drawObj[m_drawObj.size()-2];
      }
      t_firstDrawLine->SetLineColor(m_colorTable[0]);
      t_firstDrawMarker->SetMarkerColor(m_colorTable[0]);
    }
    m_colorTableNum++;
    if ( m_colorTableNum>=(int)m_colorTable.size()*(int)m_styleTable.size() ){ m_colorTableNum=0; }
    if ( m_colorTableNum<(int)m_colorTable.size() ){
      t_line->SetLineColor(m_colorTable[m_colorTableNum]);
      t_marker->SetMarkerColor(m_colorTable[m_colorTableNum]);
    }else{
      t_line->SetLineStyle(m_styleTable[m_colorTableNum/m_colorTable.size()]);
      t_line->SetLineColor(m_colorTable[m_colorTableNum%m_colorTable.size()]);
      t_marker->SetMarkerColor(m_colorTable[m_colorTableNum%m_colorTable.size()]);
    }
  }
  
  //if ( !b_same ){ m_c->Update(); }
}

TH1* TUtil::Draw(TTree* tree, const char* varexp, const char* selection, const char* option, int logScale){
  
  TString t_varexp=varexp;
  std::string t_histname="htemp";
  if ( t_varexp.Contains(">>") ){
    t_varexp.Remove(0,t_varexp.Index(">>")+2);
    if ( t_varexp.Index("(")>=0 ){ t_varexp.Remove(t_varexp.Index("(")); }
    t_varexp=t_varexp.Strip(TString::kBoth);
    t_histname=t_varexp;
  }
  
  TString t_option=option;
  t_option.Append("goff");
  tree->Draw(varexp,selection,t_option);
  TH1* t_hist=(TH1*)(gROOT->FindObject(t_histname.c_str()));
  t_hist->SetName((" "+t_histname+" ").c_str());
  
  skeletonDraw(t_hist,option,logScale,0);
  
  return t_hist;
}

TGraph* TUtil::skeletonNewTGraph(int graphType, const char* name){
  TGraph* t_gr=NULL;
  switch(graphType){
  case 0: // TGraph
    t_gr=new TGraph;
    break;
  case 1: // TGraphErrors
    t_gr=new TGraphErrors;
    break;
  case 2: // TGraphAsymmErrors
    t_gr=new TGraphAsymmErrors;
    break;
  default:
    std::cerr << "[Error] TUtil::skeletonNewTGraph >> Invalid graphType, ignored." << std::endl;
  }
  
  gDirectory->Append(t_gr);
  if ( TString(name)=="defaultName" ){ t_gr->SetName("Graph"); }else{ t_gr->SetName(name); }
  
  return t_gr;
}

void TUtil::SetPadNumXY(int padNumX, int padNumY){
  Update();
  if ( m_bps ){ m_ps->Off(); }
  if ( m_bpdf ){ m_pdf->Off(); }
  delete m_c;
  m_padNumX=padNumX;
  m_padNumY=padNumY;
  init_canvas();
  if ( m_bps ){ m_ps->On(); }
  if ( m_bpdf ){ m_pdf->On(); }
}

void TUtil::ViewHists(TFile* f, const char* searchKey, TString::ECaseCompare cmp){
  TIter t_iter(f->GetListOfKeys());
  TKey* t_key;
  while ( (t_key=(TKey*)t_iter.Next()) ){
    TString t_classname=t_key->GetClassName();
    if ( !t_classname.Contains("TH") ){ continue; }
    TString t_name=t_key->GetName();
    if ( !t_name.Contains(searchKey,cmp) ){ continue; }
    TH1* t_hist=(TH1*)(gROOT->FindObject(t_name));
    Draw(t_hist);
  }
}

void TUtil::ReOpen(const char* mode){
  if ( !m_broot ){ std::cerr << "[Error] TUtil::ReOpen >> ROOT-file was not created, ignored." << std::endl; return; }
  m_f->ReOpen(mode);
}

