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
// $Id: B1RunAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1SteppingAction.hh"


#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "B1Analysis.hh"

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction(G4double HoleZ, G4double OrganZ, G4int SourceSelect, G4String OutFileName)
: G4UserRunAction(),
fEdep("Edep", 0.),
//fEdepSiPM("EdepSiPM", 0.),
fEdep2("Edep2", 0.),
fEdkin("Edkin", 0.)
, fHoleZ(HoleZ)
, fOrganZ(OrganZ)
, fSourceSelect(SourceSelect)
, fOutFileName(OutFileName)

{
	// Register accumulable to the accumulable manager
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->RegisterAccumulable(fEdep);
//	accumulableManager->RegisterAccumulable(fEdepSiPM);
	accumulableManager->RegisterAccumulable(fEdep2);
	accumulableManager->RegisterAccumulable(fEdkin);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{
	delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run* run)
{
	// inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	
	// reset accumulable to their initial values
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Reset();
	
	CreateHistogram();
	
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	
	nbEventInRun = run->GetNumberOfEventToBeProcessed();
	analysisManager->FillNtupleIColumn(0,36, nbEventInRun);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
	G4int nofEvents = run->GetNumberOfEvent();
	if (nofEvents == 0) return;
	
	// Merge accumulable
	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
	accumulableManager->Merge();
	
	// Run conditions
	//  note: There is no primary generator action object for "master"
	//        run manager for multi-threaded mode.
	const B1PrimaryGeneratorAction* generatorAction
	= static_cast<const B1PrimaryGeneratorAction*>
	(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
	G4String runCondition;
	if (generatorAction)
	{
		const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
		runCondition += particleGun->GetParticleDefinition()->GetParticleName();
		runCondition += " of ";
		G4double particleEnergy = particleGun->GetParticleEnergy();
		runCondition += G4BestUnit(particleEnergy,"Energy");
	}
	
	// Print
	//
	if (IsMaster()) {
		G4cout
		<< G4endl
		<< "--------------------End of Global Run-----------------------";
	}
	else {
		G4cout
		<< G4endl
		<< "--------------------End of Local Run------------------------";
	}
	
	G4cout
	<< G4endl
	<< " The run consists of " << nofEvents << " "<< runCondition
	<< G4endl
	<< G4endl
	// << " N. of hits in scoring volume :" << fNumHitsDetfHits
	// << G4endl
	<< G4endl
	<< G4endl;
	
	///////////////
	// Write Histo
	//
	WriteHistogram();
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::AddEdep(G4double edep)
{
	fEdep  += edep;
	fEdep2 += edep*edep;
}

//void B1RunAction::AddEdepSiPM(G4double EdepSiPM)
//{
//	fEdepSiPM  += EdepSiPM;
//}


void B1RunAction::AddEdkin(G4double edkin)
{
	fEdkin  += edkin;
}


void B1RunAction::CreateHistogram()
{
	// Book histograms, ntuple
	//	G4cout << "##### Create analysis manager " << "  " << this << G4endl;
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	
	G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;
	
	// Create directories
	analysisManager->SetVerboseLevel(1);
	fOutFileName.append(".root");
	G4cout<<" Output file name: "<<fOutFileName<<G4endl;
	analysisManager->OpenFile(fOutFileName);

	// Creating ntuple
	
	analysisManager->CreateNtuple("B1", "physics");
	analysisManager->CreateNtuple("Source", "SourceNtuple");
	
	// ################################################################################
	// ###################### PHYSICS NTUPLE
	analysisManager->CreateNtupleDColumn(0,"Eabs");                           //0
	analysisManager->CreateNtupleDColumn(0,"EabsComp", RunVEAbsComp); //1
	
	analysisManager->CreateNtupleDColumn(0,"InPterTrackN");                   //2
	analysisManager->CreateNtupleDColumn(0,"InPterPart", RunVPterPart); //3
	analysisManager->CreateNtupleDColumn(0,"InPterEn", RunVPterEn); //4
	analysisManager->CreateNtupleDColumn(0,"InPterPrimEn", RunVPterPrimEn); //5
	analysisManager->CreateNtupleDColumn(0,"InPterPrimPart", RunVPterPrimPart); //6
	analysisManager->CreateNtupleFColumn(0,"InPterTime", RunVPterTime); //7
	analysisManager->CreateNtupleDColumn(0,"InPterX", RunVPterX); //8
	analysisManager->CreateNtupleDColumn(0,"InPterY", RunVPterY); //9
	analysisManager->CreateNtupleDColumn(0,"InPterZ", RunVPterZ); //10
	
	analysisManager->CreateNtupleDColumn(0,"PrePterTrackN");                  //11
	analysisManager->CreateNtupleDColumn(0,"PrePterPart", RunVPrePterPart); //12
	analysisManager->CreateNtupleDColumn(0,"PrePterEn", RunVPrePterEn); //13
	
	analysisManager->CreateNtupleDColumn(0,"PreProbeTrackN");                  //14
	analysisManager->CreateNtupleDColumn(0,"PreProbePart", RunVPreProbePart); //15
	analysisManager->CreateNtupleDColumn(0,"PreProbeEn", RunVPreProbeEn); //16
	
	analysisManager->CreateNtupleDColumn(0,"PostAbsTrackN");                  //17
	analysisManager->CreateNtupleDColumn(0,"PostAbsPart", RunVPostAbsPart); //18
	analysisManager->CreateNtupleDColumn(0,"PostAbsEn", RunVPostAbsEn); //19
	
	analysisManager->CreateNtupleDColumn(0,"ExitEne", RunVExitEn); //20
	
	analysisManager->CreateNtupleDColumn(0,"SourceX");                           //21
	analysisManager->CreateNtupleDColumn(0,"SourceY");                           //22
	analysisManager->CreateNtupleDColumn(0,"SourceZ");                           //23
	analysisManager->CreateNtupleDColumn(0,"SourceCosX", RunVSourceCosX); //24
	analysisManager->CreateNtupleDColumn(0,"SourceCosY", RunVSourceCosY); //25
	analysisManager->CreateNtupleDColumn(0,"SourceCosZ", RunVSourceCosZ); //26
	analysisManager->CreateNtupleDColumn(0,"SourceEne", RunVSourceEn); //27
	analysisManager->CreateNtupleDColumn(0,"SourcePart", RunVSourcePart); //28
	analysisManager->CreateNtupleDColumn(0,"SourceIsotope", RunVSourceIsotope); //29
	analysisManager->CreateNtupleIColumn(0,"Npmt");							//30
	analysisManager->CreateNtupleIColumn(0,"EnterPterFlag"); //31


	analysisManager->CreateNtupleDColumn(0,"AnnihilationX", RunVAnnihX); //32
	analysisManager->CreateNtupleDColumn(0,"AnnihilationY", RunVAnnihY); //33
	analysisManager->CreateNtupleDColumn(0,"AnnihilationZ", RunVAnnihZ); //34
	
	//analysisManager->CreateNtupleDColumn(0,"EabsSiPM");       //34
	//analysisManager->CreateNtupleDColumn(0,"EabsSiPMComp", RunVEAbsSiPMComp);
	analysisManager->CreateNtupleDColumn(0,"AnnihilationTime", RunVAnnihT); //35
	analysisManager->CreateNtupleIColumn(0,"Nev");							//36
	
	// ######################
	// ################################################################################
	
	// ################################################################################
	// ###################### SOURCE NTUPLE
	analysisManager->CreateNtupleDColumn(1,"AllX");                           //0
	analysisManager->CreateNtupleDColumn(1,"AllY");                           //1
	analysisManager->CreateNtupleDColumn(1,"AllZ");                           //2
	analysisManager->CreateNtupleDColumn(1,"AllCosX", RunVSourceCosX);                           //3
	analysisManager->CreateNtupleDColumn(1,"AllCosY", RunVSourceCosY);                           //4
	analysisManager->CreateNtupleDColumn(1,"AllCosZ", RunVSourceCosZ);                           //5
	analysisManager->CreateNtupleDColumn(1,"AllEne", RunVSourceEn);                           //6
	analysisManager->CreateNtupleDColumn(1,"AllPart", RunVSourcePart);                           //7
	analysisManager->CreateNtupleDColumn(1,"AllIsotope", RunVSourceIsotope);                           //8
	
	analysisManager->CreateNtupleDColumn(1,"ExitX", RunVExitX);                           //9
	analysisManager->CreateNtupleDColumn(1,"ExitY", RunVExitY);                           //10
	analysisManager->CreateNtupleDColumn(1,"ExitZ", RunVExitZ);                           //11
	analysisManager->CreateNtupleDColumn(1,"ExitCosX", RunVExitCosX);                           //12
	analysisManager->CreateNtupleDColumn(1,"ExitCosY", RunVExitCosY);                           //13
	analysisManager->CreateNtupleDColumn(1,"ExitCosZ", RunVExitCosZ);                           //14
	analysisManager->CreateNtupleDColumn(1,"ExitEne", RunVExitEn);                           //15
	analysisManager->CreateNtupleDColumn(1,"ExitPart", RunVExitPart);                           //16
	analysisManager->CreateNtupleDColumn(1,"ExitParentID", RunVExitParentID);                           //17
	analysisManager->CreateNtupleIColumn(1,"ExitProcess", RunExitProcess); //18
	analysisManager->CreateNtupleDColumn(1,"ExitTrackN"); //19
	
	analysisManager->CreateNtupleDColumn(1,"AnnihilationX", RunVAnnihX); //20
	analysisManager->CreateNtupleDColumn(1,"AnnihilationY", RunVAnnihY); //21
	analysisManager->CreateNtupleDColumn(1,"AnnihilationZ", RunVAnnihZ); //22
	
	// ######################
	// ################################################################################
	
	analysisManager->FinishNtuple(0);
	analysisManager->FinishNtuple(1);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void B1RunAction::WriteHistogram()
{
	
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	
	// save histograms
	//
	analysisManager->Write();
	analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

