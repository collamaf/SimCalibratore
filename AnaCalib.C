#define NHOLE 11
#define NORGAN 10
{
	gStyle->SetPalette(kRainBow);
	double THR=0;
	
	TFile* outFile=new TFile(Form("CalibOut%d.root",int(THR)),"RECREATE");
	TString fileNameTot;
	
	TDirectory* dirStack=outFile->mkdir("Stack");
	TDirectory* dirEffHole=outFile->mkdir("EffHole");
	TDirectory* dirEffOrgan=outFile->mkdir("EffOrgan");

	int NumPrim=50000;
	
	std::vector<double> vecGoodHole[NHOLE];
	std::vector<double> vecGoodOrgan[NORGAN];

	std::vector<double> vecBkgHole[NHOLE];
	std::vector<double> vecBkgOrgan[NORGAN];
	
	TGraph* EffRelHole[NHOLE];
	TMultiGraph* EffRelHoleAll=new TMultiGraph;
	EffRelHoleAll->SetTitle("EffRelHole (Fraction of events entering the scintillator that are detected); Organ Thickness [mm]; f");
	EffRelHoleAll->SetName("EffRelHoleAll");
	
	TGraph* EffAbsHole[NHOLE];
	TMultiGraph* EffAbsHoleAll=new TMultiGraph;
	EffAbsHoleAll->SetTitle("EffAbsHole (Fraction of generated events  that are detected); Organ Thickness [mm]; f");
	EffAbsHoleAll->SetName("EffAbsHoleAll");
	
	TGraph* EffRelOrgan[NORGAN];
	TMultiGraph* EffRelOrganAll=new TMultiGraph;
	EffRelOrganAll->SetTitle("EffRelOrgan (Fraction of events entering the scintillator that are detected); Hole depth [mm]; f");
	EffRelOrganAll->SetName("EffRelOrgan");
	
	TGraph* EffAbsOrgan[NORGAN];
	TMultiGraph* EffAbsOrganAll=new TMultiGraph;
	EffAbsOrganAll->SetTitle("EffAbsOrgan (Fraction of generated events  that are detected); Hole depth  [mm]; f");
	EffAbsOrganAll->SetName("EffAbsOrgan");
	
	
	int OrganZs[NORGAN]={
		10,
		20,
		30,
		40,
		50,
		60,
		70,
		80,
		90,
		100
	};
	
	int HoleZs[NHOLE]={
		0,
		10,
		20,
		30,
		40,
		50,
		60,
		70,
		80,
		90,
		100
	};
	
	TCanvas* canvHole[NHOLE];
	THStack * stackHole[NHOLE];
	
	// #####################################################################
	// ################ FIXING A HOLE DEPTH, WE VARY THE ORGAN THICKNESS
	cout<<" ################ FIXING A HOLE DEPTH, WE VARY THE ORGAN THICKNESS "<<endl;
	for (int iHole=0; iHole<NHOLE; iHole++) {
		EffRelHole[iHole]=new TGraph();
		EffRelHole[iHole]->SetName(Form("EffRelHoleZ%dmm",HoleZs[iHole]));
		EffRelHole[iHole]->SetTitle(Form("EffRelHoleZ%dmm",HoleZs[iHole]));
		EffRelHole[iHole]->GetXaxis()->SetTitle("Organ Thickness [mm]");
		EffRelHole[iHole]->GetYaxis()->SetTitle("f");
		EffRelHole[iHole]->SetMarkerStyle(23);
		
		EffAbsHole[iHole]=new TGraph();
		EffAbsHole[iHole]->SetName(Form("EffAbsHoleZ%dmm",HoleZs[iHole]));
		EffAbsHole[iHole]->SetTitle(Form("EffAbsHoleZ%dmm",HoleZs[iHole]));
		EffAbsHole[iHole]->GetXaxis()->SetTitle("Organ Thickness [mm]");
		EffAbsHole[iHole]->GetYaxis()->SetTitle("f");
		EffAbsHole[iHole]->SetMarkerStyle(23);
		
		
		stackHole[iHole]=new THStack(Form("stackHole%d",iHole),Form("stackHole%d",iHole));
		cout<<"ANALIZZO HOLE NUM :"<< iHole<<" cioè profondo [mm] "<<HoleZs[iHole]/10<< ", THR= "<<THR<<endl;
		canvHole[iHole]=new TCanvas();
		canvHole[iHole]->SetTitle(Form("Hole%d",iHole));
		canvHole[iHole]->SetName(Form("Hole%d",iHole));
		for (int iOrgan=0; iOrgan<NORGAN; iOrgan++) {
			fileNameTot=Form("build/CALIBmc_HoleZ%d_OrganZ%d_N%d.root",HoleZs[iHole], OrganZs[iOrgan], (iOrgan+1)*NumPrim);
			TFile * file=TFile::Open(fileNameTot);
			TString taglio=Form("Eabs>%lf",THR);
			vecGoodHole[iHole].push_back(B1->Draw("Eabs>>histo",taglio, ""));
			cout<<vecGoodHole[iHole].back()<<endl;
			stackHole[iHole]->Add(histo);
			
			vecBkgHole[iHole].push_back(B1->Draw("PreScintPart","PreScintPart>0", ""));
			EffRelHole[iHole]->SetPoint(iOrgan,OrganZs[iOrgan],vecGoodHole[iHole].back()/vecBkgHole[iHole].back());
			EffAbsHole[iHole]->SetPoint(iOrgan,OrganZs[iOrgan],vecGoodHole[iHole].back()/((iOrgan+1)*NumPrim));

		}
		outFile->cd();
		dirStack->cd();
		stackHole[iHole]->Write();
		dirEffHole->cd();
		EffRelHole[iHole]->Write();
		EffAbsHole[iHole]->Write();
		outFile->cd();
		EffRelHoleAll->Add(EffRelHole[iHole],"P");
		EffAbsHoleAll->Add(EffAbsHole[iHole],"P");
	}
	
	
	TCanvas* canvOrgan[NORGAN];
	THStack * stackOrgan[NORGAN];
	
	// #####################################################################
	// ################ FIXING A ORGAN THICKNESS, WE VARY THE HOLE DEPTH
	cout<<" ################ FIXING A ORGAN THICKNESS, WE VARY THE HOLE DEPTH "<<endl;
	for (int iOrgan=0; iOrgan<NORGAN; iOrgan++) {
		EffRelOrgan[iOrgan]=new TGraph();
		EffRelOrgan[iOrgan]->SetName(Form("EffRelOrganZ%dmm",OrganZs[iOrgan]));
		EffRelOrgan[iOrgan]->SetTitle(Form("EffRelOrganZ%dmm",OrganZs[iOrgan]));
		EffRelOrgan[iOrgan]->GetXaxis()->SetTitle("Organ Thickness [mm]");
		EffRelOrgan[iOrgan]->GetYaxis()->SetTitle("f");
		EffRelOrgan[iOrgan]->SetMarkerStyle(23);
		
		EffAbsOrgan[iOrgan]=new TGraph();
		EffAbsOrgan[iOrgan]->SetName(Form("EffAbsOrganZ%dmm",OrganZs[iOrgan]));
		EffAbsOrgan[iOrgan]->SetTitle(Form("EffAbsOrganZ%dmm",OrganZs[iOrgan]));
		EffAbsOrgan[iOrgan]->GetXaxis()->SetTitle("Organ Thickness [mm]");
		EffAbsOrgan[iOrgan]->GetYaxis()->SetTitle("f");
		EffAbsOrgan[iOrgan]->SetMarkerStyle(23);
		
		stackOrgan[iOrgan]=new THStack(Form("stackOrgan%d",iOrgan),Form("stackOrgan%d",iOrgan));
		cout<<"ANALIZZO ORGAN DEPTH NUM :"<< iOrgan<<" cioè profondo [mm] "<<OrganZs[iOrgan]/10<< ", THR= "<<THR<<endl;
		int whichOrgan=OrganZs[iOrgan];
		canvOrgan[iOrgan]=new TCanvas();
		canvOrgan[iOrgan]->SetTitle(Form("Organ%d",iOrgan));
		canvOrgan[iOrgan]->SetName(Form("Organ%d",iOrgan));
		for (int iHole=0; iHole<NHOLE; iHole++) {
			fileNameTot=Form("build/CALIBmc_HoleZ%d_OrganZ%d_N%d.root",HoleZs[iHole], OrganZs[iOrgan], (iOrgan+1)*NumPrim);
			TFile * file=TFile::Open(fileNameTot);
			TString taglio=Form("Eabs>%lf",THR);
			vecGoodOrgan[iOrgan].push_back(B1->Draw("Eabs>>histo",taglio, ""));
			cout<<vecGoodOrgan[iOrgan].back()<<endl;
			stackOrgan[iOrgan]->Add(histo);
			
			vecBkgOrgan[iOrgan].push_back(B1->Draw("PreScintPart","PreScintPart>0", ""));
			EffRelOrgan[iOrgan]->SetPoint(iHole,HoleZs[iHole],vecGoodOrgan[iOrgan].back()/vecBkgOrgan[iOrgan].back());
			EffAbsOrgan[iOrgan]->SetPoint(iHole,HoleZs[iHole],vecGoodOrgan[iOrgan].back()/((iOrgan+1)*NumPrim));

		}
		outFile->cd();
		dirStack->cd();
		stackOrgan[iOrgan]->Write();
		dirEffOrgan->cd();
		EffRelOrgan[iOrgan]->Write();
		EffAbsOrgan[iOrgan]->Write();
		outFile->cd();
		EffRelOrganAll->Add(EffRelOrgan[iOrgan],"P");
		EffAbsOrganAll->Add(EffAbsOrgan[iOrgan],"P");
	}
	
	EffRelHoleAll->Draw("A pmc");
	EffRelHoleAll->Write();

	EffAbsHoleAll->Draw("A pmc");
	EffAbsHoleAll->Write();

	EffRelOrganAll->Draw("A pmc");
	EffRelOrganAll->Write();
	
	EffAbsOrganAll->Draw("A pmc");
	EffAbsOrganAll->Write();
	
}
