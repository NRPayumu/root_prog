#ifndef __CINT__
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TRandom.h"
#include "TMath.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TF1.h"
#endif
#include <iostream>  // for std::cout
#include <iomanip>   // for std::setw
#include <fstream>   // for std::ifstream
#include <sstream>   // for std::istringstream
using namespace std; // for not to use std::


Int_t root_histogram()
{
  // set canvas style
  //
  gStyle -> SetOptStat(0);            // statistical information (0 = no info, 1 = show info)
  gStyle -> SetStatX(0.9);            // x position of stat info
  gStyle -> SetStatY(0.9);            // y position of stat info
  gStyle -> SetLabelSize(0.05, "XY"); // axis label font size
  gStyle -> SetTitleSize(0.06, "XY"); // axis title font size
  gStyle -> SetTitleOffset(1.0, "X"); // x title offset from axis
  gStyle -> SetTitleOffset(1.1, "Y"); // y title offset from axis
  gStyle -> SetPadLeftMargin(0.14);   // canvas left margin
  gStyle -> SetPadBottomMargin(0.14); // canvas bottom margin

  // make canvas
  //
  TCanvas *c1 = new TCanvas("c1", "", 800, 600); // x-size, y-size = 800, 600
  c1 -> SetFillStyle(4001); // canvas with transparent background 
  c1 -> SetLogy(0);          // no set log axis ,yes linier

  // 
  Int_t switc=1;  // 1-9

  cout << "Figure number = ";
  cin >> switc;

  Double_t XMIN   = 0;        //switc = 1 or 2  ,XMIN XMAX XWIDTH
  Double_t XMAX   = 3.0;
  Double_t XWIDTH = 1.;
  Double_t m=0.5;
  if(switc >=3 && switc <=5){
    XMIN   = 0;
    XMAX   = 6.0;
    m = 3.0;
  }if(switc >=6 && switc <=7){
    XMIN   = 0.0;
    XMAX   = 20.0;
    m=10.;
  }if(switc >=8 && switc <=9){
    XMIN   = 10;
    XMAX   = 30.0;
    m=20.;
  }

  Int_t count=100;

  if(switc == 2 || switc == 4 || switc == 7 || switc == 9){
  count=10000;
  }if(switc == 5){
  count=1000000;
  }

  Int_t bin = (Int_t)((XMAX - XMIN) / XWIDTH + 0.5);

  //const Double_t YMIN   = 0;
  //const Double_t YMAX   = 0.6;
  Double_t x;
  Int_t j=0;

  Int_t N=0;
  Double_t gaus=0;

  TH1D *h1;
  TGraph* graph;

  gRandom -> SetSeed( time(NULL) );

  h1 = new TH1D("", "", bin, XMIN, XMAX);
  graph = new TGraph();

  h1 -> SetXTitle("value");
  if (XWIDTH == 1.0)
    h1 -> SetYTitle("Events");
  else
    h1 -> SetYTitle(Form("Events/%.1f", XWIDTH));


  for (j = 0; j < count; j++){
    x = (gRandom -> Poisson(m));
    h1 -> Fill(x);
   }

  for (x = XMIN; x <= XMAX; x += (XMAX-XMIN)/100){
    gaus=1 / TMath::Sqrt( 2 * TMath::Pi() * m ) * TMath::Exp(-(x-m)*(x-m)/(2 * m) );
    graph -> SetPoint(N, x, gaus);
    N++;
   }


  graph -> SetLineColor(2);
  graph -> SetLineWidth(2);
  graph -> SetMarkerColor(2);
  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.8);

  // draw histogram
  //
  h1 -> Scale(1./count);
  h1 -> SetLineColor(4); // 2: red, 3: green, 4: blue
  h1 -> SetLineWidth(2);

  h1 -> Draw();
  graph -> Draw("same");

  h1 -> GetXaxis() -> SetLimits(XMIN, XMAX);
  h1->SetMinimum(0.8);
  //h1 -> GetYaxis() -> SetRangeUser(YMIN, YMAX);

  TLegend *leg = new TLegend(0.65,0.75,0.8,0.85);

  leg -> AddEntry(h1, "Poisson distribution", "l");
  leg -> AddEntry(graph, "Normal distribution", "l");
  leg -> SetMargin(0.5);
  leg -> SetTextSize(0.03);
  leg -> SetBorderSize(0);
  leg -> SetFillColor(0);
  leg -> Draw();

  // make plot
  //
  c1 -> Print("poisson_hist.pdf");

  return 0;
}


#ifndef __CINT__
Int_t main(Int_t argc, char** argv) {
  TApplication app("App", &argc, argv);
  root_histogram();
  cerr << "Finished." << endl;
  app.Run();
  return 0;
}
#endif
