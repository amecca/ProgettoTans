#ifndef PUNTO_H
#define PUNTO_H

#include <TObject.h>

class Punto: public TObject
{
	public:

		Punto();
		Punto(Double_t x, Double_t y, Double_t z);

		virtual ~Punto();

		void Generate(Float_t RMS_XY,Float_t RMS_Z,Bool_t EstraiVert);

		Double_t GetX() const {return fX;}
		Double_t GetY() const {return fY;}
		Double_t GetZ() const {return fZ;}
		
		Double_t GetPhi() const;


	private:

		Double_t fX;
		Double_t fY;
		Double_t fZ;
		Int_t fM;

	ClassDef(Punto,1)
};


#endif
