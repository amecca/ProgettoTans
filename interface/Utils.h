#ifndef UTILS_H
#define UTILS_H

#include <vector>

#include "TMath.h"
#include "TString.h"
#include "TFile.h"
#include "TSystem.h"

namespace Utils{

	Double_t ThetaFromEta(const Double_t& eta){
  	return 2*TMath::ATan(TMath::Exp(-eta));
	}
	
	TString searchDirs[] = {TString("Trees"),TString("Data")};
	TFile* findAndOpenFile(const TString& fileName, TString* dirs = searchDirs, const size_t n=2){
		TFile* sourceFile;
		for(size_t k=0; k<n; k++){
			if (!gSystem->AccessPathName(dirs[k]+'/'+fileName+".root",kFileExists)){
				sourceFile = new TFile((dirs[k]+'/'+fileName+".root").Data());
				break;
			}
			else if (!gSystem->AccessPathName(dirs[k]+'/'+fileName,kFileExists)){
				sourceFile = new TFile((dirs[k]+'/'+fileName).Data());
				break;
			}
		}
		if(!gSystem->AccessPathName(fileName+".root",kFileExists))
			sourceFile = new TFile((fileName+".root").Data());
		else if(!gSystem->AccessPathName(fileName,kFileExists))
			sourceFile = new TFile(fileName.Data());
		//cout<<"Error: \""<<fileName<<"\" not found\n";
		
		if(sourceFile->IsZombie() || !(sourceFile->IsOpen())){
			cout<<"Error: could not open \""<<fileName<<"\"\n";
			return nullptr;
		} else cout<<"Opened \""<<fileName<<"\"\n";
		
		return sourceFile;
	}
}

#endif
