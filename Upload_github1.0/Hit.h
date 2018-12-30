#ifndef HIT_H
#define HIT_H

#include "TObject.h"

class Hit : public TObject
{

public:

Hit(); //costruttore di default
Hit(Double_t Z, Double_t Phi, Int_t Label); //costruttore standard

virtual ~Hit(); //distruttore

 Double_t GetZ() const {return fZ;}
 Double_t GetPHI() const {return fPhi;}
 Int_t GetLabel() const {return label;}

private:

Double_t fZ;
Double_t fPhi;
Int_t label;

ClassDef(Hit,1)
};


#endif
