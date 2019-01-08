#ifndef INTERSEZIONE_H
#define INTERSEZIONE_H

#include <TObject.h>

class Intersezione: public TObject
{

public:

Intersezione();
Bool_t HitTrovato;

virtual ~Intersezione();

 void SetPoint(Double_t LUNG_BEAM,Double_t X0,Double_t Y0, Double_t Z0, Double_t R, Double_t PHI0,Double_t THETA0,Double_t thp,Double_t php);
 void Multiple_scattering(Double_t LUNG_BEAM,Double_t X0,Double_t Y0, Double_t Z0, Double_t R, Double_t PHI0,Double_t THETA0,Double_t thp,Double_t php);
 void Rumore(Double_t LUNG_BEAM);

 Double_t GetX() const{return x;}
 Double_t GetY() const{return y;}
 Double_t GetZ() const{return z;}
 Double_t GetPHI() const {return PHI;}
 Double_t GetTHETA() const {return THETA;}

 private:

 Double_t x,y,z;
 Double_t PHI;
 Double_t THETA;
// Bool_t zSmearAccettabile;

 ClassDef(Intersezione,1)
};


#endif
