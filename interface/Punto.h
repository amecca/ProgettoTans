#ifndef PUNTO_H
#define PUNTO_H

#include "TObject.h"

class Punto : public TObject
{
	public:
		Punto();
		Punto(Double_t X, Double_t Y, Double_t Z);

		virtual ~Punto();

		Double_t GetX() const {return fX;} 
		Double_t GetY() const {return fY;}
		Double_t GetZ() const {return fZ;}
		
		Double_t GetR() const;
		Double_t GetPhi() const;
		Double_t GetTh() const;
		
		void SetX(const double& x) { fX = x; }
		void SetY(const double& y) { fY = y; }
		void SetZ(const double& z) { fZ = z; }

	private:
		Double_t fX;
		Double_t fY;
		Double_t fZ;

		ClassDef(Punto,1)
};


#endif 
