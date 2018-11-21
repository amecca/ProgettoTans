#include <Riostream.h>
#include "TObject.h"
#include "TMath.h"
#include "GeneraTrack.h"

///////////////////////////////////////////////////////
// This class generates the event:                   //
// define molteplicity, direction, vertex            //
// position and can introduce noise in the simulation//
// Authors: Mecca Alberto & Politano' Stefano        //
///////////////////////////////////////////////////////
ClassImp(GeneraTrack)

//________________________________________________________________________
GeneraTrack::GeneraTrack():TObject(){
  
    }   // default constructor

//___________________________________________________________________________

GeneraTrack::GeneraTrack(double x, double y, double z, double teta, double phi):TObject(){

  ZL0 = CalcHitL0(x,y,z);
  ZL1 = CalcHitL1(x,y,z);
  ZL2 = CalcHitL2(x,y,z);
  c1 = (TMath::Sin(teta)*TMath::Cos(phi));
  c2 = (TMath::Sin(teta)*TMath::Sin(phi));
  c3 = (TMath::Cos(teta));
    }
 
   // stdr constructor

//___________________________________________________________________________
double GeneraTrack::CalcHitL0(double x, double y, double z){
  
  //calculates the hit on the beam pipe
  double delta = (x*c1+y*c2)*(x*c1+y*c2)-(c1*c1+c2*c2)*(x*x+y*y-r0*r0);
  double t = -((x*c1+y*c2)+TMath::Sqrt(delta))/(c1*c1+c2*c2);
  //if(-H/2.<z+c3*t && H/2.>z+c3*t)
  	return z+c3*t;
  //else if(debug) cout<<"FUORI DAL RIV"<<endl;
}

//_______________________________________________________________________________
double GeneraTrack::CalcHitL1(double x, double y, double z){

 //calculates thr hit on the l1
  double delta = (x*c1+y*c2)*(x*c1+y*c2)-(c1*c1+c2*c2)*(x*x+y*y-r1*r1);
  double t = -((x*c1+y*c2)+TMath::Sqrt(delta))/(c1*c1+c2*c2);
  //if(-H/2.<z+c3*t && H/2.>z+c3*t) 
  	return z+c3*t;
  //else if(debug) cout<<"FUORI DAL RIV"<<endl;
  
}
//___________________________________________________________________________
double GeneraTrack::CalcHitL2(double x, double y ,double z){

  //calculates the hit on the second layer
  double delta = (x*c1+y*c2)*(x*c1+y*c2)-(c1*c1+c2*c2)*(x*x+y*y-r2*r2);
  double t = -((x*c1+y*c2)+TMath::Sqrt(delta))/(c1*c1+c2*c2);
  //if(-H/2.<z+c3*t && H/2.>z+c3*t) 
  	return z+c3*t;
  //else if(debug) cout<<"FUORI DAL RIV"<<endl;
  
}
//____________________________________________
double GeneraTrack::GetHitL0(){
  return ZL0;
  if(debug) cout<<"ZL0"<<ZL0<<endl;
}
//_______________________________________
double GeneraTrack::GetHitL1(){
  return ZL1;
  if(debug) cout<<"ZL1"<<ZL1<<endl;
}
//______________________________________
double GeneraTrack::GetHitL2(){
  return ZL2;
  if(debug) cout<<"ZL2"<<ZL2<<endl;
}
//______________________________________________
////////////////////////////////
//
//MULTIPLE SCATTERING
//
///////////////////////////////

GeneraTrack::~GeneraTrack()	 {
  // destructor
}
