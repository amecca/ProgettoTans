///////////////////////////////////////
// Macro to compile the TANS project //
// Author A.Mecca (UNITO)            // 
///////////////////////////////////////

#include <iostream>
#include <vector>

#include "TSystem.h"

using std::cout;
using std::string;
using std::vector;


void compile_simulation(bool run = false, bool debug = false){
		
	vector<string> macroNames = {
		"Punto.cxx",
		"Hit.cxx",
		"Kinem_File.cxx",
		"Direzione.cxx",
		"Intersezione.cxx",
		"Trackelet.cxx",
		//"Read_dat_simulation.cxx",
		"Simulazione.C",
		"Ricostruzione.C"
	};
	
	gSystem->SetBuildDir("bin", kTRUE);
	gSystem->AddIncludePath("-I./interface");
	//cout<<"-------\n"<<gSystem->GetIncludePath()<<"\n-------\n";

	for(size_t i = 0; i < macroNames.size(); i++){
		if(debug) cout<<"+++ Processing \""<<macroNames.at(i)<<"\" \t+++\n";
		gSystem->CompileMacro(("src/"+macroNames.at(i)).c_str(),"kg");
	}
	if(run)
		gROOT->ProcessLine("Simulazione()");
}


