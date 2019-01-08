#include "Riostream.h"
#include "Punto.h"
#include "Kinem_File.h"
#include "Hit.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "Intersezione.h"
#include "Read_dat_simulation.cxx"

void Simulazione(){

  typedef struct{
    Double_t x0,y0,z0;
    Int_t m;
  }VERTICE;

  static VERTICE vtx;

  cout<<"Start simulation..."<<endl;

  Read_dat_simulation("Data/Dati_Simulazione.dat");// legge i dati e li assegna alle variabili

  Double_t z1_smear,phi1_smear,z2_smear,phi2_smear; // smearing 
  Double_t x0_scat,y0_scat,z0_scat,x1_scat,y1_scat,z1_scat,z2_scat; // scattering coord.
  Double_t phi0_scat,phi1_scat,phi2_scat,theta0_scat,theta1_scat; // scattering angle
  Double_t smear_phi1, smear_phi2; // ??? LINE 28 HAS THE SAME SMEAR
  Double_t phi0,theta0,theta1,theta2,R; //coord
  Int_t mult, Label, Successi1, Successi2; // 

  Bool_t EstraiVert; // bool per distribuzione in z gaussiana o assegnata


  for(Int_t count_z=0; count_z<=1; count_z++){ // Estrai=1 fz->gauss, Estrai=0 fz->scelta 
    if(count_z==1) EstraiVert = false;
    else EstraiVert = true;

    // apertura file di output
    TFile vertFile(Form("vertFile%d_noscat.root",count_z),"RECREATE"); // formats the string 
    TTree *vertTree = new TTree("VT","Tree del Hit");
	
    // Array layer
    TClonesArray *l1hit = new TClonesArray("Hit",400);
    TClonesArray &l1_hit = *l1hit;

    TClonesArray *l2hit = new TClonesArray("Hit",400);
    TClonesArray &l2_hit = *l2hit;

    // dichiarazione dei branches
    vertTree->Branch("Vertex",&vtx.x0,"x0/D:y0:z0:m/I");
    vertTree->Branch("L1_hit",&l1hit);
    vertTree->Branch("L2_hit",&l2hit);

    gRandom->SetSeed(clock());

    Punto pt;
    Intersezione p_int;
    Kinem_File kinem;
    mult=0;
    

    for(Int_t i=0;i<nevent;i++){

      pt.Generate(rms_xy,rms_z,EstraiVert); //genera il vertice con la distribuzione assegnata 

      vtx.x0=pt.GetX(); //coordinate vertice, 
      vtx.y0=pt.GetY(); 
      vtx.z0=pt.GetZ();

      kinem.Molteplicita(); // Kinem_File
      mult = kinem.GetM();  
      kinem.Pseudorapidita(mult);
      vtx.m = mult;

      Label=1; //etichetta assegnata alla particella
      Successi1=0; //successi1 indica le particelle arrivate al primo layer
      Successi2=0;

      //loop sulla MOLTEPLICITA
      for(Int_t j=0; j<mult; j++){

        phi0=(gRandom->Rndm())*2*TMath::Pi(); //angoli alla collisione
        theta0=kinem.GetTeta(j);

        //Beam pipe
        R=Raggio[0]; // valore assegnato in Read_dat_simulation

        phi0_scat= 2*TMath::Pi()*(gRandom->Rndm());
        theta0_scat= gRandom->Gaus(0,beam_rms);

		// Intersezione
        p_int.SetPoint(lung_beam,vtx.x0,vtx.y0,vtx.z0,R,phi0,theta0,theta0_scat,phi0_scat); //non considero lo smearing

        x0_scat=p_int.GetX();
        y0_scat=p_int.GetY();
        z0_scat=p_int.GetZ();
        phi0_scat=p_int.GetPHI();

        //1° Rivelatore
        R=Raggio[1]-Raggio[0];
      	//  R=Raggio[1];
        smear_phi1= smear_rphi/Raggio[1]; //smearing su phi normalizzato al raggio
        theta1_scat=gRandom->Gaus(0,l1_rms); 

        theta1=p_int.GetTHETA();

        p_int.SetPoint(lung_beam,x0_scat,y0_scat,z0_scat,R,phi0_scat,theta1,theta1_scat,2*TMath::Pi()*(gRandom->Rndm()));
        x1_scat=p_int.GetX();
        y1_scat=p_int.GetY();
        z1_scat=p_int.GetZ();
        phi1_scat=p_int.GetPHI();

        z1_smear= gRandom->Gaus(z1_scat, smear_z); 
        phi1_smear = gRandom->Gaus(phi1_scat, smear_phi1); // smearing 120 e 30 micron

        if (phi1_smear<0) phi1_smear = phi1_smear+ 2*TMath::Pi();
           else{
            if (phi1_smear>2*TMath::Pi()) phi1_smear = phi1_smear - 2*TMath::Pi();
          } // lo smearing gauss è compreso tra 0 e 2Pi

        if(z1_smear>=-lung_beam/2&&z1_smear<=lung_beam/2){
           new(l1_hit[Successi1])Hit(z1_smear, phi1_smear, Label);
           Successi1++; // crea un l1_hit per ogni evento e ci assegna z phi e label, successi1 sono nel beam pipe

            //2° Rivelatore
      	   R=Raggio[2]-Raggio[1];
          // R=Raggio[2];

            smear_phi2= smear_rphi/Raggio[2];
            theta2=p_int.GetTHETA();

            p_int.SetPoint(lung_beam,x1_scat,y1_scat,z1_scat,R,phi1_scat,theta2,gRandom->Gaus(0,l1_rms),2*TMath::Pi()*(gRandom->Rndm()));
            z2_scat=p_int.GetZ();
            phi2_scat=p_int.GetPHI();

            z2_smear = gRandom->Gaus(z2_scat, smear_z);
            phi2_smear = gRandom->Gaus(phi2_scat, smear_phi2);

            if (phi2_smear<0) phi2_smear = phi2_smear+ 2*TMath::Pi();
               else{
                if (phi2_smear>2*TMath::Pi()) phi2_smear = phi2_smear - 2*TMath::Pi();
              }


            if(z2_smear>=-lung_beam/2&&z2_smear<=lung_beam/2){
               new(l2_hit[Successi2])Hit(z2_smear, phi2_smear, Label); // l1 e l2 condividono il label
               Successi2++;
              }
            }
            Label++; // alla fine del loop sulla molteplicità cambia il Label: particelle provenienti da molt diverse avranno molt diverse
        }

        // RUMORE
        Label = -1;

  		// 1° Rivelatore
        for (Int_t j=Successi1; j<Successi1+count_noise; j++){//loop sul rumore
           p_int.Rumore(lung_beam);
           new(l1_hit[j])Hit(p_int.GetZ(), p_int.GetPHI(), Label);
         }

         //2° Rivelatore
         for (Int_t j=Successi2; j<Successi2+count_noise; j++){//loop sul rumore
           p_int.Rumore(lung_beam);
           new(l2_hit[j])Hit(p_int.GetZ(), p_int.GetPHI(), Label);
         }


        vertTree->Fill();
      	l1hit->Clear(); // controllare TClonesArrey
      	l2hit->Clear();
        if(i%((nevent)/100)==0&&count_z==0)cout<<"filling simulation tree: "<<i*100/(nevent)+1<<"%"<<endl;
        else if(i%((nevent)/100)==0&&count_z==1)cout<<"filling performance tree: "<<i*100/(nevent)+1<<"%"<<endl;

      }
      cout<<"tree filled successfully"<<endl;

     // vertFile.Write();
      vertFile.Close();
      cout<<"file closed\n"<<"simulation done succesfully"<<endl;

      l1hit->Clear("C"); // controllare 
      l2hit->Clear("C");

    }
    cout<<endl;
  }
