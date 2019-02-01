//-------------------------------------------------------
// Rappresentazione di una terna di coordinate
// Usata sia per rappresentare punti di intersezione con un
// layer che per la posizione del vertice
// Ha una funzione per generare la posizione del vertice
//-------------------------------------------------------
#ifndef PUNTO_H
#define PUNTO_H

#include <TObject.h>

class Punto: public TObject
{
	public:

		Punto();
		Punto(Double_t x, Double_t y, Double_t z);

		virtual ~Punto();

		void Generate(Float_t RMS_XY, Float_t RMS_Z, Int_t EstraiVert);
		//EstraiVert: 0-->z=Gaus(...) 	else-->z={-15.,-12.5,-10.,-7.5,-5.,-2.5,0.,2.5,5.,7.5,10.,12.5,15.}

		Double_t GetX() const {return fX;}
		Double_t GetY() const {return fY;}
		Double_t GetZ() const {return fZ;}
		void SetX(const Double_t& x) {fX = x;}
		void SetY(const Double_t& y) {fY = y;}
		void SetZ(const Double_t& z) {fZ = z;}
		
		Double_t GetPhi() const;


	private:

		Double_t fX;
		Double_t fY;
		Double_t fZ;
		Int_t fM;

	ClassDef(Punto,1)
};


#endif
