//-------------------------------------------------------
// Lettura del file contenente i dati della simulazione,
// che vengono inseriti in variabili definite nel namespace
// SimulationData::
//-------------------------------------------------------
#ifndef READ_DAT_SIMULATION
#define READ_DAT_SIMULATION

namespace SimulationData{
	Int_t moltTipo;
	Int_t nevent;
	Float_t lung_beam;
	Float_t Raggio[3];
	Int_t Z[2];
	Float_t thick[2];
	Float_t lung_rad[2];
	Int_t bool_scat;
	Int_t count_noise;
	Float_t smear_z;
	Float_t smear_rphi;
	Float_t beam_rms;
	Float_t l1_rms;
	Float_t rms_z;
	Float_t rms_xy;
	Int_t vertTipo; // vertTipo=1 fz->gauss, vertTipo=0 fz->scelta 
}

bool Read_dat_simulation(const char * filen, bool debug = false); //returns true if the file was found and read

#endif
