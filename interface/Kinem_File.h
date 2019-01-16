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

  UInt_t Molteplicita();
  Double_t Pseudorapidita() const;
  Double_t GenTheta() const;
  Double_t GenPhi() const;
  Double_t GetTeta(const Double_t& eta) const;
  

 private:
	
	TFile* sourceFile;
    TH1F *dishmul;
	TH1F *disteta;


  ClassDef(Kinem_File,1);
};

#endif
