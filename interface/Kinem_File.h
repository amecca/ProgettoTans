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

  void Pseudorapidita(Int_t Molt);
  void Molteplicita();

  Double_t GetTeta(Int_t k) const;
  Double_t GetM()const {return mult;}

 private:
	
	TFile* sourceFile;
  Double_t *Ps_rapidity;
  Int_t mult;
  TH1F *dishmul, *disteta;


  ClassDef(Kinem_File,1);
};

#endif
