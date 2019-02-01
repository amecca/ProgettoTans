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

		UInt_t Molteplicita(Int_t type);
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
