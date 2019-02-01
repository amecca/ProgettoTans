//-------------------------------------------------------
// Raccolta di funzioni utili che vengono chiamate da
// più classi e/o macro, opportunamente raccolte nel
// namespace Utils::
//-------------------------------------------------------
#ifndef UTILS_H
#define UTILS_H

#include "Punto.h"

#include <vector>

#include "TMath.h"
#include "TString.h"
#include "TFile.h"
#include "TSystem.h"

namespace Utils{

	Double_t ThetaFromEta(const Double_t& eta){
  	return 2.*TMath::ATan(TMath::Exp(-eta));
	}
	
	Double_t EtaFromTheta(const Double_t& theta){
  	return -TMath::Log(TMath::Tan(theta/2.));
	}
	
	Double_t GetTheta(const Punto& p){
		Double_t r = TMath::Sqrt(p.GetX()*p.GetX() + p.GetY()*p.GetY());
		return TMath::ATan2(p.GetZ(), r);
	}
	
	Double_t GetPhi(const Punto& p){
		return TMath::ATan2(p.GetY(), p.GetX());
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
