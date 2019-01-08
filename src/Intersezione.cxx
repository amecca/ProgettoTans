#include "TMath.h"
#include <Riostream.h>
#include "TRandom3.h"
#include "TObject.h"
#include "Intersezione.h"

ClassImp(Intersezione)

Intersezione::Intersezione(): TObject(),
 x(0.),
 y(0.),
 z(0.),
 PHI(0.),
 THETA(0.),
 HitTrovato(false)
{
}

//generazione della direzione associata ad ogni particella
void Intersezione::SetPoint(Double_t LUNG_BEAM,Double_t X0,Double_t Y0, Double_t Z0, Double_t R, Double_t PHI0,Double_t THETA0,Double_t thp,Double_t php){

  HitTrovato = false;

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

   // update angolo theta dopo multiple scattering
    THETA=TMath::ACos(c3);
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
   }

void Intersezione::Multiple_scattering(Double_t LUNG_BEAM,Double_t X0,Double_t Y0, Double_t Z0, Double_t R, Double_t PHI0,Double_t THETA0,Double_t thp,Double_t php){

  HitTrovato = false;

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

  //MULTIPLE SCATTERING spegnibile
 
      //definiamo thp,php in Simulazione.C 
      alpha = TMath::Sin(thp)*TMath::Cos(php); // sono gli analoghi di c1 per il caso di multiple scattering
      beta = TMath::Sin(thp)*TMath::Sin(php);
      gamma= TMath::Cos(thp);

      x1 =  TMath::Sin(THETA0);
      x2 =  TMath::Cos(PHI0);
      x3 =  TMath::Sin(PHI0);
		
	  // componnti del vettore nel sr del lab 
      k1= -alpha*x3-beta*c3*x2+gamma*c1;
      k2= alpha*x2-beta*x3*c3+gamma*c2;
      k3= beta*x1+gamma*c3;

      c1=k1;
      c2=k2;
      c3=k3;

    //Fine del MULTIPLE SCATTERING

   // update angolo theta dopo multiple scattering
    THETA=TMath::ACos(c3);
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
   }

void Intersezione::Rumore(Double_t LUNG_BEAM){
  Double_t u = gRandom->Rndm();

  PHI = 2 * TMath::Pi() *gRandom->Rndm();

  if(u>0.5) z = gRandom->Rndm()*LUNG_BEAM*0.5;
    else    z = -gRandom->Rndm()*LUNG_BEAM*0.5;

}

Intersezione::~Intersezione(){}
