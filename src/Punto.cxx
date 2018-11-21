#include <Riostream.h>
#include "TObject.h"
#include "TMath.h"
#include "Punto.h"


ClassImp(Punto)

//________________________________________________________________________
Punto::Punto():TObject(),
 fX(0.),
 fY(0.),
 fZ(0.){
   // default constructor
}


//___________________________________________________________________________
Punto::Punto(Double_t X, Double_t Y, Double_t Z):TObject(),
 fX(X),
 fY(Y),
 fZ(Z){
	//standard constructor 
}	     

//___________________________________________________________________________
Punto::~Punto()	 {
  // destructor
}

Double_t Punto::GetR() const {
	return TMath::Sqrt(fX*fX + fY*fY);
}

Double_t Punto::GetPhi() const {
	return TMath::ATan2(fY, fX);
}

Double_t Punto::GetTh() const{
	return TMath::ATan2(fZ, GetR());
}

