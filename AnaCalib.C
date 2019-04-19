{
	gStyle->SetPalette(kRainBow);
double THR=0;

	TFile* outFile=new TFile(Form("build/CalibOut%d.root",int(THR)),"RECREATE");
TString fileNameTot;
	
	int NumPrim=50000;
	
	int OrganZs[10]={
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
	
	int HoleZs[11]={
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
	
	TCanvas* canvHole[11];
	THStack * stackHole[11];
	
	// #####################################################################
	// ################ FIXING A HOLE DEPTH, WE VARY THE ORGAN THICKNESS
	cout<<" ################ FIXING A HOLE DEPTH, WE VARY THE ORGAN THICKNESS "<<endl;
	for (int iHole=0; iHole<11; iHole++) {
		stackHole[iHole]=new THStack(Form("stackHole%d",iHole),Form("stackHole%d",iHole));
		cout<<"ANALIZZO HOLE NUM :"<< iHole<<" cioè profondo [cm] "<<HoleZs[iHole]<< ", THR= "<<THR<<endl;
		canvHole[iHole]=new TCanvas();
		canvHole[iHole]->SetTitle(Form("Hole%d",iHole));
		canvHole[iHole]->SetName(Form("Hole%d",iHole));
		for (int iOrgan=0; iOrgan<10; iOrgan++) {
			fileNameTot=Form("build/CALIBmc_HoleZ%d_OrganZ%d_N%d.root",HoleZs[iHole], OrganZs[iOrgan], (iOrgan+1)*NumPrim);
			//		cout<<fileNameTot<<endl;
			TFile * file=TFile::Open(fileNameTot);
			TString taglio=Form("Eabs>%lf",THR);
			if (iOrgan==0) {
				cout<<B1->Draw("Eabs>>histo",taglio, "")<<endl;
			}
			else {
				cout<<B1->Draw("Eabs>>histo",taglio, "samesPLC")<<endl;
			}
			stackHole[iHole]->Add(histo);
		}
		outFile->cd();
//		canvHole[iHole]->Write();
		stackHole[iHole]->Write();
	}
	
	
	TCanvas* canvOrgan[10];
	THStack * stackOrgan[10];

	// #####################################################################
	// ################ FIXING A ORGAN THICKNESS, WE VARY THE HOLE DEPTH
	cout<<" ################ FIXING A ORGAN THICKNESS, WE VARY THE HOLE DEPTH "<<endl;
		for (int iOrgan=0; iOrgan<10; iOrgan++) {
			
			stackOrgan[iOrgan]=new THStack(Form("stackOrgan%d",iOrgan),Form("stackOrgan%d",iOrgan));
		cout<<"ANALIZZO ORGAN DEPTH NUM :"<< iOrgan<<" cioè profondo [cm] "<<OrganZs[iOrgan]<< ", THR= "<<THR<<endl;
		int whichOrgan=OrganZs[iOrgan];
		canvOrgan[iOrgan]=new TCanvas();
			canvOrgan[iOrgan]->SetTitle(Form("Organ%d",iOrgan));
			canvOrgan[iOrgan]->SetName(Form("Organ%d",iOrgan));
			for (int iHole=0; iHole<11; iHole++) {
				fileNameTot=Form("build/CALIBmc_HoleZ%d_OrganZ%d_N%d.root",HoleZs[iHole], OrganZs[iOrgan], (iOrgan+1)*NumPrim);
				//				cout<<fileNameTot<<endl;
				TFile * file=TFile::Open(fileNameTot);
				TString taglio=Form("Eabs>%lf",THR);
				if (iHole==0)
					cout<<B1->Draw("Eabs>>histo",taglio, "")<<endl;
				else cout<<B1->Draw("Eabs>>histo",taglio, "samesPLC")<<endl;
				
				stackOrgan[iOrgan]->Add(histo);
			}
			outFile->cd();
//			canvOrgan[iOrgan]->Write();
			stackOrgan[iOrgan]->Write();
	}
	
	
	
	
	
}
