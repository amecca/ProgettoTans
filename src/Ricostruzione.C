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
#include "TStopwatch.h"
#include "TSystem.h"

#include "Vertice.h"
#include "Trackelet.h"
#include "Utils.h"
//#include "Read_dat_simulation.cxx"

using std::cout;

//const Double_t tolleranza = 2.5; //cm; range intorno alla moda in cui i vertici ricostruiti vengono mediati
const Double_t tolMin = 0.5;
const Double_t deltaPhiMin = 0.005;//0.05;ris->0.029

Double_t mediaIntornoA(const std::vector<Double_t>& zRicostruiti, const Double_t& zModa, const Double_t& tolleranza);
Double_t findZ(const TClonesArray* L1Hits, const TClonesArray* L2Hits, const Double_t& deltaPhi, const Double_t& tolleranza, bool debug = false);

void Ricostruzione(TString fileName = "vertFile0_noscat.root", size_t nevents = 0){
	TString treeName = "VT";
	//CANVAS
	gStyle->SetOptStat(111111);
	//Risoluzione onnicomprensiva 
	TCanvas* cRisoluzioneAll = new TCanvas("cRisoluzioneAll","Risoluzione",10,10,1200,800);
	TH1I* hRisoluzioneAll = new TH1I("hRisoluzioneAll","Risoluzione inclusva", 200,-0.2,0.2);
	
	//Risoluzione al variare di Ztrue
	Double_t stdDevZ[11];
	Double_t sStdDevZ[11];
	std::vector<Double_t> xZ = {-13,-10,-7,-4,-2,0,2,4,7,10,13};
	Double_t sxZ[11];
	TCanvas* cRisoluzioneVsZtrue = new TCanvas("cRisoluzioneVsZtrue","cRisoluzioneVsZtrue",10,10,1200,800);
	TGraphErrors* hRisoluzioneVsZtrue;// = new TGraphErrors(11, xZ, stdDevZ, sxZ, sStdDevZ); //Sarà riempito prima di essere disegnato
	TH1I* hDeltaZs[11];
	for(size_t i=0; i<xZ.size(); i++){
		hDeltaZs[i] = new TH1I(Form("deltaZ%lu",i),Form("deltaZ%lu",i), 100,-0.5,0.5);
		sxZ[i] = 1./sqrt(12);
	}
	
	//Risoluzione al variare della molteplicità
	Double_t stdDevMol[10];
	Double_t sStdDevMol[10];
	std::vector<Double_t> xMol = {5,10,15,20,25,30,35,40,45,50};
	Double_t sxMol[10];
	for(size_t k=0; k<xMol.size(); k++) sxMol[k] = sqrt(xMol[k]); //Errore statistico
	TCanvas* cRisoluzioneVsMolt = new TCanvas("cRisoluzioneVsMolt","cRisoluzioneVsMolt",10,10,1200,800);
	TGraphErrors* hRisoluzioneVsMolt; //= new TGraphErrors(10, xMol, stdDevMol, sStdDevMol);
	TH1I* hRisols[11];
	for(size_t i=0; i<xMol.size(); i++){
		hRisols[i] = new TH1I(Form("Risol%lu",i),Form("RisolZ%lu",i), 100,-0.5,0.5);
		sxMol[i] = 1./sqrt(12);
	}
	
	//Efficienza al variare della molteplicità
	TCanvas* cEfficienzaVsMolt = new TCanvas("cEfficienzaVsMolt","cEfficienzaVsMolt",10,10,1200,800);
	TGraphAsymmErrors* hEfficienzaVsMolt;
	TH1I* hTotaliVsMolt = new TH1I("hTotaliVsMolt", "Totali;Molteplicita';# eventi", 10,0,50);
	TH1I* hRicostruitiVsMolt = new TH1I("hRicostruitiVsMolt", "Ricostruiti;Molteplicita';# eventi", 10,0,50);
	
	//Efficienza/Molteplicità per eventi ricostruiti entro 1 sigma (rms z)
	TCanvas* cEfficienzaVsMolt1sigma = new TCanvas("cEfficienzaVsMolt1sigma","Efficienza (entro 1 sigma)",10,10,1200,800);
	TGraphAsymmErrors* hEfficienzaVsMolt1sigma;
	TH1I* hTotaliVsMolt1sigma = new TH1I("hTotaliVsMolt1sigma", "Totali (1sigma);Molteplicita';# eventi", 10,0,50);
	TH1I* hRicostruitiVsMolt1sigma = new TH1I("hRicostruitiVsMolt1sigma", "Ricostruiti (1sigma);Molteplicita';# eventi", 10,0,50);
	
	//Efficienza al variare della Ztrue
	TCanvas* cEfficienzaVsZ = new TCanvas("cEfficienzaVsZ","cEfficienzaVsZ",10,10,1200,800);
	TGraphAsymmErrors* hEfficienzaVsZ;
	TH1I* hTotaliVsZ = new TH1I("hTotaliVsZ", "Totali;Z_{true} [cm];# eventi", 13,-19.5,19.5);
	TH1I* hRicostruitiVsZ = new TH1I("hRicostruitiVsZ", "Ricostruiti;Z_{true} [cm];# eventi", 13,-19.5,19.5);
	
	//Risoluzione al variare di maxPhi (nella ricostruzione delle tracce)
	Double_t stdDevPhi[8];
	Double_t sStdDevPhi[8];
	std::vector<Double_t> xPhi = {0.0001,0.0002,0.0003,0.0005,0.001,0.002,0.005,0.01};
	Double_t sxPhi[8];
	TCanvas* cRisoluzioneVsMaxPhi = new TCanvas("cRisoluzioneVsMaxPhi","cRisoluzioneVsMaxPhi",10,10,1200,800);
	TGraphErrors* hRisoluzioneVsMaxPhi;// = new TGraphErrors(11, &xPhi[0], stdDevPhi, sxPhi, sStdDevPhi); //Sarà riempito prima di essere disegnato
	TH1I* hRisoluzionePhis[8];
	for(size_t i=0; i<xPhi.size(); i++){
		hRisoluzionePhis[i] = new TH1I(Form("Phi%lu",i),Form("Phi%lu",i), 100,-0.5,0.5);
		sxPhi[i] = 0.;
	}
	
	//Efficienza al variare di maxPhi (nella ricostruzione delle tracce)
	TCanvas* cEfficienzaVsPhi = new TCanvas("cEfficienzaVsPhi","cEfficienzaVsPhi", 10,10,1200,800);
	TGraphAsymmErrors* hEfficienzaVsPhi;
	TH1I* hTotPhi = new TH1I("hTotPhi","hTotPhi", 101,0.,0.0101);
	TH1I* hEffPhi = new TH1I("hEffPhi","hEffPhi", 101,0.,0.0101);
	
	//Open file and tree
	TFile* sourceFile = Utils::findAndOpenFile(fileName);
	if (!sourceFile) return;
	
	TTree* tree = (TTree*)(sourceFile->Get(treeName.Data()));
	if(!tree){
		cout<<"\n\""<<treeName<<"\" not found in \""<<fileName<<"\"\n";
		return;
	}
	
	//Containers
	VERTICE vtx;
	TClonesArray* L1Hits = new TClonesArray("Hit", 400);
	TClonesArray* L2Hits = new TClonesArray("Hit", 400);
	
	tree->GetBranch("Vertex")->SetAddress(&vtx);
	tree->GetBranch("L1_hit")->SetAddress(&L1Hits);
	tree->GetBranch("L2_hit")->SetAddress(&L2Hits);
	tree->SetBranchStatus("*",1); //Activates reading of all branches
	
	TStopwatch stopwatch;
	//Main loop over entries
	size_t nEntries = tree->GetEntries();
	size_t toProcess = (nevents == 0 ? nEntries : min(nEntries, nevents));
	cout<<"Processing "<<toProcess<<" entries: ";
	UInt_t nonRicostruiti = 0;
	
	for(size_t e = 0; e < toProcess; e++){

		if(tree->GetEntry(e) <= 0) continue;
		cout<<"\r\t\t\t\t"<<e+1;

		hTotaliVsMolt->Fill(vtx.m);
		if(fabs(vtx.z0) < 5.3 /*rms_z*/) hTotaliVsMolt1sigma->Fill(vtx.m);
		hTotaliVsZ->Fill(vtx.z0);
		
		Double_t zRicostruito = findZ(L1Hits, L2Hits, deltaPhiMin, tolMin);
		if(zRicostruito <-999.){
			nonRicostruiti++;
			//cout<<"\n-1000"<<"  vtx.m = "<<vtx.m<<"  vtx.z0 = "<<vtx.z0<<"  L1Hits: "<<L1Hits->GetEntries()<<"  L2Hits: "<<L2Hits->GetEntries();
		    
		}
		else{
			Double_t deltaZ = zRicostruito - vtx.z0;
			hRisoluzioneAll->Fill(deltaZ);
			
			for(size_t i=0; i<xZ.size(); i++){
				if(fabs(vtx.z0 - xZ[i]) < 1.5){
					hDeltaZs[i]->Fill(deltaZ);
					break;
				}
			}
			for(size_t i=0; i<xMol.size(); i++){
				if(fabs(vtx.m - xMol[i]) < 1){
					hRisols[i]->Fill(deltaZ);
					break;
				}
			}
			hRicostruitiVsMolt->Fill(vtx.m);
			if(fabs(vtx.z0) < 5.3/*rms_z*/) hRicostruitiVsMolt1sigma->Fill(vtx.m);
			hRicostruitiVsZ->Fill(vtx.z0);
		}
		
		//Risoluzione al variare della phiMax
		for(size_t i = 0; i < xPhi.size(); i++){
			hTotPhi->Fill(xPhi[i]);
			Double_t zRec = findZ(L1Hits, L2Hits, xPhi[i], tolMin);
			//cout<<xPhi[i]<<"   "<<zRec<<'\n';
			if(fabs(zRec-vtx.z0) > 5.) continue;
			else{
				hRisoluzionePhis[i]->Fill(zRec - vtx.z0);
				hEffPhi->Fill(xPhi[i]);
			}
		}
		
		//hZetaV->Fill(vtx.z0,1);
		
		L1Hits->Clear("C");
		L1Hits->Clear("C");
	}
	
	cout<<"\nElapsed time: "<<stopwatch.CpuTime()<<" s\n";
	
	cRisoluzioneAll->cd();
	hRisoluzioneAll->Draw();
	
	//Recupero i dati per i grafici
	for(size_t i = 0; i < xZ.size(); i++){
		stdDevZ[i] = hDeltaZs[i]->GetStdDev();
		sStdDevZ[i] = hDeltaZs[i]->GetStdDevError();
	}
	cRisoluzioneVsZtrue->cd();
	hRisoluzioneVsZtrue = new TGraphErrors(xZ.size(), &(xZ[0]), stdDevZ, sxZ, sStdDevZ);
	hRisoluzioneVsZtrue->SetTitle("Risoluzione vs Z_{true};Z_{true} [cm];Risoluzione [cm]");
	hRisoluzioneVsZtrue->SetMinimum(0.);
	hRisoluzioneVsZtrue->Draw();
	
	for(size_t i = 0; i < xMol.size(); i++){
		stdDevMol[i] = hRisols[i]->GetStdDev();
		sStdDevMol[i] = hRisols[i]->GetStdDevError();
	}
	cRisoluzioneVsMolt->cd();
	hRisoluzioneVsZtrue = new TGraphErrors(xMol.size(), &(xMol[0]), stdDevMol, sxMol, sStdDevMol);
	hRisoluzioneVsZtrue->SetTitle("Risoluzione vs Molteplicita';Molteplicita';Risoluzione [cm]");
	hRisoluzioneVsZtrue->SetMinimum(0.);
	hRisoluzioneVsZtrue->Draw();
	
	cEfficienzaVsMolt->cd();
	hEfficienzaVsMolt = new TGraphAsymmErrors(hRicostruitiVsMolt, hTotaliVsMolt);
	hEfficienzaVsMolt->SetTitle("Efficienza;Molteplicita';Ricostruiti/Totali");
	hEfficienzaVsMolt->SetMinimum(0.);
	hEfficienzaVsMolt->Draw();
	
	cEfficienzaVsMolt1sigma->cd();
	hEfficienzaVsMolt1sigma = new TGraphAsymmErrors(hRicostruitiVsMolt1sigma, hTotaliVsMolt1sigma);
	hEfficienzaVsMolt1sigma->SetTitle("Efficienza (vertice entro 1#sigma);Molteplicita';Ricostruiti/Totali");
	hEfficienzaVsMolt1sigma->SetMinimum(0.);
	hEfficienzaVsMolt1sigma->Draw();
	
	cEfficienzaVsZ->cd();
	hEfficienzaVsZ = new TGraphAsymmErrors(hRicostruitiVsZ, hTotaliVsZ);
	hEfficienzaVsZ->SetTitle("Efficienza vs Z_{true};Z_{true} [cm]';Ricostruiti/Totali");
	hEfficienzaVsZ->SetMinimum(0.);
	hEfficienzaVsZ->Draw();
	
	//Risoluzione al variare di phiMax
	for(size_t i = 0; i < xPhi.size(); i++){
		stdDevPhi[i] = hRisoluzionePhis[i]->GetStdDev();
		sStdDevPhi[i] = hRisoluzionePhis[i]->GetStdDevError();
	}
	cRisoluzioneVsMaxPhi->cd();
	hRisoluzioneVsMaxPhi = new TGraphErrors(11, &xPhi[0], stdDevPhi, sxPhi, sStdDevPhi);
	hRisoluzioneVsMaxPhi->SetMinimum(0.);
	hRisoluzioneVsMaxPhi->SetTitle("Risoluzione Vs #phi max; #phi max [rad]");
	hRisoluzioneVsMaxPhi->Draw("AP");
	
	cEfficienzaVsPhi->cd();
	hEfficienzaVsPhi = new TGraphAsymmErrors(hEffPhi, hTotPhi);
	hEfficienzaVsPhi->SetMinimum(0.);
	hEfficienzaVsPhi->SetTitle("Efficienza Vs #phi max; #phi max [rad]");
	hEfficienzaVsPhi->Draw();
	
	sourceFile->Close();
	//cout<<"\nNon Ricostruiti: "<<nonRicostruiti<<"\n";
	return;
}





