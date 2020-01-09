const int SOLUTE=2;
const int MATERIAL=SOLUTE+1;
string solvent="LAB";
string solute[SOLUTE]={"bisMSB","PPO"};
double density[MATERIAL]={0.852, 1.3, 1.06};  // g/cm3
double A[MATERIAL]={246, 310, 221};
double hope_per[SOLUTE]={3.0, 0.030};  // g/l  1L=1000cm3
//double hope_per[SOLUTE]={0.003, 0.00003};  // g/cm3
double percent[MATERIAL];
double weight_per[MATERIAL];

double amount_liquid=0;
double amount_litter=0;
double result_dens;

void cal_dens(){
	amount_liquid=0; //mass [g]
	amount_litter=0; //L [cm3]
	amount_liquid+=density[0]*1000;
	amount_litter+=1000;
	for(int i=0; i<SOLUTE; i++){
		amount_liquid+=hope_per[i];
		amount_litter+=hope_per[i]/density[i+1];
	}
	result_dens=amount_liquid/amount_litter;
	cout << result_dens << endl;
	//amount_liquid = amount_liquid;
	amount_litter = amount_liquid*0.995/result_dens + amount_liquid*0.005/7.9;
	//amount_litter = amount_liquid*0.990/result_dens + amount_liquid*0.010/7.9;
	result_dens=amount_liquid/amount_litter;
	cout << result_dens << endl;
}
void cal_percent(){
	amount_liquid+=density[0]*1000;
	for(int i=0; i<SOLUTE; i++){
		amount_liquid+=hope_per[i];
	}
	weight_per[0]=density[0]*1000/amount_liquid*100;
	for(int i=1; i<MATERIAL; i++){
		weight_per[i]=hope_per[i-1]/amount_liquid*100;
	}
	for(int i=0; i<MATERIAL; i++){
		cout << weight_per[i] << endl;
	}
	cal_dens();
}
