#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "Punto.h"
#include "TClonesArray.h"
#include "GeneraEvento.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TString.h"

const unsigned int NUMBER_OF_EVENTS = 5;

// Definizione di una struct
typedef struct {
	Punto v;
	Int_t mult;} VTX;
    
typedef struct{
	double th;
	double phi;
} Angles;

void GeneraTree(const char* filename = nullptr)
{
  bool debug = true;
  TString tFilename = TString(filename);
  // Apertura del file di output
  TString treePathName = "Data/"+(filename != nullptr ? tFilename : "htree")+".root";
  TFile hfile(treePathName.Data(),"RECREATE");
  // dichiarazione del TTree
  TTree *tree = new TTree("T","TTree con 2 branches");
    // se invertissi l'ordine dovrei scrivere
    // tree->SetDirectory(&hfile);

  // Dichiarazione di un TClonesArray
  TClonesArray *ptrhits = new TClonesArray("Punto",NUMBER_OF_EVENTS);
  TClonesArray& hits = *ptrhits;
    
  static VTX point;

  // Dichiarazione dei 2 branch del TTree
  tree->Branch("VertMult","VTX",&point/*,32000,1*/); //non so come ottimizzare bufsize(32000)
  tree->Branch("Hits",&ptrhits);
  //tree->Branch("Particles","Angles");
  
  //GeneraEvento *event = new GeneraEvento(); // creo un oggetto GeneraEvento
 
  for(UInt_t i=0; i<NUMBER_OF_EVENTS;i++){ // loop sugli eventi (provvisorio)

    unsigned int numpart = 0;
    while(numpart <= 0){
      numpart = GeneraEvento::GeneraMolt();
    }
    if(debug) cout<<"molt"<<numpart<<endl; 
    point.mult = numpart;
    point.v = GeneraEvento::GeneraVertice();
    
		//TClonesArray* angles = new TClonesArray("Angles");
    /*
    point.phi=event->GetPhi();
    point.teta = 2*TMath::ATan(TMath::Exp(- event->GetEta()));// !!! PHI E DEF TRA 0 E 2PI
    */
    
    //________________________________________________
    for (unsigned int j=0; j<numpart; j++){

      // Genero un hit in modo del tutto random (dummy)
      
      new(hits[j])Punto(-5.+gRandom->Rndm()*10.,5.+gRandom->Rndm()*10,15.+gRandom->Rndm()*10.);
    }

      // Debug
      printf("Evento %d - moltepl: %d\n",i,numpart);
            /*
      printf("x= %f ; y= %f; z= %f \n",point.X,point.Y,point.Z);
      printf("Entries nel TClonesArray: %d\n",ptrhits->GetEntries());
      for (Int_t j=0; j<hits.GetEntries(); j++){
	Punto *tst=(Punto*)ptrhits->At(j);
	cout<<"Punto "<<j<<") x, y, z = "<<tst->GetX()<<"; "<<tst->GetY()<<"; "<<tst->GetZ()<<endl;
      }
      // fine del debug
*/

      tree->Fill();
      ptrhits->Clear();

  }

  // Save all objects in this file
  hfile.Write();

  // Close the file. 
  hfile.Close();

}




