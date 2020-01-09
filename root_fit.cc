#ifndef __CINT__
#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TRandom.h"
#include "TROOT.h"
#include "TStyle.h"
#endif
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;


Int_t root_fit()
{
  // set canvas style
  //
  //  gStyle -> SetOptStat(0);   // statistical information (0 = no info, 1 = show info)
  gStyle -> SetOptStat(1);
  //  gStyle -> SetOptFit(1111); // show output result including fit probability (1111)
  gStyle -> SetLabelSize(0.05, "XYZ");
  gStyle -> SetTitleSize(0.06, "XYZ");
  gStyle -> SetTitleOffset(1.0, "X");
  gStyle -> SetTitleOffset(1.1, "Y");
  gStyle -> SetPadLeftMargin(0.14);
  gStyle -> SetPadBottomMargin(0.14);

  TCanvas *c1 = new TCanvas("c1", "", 800, 600);
  c1 -> SetFillStyle(4001);

  // make histogram
  //
  const Double_t XMIN   = -5;
  const Double_t XMAX   = +5;
  const Double_t XWIDTH = 0.2;
  Int_t bin = (Int_t)((XMAX - XMIN) / XWIDTH + 0.5);
  TH1D *h1 = new TH1D("h1", "", bin, XMIN, XMAX);
  h1 -> SetXTitle("X");
  h1 -> SetYTitle("Y");

  // fill random number
  //
  // gRandom -> SetSeed(); // on: random output, off: same output
  const Double_t MEAN  = 0;
  const Double_t SIGMA = 1;
  for (int i = 0; i < 1000; i++) {
    h1 -> Fill(gRandom -> Gaus(MEAN, SIGMA));
  }

  // draw histogram
  //
  h1 -> SetLineColor(4);
  h1 -> SetLineWidth(2);
  h1 -> Draw();

  // fit
  //
  const Double_t FIT_XMIN = XMIN;
  const Double_t FIT_XMAX = XMAX;
  TF1 *f1 = new TF1("f1", "gaus", FIT_XMIN, FIT_XMAX);
  f1 -> SetParNames("N", "#mu", "#sigma");
  f1 -> SetParameter(0, 1);
  f1 -> SetParameter(1, 0);
  f1 -> SetParameter(2, 0.1);
  f1 -> SetLineColor(2);
  h1 -> Fit("f1", "R", "same"); 

  // legend
  //
  const Double_t XMAX_RATE = 0.89;
  const Double_t XMIN_RATE = XMAX_RATE - 0.24;
  const Double_t YMAX_RATE = 0.65;
  const Double_t YMIN_RATE = YMAX_RATE - 0.06 * 2;
  TLegend* leg = new TLegend(XMIN_RATE, YMIN_RATE, XMAX_RATE, YMAX_RATE);
  leg -> AddEntry(h1, "Simulation", "L");
  leg -> AddEntry(f1, "Fit",        "L");
  leg -> SetBorderSize(0);
  leg -> SetTextFont(42);
  leg -> SetTextSize(0.05);
  leg -> SetLineColor(0);
  leg -> SetFillStyle(0);
  leg -> Draw();

  gPad -> RedrawAxis(); // draw axis again

  c1 -> Print("plot_fit.pdf");

  return 0;
}


#ifndef __CINT__
Int_t main(Int_t argc, char** argv) {
  TApplication app("App", &argc, argv);
  root_fit();
  cerr << "Finished." << endl;
  app.Run();
  return 0;
}
#endif
