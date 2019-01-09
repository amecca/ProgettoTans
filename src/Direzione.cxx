#include "Direzione.h"
#include <Riostream.h>
#include "TObject.h"
#include "TRandom3.h"
#include "KInem_File.h"

ClassImp(Direzione)

Direzione::Direzione(): TObject()
 
{
}

Direzione::Direzione(Double_t th, Double_t ph): TObject(),
  fph(ph),
  fth(th)
{
}

Direzione::Direzione(const Kinem_File * const kinem){
	if(!kinem){
		cout<<"Error: nullptr for Kinem_File in Direzione\n";
		exit(1);
	}
	fph = kinem->GenPhi();
	fth = kinem->GenTheta();
}


void Direzione::CalcolaA(Float_t RMS_XY,Float_t RMS_Z, Bool_t EstraiVert){

   fX = gRandom->Gaus(0.,RMS_XY);
   fY = gRandom->Gaus(0.,RMS_XY);
   
   if(EstraiVert) fZ = gRandom->Gaus(0.,RMS_Z);
   else{
		Double_t ZFissa[] = {-15.,-12.5,-10.,-7.5,-5.,-2.5,0.,2.5,5.,7.5,10.,12.5,15.};
		Int_t index_fisso = (Int_t)(gRandom->Rndm()*13);
    fZ = ZFissa[index_fisso];
  }
}



 Direzione::~Direzione(){}
