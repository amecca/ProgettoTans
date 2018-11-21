#ifndef GENERATRACK_H
#define GENERATRACK_H

#include "TObject.h"
#include "TRandom.h"
#include "TMath.h"


class GeneraTrack : public TObject {

///////////////////////////////////////////////////////
// This class gives the intersection with the beam   //
// pipe and the layers                               //
// Authors: Mecca Alberto & Politano' Stefano        //
///////////////////////////////////////////////////////
  
public:
  //
  GeneraTrack(); // dflt constr
  GeneraTrack( double x0, double y0, double z0,double teta, double phi); // stdrd constr
  GeneraTrack(const GeneraTrack&); // copy constr
  virtual ~GeneraTrack(); 

  double GetHitL0(); // restituisce il valore di intersezione in z con la bp
  double GetHitL1(); // restituisce il valore di intersezione in z con il l1
  double GetHitL2(); // restiruisce il valore di intersezione in z con il l2

  double CalcHitL0(double x, double y, double z); // calcola inters con beam pipe
  double CalcHitL1(double x, double y, double z);// calcola inters l1
  double CalcHitL2(double x, double y, double z);// calcola inters l2

  
 private:

  bool debug = true; // bool di debug 
  
  double c1,c2,c3; //costanti della retta param
  
  constexpr const static double r0 = 3.; // TUTTI I RAGGI SONO ESPRESSI IN CM!!!
  constexpr const static double r1 = 4.;
  constexpr const static double r2 = 7.;
  constexpr const static double H = 27.; // lunghezza in z dei layer in CM

  double ZL0, ZL1, ZL2;
  
	ClassDef(GeneraTrack,1)
};


#endif 

