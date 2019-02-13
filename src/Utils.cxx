#include "Utils.h"
#include "Punto.h"

#include <iostream>
#include <vector>

#include "TMath.h"
#include "TString.h"
#include "TFile.h"
#include "TSystem.h"

//Initialization of this static data member at compile time
TString Utils::searchDirs[] = {TString("Trees"),TString("Data")};

Double_t Utils::ThetaFromEta(const Double_t& eta){
	return 2.*TMath::ATan(TMath::Exp(-eta));
}

Double_t Utils::EtaFromTheta(const Double_t& theta){
	return -TMath::Log(TMath::Tan(theta/2.));
}

Double_t Utils::GetTheta(const Punto& p){
	Double_t r = TMath::Sqrt(p.GetX()*p.GetX() + p.GetY()*p.GetY());
	return TMath::ATan2(p.GetZ(), r);
}

Double_t Utils::GetPhi(const Punto& p){
	return TMath::ATan2(p.GetY(), p.GetX());
}

TFile* Utils::findAndOpenFile(const TString& fileName, TString* dirs, const size_t n){
	
	TFile* sourceFile = nullptr;

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
		std::cout<<"Error: could not open \""<<fileName<<"\"\n";
		return nullptr;
	} else std::cout<<"Opened \""<<fileName<<"\"\n";
	
	return sourceFile;
}
