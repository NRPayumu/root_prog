//$ root file_name.root
//root[] .x tree_read.cc

void tree_read(){
	double Livetime;
	double livetime_total=0;
	tree->SetBranchAddress("LiveTime",&Livetime);
	const Int_t N = tree->GetEntries();

	for (Int_t ientry = 0; ientry < N; ientry++) {
		tree->GetEntry(ientry);
		livetime_total+=Livetime;
	}
	printf("Livetime:%f\n", livetime_total);
}
