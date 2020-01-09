// $ root -b -l -q 'geant_data_cut.cc("data")'
//  -b batch mode  -l no interpreter  -q quit at finish

//

void familytree_test2(const std::string ifile){
	#include <iostream>
	#include <sstream>
	#include <fstream>
	#include <string.h>
	#include <iomanip>

	// open file
	string fin_dir = "data/";
	string fout_dir = "data_cut/";
	stringstream ss;
	ss.str("");
	//ss<<fin_dir.c_str()<<ifile.c_str()<<".dat";
	//cout<<"File_in->"<<fin_dir.c_str()<<ifile.c_str()<<".dat"<<endl;
	ss <<ifile.c_str()<<".dat";
	cout<<"File_in ->"<<ifile.c_str()<<".dat"<<endl;
	ifstream fin(ss.str());

	if (!fin) {
		cerr << "Error: don't exist such a data file, " << ss.str() << " !" << endl;
		//return -1;
		//break;
	}

	int EventID;
	int ParentID;
	int TrackID;
	string ParticleName;
	double Positionx, Positiony, Positionz;
	//double PosPositionx, PosPositiony, PosPositionz;
	double VertexKineticEnergy;
	double KineticEnergy;
	double TotalEnergyDeposit;
	double Time;
	string PreProcessName;
	string PostProcessName;
	string OriginedProcessName;

	string ParName;

	//const int EventNumber =10000;
	const int TrackNumber =20000;
	//int EveID[EventNumber] = {};

	int ParID[TrackNumber] = {};
	int daughter_number[TrackNumber] = {};
	int stair[TrackNumber] = {};

	int MaxParID=0;
	int MaxTraID=0;
	int Maxstair=0;

	string line;

	ss.str("");
	ss<<"data_hoge";
	ss<<".dat";
	cout<<"File_out->"<<ss.str()<<" :data"<<endl;
	ofstream dout(ss.str());

	while (getline(fin, line)) {
		if (line.empty() || (line[0] == '#')) {
			continue;
		}

		if (!(istringstream(line) >> EventID >> ParentID
		//>> ParName
		>> TrackID
		>> ParticleName
		>> Positionx >> Positiony	>> Positionz
		//>> PosPositionx >> PosPositiony	>> PosPositionz
		>> VertexKineticEnergy
		>> KineticEnergy
		>> TotalEnergyDeposit
		>> Time
		>> PreProcessName
		>> PostProcessName
		>> OriginedProcessName)) {
			cerr << "Error: badly formatted data line: " << line << endl;
			//return -1;
			break;
		}

		if (EventID==0){
			//if (Positionx == 5000 || Positionx ==-5000 || Positiony == 5000 || Positiony ==-5000 || Positionz == 500 ||Positionz ==-500){
			//if (OriginedProcessName != "")
			//if (OriginedProcessName == "muonNuclear"){
			if (PreProcessName == "UserLimit"){
				//if (PreProcessName == "UserLimit" || PostProcessName != "hadElastic"){
				//if (PreProcessName != "hadElastic" || PostProcessName != "hadElastic")
				//if (PreProcessName == "nKiller"   || PostProcessName == "nKiller"){
				//if (ParticleName == "neutron"){
				ParID[TrackID] = ParentID;

				daughter_number[ParentID]++;
				stair[TrackID] = stair[ParentID] +1;

				dout << setw(2) << stair[TrackID]      << " "
				 		 << setw(5) << ParentID            << " "
						 << setw(5) << TrackID             << " "
						 << setw(12)<< OriginedProcessName << " " << endl;

				if (MaxParID < ParentID)MaxParID=ParentID;
				if (MaxTraID < TrackID)MaxTraID=TrackID;
				if (Maxstair < stair[TrackID])Maxstair = stair[TrackID];
				//}
			}
		}
		if (EventID==1)break;
	}

	cout << endl;

	ss.str("");
	//ss<<fout_dir.c_str()<<ifile.c_str();
	ss<<"max_hoge";
	ss<<".dat";
	cout<<"File_out->"<<ss.str()<<" :max_value"<<endl;
	ofstream mout(ss.str());

	//cout << MaxParID << " " << MaxTraID << " " << Maxstair << endl;
	mout << MaxParID << " " << MaxTraID << " " << Maxstair << " " ;
	int stair_order[Maxstair+1][MaxTraID+1];
	int stair_order_i[Maxstair+1];
	for (int s = 0; s<Maxstair+1; s++){
		for (int t = 0; t<MaxTraID+1; t++){
			stair_order[s][t]=0;
		}
		stair_order_i[s]=0;
	}
	int sta;
	int sta_i;
	int yet_sta[MaxTraID+1];
	for (int t = 0; t<MaxTraID+1; t++){
		yet_sta[t]=0;
	}

	stair_order[1][0] = 1;
	stair_order_i[1]++;
	yet_sta[1] = 1;
	for (int s = 2; s<Maxstair+1; s++){
		for (int i = 0; i<stair_order_i[s-1]; i++){
			for (int t = stair_order[s-1][i];t<MaxTraID+1; t++){
				if (ParID[t] == stair_order[s-1][i] && yet_sta[t] == 0){
					sta_i=stair_order_i[s];
					stair_order[s][sta_i]=t;
					stair_order_i[s]++;
					yet_sta[t] = 1;
				}
			}
		}
	}
	int max_stair_i=0;
	for (int s = 1; s<Maxstair+1; s++){
		if (max_stair_i < stair_order_i[s]){
			max_stair_i = stair_order_i[s];
		}
	}
	mout << max_stair_i << endl;
	for (int s = 1; s<Maxstair+1; s++){
		mout << setw(5) << stair_order_i[s]            << " "
		     << setw(5) << stair_order_i[Maxstair+1-s] << endl;
	}
	mout.close();

	/*ss.str("");
	//ss<<fout_dir.c_str()<<ifile.c_str();
	ss<<"stair_order_hoge";
	ss<<".dat";
	cout<<"File_out->"<<ss.str()<<" :stair_order"<<endl;
	ofstream sout(ss.str());

	for (int s = 1; s<Maxstair+1; s++){
		for (int i = 0; i<stair_order_i[s]; i++){
			sout << setw(2) << s                  << " "
					 << setw(5) << i+1                << " "
					 << setw(5) << stair_order[s][i]  << " " ;
			int t = stair_order[s][i];
			sout << setw(5) << ParID[t]           << " "
					 << setw(5) << daughter_number[t] << " "
					 << endl;
		}
	}
	sout.close();*/

	ss.str("");
	//ss<<fout_dir.c_str()<<ifile.c_str();
	ss<<"stair_order_hoge-r";
	ss<<".dat";
	cout<<"File_out->"<<ss.str()<<" :stair_order"<<endl;
	ofstream rout(ss.str());

	for (int s = Maxstair; s>=1; s--){
		for (int i = 0; i<stair_order_i[s]; i++){
			rout << setw(2) << s                  << " "
					 << setw(5) << i+1                << " "
					 << setw(5) << stair_order[s][i]  << " " ;
			int t = stair_order[s][i];
			rout << setw(5) << ParID[t]           << " "
					 << setw(5) << daughter_number[t] << " "
					 << endl;
		}
	}

	rout.close();

	ss.str("");
	ss <<"max_hoge"<<".dat";
	//cout<<"File_in ->"<<ss.str()<<" :max_value"<<endl;
	ifstream min(ss.str());
	getline(min, line);
	int mp,mt,ms,msi;
	if (!(istringstream(line) >> mp >> mt
	>> ms >> msi)) {
		cerr << "Error: badly formatted data line: " << line << endl;
		//return -1;
		return 0;
	}

	int dau_i[mt+1];
	for (int t = 0; t<MaxTraID+1; t++){
		dau_i[t]=0;
	}

	ss.str("");
	ss <<"stair_order_hoge-r"<<".dat";
	//cout<<"File_in ->"<<ss.str()<<" :stair_order"<<endl;
	ifstream sin(ss.str());

	ss.str("");
	//ss<<fout_dir.c_str()<<ifile.c_str();
	ss<<"last_hoge";
	ss<<".dat";
	cout<<"File_out->"<<ss.str()<<" :last"<<endl;
	ofstream lout(ss.str());

	int st,st_i,tr,pa,da_n;
	int sin_tr[mt+1],sin_pa[mt+1],sin_da[mt+1];
	int sin_order[mt];
	int o=0;
	while (getline(sin, line)) {
		if (line.empty() || (line[0] == '#')) {
			continue;
		}

		if (!(istringstream(line) >> st >> st_i
		>> tr >> pa >> da_n)) {
			cerr << "Error: badly formatted data line: " << line << endl;
			//return -1;
			break;
		}
		sin_order[o]=tr;
		sin_pa[tr]=pa;
		sin_da[tr]=da_n;

		if (da_n==0)dau_i[tr]++;
		dau_i[pa]+=dau_i[tr];
		//cout << tr << " ";
		//cout << dau_i[tr] << endl;
		o++;
	}
	int a=1;
	int cnt=0;
	getline(min, line);
	istringstream(line) >> msi;
	//cout << mt << endl;
	for(int o=0; o<mt; o++){
		tr=sin_order[mt+o-a];
		lout << setw(5) << tr         << " "
				 << setw(5) << sin_pa[tr] << " "
				 << setw(5) << sin_da[tr] << " "
				 << setw(5) << dau_i[tr]  << endl;
		cnt++;
		if(cnt==msi){
			cnt=0;
			a+=msi;
			getline(min, line);
			istringstream(line) >> msi ;
			a+=msi;
		}
	}
	int d1 = dau_i[1];
	min.close();
	lout.close();

	int my_row[ms+1];
	int yet_sell[ms+1][d1+1];
	stringstream each_row_s[d1+1];
	for (int s = 0; s<=ms; s++){
		my_row[s] = 0;
		for (int t = 0;t<=d1; t++){
			yet_sell[s][t]=0;
			if (s==0)each_row_s[t].str("");
		}
	}

	ss.str("");
	ss <<"max_hoge"<<".dat";
	ifstream min2(ss.str());
	getline(min2, line);
	istringstream(line) >> mp >> mt
	>> ms >> msi;
	ss.str("");
	ss <<"last_hoge"<<".dat";
	ifstream lin(ss.str());

	int da_i;
	int pa_bef  = 0;
	int now_row = 0;
	int da_i_bef = 1;
	int s = 0;
	int row[mt+1];
	for(int t = 0; t <= mt; t++)row[t] = 0;

	getline(min2, line);
	istringstream(line) >> msi ;
	getline(lin, line);
	istringstream(line) >> tr  >> pa
											>> da_n>> da_i;
	yet_sell[s][0] = 1;
	s++;

	cnt = 0;
	getline(min2, line);
	istringstream(line) >> msi ;
	for(int o = 1; o < mt; o++){
		getline(lin, line);
		istringstream(line) >> tr  >> pa
												>> da_n>> da_i;
		if(pa != pa_bef){
			now_row = row[pa];
			row[tr] = now_row;
			yet_sell[s][now_row] = 1;
		} else {
			for(int ys2 = 1; ys2 < da_i_bef; ys2++)
				yet_sell[s][now_row + ys2] = 2;
			now_row += da_i_bef;
			row[tr] = now_row;
			yet_sell[s][now_row] = 3;
		}
		cnt++;
		if(cnt == msi){
			cnt = 0;
			getline(min2, line);
			istringstream(line) >> msi ;
			s++;
		}
		pa_bef = pa;
		da_i_bef = da_i;
	}
	lin.close();

	ss.str("");
	ss <<"last_hoge"<<".dat";
	ifstream lin2(ss.str());

	ss.str("");
	ss<<fout_dir.c_str()<<ifile.c_str();
	//ss<<"_PrePos";
	//ss<<"_PostPos";
	//ss<<"_particle";
	//ss<<"_pre-process";
	//ss<<"_pos-process";
	//ss<<"_org-process";
	//ss<<"_nKiller";
	ss<<"_familytree";
	ss<<"_trackID";
	//ss<<"_posiD";
	ss<<".dat";
	cout<<"File_out->"<<ss.str()<<endl;
	ofstream fout(ss.str());
	/*
		TString fin_dir   = "data";
		//TString fin_name = Form("%s/%s.dat", fin_dir.Data(), ifile);
		TString fin_name = Form("%s.dat", ifile);
		ifstream fin(fin_name.Data());
		TString fout_name = "data_cut/data.dat";

		ofstream fout(fout_name.Data());
		*/
	for (int s = 0; s <ms; s++){
		for (int s_i=0; s_i<d1; s_i++){
			if (yet_sell[s][s_i] == 0){
				each_row_s[s_i] << setw(6) << "      ";
			}
			if (yet_sell[s][s_i] == 1){
				getline(lin2, line);
				istringstream(line) >> tr  >> pa
														>> da_n>> da_i;
				each_row_s[s_i] << "_" << setw(5) << tr <<"";
			}
			if (yet_sell[s][s_i] == 2){
				each_row_s[s_i] << setw(6) << "|     ";
			}
			if (yet_sell[s][s_i] == 3){
				getline(lin2, line);
				istringstream(line) >> tr  >> pa
														>> da_n>> da_i;
				each_row_s[s_i] << "L" << setw(5) << tr <<"";
			}
		}
	}
	for(int s_i=0; s_i<d1; s_i++){
		fout<< each_row_s[s_i].str()
				<< endl;
	}
	//cout << endl;
	fout.close();
	//return 0;
}
