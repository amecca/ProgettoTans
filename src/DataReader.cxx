#include "Riostream.h"
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream> //stringstream
#include <fstream> //filestream
#include <vector>
#include "TMath.h"

#include "DataReader.h"
// 
//
// Questa macro legge i dati dal file .dat e li assegna alle variabili
//
//

using std::cout;
//Global static pointer to ensure a single instance of the class
DataReader* DataReader::fInstance = NULL; 

DataReader* DataReader::getInstance(){
	if(!fInstance)
		fInstance = new DataReader();
	return fInstance;
}

DataReader::DataReader() : fsuccesfulRead(false) {
	
}

SimulationData DataReader::Read_dat_simulation(const char * filen, bool debug){
	
	SimulationData data;
	cout<<"Opening \""<<filen<<"\"...\n";
	
	FILE * ff = fopen(filen,"r");// r open file for input operation
	if (ff == nullptr) {
		cout << "ATTENTION: NO \""<<filen<<"\" FOUND\n";
		fsuccesfulRead = false;
		return data;
	}
	// analysis type
	int rt;
	rt=fscanf(ff,"nevent: %u\n",&data.nevent); //formatto le variabili in base al loro utilizzo
	
	rt=fscanf(ff,"lung_beam: %f\n",&data.lung_beam);

	rt=fscanf(ff,"radius beam: %f\n",&data.Raggio[0]);

	rt=fscanf(ff,"radius layer 1: %f\n",&data.Raggio[1]);

	rt=fscanf(ff,"radius layer 2: %f\n",&data.Raggio[2]);

	rt=fscanf(ff,"rms on z: %f\n",&data.rms_z);
	
	rt=fscanf(ff,"rms on xy: %f\n",&data.rms_xy);
	rt=fscanf(ff,"numero atomico beam pipe: %d\n",&data.Z[0]);
	rt=fscanf(ff,"numero atomico silicio: %d\n",&data.Z[1]);
	rt=fscanf(ff,"spessore beam pipe: %f\n",&data.thick[0]);
	rt=fscanf(ff,"spessore layer 1: %f\n",&data.thick[1]);
	rt=fscanf(ff,"lunghezza radiazione beam pipe: %f\n",&data.lung_rad[0]);
	rt=fscanf(ff,"lunghezza radiazione silicio: %f\n",&data.lung_rad[1]);
	rt=fscanf(ff,"smearing on z: %f\n",&data.smear_z);
	rt=fscanf(ff,"smearing on rphi: %f\n",&data.smear_rphi);
	rt=fscanf(ff,"scattering on/off: %i\n",&data.bool_scat);
	rt=fscanf(ff,"tipo di rumore: %i\n",&data.noise_type);
	rt=fscanf(ff,"contatore rumore: %u\n",&data.count_noise);
	rt=fscanf(ff,"tipo di molteplicita: %i\n",&data.moltTipo);
	rt=fscanf(ff,"tipo di distribuzione z: %i\n",&data.vertTipo);

	data.beam_rms = 13.6/800*TMath::Sqrt(data.thick[0] / data.lung_rad[0])*(1 + 0.038*TMath::Log(data.thick[0] / data.lung_rad[0]));
	data.l1_rms = 13.6/800*TMath::Sqrt(data.thick[1] / data.lung_rad[1])*(1+0.038*TMath::Log(data.thick[1] / data.lung_rad[1]));
	
	data.smear_phi1 = data.smear_rphi/data.Raggio[1]; //smearing su phi normalizzato al raggio
	data.smear_phi2 = data.smear_rphi/data.Raggio[2];
	
	if(debug) checkSimData(data);
	
	fsuccesfulRead = true;
	return data;
}

void DataReader::checkSimData(const SimulationData& data){
	cout<<"nevent "<<data.nevent;
	cout<<"\nbool_scat = "<<data.bool_scat;
	cout<<"\ncount_noise = "<<data.count_noise;
	cout<<"\nmoltTipo = "<<data.moltTipo;
	switch(data.moltTipo){
		case 0:
			cout<<" \t(da \"kinem.root\")";
			break;
		case 1:
			cout<<" \t(uniforme)";
			break;
		case 2:
			cout<<" \t(punti fissi)";
			break;
	}
	cout<<"\nvertTipo = "<<data.vertTipo;
	switch(data.vertTipo){
		case 0:
			cout<<" \t(gaussiana)";
			break;
		case 1:
			cout<<" \t(punti fissi)";
			break;
	}
	cout<<"\nsmear_z = "<<data.smear_z*100000<<" um";
	cout<<"\nsmear_rphi = "<<data.smear_rphi*100000<<" um";
		cout<<" \tsmear_phi1 = "<<data.smear_phi1*1000<<" mrad";
		cout<<" \tsmear_phi2 = "<<data.smear_phi2*1000<<" mrad";
	cout<<"\nbeam_rms = "<<data.beam_rms*1000<<" mrad";
	cout<<"\nlayer_rms = "<<data.l1_rms*1000<<" mrad\n";
	return;
}

