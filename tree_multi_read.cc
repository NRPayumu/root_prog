//tree to histgram
//root -l 'root_tree_histgram.cc("root_file.root")'

//Tree -> Print()
//SEne
//SposX SposY SposZ
//CapID
//CposX CposY CposZ
//Eabs Elos

#include <string>
#include "TFile.h"
#include "TTree.h"

using namespace std;

void tree_multi_read(TString root_file, TString root_file2){
  TCanvas *c1 = new TCanvas("c1","canvas",600,400);
  gStyle -> SetOptStat(0);

  TFile *tf = new TFile(root_file);
  TTree *tr = (TTree*)tf->Get("Tree");

  TFile *tf2 = new TFile(root_file2);
  TTree *tr2 = (TTree*)tf2->Get("Tree");

  double SEne1,LenG1;
  tr->SetBranchAddress("SEne",&SEne1);
  tr->SetBranchAddress("LenG",&LenG1);
  const Int_t N = tr->GetEntries();

  double SEne2,LenG2;
  tr2->SetBranchAddress("SEne",&SEne2);
  tr2->SetBranchAddress("LenG",&LenG2);
  const Int_t N2 = tr2->GetEntries();

  const Double_t XMIN   = 0;
  const Double_t XMAX   = 400;
  const Double_t XWIDTH = 100;

  Int_t bin = (Int_t)((XMAX - XMIN) / XWIDTH + 0.5);
  TH1D *h1 = new TH1D("h1", "", bin, XMIN, XMAX);
  TH1D *h2 = new TH1D("h2", "", bin, XMIN, XMAX);
  TH1D *h3 = new TH1D("h3", "", bin, XMIN, XMAX);
  TH1D *h4 = new TH1D("h4", "", bin, XMIN, XMAX);
  TH1D *h5 = new TH1D("h5", "", bin, XMIN, XMAX);
  //h1->SetFillColor(kRed);
  //h3->SetFillColor(kGreen);
  //h2->SetFillColor(kRed);
  //h4->SetFillColor(kBlue);

  h2->SetLineColor(2);
  h4->SetLineColor(4);
  h5->SetLineColor(4);

  for (Int_t ientry = 0; ientry < N; ientry++) {
    tr ->GetEntry(ientry);
    tr2->GetEntry(ientry);

    h1->Fill(SEne1);
    h2->Fill(LenG1);
    h3->Fill(SEne2);
    h4->Fill(LenG2);
  }

  double per = h2->GetMaximum()/h4->GetMaximum();
  for (Int_t ientry = 0; ientry < N; ientry++) {
    //h5->SetBinContent(ientry,h4->GetBinContent(ientry) *0.5 );
    h5->SetBinContent(ientry,h4->GetBinContent(ientry) * per );
  }

  //h4->Scale(0.6);

  //h1->Draw();
  h2->Draw();
  //h2->Draw("same");
  //h3->Draw("same");
  //h4->Draw("same");
  h5->Draw("same");
  /*THStack *hs = new THStack("hs","test stacked histograms");
  //hs->Add(h1);
  //hs->Add(h2);
  hs->Add(h4);
  hs->Add(h3);
  hs->Draw();*/
  c1->Print("stac_hist.pdf");
}

int main(){
  tree_multi_read("test.root","test2.root");
  return 0;
}
