#include "TMath.h"
#include "Kinem_File.h"
#include <Riostream.h>
#include "TRandom3.h"
#include "TH1F.h"
#include "TFile.h"
#include <TAxis.h>

ClassImp(Kinem_File)

Kinem_File::Kinem_File(): TObject()
{
	sourceFile = new TFile("Data/kinem.root"); //leggo il file kinem.root e prendo heta e hmul
	if(! sourceFile->IsOpen()) exit(1); //Se non trova il file esce dal programma
	dishmul = (TH1F*)sourceFile->Get("hmul");
	disteta = (TH1F*)sourceFile->Get("heta"); //distribuzione in pseudorapidità
}
// 01/01/19
//generazione della pseudorapidità e della molteplicità 

//__________________________________________________________
Double_t Kinem_File::Pseudorapidita() const{
	return disteta->GetRandom();
}

Double_t Kinem_File::GenTheta() const{
	return 2*TMath::ATan(TMath::Exp(-(disteta->GetRandom())));
}

Double_t Kinem_File::GenPhi() const{
  return 2*TMath::Pi()*(gRandom->Rndm());
}
//___________________________________________________________
Double_t Kinem_File::GetTeta(const Double_t& eta) const{
  return 2*TMath::ATan(TMath::Exp(-eta));
}
//_________________________________________________________
UInt_t Kinem_File::Molteplicita(Int_t type){
	if(type==0) 
		return (UInt_t) dishmul->GetRandom();
	else if(type==1)
		return (UInt_t) (gRandom->Rndm()*50);
	/*else if(type == 2){
		UInt_t possibili[10] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
		return possibili[(Uint_t)(gRandom->Rndm() *10 - 0.5)];
	}*/
	else return 0;
}

Kinem_File::~Kinem_File(){
	delete disteta;
	delete dishmul;
   sourceFile->Close();
   delete sourceFile;
}
