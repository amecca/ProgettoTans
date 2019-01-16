#include "Punto.h"
#include <Riostream.h>
#include "TObject.h"
#include "TRandom3.h"
#include "TMath.h"

ClassImp(Punto)

Punto::Punto(): TObject(),
  fX(0.),
  fY(0.),
  fZ(0.)
{
}

Punto::Punto(Double_t x, Double_t y, Double_t z): TObject(),
  fX(x),
  fY(y),
  fZ(z)
{
}


void Punto::Generate(Float_t RMS_XY,Float_t RMS_Z, Bool_t EstraiVert){

   fX = gRandom->Gaus(0.,RMS_XY);
   fY = gRandom->Gaus(0.,RMS_XY);
   
   if(EstraiVert) fZ = gRandom->Gaus(0.,RMS_Z);
   else{
		Double_t ZFissa[] = {-15.,-12.5,-10.,-7.5,-5.,-2.5,0.,2.5,5.,7.5,10.,12.5,15.};
		Int_t index_fisso = (Int_t)(gRandom->Rndm()*13);
    fZ = ZFissa[index_fisso];
  }
}

Double_t Punto::GetPhi() const{
	return TMath::ATan2(fY,fX);
}

 Punto::~Punto(){}
