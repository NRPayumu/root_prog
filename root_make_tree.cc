#ifndef __CINT__
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#endif
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;


Int_t root_make_tree()
{
	//-----------------------------------------------------//
	TString fin_dir   = "data/pulse_shape";
	TString fout_name = "output/root_pulse_shape_info.root";
	//-----------------------------------------------------//

	// make output root file
	//
	TFile *fout = new TFile(fout_name, "RECREATE");

	Int_t    EventID;
	Double_t Vpeak;
	Double_t Sum;
	TTree *tree = new TTree("tree", "");
	tree -> Branch("EventID", &EventID, "EventID/I");
	tree -> Branch("Vpeak",   &Vpeak,   "Vpeak/D");
	tree -> Branch("Sum",     &Sum,     "Sum/D");

	// read input file
	//
	Double_t x, y; // pulse time and height
	string sx, sy; // string type of pulse time and height
	string line;
	Int_t N = 0;

	while (1) {
		// show process
		if (N % 100 == 0) cerr << setw(3) << N << endl;

		// open file
		TString fin_name = Form("%s/C3-01-%05d.csv", fin_dir.Data(), N);
		ifstream fin(fin_name.Data());

		// check whether file exists or not
		if (!fin) {
			cerr << "Number of files: " << N << endl;
			break;
		}

		// read waveform
		Vpeak = 1e9;
		Sum   = 0;
		while (getline(fin, line)) {
			if (line.empty() || (line[0] == '#')) {
				continue;
			}

			//------ read .csv format file -----------------------------------
			sx = line.substr(0, line.find(","));             // read 1st value
			sy = line.substr(line.find(",")+1, line.size()); // read 2nd value

			x = atof(sx.c_str()); // convert string to Double_t
			y = atof(sy.c_str());
			//----------------------------------------------------------------

			// calculate sum
			Sum += y;

			// get minimum height
			if (y < Vpeak)
			Vpeak = y;
		}
		EventID = N;
		Sum   *= -1;
		Vpeak *= -1;

		tree -> Fill();
		N++;
	}
	cout << endl
	<< "output = " << fout_name << endl
	<< "# of filled events = " << tree -> GetEntries() << endl
	<< endl;

	// write tree object to output root file
	tree -> Write();
	fout -> Close();

	return 0;  ///////////****\(^o^)/***///////////
}

#ifndef __CINT__
Int_t main(Int_t argc, char** argv) {
	TROOT root("root", "root");
	return root_make_tree();
}
#endif
