#ifndef __CINT__
#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TStyle.h"
#endif
#include <iostream>  // for std::cout
#include <iomanip>   // for std::setw
#include <fstream>   // for std::ifstream
#include <sstream>   // for std::istringstream
using namespace std; // for not to use std::


Int_t decay_graph()
{
  //----------------------------------//
  TString fin_name = "test.dat";
  //----------------------------------//

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

  // read file
  //
  ifstream fin(fin_name.Data());
  if (!fin) {
    cerr << "Error: don't exist such a data file, " << fin_name << " !" << endl;
    return -1;
  }

  TGraph* graph = new TGraph();  // no error
  //  TGraphErrors* graph = new TGraphErrors();  // with error

  Int_t N = 0;
  string line;
	double X;
	double Value;
  while (getline(fin, line)) {
    if (line.empty() || (line[0] == '#')) {
      continue;
    }
		double y[10];
    if (!(istringstream(line) >> X >>y[0] >>y[1] >> Value)) {
      cerr << "Error: badly formatted data line: " << line << endl;
      return -1;
    }

		//cout << X<< ":"<< Value << endl;
    graph -> SetPoint(N, N, Value);
    //    graph -> SetPointError(N, 0, TMath::Sqrt(NofAllTriggers) / LiveTime);

    N++;
  }

  // draw graph
  //
  graph -> SetLineColor(4); // 2: red, 4: blue
  graph -> SetLineWidth(2);
  graph -> SetMarkerColor(4);
  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(1.2);
  graph -> Draw("ALP"); // A: axis, L: line, P: point

  // set frame
  //
  const Double_t XMIN = 0;
  const Double_t XMAX = 100;
  const Double_t YMIN = 0;
  const Double_t YMAX = 1e-8;
  graph -> GetXaxis() -> SetLimits(XMIN, XMAX);
  //graph -> GetYaxis() -> SetRangeUser(YMIN, YMAX);
  //graph -> SetTitle("Title");
  graph -> GetXaxis() -> SetTitle("time [s]");
  graph -> GetYaxis() -> SetTitle("mass [g]");

  // make plot
  //
  c1 -> Print("plot_graph.pdf");

  return 0;
}


#ifndef __CINT__
Int_t main(Int_t argc, char** argv) {
  TApplication app("App", &argc, argv);
  decay_graph();
  cerr << "Finished." << endl;
  app.Run();
  return 0;
}
#endif
