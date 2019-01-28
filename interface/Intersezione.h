#ifndef INTERSEZIONE_H
#define INTERSEZIONE_H

#include "TObject.h"
#include "Direzione.h"
#include "Punto.h"
#include "Hit.h"
class Intersezione: public TObject
{

public:

Intersezione();
Bool_t HitTrovato;

virtual ~Intersezione();

 //void SetPoint(Double_t LUNG_BEAM,Double_t X0,Double_t Y0, Double_t Z0, Double_t R, Direzione direz,Double_t thp,Double_t php);
Punto TrovaIntersezione(Double_t LUNG_BEAM,Punto pStart, Double_t R, Direzione direz);
Direzione Multiple_Scattering(const Direzione& direzStart, const Double_t& rms_theta);
Hit Rumore(Double_t LUNG_BEAM, Int_t label = -1);


 private:

 Double_t x,y,z;
 Double_t PHI;
 Double_t THETA;
// Bool_t zSmearAccettabile;

 ClassDef(Intersezione,1)
};


#endif
