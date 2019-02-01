//-------------------------------------------------------
// Classe che ricostruisce la posizione lungo l'asse z del
// vertice, date due Hit. 
//-------------------------------------------------------
#ifndef TRACKELET_H
#define TRACKELET_H

#include "TObject.h"

#include "Hit.h"

class Trackelet : public TObject{
	public:
		Trackelet() : TObject() {}
		Trackelet(const Hit& hitL1, const Hit& hitL2);
		Trackelet(const Hit* const phitL1, const Hit* const phitL2);
		Double_t findZVertex() const;
		bool isAcceptable(const Double_t& maxPhi) const;
	private:
		Hit fHitL1;
		Hit fHitL2;
		
	ClassDef(Trackelet, 0);
};

#endif