//Implementazione
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
		//cout<<"\n-----------------------------------------\n";
		//cout<<"zModa = "<<zModa<<"\nzRicostruiti = ";
		//for(size_t k = 0; k < zRicostruiti.size(); k++)
			//cout<<zRicostruiti.at(k)<<"  ";
		return -1000.;
	}
}

Double_t findZ(const TClonesArray* L1Hits, const TClonesArray* L2Hits, const Double_t& maxPhi, const Double_t& tolleranza, bool debug){
	
	TObject* thisObj1;
	TObject* thisObj2;
	Hit* thisHit1; 
	Hit* thisHit2;
	
	std::vector<Double_t> zRicostruiti;
	for(int i = 0; i < L1Hits->GetEntries(); i++){
		thisObj1 = (*L1Hits)[i];
		thisHit1 = (Hit*)thisObj1;
		
		for(int j = 0; j < L2Hits->GetEntries(); j++){
			thisObj2 = (*L2Hits)[j];
			thisHit2 = (Hit*)thisObj2;
				
			if(Hit::deltaPhi(thisHit1,thisHit2) < maxPhi){ 
				Trackelet dummyTrackelet(thisHit1, thisHit2);
				zRicostruiti.push_back(dummyTrackelet.findZVertex());
			}
		}
	}
	
			
	TH1I hFindModa = TH1I("findModa", "findModa",40,-20,20);
	for(size_t j = 0; j < zRicostruiti.size(); j++){
		hFindModa.Fill(zRicostruiti.at(j));
	}
		
	if(zRicostruiti.size() == 0){
		if(debug){
			//cout<<"  vtx.m = "<<vtx.m<<"  vtx.z0 = "<<vtx.z0<<"  L1Hits: "<<L1Hits->GetEntries()<<"  L2Hits: "<<L2Hits->GetEntries();
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
		}
		return -1050.;
	}

	Int_t binMaximum = hFindModa.GetMaximumBin();
	Double_t zModa = hFindModa.GetXaxis()->GetBinCenter(binMaximum);
	Double_t zRicostruitoMean = mediaIntornoA(zRicostruiti, zModa, tolleranza);
	/*
	if(zRicostruitoMean <-999.){
		nonRicostruiti++;
		cout<<"\n-1000"<<"  vtx.m = "<<vtx.m<<"  vtx.z0 = "<<vtx.z0<<"  L1Hits: "<<L1Hits->GetEntries()<<"  L2Hits: "<<L2Hits->GetEntries();
	}
	*/
	return zRicostruitoMean;
}
