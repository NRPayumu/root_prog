/* root_tree_control.mac */

// $ root root_File_name.root
// root[] .x root_tree_graph.mac

void tree_control(){
	////////canvasの準備////////
	TCanvas *c1 = new TCanvas("c1", "canvas", 600, 400);
	//gPad->SetLogy();
	gStyle->SetOptStat(1000001011); //凡例の設定

	////////rootファイルの操作////////
	//.ls

  ////////treeの操作////////
	//tree->Print();  //treeの中身を表示
	//tree->Draw("Value:point","","");  //Drow("対象","条件","オプション")
	//tree->Draw("Value:point","EventID<=1000","l"); //1000イベントだけプロット //lineで点をつなぐ
	tree->Draw("Q","","");        //Q値のヒストグラム
	//tree->Draw("Q:Qtail","EventID<=1000","");  //PSD
	//tree->Draw("Q>>h(100,0,10000)")       //histgram 分割数 最小値 最大値
}
