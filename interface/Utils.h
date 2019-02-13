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

//This was meant to be a namespace
class Utils{
	public:
		static Double_t ThetaFromEta(const Double_t& eta);
		static Double_t EtaFromTheta(const Double_t& theta);
		static Double_t GetTheta(const Punto& p);
		static Double_t GetPhi(const Punto& p);
		static TString searchDirs[];
		static TFile* findAndOpenFile(const TString& fileName, TString* dirs = searchDirs, const size_t n=2);
};

#endif
