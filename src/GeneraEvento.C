#include <Riostream.h>
#include "TObject.h"
#include "TMath.h"
#include "GeneraEvento.h"

///////////////////////////////////////////////////////
// This class generates the event:                   //
// define molteplicity, direction, vertex            //
// position and can introduce noise in the simulation// 
///////////////////////////////////////////////////////
ClassImp(GeneraEvento)

//________________________________________________________________________
/*GeneraEvento::GeneraEvento() : TObject(){
  if(debug) std::cout<<"Default Constructor - GeneraEvento"<<std::endl;

	GeneraMolt();
	GeneraVertice();
 }*/

//___________________________________________________________________________

inline unsigned int GeneraEvento::GeneraMolt(){
  
  //returns molteplicity for each event
  return (size_t)(0.5*gRandom->Gaus(50.,20.)); // la scelta di gaus e provvisoria
}
//___________________________________________________________________________

inline double GeneraEvento::GeneraX(){
  //(gaussian distribution)
  return gRandom->Gaus(0.,0.01);
}

//_____________________________________________________________________________
inline double GeneraEvento::GeneraY(){
  //(gaussian distribution)
  return gRandom->Gaus(0.,0.01);
}
//___________________________________________________________________________
inline double GeneraEvento::GeneraZ(){
  //(gaussian distribution)
  return gRandom->Gaus(0.,5.3);
}

Punto GeneraEvento::GeneraVertice(){
	Punto vertex(
		GeneraX(),
		GeneraY(),
		GeneraZ()
	);
	return vertex;
}
//__________________________________________________________________________
double GeneraEvento::GeneraPhi(){
  //returns phi angle  (uniform distribution)
  return gRandom->Rndm()*2*TMath::Pi();
  
}
//__________________________________________________________________________
double GeneraEvento::GeneraEta(){

  //returns eta (pseudorapidity)  (uniform distr)
  return gRandom->Rndm();
}

inline double GeneraEvento::GetTh(const double& eta){
	return 2*TMath::ATan(TMath::Exp(- eta));
}

inline double GetEta(const double& th){
	return -TMath::Log(TMath::Tan(th/2));
}

//__________________________________________________________________________
////////////////////////////////
//
//NOISE
//
///////////////////////////////
GeneraEvento::~GeneraEvento()	 {
  // destructor
}
