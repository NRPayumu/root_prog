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

Int_t bete()
{
	int i;
	FILE *fp;
	char output_file[30];
	//const double E=200e+6;   /* [eV] */
	const double E=2e+6;   /* [eV] */
	const double M=9.11e-31;   /* [kg] */
	const double c=3.0e+8;   /* [m/s] */
	const double el=1.6e-19;   /* [C] */
	double MC=M*c*c/el;   /* [eV] */
	const double m=1.67e-27;   /* [kg] */
	const double h=6.62e-34;
	const double NA=6e+23;   /* [/mol] */
	const double re=2.8e-15; /* [m] */
	//double density=1000; /* [kg/m^3] */
	//double Z=10;   /* 原子番号 */
	//double A=18;   /* 質量数 */
	//double z=1.0;   /* 荷数 */
	//double I=75;   /* [eV] */
	double density=852; /* LAB [kg/m^3] */
	double Z=138;   /* 原子番号 */
	double A=246;   /* 質量数 */
	double z=1.0;   /* 荷数 */
	//double I=10*Z;   /* [eV] */
	double I=16*pow(Z,0.9);

	double e=E;
	double S=0;
	int R=0;
	double beta2=1,beta=1;
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

	TGraph* graph = new TGraph();  // no error
	//  TGraphErrors* graph = new TGraphErrors();  // with error

	//    graph -> SetPointError(N, 0, TMath::Sqrt(NofAllTriggers) / LiveTime);
	for(i = 0; e > 0; i++){
		graph -> SetPoint(i, i, e/(1000*1000) );
		beta2=1-1/((((e*el)/(m*c*c))+1)*(((e*el)/(m*c*c))+1));
		beta=sqrt(beta2);
		S=(density*4*M_PI*re*re*MC*NA*Z*z*z)/(A*beta2)*(log((2*MC*beta2)/((1-beta2)*I))-beta2);
		e -=S;
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
	//const Double_t XMAX = 300;
	const Double_t XMAX = 10;
	const Double_t YMIN = 0;
	const Double_t YMAX = E/1000/1000;
	graph -> GetXaxis() -> SetLimits(XMIN, XMAX);
	graph -> GetYaxis() -> SetRangeUser(YMIN, YMAX);
	graph -> SetTitle("Energy Decay");
	graph -> GetXaxis() -> SetTitle("Range");
	graph -> GetYaxis() -> SetTitle("Energy[MeV]");

	// make plot
	//
	c1 -> Print("bete.pdf");

	return 0;
}

#ifndef __CINT__
Int_t main(Int_t argc, char** argv) {
	TApplication app("App", &argc, argv);
	bete();
	cerr << "Finished." << endl;
	app.Run();
	return 0;
}
#endif
