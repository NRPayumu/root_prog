#ifndef __CINT__
#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TStyle.h"
#include "TLegend.h"
#endif
#include <iostream>  // for std::cout
#include <iomanip>   // for std::setw
#include <fstream>   // for std::ifstream
#include <sstream>   // for std::istringstream
using namespace std; // for not to use std::


Int_t root_graph()
{
  // set canvas style
  //
  gStyle -> SetLabelSize(0.05, "XYZ");
  gStyle -> SetTitleSize(0.06, "T");
  gStyle -> SetTitleSize(0.06, "XYZ");
  gStyle -> SetTitleOffset(1.0, "X");
  gStyle -> SetTitleOffset(1.1, "Y");
  gStyle -> SetPadLeftMargin(0.14);
  gStyle -> SetPadBottomMargin(0.14);

  TCanvas *c1 = new TCanvas("c1", "", 800, 600);
  c1 -> SetFillStyle(4001); // set transparent background
  c1 -> SetLogy(0); 
  const Double_t XMIN = 300;
  const Double_t XMAX = 700;
  const Double_t YMIN = 0;
  const Double_t YMAX = 0.01;

  Int_t ave=400;

  Int_t N = 0;  
    
  const Int_t point=1000;
  const Int_t point2=point*3;
  Double_t x[point2];
  Double_t x_lan[point2];
  Double_t x_gPeek;
  Double_t x_lPeek;
  Double_t y_gauss[point2];
  Double_t y_landau[point2];
  Double_t gau=0.;
  Double_t gauymax=0.;
  Double_t lanymax=0.;

  Int_t N_minus=50;

  Double_t XWID = (XMAX-XMIN)/point;

  for(N=0; N<point2; N++){
    x[N] = XMIN+(XWID * (N-N_minus));
    y_gauss[N] =TMath::Exp(-(x[N]-ave)*(x[N]-ave)/(2 * ave) );
    y_landau[N] =TMath::Landau(x[N], ave, TMath::Sqrt(ave));

    if(gauymax < y_gauss[N] ){
      gauymax= y_gauss[N];
      x_gPeek=x[N];
    }
    if(lanymax < y_landau[N]){
      lanymax= y_landau[N];
      x_lPeek=x[N];
    }
    gau+=y_gauss[N];
  }

  for(N=0; N<point2; N++){
    y_gauss[N]/=gau;
    y_landau[N]*=gauymax/(gau*lanymax);
    //x_lan[N]=x[N]+(x_gPeek-x_lPeek);
  }

  cout<<x_gPeek<<" "<<x_lPeek<<" "<<(x_gPeek-x_lPeek)/x_gPeek<<endl;

  TGraph*graph = new TGraph(point2,x,y_gauss);
  TGraph*Landau = new TGraph(point2,x,y_landau);
    //    graph -> SetPointError(N, 0, TMath::Sqrt(NofAllTriggers) / LiveTime);

    N++;

  // draw graph
  //
  graph -> SetLineColor(4); // 2: red, 4: blue
  graph -> SetLineWidth(2);
  graph -> SetMarkerColor(4);
  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.4);
  graph -> Draw("ALP"); // A: axis, L: line, P: point

  Landau -> SetLineColor(2); // 2: red, 4: blue
  Landau -> SetLineWidth(2);
  Landau -> SetMarkerColor(2);
  Landau -> SetMarkerStyle(20);
  Landau -> SetMarkerSize(0.4);
  Landau -> Draw("LP"); // A: axis, L: line, P: point

  // set frame
  //
  graph -> GetXaxis() -> SetLimits(XMIN, XMAX);
  graph -> GetYaxis() -> SetRangeUser(YMIN, YMAX);
  graph -> SetTitle("");

  TLegend *leg = new TLegend(0.65,0.75,0.75,0.85);

  leg -> AddEntry(graph, "Gaussian Function", "l");
  leg -> AddEntry(Landau, "Landau Function", "l");
  leg -> SetMargin(0.5);
  leg -> SetTextSize(0.03);
  leg -> SetBorderSize(0);
  leg -> SetFillColor(0);
  leg -> Draw();

  // make plot
  //
  c1 -> Print("plot_graph.pdf");

  return 0;
}
  

#ifndef __CINT__
Int_t main(Int_t argc, char** argv) {
  TApplication app("App", &argc, argv);
  root_graph();
  cerr << "Finished." << endl;
  app.Run();
  return 0;
}
#endif
