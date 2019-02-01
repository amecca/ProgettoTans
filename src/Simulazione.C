#include "Riostream.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TString.h"
#include "TRandom3.h"
#include "Direzione.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "TSystem.h"

#include "Vertice.h"
#include "Punto.h"
#include "Kinem_File.h"
#include "Hit.h"
#include "Intersezione.h"
#include "Read_dat_simulation.h"

//using namespace SimulationData;

TString nullTString = TString();
void Simulazione(TString filename = nullTString){

	static VERTICE vtx;

	cout<<"Start simulation..."<<endl;

	if(!Read_dat_simulation("Data/Dati_Simulazione.dat", true))// legge i dati e li assegna alle variabili
		return; //Se non trova il file non c'è niente da fare
	Double_t z1_smeared,phi1_smeared,z2_smeared,phi2_smeared; // smearing 
	Double_t smear_phi1 = SimulationData::smear_rphi/SimulationData::Raggio[1]; //smearing su phi normalizzato al raggio
	Double_t smear_phi2 = SimulationData::smear_rphi/SimulationData::Raggio[2];
	Int_t mult, Label, Successi1, Successi2;

	TStopwatch stopwatch; 

	// apertura file di output
	TString trueName;
	if(filename.Length()==0)
		trueName = TString("simulazione.root");
	else
		trueName = filename.Append(".root");
		
	if(!gSystem->AccessPathName("Trees",kFileExists))
		gSystem->MakeDirectory("Trees");
	TFile vertFile(("Trees/"+trueName).Data(),"RECREATE"); // formats the string 
		if(vertFile.IsZombie()) return;
    	
	TTree* vertTree = new TTree("VT","Tree del Hit");

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

	Punto pt; //Punto su cui viene generata la posizione del vertice
	Intersezione p_int; //Usata per trovare il punto di intersezione
	Kinem_File kinem; //Interfaccia col file contenente le distribuzioni di molteplicità e pseudorapidità
	mult=0;
	
	//____________________________________________________________________________________
	//LOOP ON EVENTS
	for(Int_t i = 0; i < SimulationData::nevent; i++){
		//genera il vertice con la distribuzione assegnata: vertTipo=1 fz->gauss, vertTipo=0 fz->scelta 
		pt.Generate(SimulationData::rms_xy, SimulationData::rms_z, SimulationData::vertTipo); 

		vtx.x0=pt.GetX(); // coordinate vertice la struct VERTICE serve per scrivere sul tree 
		vtx.y0=pt.GetY(); 
		vtx.z0=pt.GetZ();

		//Usa la distribuzione scelta in Dati_simulazione.txt
		mult = kinem.Molteplicita(SimulationData::moltTipo); //0 -> distribuzione salvata in KinemFile.root; 1 -> uniforme fra 0 e 50
		vtx.m = mult;

		Label = 1; //etichetta assegnata alla particella a fini di debug
		Successi1 = 0; //successi1 indica le particelle arrivate al primo layer
		Successi2 = 0;

		//loop sulla MOLTEPLICITA
		for(Int_t j=0; j<mult; j++){

			Direzione direz(&kinem); // direz aggiornata della prticella

			//BEAM PIPE
			Punto pHitBeam = p_int.TrovaIntersezione(SimulationData::lung_beam, pt, SimulationData::Raggio[0] ,direz);
			if (SimulationData::bool_scat==1) 
				direz = p_int.Multiple_Scattering(direz, SimulationData::beam_rms); //Estrae i random all'interno
		
			//1° RIVELATORE
			Punto pHitL1 = p_int.TrovaIntersezione(SimulationData::lung_beam, pHitBeam,  SimulationData::Raggio[1], direz);
			bool hitValido1 = p_int.isValid();
			
			if (SimulationData::bool_scat==1) direz = p_int.Multiple_Scattering(direz, SimulationData::l1_rms);
		   
			z1_smeared = gRandom->Gaus(pHitL1.GetZ(), SimulationData::smear_z); 
			phi1_smeared = gRandom->Gaus(pHitL1.GetPhi(), smear_phi1); // smearing 120 e 30 micron

			if (phi1_smeared<0) 
				phi1_smeared = phi1_smeared+ 2*TMath::Pi();
			else if (phi1_smeared>2*TMath::Pi())
				phi1_smeared = phi1_smeared - 2*TMath::Pi();
			// lo smearing gauss è compreso tra 0 e 2Pi

			//if(z1_smeared>=-SimulationData::lung_beam/2&&z1_smeared<=SimulationData::lung_beam/2){
			if(hitValido1){
				new(l1_hit[Successi1])Hit(z1_smeared, phi1_smeared, Label);
				Successi1++; // crea un l1_hit per ogni evento e gli assegna z phi e label, successi1 sono nel beam pipe

				//2° RIVELATORE (solo se il primo è stato colpito)
				Punto pHitL2 = p_int.TrovaIntersezione(SimulationData::lung_beam, pHitL1, SimulationData::Raggio[2], direz);
				bool hitValido2 = p_int.isValid();
				
				z2_smeared = gRandom->Gaus(pHitL2.GetZ(), SimulationData::smear_z);
				phi2_smeared = gRandom->Gaus(pHitL2.GetPhi(), smear_phi2);

				if (phi2_smeared<0)
					phi2_smeared = phi2_smeared+ 2*TMath::Pi();
				else if (phi2_smeared>2*TMath::Pi()) 
						phi2_smeared = phi2_smeared - 2*TMath::Pi();
				        
				//if(z2_smeared>=-SimulationData::lung_beam/2 && z2_smeared<=SimulationData::lung_beam/2){
				if(hitValido2){
					new(l2_hit[Successi2])Hit(z2_smeared, phi2_smeared, Label); // l1 e l2 condividono il label
					Successi2++;
				}
			}
			Label++; // alla fine del loop sulla molteplicità cambia il Label: particelle dello stesso vertice avranno label diverse
		} //close loop on Molteplicita

		// RUMORE
		Label = -1;
		// 1° Rivelatore

		for (Int_t k=Successi1; k<Successi1+SimulationData::count_noise; k++){//loop sul rumore
			Hit temp = p_int.Rumore(SimulationData::lung_beam,Label);
			//cout<<"\nNoise "<<j-Successi1;
			new(l1_hit[k])Hit(temp.GetZ(),temp.GetPHI(),Label);
		}

		//2° Rivelatore
		for (Int_t k=Successi2; k<Successi2+SimulationData::count_noise; k++){//loop sul rumore
			Hit temp = p_int.Rumore(SimulationData::lung_beam,Label);
			new(l2_hit[k])Hit(temp.GetZ(),temp.GetPHI(),Label);
		}


		vertTree->Fill();
		l1hit->Clear();
		l2hit->Clear();
		if(i%((SimulationData::nevent)/100))
			cout<<"\rFilling simulation tree: "<<i*100/(SimulationData::nevent)+1<<"%";
	} //close loop on events
		
	cout<<"\nElapsed time: "<<stopwatch.CpuTime()<<" s";
		
	vertFile.Write("", TObject::kOverwrite);
	cout<<"\nTree filled successfully and saved in \""<<trueName<<"\"\n";

	vertFile.Close();
	cout<<"File closed\n";
	
	cout<<"Succesful simulation\n";
} //close Simulazione()
