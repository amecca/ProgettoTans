#ifndef GENERAEVENTO_H
#define GENERAEVENTO_H

#include "TObject.h"
#include "TRandom.h"
#include "TMath.h"

#include "Punto.h"

class GeneraEvento : public TObject {

///////////////////////////////////////////////////////
// This class generates the event:                   //
// define molteplicity, direction, vertex            //
// position and can introduce noise in the simulation//
// Authors: Mecca Alberto & Politano' Stefano        //
///////////////////////////////////////////////////////
  
public:
		//GeneraEvento(); // dflt constr
		//GeneraEvento(const GeneraEvento&); // copy constr
		virtual ~GeneraEvento();
		/*
		unsigned int  GetMolteplicity() const { return molt; } // moltplicity
		double GetX() const { return vertex.GetX(); }  // Xcoord
		double GetY() const { return vertex.GetY(); }  // Ycoord
		double GetZ() const { return vertex.GetZ(); }  // Zcoord
		double GetPhi() const { return phi; } // phi angle
		double GetEta() const { return eta; } // pseudorapidity
		*/
		static double GetTh(const double& eta); //Converts pseudorapidity to theta
		//^Da spostare in Punto (?)
		static double GetEta(const double& th);
		// manca ancora il NOISE
		
		static unsigned int GeneraMolt();
		
		static Punto GeneraVertice();
		static double GeneraPhi();
		static double GeneraEta();
		static double GeneraTh();
		
	protected:
	
		static inline double GeneraX();
		static inline double GeneraY();
		static inline double GeneraZ();
	
	
	private:
	/*
		unsigned int molt;
		Punto vertex;
		double phi;
		double eta;
		bool debug;
	*/

ClassDef(GeneraEvento,1)
};


#endif 

