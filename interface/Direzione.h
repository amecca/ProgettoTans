//-------------------------------------------------------
// Rappresentazione di una direzione come coppia (theta,phi)
// Usata per propagare le particelle nel rivelatore
// Contiene la generazione casuale di theta e phi attraverso 
// Kinem_File
//-------------------------------------------------------
#ifndef DIREZIONE_H
#define DIREZIONE_H

#include "TObject.h"
#include "Kinem_File.h"

class Direzione: public TObject
{
	public:

		Direzione();
		Direzione(Double_t th, Double_t ph);
		Direzione(const Kinem_File * const); 

		virtual ~Direzione();

		Double_t GetPHI() const {return fph;}
		Double_t GetTHETA() const {return fth;}
		void SetPhi(const Double_t& phi) {fph = phi;}
		void SetTheta(const Double_t& th) {fth = th;}

	private:

		Double_t fph;
		Double_t fth;
	

	ClassDef(Direzione,1)
};


#endif
