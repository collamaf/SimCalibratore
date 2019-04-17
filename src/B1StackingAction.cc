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
// $Id: B3StackingAction.cc 66536 2012-12-19 14:32:36Z ihrivnac $
// 
/// \file B3StackingAction.cc
/// \brief Implementation of the B3StackingAction class

#include "B1StackingAction.hh"
#include "B1RunAction.hh"
#include "B1EventAction.hh"

#include "G4Track.hh"
#include "G4NeutrinoE.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1StackingAction::B1StackingAction(B1RunAction* runAction, B1EventAction* EventAction)
:fRunningAction(runAction), fEventAction(EventAction)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1StackingAction::~B1StackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
B1StackingAction::ClassifyNewTrack(const G4Track* track)
{
	G4int debug=0;
	if (fabs(track->GetDynamicParticle() ->GetPDGcode())==12) return fKill; //kill neutrinos
	
	if (debug) G4cout<<"ScintDEBUG PROVA STACKING creata nuova traccia tipo= "<< track->GetDynamicParticle() ->GetPDGcode()<<", MotherIsotope Val= "<< fRunningAction->GetMotherIsotope()<<G4endl;
	
	const G4VProcess* creator=track->GetCreatorProcess();
	std::string CreatorProcname="undefined";
	if(creator) CreatorProcname=creator->GetProcessName();
	
	fEventAction->ResetSourceExitPassCounter(); //collamaf: at each new track we reset the pass counter
	fEventAction->ResetScintPassCounter(); //collamaf: at each new track we reset the pass counter
	fEventAction->ResetPostAbsPassCounter();
	fEventAction->ResetPreProbePassCounter();
	
	// ################################################################################
	// ###################### Interception of decay products
	if (CreatorProcname=="RadioactiveDecay" && track->GetDynamicParticle()->GetPDGcode()<9e8 && track->GetDynamicParticle()->GetPDGcode()!=0 && track->GetCurrentStepNumber()==0) { //to exclude optical photons and to avoid counting several times particles that undergo optical interactions (eg scintillation) - added on 2018.06.21
		//		G4cout<<"Aggiungo al calderone sorgente! ParentID= "<<track->GetParentID()<<" PID= "<<track->GetDynamicParticle() ->GetPDGcode()<<" EkeV= "<<track->GetKineticEnergy()/CLHEP::keV<<G4endl;
		
		fRunningAction->SetMotherIsotope(track->GetParentID()-1);
		(fRunningAction->SetMotherEnergy(track->GetKineticEnergy()/CLHEP::keV));
		(fRunningAction->SetMotherPID(track->GetDynamicParticle()->GetPDGcode()));
		 (fRunningAction->SetMotherTime(track->GetGlobalTime()/CLHEP::ns));
		 (fRunningAction->GetSourceEn()).push_back(track->GetKineticEnergy()/CLHEP::keV);
		 (fRunningAction->GetSourcePart()).push_back(track->GetDynamicParticle()->GetPDGcode());
		 (fRunningAction->GetSourceIsotope()).push_back(track->GetParentID()-1);
		 (fRunningAction->GetSourceCosX()).push_back(track->GetMomentumDirection().x());
		 (fRunningAction->GetSourceCosY()).push_back(track->GetMomentumDirection().y());
		 (fRunningAction->GetSourceCosZ()).push_back(track->GetMomentumDirection().z());
		 
		 }
		 // ###################### End of Interception of decay products
		 // ################################################################################
		 
		 // ################################################################################
		 // ###################### Direct electron production
		 if (track->GetDynamicParticle() ->GetPDGcode()==11 && track->GetTrackID()==1) {
			 fRunningAction->SetMotherIsotope(0);
			 (fRunningAction->SetMotherEnergy(track->GetKineticEnergy()/CLHEP::keV));
			 (fRunningAction->SetMotherPID(track->GetDynamicParticle()->GetPDGcode()));
			 (fRunningAction->SetMotherTime(track->GetGlobalTime()/CLHEP::ns));
			 (fRunningAction->GetSourceEn()).push_back(track->GetKineticEnergy()/CLHEP::keV);
			 (fRunningAction->GetSourcePart()).push_back(track->GetDynamicParticle() ->GetPDGcode());
			 (fRunningAction->GetSourceIsotope()).push_back(0);
			 (fRunningAction->GetSourceCosX()).push_back(track->GetMomentumDirection().x());
			 (fRunningAction->GetSourceCosY()).push_back(track->GetMomentumDirection().y());
			 (fRunningAction->GetSourceCosZ()).push_back(track->GetMomentumDirection().z());
		 }
		 // ###################### End of Direct electron production
		 // ################################################################################
		 
		 // ################################################################################
		 // ###################### Direct gamma production
		 if (track->GetDynamicParticle() ->GetPDGcode()==22 && track->GetTrackID()==1) {
			 fRunningAction->SetMotherIsotope(0);
			 (fRunningAction->SetMotherEnergy(track->GetKineticEnergy()/CLHEP::keV));
			 (fRunningAction->SetMotherPID(track->GetDynamicParticle()->GetPDGcode()));
			 (fRunningAction->SetMotherTime(track->GetGlobalTime()/CLHEP::ns));
			 (fRunningAction->GetSourceEn()).push_back(track->GetKineticEnergy()/CLHEP::keV);
			 (fRunningAction->GetSourcePart()).push_back(track->GetDynamicParticle() ->GetPDGcode());
			 (fRunningAction->GetSourceIsotope()).push_back(0);
			 (fRunningAction->GetSourceCosX()).push_back(track->GetMomentumDirection().x());
			 (fRunningAction->GetSourceCosY()).push_back(track->GetMomentumDirection().y());
			 (fRunningAction->GetSourceCosZ()).push_back(track->GetMomentumDirection().z());
		 }
	// ###################### End of Direct gamma production
	// ################################################################################
	
	return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
