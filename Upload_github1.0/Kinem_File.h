#ifndef KINEM_FILE_H
#define KINEM_FILE_H

#include <TObject.h>
#include "TCanvas.h"

class Kinem_File:public TObject
{
 public:
  Kinem_File();

  virtual ~Kinem_File();

  void OpenFile();
  void Pseudorapidita(Int_t Molt);
  void Molteplicita();

  Double_t GetTeta(Int_t k) const;
  Double_t GetM()const {return mult;}

 private:

  Double_t *Ps_rapidity;
  Int_t mult;
  Int_t n_bins,n_binsm;
  Double_t xlow,xhig,xlowm,xhigm;
  TAxis *xa;
  Int_t b1,b2, b1m,b2m;
  TH1F *dishmul, *disteta;


  ClassDef(Kinem_File,1);
};

#endif
