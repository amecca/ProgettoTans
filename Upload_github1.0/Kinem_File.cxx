#include "TMath.h"
#include "Kinem_File.h"
#include <Riostream.h>
#include "TRandom3.h"
#include "TH1F.h"
#include "TFile.h"
#include <TAxis.h>

ClassImp(Kinem_File)

Kinem_File::Kinem_File(): TObject(),
mult(0),
Ps_rapidity(NULL),
dishmul(NULL),
disteta(NULL),
xa(NULL)

{
}
// 01/01/19
//generazione della pseudorapidità e della molteplicità 

//__________________________________________________________
void Kinem_File::Pseudorapidita(Int_t Molt){
 

  Ps_rapidity = new Double_t[Molt];

  TFile F("kinem.root"); //leggo il file kinem.root e prendo heta e hmul
  disteta = (TH1F*)F.Get("heta");

  for(Int_t i=0; i<Molt; i++){
    Ps_rapidity[i]=disteta->GetRandom();
  }
  F.Close();
//  delete Ps_rapidity;

}
//___________________________________________________________
Double_t Kinem_File::GetTeta(Int_t p) const{
  return 2*TMath::ATan(TMath::Exp(-(Ps_rapidity[p])));
}
//___________________________________________________________
 void Kinem_File::Molteplicita(){

  TFile F("kinem.root"); //leggo il file kinem.root e prendo heta e hmul
  dishmul = (TH1F*)F.Get("hmul");
  mult = dishmul->GetRandom();
  F.Close();
}

 Kinem_File::~Kinem_File(){
   delete [] Ps_rapidity; // elete[] expressions that use global array deallocation functions
 }
