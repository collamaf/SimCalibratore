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
// $Id: B1EventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "B1Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
fRunAction(runAction),
fEdep(0.),
fEdepSiPM(0.),
fEdkin(0.),
fNumHitsDet(0),
fPreScintNo(0),
fPreProbeNo(0),
fPostAbsNo(0),
fEdepEle(0.),
fEdepPos(0.),
fEdepFot(0.),
fEdepSiPMpos(0.),
fEdepSiPMfot(0.),
fEnteringParticle(0),
fSourceExitPassCounter(0.),
fScintPassCounter(0.),
fPostAbsPassCounter(0.),
fPreProbePassCounter(0.),
fNSourceExit(0.),
fSourceExitStoreTrackID(0),
fScintStoreTrackID(0),
fPostAbsStoreTrackID(0),
fPreProbeStoreTrackID(0),
fEnterScintFlag(0)
{}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event* )
{
	fEdep = 0.;
	fEdepSiPM=0.;
	fEdkin = 0.;
	(fRunAction->GetPreScintEn()).clear();
	(fRunAction->GetPreScintPart()).clear();
	(fRunAction->GetPreProbeEn()).clear();
	(fRunAction->GetPreProbePart()).clear();
	(fRunAction->GetPostAbsEn()).clear();
	(fRunAction->GetPostAbsPart()).clear();
	(fRunAction->GetScintEn()).clear();
	(fRunAction->GetScintEnPrim()).clear();
	(fRunAction->GetScintPartPrim()).clear();
	(fRunAction->GetScintTime()).clear();
	(fRunAction->GetScintX()).clear();
	(fRunAction->GetScintY()).clear();
	(fRunAction->GetScintZ()).clear();
	(fRunAction->GetScintPart()).clear();
	
	
	(fRunAction->GetAnnihX()).clear();
	(fRunAction->GetAnnihY()).clear();
	(fRunAction->GetAnnihZ()).clear();
	
	(fRunAction->GetAnnihT()).clear();
	
	(fRunAction->GetPostAbsPart()).clear();

	(fRunAction->GetSourceCosX()).clear();
	(fRunAction->GetSourceCosY()).clear();
	(fRunAction->GetSourceCosZ()).clear();

	(fRunAction->GetSourceEn()).clear();
	(fRunAction->GetSourcePart()).clear();
	(fRunAction->GetSourceIsotope()).clear();

	(fRunAction->SetMotherIsotope(-10));
	(fRunAction->SetMotherEnergy(-10));
	(fRunAction->SetMotherPID(999));
	(fRunAction->SetMotherTime(0));

	(fRunAction->GetExitEn()).clear();
	(fRunAction->GetExitX()).clear();
	(fRunAction->GetExitY()).clear();
	(fRunAction->GetExitZ()).clear();
	(fRunAction->GetExitCosX()).clear();
	(fRunAction->GetExitCosY()).clear();
	(fRunAction->GetExitCosZ()).clear();
	(fRunAction->GetExitPart()).clear();
	(fRunAction->GetExitParentID()).clear();
	
	(fRunAction->GetExitProcess()).clear();
	
	(fRunAction->GetEAbsComp()).clear();
	(fRunAction->GetEAbsSiPMComp()).clear();

	fNumHitsDet=0;
	fPreScintNo=0;
	fPreProbeNo=0;
	fPostAbsNo=0;

	fEdepEle=0.;
	fEdepPos=0;
	fEdepFot=0.;
	fEdepSiPMpos=0.,
	fEdepSiPMfot=0.,
	fEnteringParticle=0;
	fNSourceExit=0;
	fSourceExitPassCounter=0;
	fScintPassCounter=0;
	fPostAbsPassCounter=0;
	fPreProbePassCounter=0;
	fSourceExitStoreTrackID=0;
	fScintStoreTrackID=0;
	fPostAbsStoreTrackID=0;
	fPreProbeStoreTrackID=0;
	fNPMT=0;
	fEnterScintFlag=0;

//	fSourceX=0;
//	fSourceY=0;
//	fSourceZ=0;
	/*
		 fSourceEne=0;
	 */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void B1EventAction::EndOfEventAction(const G4Event* evento)
{

	fRunAction->AddEdep(fEdep);
//	fRunAction->AddEdepSiPM(fEdepSiPM);
	fRunAction->AddEdkin(fEdkin);

	(fRunAction->GetEAbsComp()).push_back(fEdepEle/keV);
	(fRunAction->GetEAbsComp()).push_back(fEdepPos/keV);
	(fRunAction->GetEAbsComp()).push_back(fEdepFot/keV);
//	(fRunAction->GetEAbsSiPMComp()).push_back(fEdepSiPMpos/keV);
//	(fRunAction->GetEAbsSiPMComp()).push_back(fEdepSiPMfot/keV);

	G4int NevTot=fRunAction->GetEventNumber();
	
	if ((10*evento->GetEventID())%NevTot==0) {
		G4cout<<"Progress status: "<<(evento->GetEventID()/(G4double)NevTot)*100<<" %, Nev= "<<evento->GetEventID()<<", NTotEv= "<<NevTot<<G4endl;
	}
	// get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	
	// fill ntuple
	analysisManager->FillNtupleDColumn(0, 0, fEdep/keV);
	//analysisManager->FillNtupleDColumn(0, 34, fEdepSiPM/keV);
	analysisManager->FillNtupleDColumn(0, 2, fNumHitsDet); //number of hits into the detector
	analysisManager->FillNtupleDColumn(0, 11, fPreScintNo);
	analysisManager->FillNtupleDColumn(0, 14, fPreProbeNo);
	analysisManager->FillNtupleDColumn(0, 17, fPostAbsNo);
	
	analysisManager->FillNtupleDColumn(0,21, fSourceX/mm);
	analysisManager->FillNtupleDColumn(0,22, fSourceY/mm);
	analysisManager->FillNtupleDColumn(0,23, fSourceZ/mm);

	analysisManager->FillNtupleIColumn(0,30, fNPMT);
	analysisManager->FillNtupleIColumn(0,31, fEnterScintFlag);
	
	if(1||fEdep>0) analysisManager->AddNtupleRow(0);    //1|| toglie l'if
	
	if(evento->GetEventID()<=1e5){ //to write to proper ntuple all the source particles info
		analysisManager->FillNtupleDColumn(1,0, fSourceX/mm);
		analysisManager->FillNtupleDColumn(1,1, fSourceY/mm);
		analysisManager->FillNtupleDColumn(1,2, fSourceZ/mm);
		analysisManager->FillNtupleDColumn(1,19, fNSourceExit);
		
		analysisManager->AddNtupleRow(1);
	} 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
