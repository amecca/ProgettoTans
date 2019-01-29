#include "Riostream.h"
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream> //stringstream
#include <fstream> //filestream
#include <vector>
#include "TMath.h"

// 
//
// Questa macro legge i dati dal file .dat e li assegna alle variabili
//
//

using namespace std;
static bool debug = true;
static Int_t moltTipo;
static Int_t nevent;
static Float_t lung_beam;
static Float_t Raggio[3];
static Int_t Z[2];
static Float_t thick[2];
static Float_t lung_rad[2];
static Int_t bool_scat;
static Int_t count_noise;
static Float_t smear_z;
static Float_t smear_rphi;
static Float_t beam_rms;
static Float_t l1_rms;
//static Int_t total_reco;
static Float_t rms_z;
static Float_t rms_xy;
static Int_t vertTipo; // vertTipo=1 fz->gauss, vertTipo=0 fz->scelta 

bool  Read_dat_simulation(const char * filen){
	
	cout<<"Opening \""<<filen<<"\"...\n";
	
	FILE * ff = fopen(filen,"r");// r open file for input operation
	if (ff == nullptr) {
		cout << "ATTENTION : NO \""<<filen<<"\" FOUND\n";
		return false;
	}
	// analysis type
	int rt;
	rt=fscanf(ff,"nevent: %d\n",&nevent); //formatto le variabili in base al loro utilizzo
	
	rt=fscanf(ff,"lung_beam: %f\n",&lung_beam);

	rt=fscanf(ff,"radius beam: %f\n",&Raggio[0]);

	rt=fscanf(ff,"radius layer 1: %f\n",&Raggio[1]);

	rt=fscanf(ff,"radius layer 2: %f\n",&Raggio[2]);

	rt=fscanf(ff,"rms on z: %f\n",&rms_z);
	//cout<<rms_z<<endl;
	rt=fscanf(ff,"rms on xy: %f\n",&rms_xy);
	rt=fscanf(ff,"numero atomico beam pipe: %d\n",&Z[0]);
	rt=fscanf(ff,"numero atomico silicio: %d\n",&Z[1]);
	rt=fscanf(ff,"spessore beam pipe: %f\n",&thick[0]);
	rt=fscanf(ff,"spessore layer 1: %f\n",&thick[1]);
	rt=fscanf(ff,"lunghezza radiazione beam pipe: %f\n",&lung_rad[0]);
	rt=fscanf(ff,"lunghezza radiazione silicio: %f\n",&lung_rad[1]);
	rt=fscanf(ff,"smearing on z: %f\n",&smear_z);
	rt=fscanf(ff,"smearing on rphi: %f\n",&smear_rphi);
	rt=fscanf(ff,"scattering on/off: %i\n",&bool_scat);
	rt=fscanf(ff,"contatore rumore: %i\n",&count_noise);
	rt=fscanf(ff,"tipo di molteplicita: %i\n",&moltTipo);
	rt=fscanf(ff,"tipo di distribuzione z: %i\n",&vertTipo);
	//rt=fscanf(ff,"total recostruction: %d\n",&total_reco);
	//  rt=fscanf(ff,"estrai vertice: %d\n",&EstraiVert);

	beam_rms = 13.6/800*TMath::Sqrt(thick[0]/lung_rad[0])*(1+0.038*TMath::Log(thick[0]/lung_rad[0]));
	l1_rms = 13.6/800*TMath::Sqrt(thick[1]/lung_rad[1])*(1+0.038*TMath::Log(thick[1]/lung_rad[1]));
	
	cout<<"smear_z = "<<smear_z;
	cout<<"\nsmear_rphi = "<<smear_rphi;
	cout<<"\nbool_scat = "<<bool_scat;
	cout<<"\ncount_noise = "<<count_noise;
	cout<<"\nmoltTipo = "<<moltTipo<<'\n';

	//  beam_rms = TMath::Sqrt(2)*13.6/800*TMath::Sqrt(thick[0]/lung_rad[0])*(1+0.038*TMath::Log(thick[0]/lung_rad[0]));
	//  l1_rms = TMath::Sqrt(2)*13.6/800*TMath::Sqrt(thick[1]/lung_rad[1])*(1+0.038*TMath::Log(thick[1]/lung_rad[1]));
	/*
	delete ff;*/
	return true;
}

