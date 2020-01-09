#ifndef __CINT__
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TStyle.h"
#endif
#include <iostream>  // for std::cout
#include <iomanip>   // for std::setw
#include <fstream>   // for std::ifstream
#include <sstream>   // for std::istringstream
using namespace std; // for not to use std::

Int_t root_histogram()
{
  //---------------------------------//
  TString fin_name = "output_a.dat";
  //---------------------------------//

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
  c1 -> SetLogy();          // set log axis

  // make histogram
  //
  const Double_t XMIN   = 0;
  const Double_t XMAX   = 0.6;
  const Double_t XWIDTH = 1.0;
  Int_t bin = (Int_t)((XMAX - XMIN) / XWIDTH + 0.5);
  TH1D *h1 = new TH1D("h1", "", bin, XMIN, XMAX);
  h1 -> SetXTitle("Energy [keV]");
  if (XWIDTH == 1.0)
    h1 -> SetYTitle("Events/keV");
  else
  h1 -> SetYTitle(Form("Events/%.0fkeV", XWIDTH));

  // read file
  //
  ifstream fin(fin_name.Data());
  if (!fin) {
    cerr << "Error: don't exist such a data file, " << fin_name << " !" << endl;
    return -1;
  }

  Double_t x;
  while (fin >> x) {
    h1 -> Fill(x); // fill histogram
  }

  cout << endl
       << "Input       = " << fin_name           << endl
       << "# of events = " << h1 -> GetEntries() << endl
       << endl;

  // draw histogram
  //
  h1 -> SetLineColor(4); // 2: red, 3: green, 4: blue
  h1 -> SetLineWidth(2);
  h1 -> Draw();

  // make plot
  //
  c1 -> Print("plot_histogram.pdf");

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
