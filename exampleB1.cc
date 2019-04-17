//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: exampleB1.cc 86065 2014-11-07 08:51:15Z gcosmo $
//
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"
#include "MyExceptionHandler.hh"


#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "FTFP_BERT_HP.hh"
#include "B1PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "G4StepLimiter.hh"
#include "G4UserLimits.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4OpticalPhysics.hh"

#include "G4ScoringManager.hh"

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
	G4bool VisFlag=false;
	
	// Detect interactive mode (if no arguments) and define UI session
	G4UIExecutive* ui = 0;
	G4String MacroName ="";

	G4double HoleZ=3., OrganZ=5., OrganR=0.;
	//AbsorberHoleDiam=-1., TBRvalue=1.,ScintDiameter=6.,ScintThickness=5.,SourceDiameter=10.,SourceThickness=7., AbsorberThickness=1.,ProbeCaseDepth=-155., ProbeCaseLateralThickness=1.25, ProbeCaseBackThickness=20. , HSLateralThickness=1., HSBackThickness=2., AbsCenter=2.75;
	
	G4int SourceSelect=0, NoOfPrimToGen=99, DefNoOfPrimToGen=100000, Verbose=0; //, AbsorberMaterial=1, HousingCase=3, GaSetting=1,ApparatusMat=1,PosAbsorber=1;
	//G4bool ScintFlag=0;
	
	G4String fileName ="";
	G4String FileNameLabel="";
	
	for(int i=1;i<argc;i++)
		if(argv[i][0] =='-')
		{
			G4String option(argv[i]);
			G4cout<<"option: "<<i<<" "<<option<<G4endl;
			if(option.compare("-OrganZ")==0)
			{
				OrganZ=strtod (argv[++i], NULL);;
			}
			else if(option.compare("-OrganR")==0)
			{
				OrganR=strtod (argv[++i], NULL);;
			}
			else if(option.compare("-Source")==0)
			{
				SourceSelect=strtod (argv[++i], NULL);;
			}
			else if(option.compare("-HoleZ")==0)
			{
				HoleZ=strtod (argv[++i], NULL);;
			}
			else if(option.compare("-Label")==0)
			{
				FileNameLabel= argv[++i];;
			}
			else if(option.compare("-NPrim")==0)
			{
				NoOfPrimToGen=strtod (argv[++i], NULL);;
			}
			else if(option.compare("-Verbose")==0)
			{
				Verbose=strtod (argv[++i], NULL);;
			}
			else if(option.compare("-Vis")==0)
			{
				VisFlag=strtod (argv[++i], NULL);;
			}
		}
		else
		{
			MacroName = argv[i]; //se ho trovato una macro (senza il "-" davanti) significa che NON voglio l'interattivo
			fileName = argv[i]; //se ho trovato una macro (senza il "-" davanti) significa che NON voglio l'interattivo
			VisFlag=false;
		}
	
	if ( VisFlag ) { //Prepare for vis
		ui = new G4UIExecutive(argc, argv);
	}
	
	
	G4String OutFileName="CALIBmc";
	G4String FileNameCommonPart;
	
	// ###### DETECTOR:
	FileNameCommonPart.append("_HoleZ" + std::to_string((G4int)(10*HoleZ)));
	
	// ###### ORGAN
	if (SourceSelect==0) {
		if (NoOfPrimToGen==99) NoOfPrimToGen=DefNoOfPrimToGen*(OrganZ/2); //if untouched means it is ok to use default number of primaries scaling for different volume
		FileNameCommonPart.append("_OrganZ"+ std::to_string((G4int)(10*OrganZ)));
		if (OrganR!=0) FileNameCommonPart.append("_OrganR"+ std::to_string((G4int)(10*OrganR)));
	} else if (SourceSelect==1) {
		FileNameCommonPart.append("_ExtSr");
	}
	
	// ####### MISCELLANEUS
	if (FileNameLabel!="") FileNameCommonPart.append("_" + FileNameLabel);
	if (VisFlag) FileNameCommonPart.append("TEST"); //if it was a TEST run under vis
	
	
	FileNameCommonPart.append("_N"+std::to_string((G4int)NoOfPrimToGen));
	
	OutFileName.append(FileNameCommonPart);
	
	// Choose the Random engine
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	G4long seed = time(NULL);
	if (VisFlag) seed=12345; //If vis was requested same always the same seed to have reproducibility
	G4Random::setTheSeed(seed);
	// Construct the default run manager
	//
	//#ifdef G4MULTITHREAD
	//  G4MTRunManager* runManager = new G4MTRunManager;
	//#else
	
#if 0
	G4MTRunManager* runManager = new G4MTRunManager;
	//	runManager->SetNumberOfThreads( G4Threading::G4GetNumberOfCores() );
	runManager->SetNumberOfThreads( 6 );
#endif
	
	G4RunManager* runManager = new G4RunManager;
	
	// Set mandatory initialization classes
	// Detector construction
	runManager->SetUserInitialization(new B1DetectorConstruction(HoleZ, OrganZ, OrganR, SourceSelect));
	
	// Physics list
	//G4VModularPhysicsList* physicsList = new QBBC;
	//physicsList->SetVerboseLevel(1);
	
	//  runManager->SetUserInitialization(new B1PhysicsList);
	
	//	G4VModularPhysicsList* physicsList = new QBBC;
	//	physicsList->RegisterPhysics(new G4OpticalPhysics());
	
	B1PhysicsList* physicsList=new B1PhysicsList;
	physicsList->RegisterPhysics(new G4StepLimiterPhysics());
	
	runManager->SetUserInitialization(physicsList);
	
	// User action initialization
	//	runManager->SetUserInitialization(new B1ActionInitialization(x0Scan, OrganZ, AbsHoleDiam, FilterFlag, primFile, TBRvalue,SourceSelect, SourceSelect));
	runManager->SetUserInitialization(new B1ActionInitialization(HoleZ, OrganZ, SourceSelect, OutFileName));
	
	// Initialize visualization
	//
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();
	
	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	
	// Process macro or start UI session
	//
	runManager->Initialize();

	if ( ! ui ) {
		// batch mode
		if (MacroName!="") {
			G4String command = "/control/execute ";
			UImanager->ApplyCommand(command+MacroName);
		} else {
			UImanager->ApplyCommand("/tracking/verbose " + std::to_string(Verbose));
			UImanager->ApplyCommand("/run/beamOn " + std::to_string(NoOfPrimToGen));
			//			UImanager->ApplyCommand("/run/beamOn 100");
		}
	}
	else {
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
		delete ui;
	}
	
	delete visManager;
	delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
