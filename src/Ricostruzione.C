///////////////////////////////////////////////
// Macro to reconstruct events from the tree //
// Author A.Mecca (UNITO)                    // 
///////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <algorithm> //std::min

#include "TFile.h"
#include "TStyle.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TCanvas.h"
#include "TH1F.h"
//#include "TMath.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
//#include "Read_dat_simulation.cxx"

#include "Trackelet.h"

using std::cout;

const Double_t tolleranza = 2.5; //cm; range intorno alla moda in cui i vertici ricostruiti vengono mediati

//Move to a separate file
typedef struct{
	Double_t x0,y0,z0;
	Int_t m;
}VERTICE;

Double_t mediaIntornoA(const std::vector<Double_t>& zRicostruiti, const Double_t& zModa, const Double_t& tolleranza);
//Double_t deltaPhi(const Hit* hit1, const Hit* hit2);
void Ricostruzione(TString fileName = "vertFile0_noscat.root", TString treeName = "VT"){
	
	Double_t stdDev[5];
	Double_t sStdDev[5];
	
	//Histograms and Canvas
	//TCanvas* ctagli = new TCanvas("ctagli","ctagli",10,10,1200,800);
	//TH1I* tagli = new TH1I("tagli","Tagli",3,0,3);
	//tagli->SetMinimum(0);
	
	TCanvas* cTotvsMult = new TCanvas("cTotvsMult","cTotvsMult",10,10,1200,800);
	TCanvas* cRecvsMult = new TCanvas("cRecvsMult","cRecvsMult",10,10,1200,800);
	
	TCanvas* deltaZ = new TCanvas("deltaZ","#Delta z",10,10,1200,800);
	deltaZ->Divide(3,2);
	gStyle->SetOptStat(1111111);
	TH1F* hDeltaZs[5];
	
	TCanvas* EffVsPhi = new TCanvas("EffVsPhi"," EffVsPhi",10,10,1200,800);
	EffVsPhi->Divide(3,2);
	TH1F* hRecVsPhiVsMolt[5];
	
	TCanvas* cEffVsMolt = new TCanvas("cEffVsMolt","Efficienza", 10,10,1200,800);
	TH1I* hRecVsMolt = new TH1I("hRecVsMolt","Ricostruiti",6,0,60);
	TH1I* hTotVsMolt = new TH1I("hTotVsMolt","Totali",6,0,60);
	
	TCanvas* cRisoluzione = new TCanvas("cRisoluzione","Risoluzione", 10,10,1200,800);
	
	for(int i = 0; i < 5; i++){
		char hName[30] = "hDeltaZ";
		strcat(hName,(char*)&i);
		char hTitle[30] = "Z_{rec} - Z_{true} ";
		strcat(hTitle,(char*)&i);
		hDeltaZs[i] = new TH1F(hName, hTitle, 100,-0.5,0.5);
		
		char hName2[30] = "hRecVsPhiVsMolt";
		strcat(hName2,(char*)&i);
		char hTitle2[30] = "Titolo ";
		strcat(hTitle2,(char*)&i);
		hRecVsPhiVsMolt[i] = new TH1F(hName2, hTitle2, 6,0,60);
		
	}
	
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
	
	TTree* tree = (TTree*)(sourceFile->Get(treeName.Data()));
	if(!tree){
		cout<<"\n\""<<tree<<"\" not found in \""<<fileName<<"\"\n";
		return;
	}
	
	//Read constants from file
	//Read_dat_simulation("Data/Dati_Simulazione.dat");
	
	//Containers
	VERTICE vtx;
	TClonesArray* L1Hits = new TClonesArray("Hit", 400);
	TClonesArray* L2Hits = new TClonesArray("Hit", 400);
	
	tree->GetBranch("Vertex")->SetAddress(&vtx);
	tree->GetBranch("L1_hit")->SetAddress(&L1Hits);
	tree->GetBranch("L2_hit")->SetAddress(&L2Hits);
	tree->SetBranchStatus("*",1); //Activates reading of all branches
	
	//Main loop
	long nEntries = tree->GetEntries();
	cout<<"Processing "<<nEntries<<" entries: ";
	UInt_t nonRicostruiti = 0;
	const bool tutti = false;
	for(long e = 0; e < (tutti ? nEntries : 1000); e++){
		//tagli->Fill(0);
		if(tree->GetEntry(e) <= 0) continue;
		//cout<<"\r\t\t\t\t"<<e+1;
		
		//Riempimento dei grafici "Numero totale di eventi"
		hTotVsMolt->Fill(vtx.m);

		
		Trackelet* dummyTrackelet = new Trackelet(); //memory allocation
		TObject* thisObj1;
		TObject* thisObj2;
		Hit* thisHit1; 
		Hit* thisHit2;
//		std::vector<Double_t> zRicostruiti;
		std::vector<Double_t> zRicostruiti[5];
		
		for(Int_t l = 0; l<5; l++){
		
			for(int i = 0; i < L1Hits->GetEntries(); i++){
				thisObj1 = (*L1Hits)[i];
				thisHit1 = (Hit*)thisObj1;
			
				for(int j = 0; j < L2Hits->GetEntries(); j++){
					thisObj2 = (*L2Hits)[j];
					thisHit2 = (Hit*)thisObj2;
				
					
					if(Hit::deltaPhi(thisHit1,thisHit2)<0.03*(l+1)){ 
						 new(dummyTrackelet) Trackelet(thisHit1, thisHit2); //Avoid alloc-dealloc overhead by reusing the same memory
						zRicostruiti[l].push_back(dummyTrackelet->findZVertex());
					}
				}
			}
		}
	
		//tagli->Fill(1);
		
		TH1I hFindModa[5];
		for(size_t k = 0; k<5; k++){
			char hName[20] = "hciccio";
			strcat(hName,(char*)&k);
			hFindModa[k] = TH1I(hName, hName,40,-20,20);
			for(size_t j = 0; j < zRicostruiti[k].size(); j++){
				hFindModa[k].Fill(zRicostruiti[k].at(j));
				
			}	
		}
		
		//GetMaximumBin() -> trova il numero del bin (asse x) corrispondente al valore massimo (sulle y)
		//GetBinLowEdge() -> trova il valore dell'estremo inf del bin (asse y)
		//GetBinWidth() -> larghezza del bin: vogliamo ottenere il centro della cella
		
		for(size_t j = 0; j<5;j++){
		if(zRicostruiti[j].size() == 0){
		
			cout<<"\nj = "<<j<<"  vtx.m = "<<vtx.m<<"  vtx.z0 = "<<vtx.z0<<"  L1Hits: "<<L1Hits->GetEntries()<<"  L2Hits: "<<L2Hits->GetEntries();
			cout<<"\n\tL1: ";
			for(int i = 0; i < L1Hits->GetEntries(); i++){
				thisObj1 = (*L1Hits)[i];
				thisHit1 = (Hit*)thisObj1;
				cout<<thisHit1->GetPHI()<<" ";
			}
			cout<<"\n\tL2: ";
			for(int in = 0; in < L2Hits->GetEntries(); in++){
				thisObj2 = (*L2Hits)[in];
				thisHit2 = (Hit*)thisObj2;
				cout<<thisHit2->GetPHI()<<" ";
			}
			continue;
		}
		Int_t binMaximum = hFindModa[j].GetMaximumBin();
		Double_t zModa = hFindModa[j].GetXaxis()->GetBinCenter(binMaximum);
		Double_t zRicostruitoMean = mediaIntornoA(zRicostruiti[j], zModa, tolleranza);
			if(zRicostruitoMean <-999.){
				nonRicostruiti++;
				cout<<"\n-1000"<<"  vtx.m = "<<vtx.m<<"  vtx.z0 = "<<vtx.z0<<"  L1Hits: "<<L1Hits->GetEntries()<<"  L2Hits: "<<L2Hits->GetEntries();
			}
			else{
				hRecVsPhiVsMolt[j]->Fill(vtx.m);
				//if(j == 2) hRecVsMolt->Fill(vtx.m,1);
				const Double_t risoluzione = zRicostruitoMean - vtx.z0;
				hDeltaZs[j]->Fill(risoluzione); //Risoluzione inclusiva
				//tagli->Fill(2);
			}	
			
		}
		delete dummyTrackelet;
		//delete thisObj1;
		//delete thisHit1;
		
		//hZetaV->Fill(vtx.z0,1);
		
		L1Hits->Clear("C");
		L1Hits->Clear("C");
	}
	//cout<<"\nEventi non ricostruiti: "<<nonRicostruiti<<'\n';
	//Draw
	//cZetaV->cd();
	//hZetaV->Draw();
	/*
	Double_t BestSigma = 100.;
	Int_t indexOfBest = 0;
	for(Int_t i=0;i<5;i++){
	 	Double_t temp = hDeltaZs[i]->GetStdDev();
	 	if(temp < BestSigma){
	 		BestSigma = temp;
	 		indexOfBest = i;
 		}
	}
	cout<<"\nBest phi choice: "<<indexOfBest<<" --> phi = "<<(indexOfBest+1)*0.02<<'\n';
	deltaZ->cd();
	hDeltaZs[indexOfBest]->Draw();*/

	for(int intero = 0; intero <5; intero++){
		//deltaZ->cd(intero+1);
		//hDeltaZs[intero]->Draw();
		stdDev[intero] = hDeltaZs[intero]->GetStdDev();
		sStdDev[intero] = hDeltaZs[intero]->GetStdDevError();
	}
	
	Double_t x[] = {1,2,3,4,5};
	Double_t sx[] = {0,0,0,0,0};
	TGraphErrors* hRisoluzione = new TGraphErrors(5, x, stdDev, sx, sStdDev);
	
	cRisoluzione->cd();
	hRisoluzione->Draw();
	
	TGraphAsymmErrors* resVsPhi[5];
	for(int c= 0; c<5; c++){
		EffVsPhi->cd(c+1);
		resVsPhi[c] = new TGraphAsymmErrors(hRecVsPhiVsMolt[c], hTotVsMolt, "cp");
		resVsPhi[c]->SetMinimum(0.);
		resVsPhi[c]->Draw();
	}
	
	//ctagli->cd();
	//tagli->Draw();
	
	//cTotvsMult->cd();
	//hTotVsMolt->Draw();
	
	//cRecvsMult->cd();
	//hRecVsMolt->Draw();
	
	/*
	cEffVsMolt->cd();
	TGraphAsymmErrors* hEffVsMolt = new TGraphAsymmErrors(hRecVsMolt, hTotVsMolt,"cp"); //cp -> Clopper-Pearson
	hEffVsMolt->SetMinimum(0.);
	hEffVsMolt->SetMaximum(1.);
	hEffVsMolt->Draw();
	*/
	
	//cZetaL1->cd();
	//hZetaL1->Draw();
	
	delete L1Hits;
	delete L2Hits;
	sourceFile->Close();
	cout<<"\nNon Ricostruiti: "<<nonRicostruiti<<"\n";
	return;
}

Double_t mediaIntornoA(const std::vector<Double_t>& zRicostruiti, const Double_t& zModa, const Double_t& tolleranza){
	Double_t temp = 0.;
	UInt_t zMediati = 0;
	for(size_t k = 0; k < zRicostruiti.size(); k++){
		if(fabs(zRicostruiti.at(k) - zModa) < tolleranza){
			temp += zRicostruiti.at(k);
			zMediati++;
		}
	}
	if(zMediati > 0)
		return temp/zMediati;
	else{
		cout<<"\n-----------------------------------------\n";
		cout<<"zModa = "<<zModa<<"\nzRicostruiti = ";
		for(size_t k = 0; k < zRicostruiti.size(); k++)
			cout<<zRicostruiti.at(k)<<"  ";
		return -1000.;
	}
}

/*
Double_t deltaPhi(const Hit* hit1, const Hit* hit2){
	Double_t phi1 = hit1->GetPHI();
	Double_t phi2 = hit2->GetPHI();
	
	double result = phi1 - phi2;
    while (result > TMath::Pi()) result -= 2*M_PI;
    while (result <= -TMath::Pi()) result += 2*M_PI;
    return fabs(result);
}*/
