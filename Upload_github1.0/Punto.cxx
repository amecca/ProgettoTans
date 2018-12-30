#include "Punto.h"
#include <Riostream.h>
#include "TObject.h"
#include "TRandom3.h"

ClassImp(Punto)

Punto::Punto(): TObject(),
  fX(0.),
  fY(0.),
  fZ(0.)
{
}


void Punto::Point(Float_t RMS_XY,Float_t RMS_Z, Bool_t EstraiVert){

   Double_t ZFissa[]={-15.,-12.5,-10.,-7.5,-5.,-2.5,0.,2.5,5.,7.5,10.,12.5,15.};
   Int_t index_fisso=gRandom->Rndm()*13;
   fX=gRandom->Gaus(0.,RMS_XY);
   fY=gRandom->Gaus(0.,RMS_XY);
   
   if(EstraiVert==1)fZ=gRandom->Gaus(0.,RMS_Z);
   else fZ=ZFissa[index_fisso];

 }


 Punto::~Punto(){}
