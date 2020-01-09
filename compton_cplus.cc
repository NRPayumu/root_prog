#include <stdio.h>
#include <stdlib.h>  /*randの使用*/
#include <time.h>    /*randの初期化に必要*/
#include <math.h>

#include<iostream>
#include<fstream>
#include "TROOT.h" //FormにROOT系ヘッダーが必要

using namespace std;

double compton_cal(double E,double phi){
	//const double E=0.662e+6;
	const double c=3.0e+8;
	const double M=9.1e-31;
	const double el=1.6e-19;
	const double h=6.62e-34;
	return E*el/(1+(M*c*c/(E*el*(1-cos(phi*M_PI/180)))))/el/1000/1000;
}

int compton(){
	const double E=0.662e+6;
	int i;
	int r,r2;
	double count=10;
	srand(time(NULL));  /*乱数を初期化*/

	char output_file[20];
	sprintf(output_file,"out_rand_%.0f.dat",count);  //変数output_fileを""内に書き換え*/

	FILE *fp;
	fp = fopen(output_file,"w");  /*ファイル名output_fileをwモード(書込み)で開く*/

	for(i = 0; i <count; i++){
		r=rand()%181;
		double Ee = compton_cal(E,r);
		fprintf(fp,"%fMeV\n",Ee);  /*ファイルfpに入力*/
	}
	fclose (fp);
	printf("%s\n",output_file);
	return 0;
}

int compton_cplus_standardout(){
	const double E=0.662e+6;
	int i;
	int r,r2;
	double count=10;
	srand(time(NULL));  /*乱数を初期化*/
	for(i = 0; i <count; i++){
		r=rand()%181;
		double Ee = compton_cal(E,r);
		cout << Ee << "MeV" << endl;  //標準出力　　　 //実効時  >ファイル名
		//cerr << Ee << "MeV" << endl;	//標準エラー出力 //実効時 2>ファイル名
	}
	return 0;
}

int compton_cplus(){
	const double E=0.662e+6;
	int i;
	int r,r2;
	double count=10;
	srand(time(NULL));  /*乱数を初期化*/

	string output_file;

	output_file = Form("out_rand_%.0f.dat",count); //
	ofstream fout(output_file);
	for(i = 0; i <count; i++){
		r=rand()%181;
		double Ee = compton_cal(E,r);
		fout<< Ee << "MeV" << endl;		//ファイル出力
	}
	printf("%s\n",output_file.c_str());
	return 0;
}

int main(){
	compton();  										// fprintf
	//compton_cplus_standardout();		// cout cerr
	//compton_cplus();								// ofstream fout
	return 0;
}
