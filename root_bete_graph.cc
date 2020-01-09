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
#include <math.h>
using namespace std; // for not to use std::

// alias rg++='g++ `root-config --cflags --libs` -lMinuit' //

Int_t root_graph(){
	Int_t switc=0;  // 0 or 1

	cout << "Figure Number (0 or 1) = ";
	cin >> switc;

	if(switc != 0 && switc != 1){
		cout << switc << " can't use Figure Number" << endl
		     << "use Figure Number 0 instead of " << switc << endl << endl;
		switc = 0;
	}

	const Double_t XMIN = 1.e-3;
	const Double_t XMAX = 1.e5;
	const Double_t YMIN = 1.0;
	Double_t YMAX = 1.e4;
	if(switc == 1)YMAX = 1.e2;   /* change to 1.e2 if fig.b */

	Int_t N = 0;
	string line;

	Int_t i = 0;
	Int_t j = 0;     /* particle suffix */
	Int_t tar = 0;   /* target suffix */

	const Double_t M=9.109e-31;   /* [kg] */
	const Double_t c=3.0e+8;   /* [m/s] */
	const Double_t el=1.6e-19;   /* [C] */
	const Double_t el2_cgs=1.44e-13;   /* [MeV cm] */
	Double_t el2=el2_cgs*1000*1000;   /*[eV cm]*/
	Double_t MC=M*c*c/el;   /* [eV] */
	Double_t m[5]={6.644e-27,1870*1000*1000*el/c/c,1.673e-27,105.7*1000*1000*el/c/c,9.109e-31};   /*α D.meson Proton Muon electron [kg] */
	Double_t m_eV[5]={3733e+6,1870e+6,938e+6,105.7e+6,0.511e+6}; /*[eV]*/
	const Double_t h=6.62e-34;
	const Double_t NA=6e+23;   /* [/mol] */
	Double_t z[5]={2.0,1.0,1.0,1.0,1.0};   /* incident partical's electrical charge */
	//Double_t density=1000; /* water's density[kg/m^3] */
	Double_t Z[5]={14.4,29,37,13,10};   /* Air、Cu、Pb、Al、H2O 's atomic number */
	Double_t A[5]={29,63.5,85.5,27.0,18};  /* molar mass[g/mol] */
	Double_t I[5]={85.7,322,823,166,75};   /* mean excitation potential[eV] */
	Double_t K=1.0;    /* part of constant */

	K = M_PI * NA * el2*el2;  /* [MeV^2 cm^2/mol] */

	Double_t beta2=1;
	Double_t start_E=1.0e+6 * sqrt(10);     /* start E (changeable) [eV] */
	Double_t start_Ee=1.0e+4;               /* start E energy [eV] */
	Double_t e=start_E;                    /* energy [eV] */
	Double_t magni=sqrt(sqrt(sqrt(10))) ;  /* magnification */
	Double_t S=0;     /* stopping power */

	Double_t temp1=0;
	Double_t temp2=0;
	Double_t temp3=0;

	Int_t count=36;      /* number of repeat */

	// set canvas style
	//
	gStyle -> SetLabelSize(0.05, "XYZ");
	gStyle -> SetTitleSize(0.06, "T");
	gStyle -> SetTitleSize(0.06, "XYZ");
	gStyle -> SetTitleOffset(1.0, "X");
	gStyle -> SetTitleOffset(1.1, "Y");
	gStyle -> SetPadLeftMargin(0.14);
	gStyle -> SetPadBottomMargin(0.14);

	TCanvas *c1 = new TCanvas("c1", "", 600, 400);
	//c1 -> SetFillStyle(4001); // set transparent background

	gPad->SetLogx();
	gPad->SetLogy();    /*set log axis*/

	TGraph* graph[5];  // no error
	//  TGraphErrors* graph = new TGraphErrors();  // with error

	for(j = 0; j < 5; j++){
		graph[j] = new TGraph();
		// draw graph
		//
		graph[j] -> SetLineColor(j+1);
		graph[j] -> SetLineWidth(1);
		graph[j] -> SetMarkerColor(j+1);
		graph[j] -> SetMarkerStyle(20);
		if(switc==0){ graph[j] -> SetMarkerSize(0.8);}
		if(switc==1){ graph[j] -> SetMarkerSize(0.4);}
		// set frame
		//
	}
	j = 0;

	graph[0] -> GetXaxis() -> SetLimits(XMIN, XMAX);
	graph[0] -> GetYaxis() -> SetRangeUser(YMIN, YMAX);
	/* target fixed、 particle changed */
	if (switc==0){
		tar=0;
		for(j = 0; j < 4; j++){
			for(i = 0; i <= count; i++){
				beta2=1-1/((((e*el)/(m[j]*c*c))+1)*(((e*el)/(m[j]*c*c))+1));
				temp1=(4*K*z[j]*z[j]*Z[tar])/(MC*A[tar]*beta2);
				temp2=log((2*MC*beta2)/I[tar]);
				temp3=log(1-beta2);
				S= temp1 * (temp2 -temp3 - beta2);

				graph[j] -> SetPoint(N, e/(1000*1000), S/(1000*1000));

				e*=magni;
				N++;
			}
			count +=4;
			start_E /=sqrt(10);
			e=start_E;
			N = 0;
			if (j == 0){
				graph[0] -> Draw("ALP"); // A: axis, L: line, P: point
			}
			else {
				graph[j] -> Draw("LP");
			}
		}
		/* j = 4 (electron) */
		e=start_Ee;
		for(i = 0; i <=count; i++){
			beta2=1-1/((((e*el)/(m[j]*c*c))+1)*(((e*el)/(m[j]*c*c))+1));
			S=(2*K*Z[tar])/(m_eV[j]*A[tar]*beta2) * (log( (m_eV[j]*e*beta2)/(2*(1-beta2)*I[tar]*I[tar]) )-( 2*sqrt(1-beta2)-1+beta2 )*log(2)+1-beta2+( (1-sqrt(1-beta2))*(1-sqrt(1-beta2)) )/8 );

			graph[j] -> SetPoint(N, e/(1000*1000), S/(1000*1000));
			e*=magni;
			N++;
		}
		graph[j] -> Draw("LP");
	}

	/* particle fixed(electron)、target changed */
	if (switc==1){
		count=56;
		j=4;

		magni=sqrt(magni);
		count*=2;

		for(tar = 0; tar <5; tar++){
			e=start_Ee;
			N=0;
			for(i = 0; i <=count; i++){
				beta2=1-1/((((e*el)/(m[j]*c*c))+1)*(((e*el)/(m[j]*c*c))+1));
				S=(2*K*Z[tar])/(m_eV[j]*A[tar]*beta2) * (log( (m_eV[j]*e*beta2)/(2*(1-beta2)*I[tar]*I[tar]) )-( 2*sqrt(1-beta2)-1+beta2 )*log(2)+1-beta2+( (1-sqrt(1-beta2))*(1-sqrt(1-beta2)) )/8 );

				graph[tar] -> SetPoint(N, e/(1000*1000), S/(1000*1000));
				e*=magni;
				N++;
			}
			if (tar == 0){
				graph[0] -> Draw("ALP"); // A: axis, L: line, P: point
			}
			else {
				graph[tar] -> Draw("LP");
			}
		}
	}
	graph[0] -> GetXaxis() -> SetLimits(XMIN, XMAX);
	graph[0] -> GetYaxis() -> SetRangeUser(YMIN, YMAX);
	graph[0] -> SetTitle("Energy Decay");
	graph[0] -> GetXaxis() -> SetTitle("Energy[MeV]");
	graph[0] -> GetYaxis() -> SetTitle("-dE/d#xi[MeV g^{-1} cm^{2}]");   /*LaTeX format*/

	TLegend *leg = new TLegend(0.7,0.6,0.85,0.85);
	if (switc==0){
		leg -> AddEntry(graph[0], "#alpha", "lp");
		leg -> AddEntry(graph[1], "d", "lp");
		leg -> AddEntry(graph[2], "p", "lp");
		leg -> AddEntry(graph[3], "#mu", "lp");
		leg -> AddEntry(graph[4], "e", "lp");
	}
	if (switc==1){
		leg -> AddEntry(graph[0], "Air", "lp");
		leg -> AddEntry(graph[1], "Cu", "lp");
		leg -> AddEntry(graph[2], "Pb", "lp");
		leg -> AddEntry(graph[3], "Al", "lp");
		leg -> AddEntry(graph[4], "H_{2}O", "lp");
	}
	leg -> SetTextSize(0.08);
	leg -> SetMargin(0.5);
	leg -> SetTextSize(0.04);
	leg -> SetBorderSize(0);
	leg -> SetFillColor(0);
	leg -> Draw();

	// make plot
	//
	if(switc==0){ c1 -> Print("hou_kad_bete2_a.pdf");}
	if(switc==1){ c1 -> Print("hou_kad_bete2_b.pdf");}

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
