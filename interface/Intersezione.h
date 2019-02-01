//-------------------------------------------------------
// Classe contenente funzioni per trovare il punto di intersezione
// di una particella con una certa direzione con un layer,
// calcolare la nuova direzione in seguito al multiple scattering,
// e generare Hit di rumore casuale 
//-------------------------------------------------------
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
		Bool_t isValid() const{ return HitTrovato; }

		virtual ~Intersezione();

		Punto TrovaIntersezione(Double_t LUNG_BEAM,Punto pStart, Double_t R, Direzione direz);
		Direzione Multiple_Scattering(const Direzione& direzStart, const Double_t& rms_theta);
		Hit Rumore(Double_t LUNG_BEAM, Int_t label = -1);


	private:

		Double_t x,y,z;
		Double_t PHI;
		Double_t THETA;
		Bool_t HitTrovato;

	ClassDef(Intersezione,1)
};


#endif
