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
// $Id: B1SteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1RunAction.hh"
#include "B1DetectorConstruction.hh"


#include "G4Step.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

#include "B1Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction, B1RunAction* RunningAction,G4int SourceChoice)
: G4UserSteppingAction(),
fEventAction(eventAction),
fScoringVolume(0),
fRunningAction(RunningAction),
fSourceChoice(SourceChoice)
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
	if (fGaSet) {
		//to mute the "unused" warning, could be useful in the future!
	}
	G4VPhysicalVolume* ThisVol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
	G4VPhysicalVolume* NextVol = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
	
	G4int debug=0;
	


	
#pragma mark Entering Pter
	// ################################################################################
	// ###################### ENTERING Pter (from wherever)
	if((NextVol && ThisVol->GetName()!="Pter" && NextVol->GetName()=="Pter")) { //what enters Pter (form every different volume)
		
		if (debug) G4cout<<"\nCIAODEBUG\n Particella entrata in PTER - fEventAction->GetEnteringParticle() ERA = "<<fEventAction->GetEnteringParticle();
		fEventAction->SetEnteringParticle(step->GetTrack()->GetDynamicParticle()->GetPDGcode()); //TODO: sistemare per evitare errori dovuti al fatto che è l'ultima traccia creata la prima ad essere tracciata..
		if (debug) G4cout<<" SETTO fEventAction->GetEnteringParticle()= "<<fEventAction->GetEnteringParticle()<<G4endl<<G4endl;
		
		// Check if the current track had already enetered somehow the PTER (to avoid double counting), otherwise increase the counter
		if (fEventAction->GetPterStoreTrackID()==step->GetTrack()->GetTrackID()) { //if I already saw this track exiting the source...
			fEventAction->AddPterPassCounter(1);  //increase the counter
			//			G4cout<<"PterDEBUG CONTROLLA "<<fEventAction->GetPterStoreTrackID()<<", PassCounter= "<<fEventAction->GetPterPassCounter()<<G4endl;
		}else {
			fEventAction->SetPterStoreTrackID(step->GetTrack()->GetTrackID());
			//			G4cout<<"PterDEBUG PRIMO PASSAGGIO!! "<<fEventAction->GetPterStoreTrackID()<<", PassCounter= "<<fEventAction->GetPterPassCounter()<<G4endl;
			//            if (fEventAction->GetPassCounter()!=0) G4cout<<"MERDAAAAA Primo passaggio di"<<fEventAction->GetStoreTrackID()<<" ma con PassCounter= "<<fEventAction->GetPassCounter()<<G4endl;
		}
		
		// Salvo le info solo della prima volta che una particella entra in pter
		if (fEventAction->GetPterPassCounter()==0) {
			(fRunningAction->GetPrePterEn()).push_back(step->GetPostStepPoint()->GetKineticEnergy()/keV);
			fEventAction->AddPrePterNo(1); //update the counter of particles entering Pter in the event
			(fRunningAction->GetPrePterPart()).push_back(step->GetTrack()->GetDynamicParticle()->GetPDGcode()); //add PID of particle enetering Pter
		}
	}
	// ###################### END ENTERING Pter
	// ################################################################################

#pragma mark Inside Pter
	//Retrieve scoring volume
	if (!fScoringVolume) {
		const B1DetectorConstruction* detectorConstruction
		= static_cast<const B1DetectorConstruction*>
		(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		fScoringVolume = detectorConstruction->GetScoringVolume();
	}
	
	// ########################################
	// ###################### INSIDE Pter - Per each hit into sensitive detector
	// check if we are in scoring volume
	if (ThisVol->GetLogicalVolume() == fScoringVolume && step->GetTrack()->GetDynamicParticle() ->GetPDGcode()!=0 ) {
		fEventAction->SetEnterPterFlag(); //Something entered pter in this event
		fEventAction->AddNumHitsDet(1); //Update the counter of number of interactions in detector
		
		// collect energy deposited in this step
		G4double edepStep = step->GetTotalEnergyDeposit();
		
		//Fill vector
		(fRunningAction->GetPterEn()).push_back(step->GetTotalEnergyDeposit()/keV);
		(fRunningAction->GetPterEnPrim()).push_back(fRunningAction->GetMotherEnergy());
		(fRunningAction->GetPterPartPrim()).push_back(fRunningAction->GetMotherPID());
		//		(fRunningAction->GetPterTime()).push_back(step->GetTrack()->GetLocalTime()/ns);
		(fRunningAction->GetPterTime()).push_back(step->GetTrack()->GetGlobalTime()/ns-fRunningAction->GetMotherTime());
		//		G4cout<<"PterDEBUG  MotherTime= "<< fRunningAction->GetMotherTime()<<" PostDiff= "<<  step->GetTrack()->GetGlobalTime()/ns-fRunningAction->GetMotherTime() <<G4endl;
		(fRunningAction->GetPterX()).push_back(step->GetPreStepPoint()->GetPosition().x()/mm);
		(fRunningAction->GetPterY()).push_back(step->GetPreStepPoint()->GetPosition().y()/mm);
		(fRunningAction->GetPterZ()).push_back(step->GetPreStepPoint()->GetPosition().z()/mm);
		(fRunningAction->GetPterPart()).push_back(step->GetTrack()->GetDynamicParticle() ->GetPDGcode());
		
		if (debug)  G4cout<<"CIAODEBUG Ho un rilascio di energia ("<< step->GetTotalEnergyDeposit()/keV<<" [KeV]) dovuto ad una particella entrata nel CMOS di tipo: "<<fEventAction->GetEnteringParticle()<<G4endl;
		
		if (fEventAction->GetEnteringParticle()==11) {  //if son of electron
			fEventAction->AddEdepEle(step->GetTotalEnergyDeposit());
		}
		else if (fEventAction->GetEnteringParticle()==-11) {  //if son of positron
			fEventAction->AddEdepPos(step->GetTotalEnergyDeposit());
		} else if (fEventAction->GetEnteringParticle()==22) {  //if son of photon
			fEventAction->AddEdepFot(step->GetTotalEnergyDeposit());
			if (debug&&step->GetTotalEnergyDeposit()>0) G4cout<<"CONTROLLA"<<G4endl;
		}
		
		fEventAction->AddEdep(edepStep);
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
