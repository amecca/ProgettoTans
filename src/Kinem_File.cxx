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
	disteta = (TH1F*)sourceFile->Get("heta");
}
// 01/01/19
//generazione della pseudorapidità e della molteplicità 

//__________________________________________________________
void Kinem_File::Pseudorapidita(Int_t Molt){

  Ps_rapidity = new Double_t[Molt];

  for(Int_t i=0; i<Molt; i++){
    Ps_rapidity[i]=disteta->GetRandom();
  }
//  delete Ps_rapidity;

}
//___________________________________________________________
Double_t Kinem_File::GetTeta(Int_t p) const{
  return 2*TMath::ATan(TMath::Exp(-(Ps_rapidity[p])));
}
//___________________________________________________________
void Kinem_File::Molteplicita(){
	mult = dishmul->GetRandom();
}

Kinem_File::~Kinem_File(){
   delete [] Ps_rapidity; // elete[] expressions that use global array deallocation functions
   sourceFile->Close();
   delete sourceFile;
}
