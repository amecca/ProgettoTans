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

Hit::~Hit(){}
