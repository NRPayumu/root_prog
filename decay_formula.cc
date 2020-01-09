#include <stdio.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>   // for std::istringstream

#include "TROOT.h" //FormにROOT系ヘッダーが必要

using namespace std;

//./a.out InFileName outFileName NNuclide

const double Na=6.022e23;

double decay_quantity(int Suffix, double halflife, double Quantity){
	double Reduction_quantity = 0;
	if(halflife!=0)Reduction_quantity = Quantity * (1 - pow(0.5,1/halflife));
	return Reduction_quantity;
}

double unittime_halflife(double halflife, string unit){
	string u = unit;
	if(u == "y" ){
		u="d";
		halflife * 365.;
	}
	if(u == "d" ){
		u="h";
		halflife * 24;
	}
	if(u == "h" ){
		u="m";
		halflife * 60;
	}
	if(u == "m" ){
		u="s";
		halflife * 60;
	}
	return halflife;
}

//root -l -q 'decay_formula.cc("Decay_Th.dat","decay_mass_Th.dat",11)'
void decay_formula(string InFileName, string OutFileName, int NNuclide){
	const int NN = NNuclide;
	string  in_file= InFileName;
	string out_file= OutFileName;

	string Nuclide[NN];
	int mass_number[NN];
	double halflife[NN];
	string unit[NN];
	double mass[NN];

	int unit_time = 0;

	ifstream fin(in_file);
	if (!fin) {
		cerr << "Error: don't exist such a data file, " << in_file << " !" << endl;
		//return -1;
		//break;
		exit(1);
	}
	string line;
	/*while (getline(fin, line)) {
	if (line.empty() || (line[0] == '#')) {
	continue;
	}*/
	double Quantity[NN];
	double ChangeQuantity[NN];
	for(int i=0; i<NN;i++){
		getline(fin, line);
		if (!(istringstream(line) >> Nuclide[i] >> mass_number[i]
		 >> halflife[i]
		 >> unit[i]
		 >> mass[i])) {
			cerr << "Error: badly formatted data line: " << line << endl;
			//return -1;
			exit(1);
		}
		halflife[i] = unittime_halflife(halflife[i],unit[i]);
		Quantity[i] = mass[i] / mass_number[i] *Na; // mol=g/Z // N=mol*Na
	}
	//out_file = Form("out_%.0f.dat",Number); //
	ofstream fout(out_file);
	for(int passtime=1;passtime<=10000;passtime++){
		fout << setw(5) << passtime;
		for(int i=0;i<NN;i++){
			if(i==0){
				ChangeQuantity[i] = -decay_quantity(i, halflife[i], Quantity[i]);
			}else{
				ChangeQuantity[i] = -decay_quantity(i, halflife[i], Quantity[i])
														+decay_quantity(i-1, halflife[i-1],Quantity[i-1]);
			}
		}
		for(int i=0;i<NN;i++){
			Quantity[i] += ChangeQuantity[i];
			mass[i] = Quantity[i] / Na *mass_number[i];
			fout  << " " << setw(11) << mass[i];			//<< " g ";
		}
		//graph -> SetPoint(passtime-1, passtime-1, mass[1]);
		fout << endl;
	}
	printf("OutputFile->%s\n",out_file.c_str());
	//return 0;
	//exit(0);
}

int main(int argc, char *argv[]){
	string IFN =  argv[1];
	string OFN = argv[2];
	int NN = atoi(argv[3]);
	//cout << argv[1] << argv[2] << argv[3];
	decay_formula(IFN,OFN,NN);
	return 0;
}
