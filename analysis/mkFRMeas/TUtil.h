/* *********************************************************

class TUtil : Utility class for ROOT
  author    : Eito IWAI ( iwai # champ.hep.sci.osaka-u.ac.jp )

history :
   v0.5.0   support PDF file output
   v0.4.2   enable to deal with the recent g++ compiler
   v0.4.1   add TUtil::DrawNormalized method
   v0.4.0   enable to choose whether making output files in default constructor
   v0.3.3   add TUtil::SetStyleTable method
            replace some char* to const char*
   v0.3.2   add TUtil::ReOpen() method to activate the output ROOT file
            add gStyle->SetFrameFillColor(0) to init_style().
   v0.3.1   add some handlings in the constructor for the odd behavior with CINT
   v0.3.0   enable to treat TProfile with TUtil::Draw
            remove m_drawObj.clear() in destructor
            change TUtil::(Get)Version() method to a static function
            add TMath.h to include header file
   v0.2.7   check the validity of the superimpose option
            a overlaid color processing is modified
   v0.2.6   add TUtil::NewTGraphAsymmErrors method
            change the timing of deleting clone objects in destructor
   v0.2.5   rename the output file ( *.ps, *.root ), if already exists
   v0.2.4   delete clone objects in destructor
   v0.2.3   add a flag m_applyColorToFirstDraw and also its setter method
            update the color processes on overlaid drawing
            check the pointer to an drawed object in skeletonDraw
   v0.2.2   fix a bug with TUtil::Fit( ..., TF1* ...)
   v0.2.1   futher more update draw methods, especially TUtil::Fit
            update SetPadNumXY
   v0.2.0   change the treatment of draw objects ( make clone )
            change the specification & usage for TUtil::Draw(TTree... )
            fix a bug with m_colorTable
   v0.1.7   add "#define TUtil_h" to avoid redefinitions
            change private to protected
            add const statement with extractor
   v0.1.6   add TFile::Clear() after Write()
   v0.1.5b  change the treatment of TGraph/TGraphErrors
   v0.1.5a  NewTGraph/NewTGraphErrors are modified ( add skeletonNewTGraph )
            add the certain process to store TGraphs to ROOT file
   v0.1.4   Fix red-line shade around draw area ( Thanks to Nakaya )
            NewTGraph and NewTGraphErrors are modified
   v0.1.3   Draw and Fit are modified ( update skeletonDraw )
            Draw(TTree*) is added
            ViewHists is added
   v0.1.2   Update and CdPad are modified
            SetPadNum,SetPadNumX,SetPadNumY and SetPadNumXY are added
            Draw is modified ( add skeletonDraw )
            remove ClassDef/ClassImp temporary?
   v0.1.1   NewTGraphErrors and Draw(TGraph*) are added
            ClassDef/ClassImp, virtualize destructor are added
   v0.1.0   first version
   
********************************************************** */

#ifndef TUtil_h
#define TUtil_h

#define TUTIL_PSFILE "t.ps"
#define TUTIL_ROOTFILE "t.root"
#define TUTIL_VERSION "TUtil v0.5.0_20090922"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <errno.h>

#include <TH1.h>
#include <TCanvas.h>
#include <TPostScript.h>
#include <TPDF.h>
#include <TSystem.h>
#include <TFile.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TTree.h>
#include <TKey.h>
#include <TCollection.h>
#include <TF1.h>
#include <TMath.h>

#include "AtlasStyle.h"

std::string get_file_contents(const char *filename);

class TUtil{
 public:
  TUtil(const char* psFile=TUTIL_PSFILE, const char* rootFile=TUTIL_ROOTFILE, const Int_t canv_x=640, const Int_t canv_y=856, const Int_t debug=false);
  TUtil(const char* psFile=TUTIL_PSFILE, const char* rootFile=TUTIL_ROOTFILE, const Bool_t debug=false);
  virtual ~TUtil(void);
  
  // Pad Util
  void CdPad(void);
  void cdPad(void){ CdPad(); }
  void Update(int padNum=1);
  
  // Draw
  void Draw(TH1* hist, const char* option="", int logScale=000){ skeletonDraw(hist,option,logScale); }
  void Draw(TH1& hist, const char* option="", int logScale=000){ Draw(&hist,option,logScale); }
  void Draw(const char* histName, const char* option="", int logScale=000){ Draw((TH1*)gROOT->FindObject(histName),option,logScale); }
  void Draw(TH1* hist, int logScale, const char* option=""){ Draw(hist,option,logScale); }
  void Draw(TH1& hist, int logScale, const char* option=""){ Draw(&hist,option,logScale); }
  void Draw(const char* histName, int logScale, const char* option=""){ Draw((TH1*)gROOT->FindObject(histName),option,logScale); }
  
  void Draw(TGraph* graph, const char* option="", int logScale=000){ skeletonDraw(graph,option,logScale); }
  void Draw(TGraph& graph, const char* option="", int logScale=000){ Draw(&graph,option,logScale); }
  
  TH1* Draw(TTree* tree, const char* varexp, const char* selection="", const char* option="", int logScale=000);
  TH1* Draw(TTree& tree, const char* varexp, const char* selection="", const char* option="", int logScale=000){ return Draw(&tree,varexp,selection,option,logScale); }
  
