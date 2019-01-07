#include <Riostream.h>
#include "TObject.h"
#include "Hit.h"


ClassImp(Hit)

Hit::Hit():TObject(),
	fZ(0.),
	fPhi(0.),
	label(0){
}

Hit::Hit(Double_t Z, Double_t Phi, Int_t Label):TObject(),
	fZ(Z),
	fPhi(Phi),
	label(Label){
}

Hit::Hit(const Hit& original) : TObject(),
	fZ(original.GetZ()),
	fPhi(original.GetPHI()),
	label(original.GetLabel())
{}

Hit::~Hit(){}

Double_t Hit::deltaPhi(const Hit& hit1, const Hit& hit2){
	return hit1.fPhi - hit2.fPhi;
}
