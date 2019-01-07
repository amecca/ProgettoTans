///////////////////////////////////////////////
// Macro to reconstruct events from the tree //
// Author A.Mecca (UNITO)                    // 
///////////////////////////////////////////////

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TCanvas.h"
#include "TH1F.h"

#include "Trackelet.h"

using std::cout;

//Move to a separate file
typedef struct{
	Double_t x0,y0,z0;
	Int_t m;
}VERTICE;


void Ricostruzione(TString fileName = "vertFile0_noscat.root", TString treeName = "VT"){
	//Histograms and Canvas
	TCanvas* cZetaV = new TCanvas("cZetaV","cZetaV",10,10,1200,1000);
	TH1F* hZetaV = new TH1F("hZetaV","hZetaV",100,-50,50);
	
	TCanvas* cZetaL1 = new TCanvas("cZetaL1","cZetaL1",10,10,1200,1000);
	TH1F* hZetaL1 = new TH1F("hZetaL1","hZetaL1",100,-50,50);
	TH1F* hZetaL2 = new TH1F("hZetaL2","hZetaL2",100,-50,50);
	
	//Open file and tree
	TFile* sourceFile = new TFile(fileName.Data());
	if(sourceFile->IsZombie()){
		cout<<"Error: \""<<fileName<<"\" not found\n";
		return;
	}
	else if(!(sourceFile->IsOpen())){
		cout<<"Error: could not open \""<<fileName<<"\"\n";
		return;
	}
	
	TTree* tree = dynamic_cast<TTree*>(sourceFile->Get(treeName.Data()));
	if(!tree){
		cout<<"\n\""<<tree<<"\" not found in \""<<fileName<<"\"\n";
		return;
	}
	
	//Containers
	VERTICE vtx;
	TClonesArray* L1Hits = new TClonesArray("Hit", 400);
	TClonesArray* L2Hits = new TClonesArray("Hit", 400);
	
	TBranch* b_vtx = tree->GetBranch("Vertex");
	b_vtx->SetAddress(&vtx);
	TBranch* b_L1Hits = tree->GetBranch("L1_hit");
	b_L1Hits->SetAddress(&L1Hits);
	TBranch* b_L2Hits = tree->GetBranch("L2_hit");
	b_L2Hits->SetAddress(&L2Hits);
	/*
	tree->GetBranch("Vertex")->SetAddress(&vtx);
	tree->GetBranch("L1_hit")->SetAddress(&L1Hits);
	tree->GetBranch("L2_hit")->SetAddress(&L2Hits);
	tree->SetBranchStatus("*",1); //Activates reading of all branches
	*/
	//Main loop
	long nEntries = tree->GetEntries();
	cout<<"Processing "<<nEntries<<" entries: ";
	for(long k = 0; k < 1000/*nEntries*/; k++){
		//if((100*k)%nEntries == 0) cout<<"\r\t\t\t\t"<< (100*k)/nEntries << " %";
		//if(tree->GetEntry(k) <= 0) continue;
		b_vtx->GetEntry(k);
		b_L1Hits->GetEntry(k);
		b_L2Hits->GetEntry(k);
		cout<<"\r\t\t\t\t"<<k+1;
		
		/*
		Trackelet* dummyTrackelet = new Trackelet(); //memory allocation
		for(int i = 0; i < L1Hits->GetEntries(); i++){
			Hit thisHit1 = *(Hit*)&((*L1Hits)[i]);
			hZetaL1->Fill(thisHit1.GetZ());
			for(int j = 0; j < L2Hits->GetEntries(); j++){
				/*
				new(dummyTrackelet) Trackelet(
					(Hit*)&((*L1Hits)[i]), 
					(Hit*)&((*L2Hits)[j])
				);//Avoid alloc-dealloc overhead by reusing the same memory
				
			}
		}
		delete dummyTrackelet;
		*/
		
		hZetaV->Fill(vtx.z0,1);
		
		L1Hits->Clear("C");
		L1Hits->Clear("C");
	}
	cout<<'\n';
	//Draw
	cZetaV->cd();
	hZetaV->Draw();
	
	cZetaL1->cd();
	hZetaL1->Draw();
	
	delete L1Hits;
	delete L2Hits;
	sourceFile->Close();
	return;
}
