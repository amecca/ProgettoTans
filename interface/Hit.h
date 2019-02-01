//-------------------------------------------------------
// Rappresentazione di una coppia (z,phi) corrispondente
// ad una posizione su un generico layer, cui viene aggiunta 
// un'etichetta che identifica la particella che l'ha generata
// Ha funzioni base per la distanza in phi fra due Hit 
//-------------------------------------------------------
#ifndef HIT_H
#define HIT_H

#include "TObject.h"

class Hit : public TObject
{
	public:

		Hit(); //costruttore di default
		Hit(Double_t Z, Double_t Phi, Int_t Label); //costruttore standard
		Hit(const Hit&); //copy constructor

		virtual ~Hit(); //distruttore

		Double_t GetZ() const {return fZ;}
		Double_t GetPHI() const {return fPhi;}
		Int_t GetLabel() const {return label;}
		
		static Double_t deltaPhi(const Hit&, const Hit&);
		static Double_t deltaPhi(const Hit*, const Hit*);

	private:

		Double_t fZ;
		Double_t fPhi;	//Guaranteed to be between 0 and 2pi
		Int_t label;

		ClassDef(Hit,1)
};


#endif
