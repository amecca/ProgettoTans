//-------------------------------------------------------
// Lettura del file contenente i dati della simulazione,
// che vengono inseriti in variabili definite nel namespace
// SimulationData::
//-------------------------------------------------------
#ifndef DATA_READER
#define DATA_READER

struct SimulationData{
	
	UInt_t nevent;
	Float_t lung_beam;
	Float_t Raggio[3];
	Float_t rms_z;
	Float_t rms_xy;
	Int_t Z[2];
	Float_t thick[2];
	Float_t lung_rad[2];
	Float_t smear_z;
	Float_t smear_rphi;
	Int_t bool_scat;
	Int_t noise_type;
	UInt_t count_noise;
	Int_t moltTipo;
	Int_t vertTipo; // vertTipo=1 fz->gauss, vertTipo=0 fz->scelta 
	
	Double_t beam_rms;
	Double_t l1_rms;
	Double_t smear_phi1;
	Double_t smear_phi2;
};

class DataReader{
	private:
		DataReader();
		static DataReader* fInstance;
		bool fsuccesfulRead = false;
		
	public:
		static DataReader* getInstance();
		SimulationData Read_dat_simulation(const char * filen, bool debug = false);
		static void checkSimData(const SimulationData& simdata); //cout some stuff
		bool fileRead() const{ return fsuccesfulRead; }
		
};

#endif
