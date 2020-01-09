#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

//int rm{
int main(){
	//const std::string filepath = argv[1];
	const std::string filepath = "hoge.dat";
	if (int s = remove("hoge.dat") != 0){
	cout << "can't remove it" << endl << endl;
	//cout << "finish at -" << s << endl;
	}
	//remove(filepath.c_str());
	return 0;
}
