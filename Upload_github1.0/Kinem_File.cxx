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

//generazione della pseudorapidità e della molteplicità

//apro il file
void Kinem_File::OpenFile(){

  TFile F("kinem.root"); //leggo il file kinem.root e prendo heta e hmul
  disteta = (TH1F*)F.Get("heta");
  disteta->SetDirectory(0);
  disteta->SetMinimum(0);
  dishmul = (TH1F*)F.Get("hmul");
  dishmul->SetDirectory(0); //lavori sul file mentre sei su hmul
  dishmul->SetMinimum(0);
  F.Close();

  //PSEUDORAPIDITA'
  xa=disteta->GetXaxis();
  b1=xa->FindBin(-2.); //restituisce il numero del bin
  b2=xa->FindBin(2.);
  xlow=xa->GetBinLowEdge(b1);
  xhig=xa->GetBinUpEdge(b2);
  n_bins=b2-b1+1; //numero di celle comprese tra b1 e b2

  //MOLTEPLICITA'
  xa=dishmul->GetXaxis();
  b1m=xa->FindBin(3); //restituisce il numero del bin
  b2m=xa->FindBin(52);
  xlowm=xa->GetBinLowEdge(b1m);
  xhigm=xa->GetBinUpEdge(b2m);
  n_binsm=b2m-b1m+1;
}


void Kinem_File::Pseudorapidita(Int_t Molt){
  TH1F* heta = new TH1F("heta","#eta distribution 2",n_bins,xlow,xhig);

  Int_t i=0;
  Int_t j=1;

  for(i=b1;i<=b2;i++){
    heta->SetBinContent(j++,disteta->GetBinContent(i));
  }

  Ps_rapidity = new Double_t[Molt];

  for(i=0; i<Molt; i++){
    Ps_rapidity[i]=heta->GetRandom();
  }

  delete heta;
//  delete Ps_rapidity;

}

Double_t Kinem_File::GetTeta(Int_t p) const{
  return 2*TMath::ATan(TMath::Exp(-(Ps_rapidity[p])));
}

void Kinem_File::Molteplicita(){
  TH1F* hmul = new TH1F("hmul","Mult distribution 2",n_binsm,xlowm,xhigm);
  Int_t i=0;
  Int_t j=1;

  for(i=b1m;i<=b2m;i++)hmul->SetBinContent(j++,dishmul->GetBinContent(i));
  mult = hmul->GetRandom();

  delete hmul;

}

 Kinem_File::~Kinem_File(){
   delete [] Ps_rapidity; // elete[] expressions that use global array deallocation functions
 }
