#include <Riostream.h>
#include "TObject.h"
#include "TMath.h"
#include "Hit.h"


ClassImp(Hit)

Hit::Hit():TObject(),
	fZ(0.),
	fPhi(0.),
	label(0){
}

Hit::Hit(Double_t Z, Double_t Phi, Int_t Label):TObject(),
	fZ(Z),
	label(Label){
	while (Phi > 2*TMath::Pi()) Phi -= 2*TMath::Pi();
	while (Phi <= 0) Phi += 2*TMath::Pi();
	fPhi = Phi;
}

Hit::Hit(const Hit& original) : TObject(),
	fZ(original.GetZ()),
	fPhi(original.GetPHI()),
	label(original.GetLabel())
{}

Hit::~Hit(){}

Double_t Hit::deltaPhi(const Hit& hit1, const Hit& hit2){
	return deltaPhi(&hit1, &hit2);
}

Double_t Hit::deltaPhi(const Hit* hit1, const Hit* hit2){
	Double_t phi1 = hit1->GetPHI();
	Double_t phi2 = hit2->GetPHI();
	
	double result = phi1 - phi2;
    while (result > TMath::Pi()) result -= 2*M_PI;
    while (result <= -TMath::Pi()) result += 2*M_PI;
    return fabs(result);
}
