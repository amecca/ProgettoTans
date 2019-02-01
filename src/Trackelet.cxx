#include "Trackelet.h"
//#include "Hit.h"
#include "Constants.h"

ClassImp(Trackelet)

Trackelet::Trackelet(const Hit& hitL1, const Hit& hitL2) : 
	TObject(),
	fHitL1(hitL1),
	fHitL2(hitL2)
{}

Trackelet::Trackelet(const Hit* const phitL1, const Hit* const phitL2) : 
	TObject(),
	fHitL1(*phitL1),
	fHitL2(*phitL2)
{}

Double_t Trackelet::findZVertex() const{
	// r = a*z + b
	Double_t a = (LAYER2_RADIUS-LAYER1_RADIUS)/(fHitL2.GetZ() - fHitL1.GetZ());//a=(r2-r1)/(z2-z1)
	Double_t b = LAYER2_RADIUS - a*fHitL2.GetZ(); //b = r2 - a*z2
	return -b/a; //r==0 --> z = -b/a
};

bool Trackelet::isAcceptable(const Double_t& maxPhi) const{
	return (Hit::deltaPhi(fHitL1,fHitL2) < maxPhi);
}
