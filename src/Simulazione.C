#include "Riostream.h"
#include "Punto.h"
#include "Kinem_File.h"
#include "Hit.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TRandom3.h"
#include "Direzione.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "Intersezione.h"
#include "Read_dat_simulation.cxx"

void Simulazione(){

  typedef struct{
    Double_t x0,y0,z0;
    UInt_t m;
  }VERTICE;

  static VERTICE vtx;

  cout<<"Start simulation..."<<endl;

  Read_dat_simulation("Data/Dati_Simulazione.dat");// legge i dati e li assegna alle variabili

  Double_t z1_smeared,phi1_smeared,z2_smeared,phi2_smeared; // smearing 
  Double_t x0_scat,y0_scat,z0_scat,x1_scat,y1_scat,z1_scat,z2_scat; // scattering coord.
  Double_t phi0_scat,phi1_scat,phi2_scat,theta0_scat,theta1_scat; // scattering angle
  Double_t smear_phi1, smear_phi2; // ??? LINE 28 HAS THE SAME SMEAR
  Double_t phi0,R; //coord
  Int_t mult, Label, Successi1, Successi2; // 

  Bool_t EstraiVert; // bool per distribuzione in z gaussiana o assegnata


  for(Int_t count_z=0; count_z<=0/*1*/; count_z++){ // Estrai=1 fz->gauss, Estrai=0 fz->scelta 
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

      vtx.x0=pt.GetX(); //coordinate vertice la struct VERTICE serve per scrivere sul tree 
      vtx.y0=pt.GetY(); 
      vtx.z0=pt.GetZ();

     // while(mult <1)
      mult = kinem.Molteplicita(); 
      vtx.m = mult;

      Label=1; //etichetta assegnata alla particella
      Successi1=0; //successi1 indica le particelle arrivate al primo layer
      Successi2=0;

      //loop sulla MOLTEPLICITA
      for(Int_t j=0; j<mult; j++){

 		Direzione direz(&kinem); // direz aggiornata della prticella
		
        //Beam pipe
        R=Raggio[0]; // valore assegnato in Read_dat_simulation
        Punto pHitBeam = p_int.SetPoint(lung_beam,pt,R,direz); //non considero lo smearing
		if (bool_scat==1) 
			direz = p_int.Multiple_Scattering(direz,beam_rms); //Estrae i random all'interno
		
        //1° Rivelatore
        R=Raggio[1];
        smear_phi1= smear_rphi/Raggio[1]; //smearing su phi normalizzato al raggio
       
		Punto pHitL1 = p_int.SetPoint(lung_beam,pHitBeam,R,direz);
  		if (bool_scat==1) direz = p_int.Multiple_Scattering(direz,l1_rms);
       
        z1_smeared = gRandom->Gaus(pHitL1.GetZ(), smear_z); 
        phi1_smeared = gRandom->Gaus(pHitL1.GetPhi(), smear_phi1); // smearing 120 e 30 micron

        if (phi1_smeared<0) phi1_smeared = phi1_smeared+ 2*TMath::Pi();
           else{
            if (phi1_smeared>2*TMath::Pi()) phi1_smeared = phi1_smeared - 2*TMath::Pi();
          } // lo smearing gauss è compreso tra 0 e 2Pi

        if(z1_smeared>=-lung_beam/2&&z1_smeared<=lung_beam/2){
           new(l1_hit[Successi1])Hit(z1_smeared, phi1_smeared, Label);
           Successi1++; // crea un l1_hit per ogni evento e ci assegna z phi e label, successi1 sono nel beam pipe

          //2° Rivelatore
      	   R=Raggio[2];
          // R=Raggio[2];

            smear_phi2= smear_rphi/Raggio[2];
           

            Punto pHitL2 = p_int.SetPoint(lung_beam,pHitL1,R,direz);

            z2_smeared = gRandom->Gaus(pHitL2.GetZ(), smear_z);
            phi2_smeared = gRandom->Gaus(pHitL2.GetPhi(), smear_phi2);

            if (phi2_smeared<0) phi2_smeared = phi2_smeared+ 2*TMath::Pi();
               else{
                if (phi2_smeared>2*TMath::Pi()) phi2_smeared = phi2_smeared - 2*TMath::Pi();
              }


            if(z2_smeared>=-lung_beam/2&&z2_smeared<=lung_beam/2){
               new(l2_hit[Successi2])Hit(z2_smeared, phi2_smeared, Label); // l1 e l2 condividono il label
               Successi2++;
              }
            }
            Label++; // alla fine del loop sulla molteplicità cambia il Label: particelle provenienti da molt diverse avranno molt diverse
        }
		
        // RUMORE
        Label = -1;
       	// 1° Rivelatore
  		
        for (Int_t j=Successi1; j<Successi1+count_noise; j++){//loop sul rumore
        	Hit temp = p_int.Rumore(lung_beam,Label);
           new(l1_hit[j])Hit(temp.GetZ(),temp.GetPHI(),Label);
         }

         //2° Rivelatore
         for (Int_t j=Successi2; j<Successi2+count_noise; j++){//loop sul rumore
           Hit temp = p_int.Rumore(lung_beam,Label);
           new(l2_hit[j])Hit(temp.GetZ(),temp.GetPHI(),Label);
         }
		

        vertTree->Fill();
      	l1hit->Clear(); // controllare TClonesArrey
      	l2hit->Clear();
        if(i%((nevent)/100)==0&&count_z==0)cout<<"\rFilling simulation tree: "<<i*100/(nevent)+1<<"%";
        else if(i%((nevent)/100)==0&&count_z==1)cout<<"\rFilling performance tree: "<<i*100/(nevent)+1<<"%";

      }
      cout<<"\nTree filled successfully"<<endl;

     // vertFile.Write();
      vertFile.Close();
      cout<<"File closed\n"<<"Simulation done succesfully"<<endl;

      l1hit->Clear("C"); // controllare 
      l2hit->Clear("C");

    }
    cout<<endl;
  }
