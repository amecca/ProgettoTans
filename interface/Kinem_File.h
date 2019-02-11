//-------------------------------------------------------
// Interfaccia col file kinem.root, in cui sono presenti 
// grafici contenenti le distribuzioni desiderate di molteplicità
// del vertice e pseudorapidità iniziale per le particelle
//-------------------------------------------------------
#ifndef KINEM_FILE_H
#define KINEM_FILE_H

#include "TObject.h"
#include "TFile.h"
#include "TCanvas.h"

class Kinem_File:public TObject
{
	public:
		Kinem_File();

		virtual ~Kinem_File();

		UInt_t Molteplicita(Int_t type) const;
		//0 -> heta (kinem.root) 1 -> Uniform(0,50) 2 -> {5, 10, 15, 20, 25, 30, 35, 40, 45, 50}
		UInt_t Rumore(Int_t type, UInt_t mean = 0, UInt_t sigma = 3)  const;
		//1 -> Gaus(mean, sigma) 2->{0, 1, 2, 5, 8, 12, 16, 20, 25, 30}
		Double_t Pseudorapidita() const;
		Double_t GenTheta() const;
		Double_t GenPhi() const;

	private:
	
		TFile* sourceFile;
  	TH1F *dishmul;
		TH1F *disteta;

		ClassDef(Kinem_File,1);
};

#endif