  void DrawNormalized(TH1* hist, const char* option="", int logScale=000){ skeletonDraw(hist,option,logScale,3); }
  void DrawNormalized(TH1& hist, const char* option="", int logScale=000){ DrawNormalized(&hist,option,logScale); }
  void DrawNormalized(const char* histName, const char* option="", int logScale=000){ DrawNormalized((TH1*)gROOT->FindObject(histName),option,logScale); }
  void DrawNormalized(TH1* hist, int logScale, const char* option=""){ DrawNormalized(hist,option,logScale); }
  void DrawNormalized(TH1& hist, int logScale, const char* option=""){ DrawNormalized(&hist,option,logScale); }
  void DrawNormalized(const char* histName, int logScale, const char* option=""){ DrawNormalized((TH1*)gROOT->FindObject(histName),option,logScale); }
  
  // Fit
  void Fit(TH1* hist, char* formula, const char* option="", int logScale=000, const char* goption="", double xmin=0, double xmax=0){ skeletonDraw(hist,goption,logScale,1,formula,option,xmin,xmax); }
  void Fit(TH1& hist, char* formula, const char* option="", int logScale=000, const char* goption="", double xmin=0, double xmax=0){ Fit(&hist,formula,option,logScale,goption,xmin,xmax); }
  void Fit(const char* histName, char* formula, const char* option="", int logScale=000, const char* goption="", double xmin=0, double xmax=0){ Fit((TH1*)gROOT->FindObject(histName),formula,option,logScale,goption,xmin,xmax); }
  void Fit(TH1* hist, TF1* f1, const char* option="", int logScale=000, const char* goption="", double xmin=0, double xmax=0){ skeletonDraw(hist,goption,logScale,2,f1,option,xmin,xmax); }
  void Fit(TH1& hist, TF1* f1, const char* option="", int logScale=000, const char* goption="", double xmin=0, double xmax=0){ Fit(&hist,f1,option,logScale,goption,xmin,xmax); }
  void Fit(const char* histName, TF1* f1, const char* option="", int logScale=000, const char* goption="", double xmin=0, double xmax=0){ Fit((TH1*)gROOT->FindObject(histName),f1,option,logScale,goption,xmin,xmax); }
  
  // new blank TGraph ( to be stored to ROOT file )
  TGraph* NewTGraph(const char* name="defaultName"){ return skeletonNewTGraph(0,name); }
  TGraphErrors* NewTGraphErrors(const char* name="defaultName"){ return (TGraphErrors*)skeletonNewTGraph(1,name); }
  TGraphAsymmErrors* NewTGraphAsymmErrors(const char* name="defaultName"){ return (TGraphAsymmErrors*)skeletonNewTGraph(2,name); }
  
  void ViewHists(TFile* f, const char* searchKey="", TString::ECaseCompare cmp=TString::kIgnoreCase);
  void ReOpen(const char* mode="UPDATE");
  
  // setter
  void SetPadNum(int padNum){ m_padNum=padNum; }
  void SetPadNumX(int padNumX){ SetPadNumXY(padNumX,m_padNumY); }
  void SetPadNumY(int padNumY){ SetPadNumXY(m_padNumX,padNumY); }
  void SetPadNumXY(int padNumX, int padNumY);
  //void SetCanvas(TCanvas& c);
  void SetColorTable(std::vector<int> colorTable){ m_colorTable=colorTable; }
  void SetStyleTable(std::vector<int> styleTable){ m_styleTable=styleTable; }
  void SetApplyColorToFirstDraw(bool applyColor=true){ m_applyColorToFirstDraw=applyColor; }
  
  // extractor, getter
  int GetPadNum(void) const { return m_padNum; }
  int GetPadNumX(void) const { return m_padNumX; }
  int GetPadNumY(void) const { return m_padNumY; }
  TCanvas* GetCanvas(void) const { return m_c; }
  TFile*   GetFile(void) const { return m_f; }
  TString  GetPsFileName(void) const { return m_psFileName; }

  // static function
  static const std::string GetVersion(void){ return TUTIL_VERSION; }
  static const std::string Version(void){ return GetVersion(); }
  
 protected:
  
  // init
  void init_style(void);
  void init_canvas(void);
  void init_ps(const char* psFile);
  void init_ofile(const char* rootFile);
  void init_colortable(void);
  
  // skeleton
  void skeletonDraw(void* obj, const char* option, int logScale, int drawType=0, void* fitFunc=0, const char* fitOption="", double xmin=0., double xmax=0.);
  TGraph* skeletonNewTGraph(int graphType, const char* name);
  
  bool m_debugPlot;
  bool m_bps;
  bool m_bpdf;
  bool m_broot;
  TString m_psFileName;
  TString m_rootFileName;
  int m_padNum;
  int m_padNumX;
  int m_padNumY;
  TCanvas* m_c;
  TPostScript* m_ps;
  TPDF* m_pdf;
  TFile* m_f;
  int m_colorTableNum;
  std::vector<int> m_colorTable;
  std::vector<int> m_styleTable;
  std::vector<TObject*> m_drawObj;
  bool m_applyColorToFirstDraw;
  
  Int_t m_canv_x;
  Int_t m_canv_y;
  //ClassDef(TUtil,1)
};

//#if !defined(__CINT__)
//ClassImp(TUtil)
//#endif
#endif // TUtil_h
