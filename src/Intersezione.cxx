#include "TMath.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TObject.h"
#include "Intersezione.h"
#include "Hit.h"
ClassImp(Intersezione)

Intersezione::Intersezione(): TObject(),
 HitTrovato(false)
{
}

Punto Intersezione::SetPoint(Double_t LUNG_BEAM,Punto pStart, Double_t R,Direzione direz){

  HitTrovato = false;

  Double_t X0 = pStart.GetX();
  Double_t Y0 = pStart.GetY();
  Double_t Z0 = pStart.GetZ();
  Double_t PHI0 = direz.GetPHI();
  Double_t THETA0 = direz.GetTHETA();
  Double_t a,b,c;
  Double_t alpha,beta,gamma;
  Double_t x1,x,x2,x3;
  Double_t k1,y,k2,k3,c1,c2,c3;
  Double_t delta;
  Double_t t_temp, t;

  if(THETA0>TMath::Pi()) THETA0=THETA0-TMath::Pi();

  //intersezione traccia-rivelatore tramite equazione parametrica della retta
  c1 = TMath::Sin(THETA0)*TMath::Cos(PHI0);
  c2 = TMath::Sin(THETA0)*TMath::Sin(PHI0);
  c3 = TMath::Cos(THETA0);

	// scompongo delta in tre contributi a,b,c
    a = (X0*c1+Y0*c2);
    b = (c1*c1+c2*c2);
    c = (X0*X0+Y0*Y0-R*R);

    delta = a*a-b*c;
    t_temp = -((X0*c1+Y0*c2)+TMath::Sqrt(delta))/b;

	// dato che la traiettoria della particella è una semiretta orientata consideriamo t positivo
    if(t_temp>0) t = t_temp;
    else{
      t = -((X0*c1+Y0*c2)-TMath::Sqrt(delta))/b;
     }

    z = Z0+c3*t;

    if(z>=-LUNG_BEAM/2&&z<=LUNG_BEAM/2){ // condizione appartenenza al beam pipe
     HitTrovato = true;
      x = X0+c1*t;
      y = Y0+c2*t;

 //PASSAGGIO IN COORDINATE CILINRICHE
      if(x>0)PHI=TMath::ATan(y/x);
      else{
          if(y>0)PHI=TMath::Pi()+TMath::ATan(y/x);
          else PHI=-TMath::Pi()+TMath::ATan(y/x);
       }
      if (x==0){if(y>0)PHI=TMath::Pi()/2;
          else PHI=-TMath::Pi()/2;
     }
    }
	return Punto(x,y,z);
}

Direzione Intersezione::Multiple_Scattering(const Direzione& direzStart, const Double_t& rms_theta){

  HitTrovato = false;

  Double_t a,b,c;
  Double_t alpha,beta,gamma;
  Double_t x1,x2,x3;
  Double_t k1,k2,k3,c1,c2,c3;
  Double_t THETA0 = direzStart.GetTHETA();
	Double_t PHI0 = direzStart.GetPHI();

	Double_t dTHETA = gRandom->Gaus(0,rms_theta); //angoli di cui viene ruotata la direzione entrante
	Double_t dPHI = 2*TMath::Pi()*gRandom->Rndm();

  if(THETA0>TMath::Pi()) THETA0=THETA0-TMath::Pi();

  //intersezione traccia-rivelatore tramite equazione parametrica della retta
  c1 = TMath::Sin(THETA0)*TMath::Cos(PHI0);
  c2 = TMath::Sin(THETA0)*TMath::Sin(PHI0);
  c3 = TMath::Cos(THETA0);

  //MULTIPLE SCATTERING spegnibile
 
      //definiamo dTHETA,dPHI in Simulazione.C 
      alpha = TMath::Sin(dTHETA)*TMath::Cos(dPHI); // sono gli analoghi di c1 per il caso di multiple scattering
      beta = TMath::Sin(dTHETA)*TMath::Sin(dPHI);
      gamma= TMath::Cos(dTHETA);

      x1 =  TMath::Sin(THETA0);
      x2 =  TMath::Cos(PHI0);
      x3 =  TMath::Sin(PHI0);
		
	  // componnti del vettore nel sr del lab 
      k1= -alpha*x3-beta*c3*x2+gamma*c1;
      k2= alpha*x2-beta*x3*c3+gamma*c2;
      k3= beta*x1+gamma*c3;

   // update angolo theta dopo multiple scattering
    THETA=TMath::ACos(k3);
	  PHI = TMath::ATan2(k2,k1);
	
	return Direzione(THETA, PHI);
}

Hit Intersezione::Rumore(Double_t LUNG_BEAM, Int_t label){

  PHI = 2 * TMath::Pi() *gRandom->Rndm();
	
  Double_t u = gRandom->Rndm();
  if(u>0.5) z = gRandom->Rndm()*LUNG_BEAM*0.5;
    else    z = -gRandom->Rndm()*LUNG_BEAM*0.5;
	
	return Hit(z, PHI, label);
}

Intersezione::~Intersezione(){}
