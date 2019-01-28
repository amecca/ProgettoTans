#include "Direzione.h"
#include <Riostream.h>
#include "TObject.h"
#include "TRandom3.h"
#include "Kinem_File.h"

ClassImp(Direzione)

Direzione::Direzione(): TObject()
{
}

Direzione::Direzione(Double_t th, Double_t ph): TObject(),
  fph(ph),
  fth(th)
{
}

Direzione::Direzione(const Kinem_File * const kinem){
	if(!kinem){
		cout<<"Error: nullptr for Kinem_File in Direzione\n";
		exit(1);
	}
	fph = kinem->GenPhi();
	fth = kinem->GenTheta();
}


Direzione::~Direzione(){}
