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
// $Id: B1EventAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1EventAction.hh
/// \brief Definition of the B1EventAction class

#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class B1RunAction;

/// Event action class
///

class B1EventAction : public G4UserEventAction
{
public:
	B1EventAction(B1RunAction* runAction);
	virtual ~B1EventAction();
	
	virtual void BeginOfEventAction(const G4Event* event);
	virtual void EndOfEventAction(const G4Event* event);
	
	void AddEdep(G4double edep) { fEdep += edep; }
	void AddEdepSiPM(G4double EdepSiPM) { fEdepSiPM += EdepSiPM; }
	void AddEdkin(G4double edkin) { fEdkin += edkin; }
	void AddNumHitsDet(G4int no) { fNumHitsDet += no; }
	
	void AddPreScintNo(G4int No) { fPreScintNo += No; }
	void AddPreProbeNo(G4int No) { fPreProbeNo += No; }
	void AddPostAbsNo(G4int No) { fPostAbsNo += No; }

	void AddEdepEle(G4double edepEle) { fEdepEle += edepEle; }
	void AddEdepPos(G4double edepPos) { fEdepPos += edepPos; }
	void AddEdepFot(G4double edepFot) { fEdepFot += edepFot; }
	void AddEdepSiPMPos(G4double EdepSiPMpos) { fEdepSiPMpos += EdepSiPMpos; }
	void AddEdepSiPMFot(G4double EdepSiPMfot) { fEdepSiPMfot += EdepSiPMfot; }
	
	void SetSourceX(G4double sx) {fSourceX=sx;}
	void SetSourceY(G4double sy) {fSourceY=sy;}
	void SetSourceZ(G4double sz) {fSourceZ=sz;}
	
	void SetSourceCosX(G4double scx) {fSourceCosX=scx;}
	void SetSourceCosY(G4double scy) {fSourceCosY=scy;}
	void SetSourceCosZ(G4double scz) {fSourceCosZ=scz;}
	void SetSourceEne(G4double sene) {fSourceEne=sene;}
	void SetSourceIsotope(G4double siso) {fSourceIsotope=siso;}
	
	void AddNSourceExit(G4double snsexit) {fNSourceExit+=snsexit;}
	G4double GetNSourceExit(void) {return fNSourceExit;}
	
	void SetSourceExitStoreTrackID(G4int trid) {fSourceExitStoreTrackID=trid;}
	G4int GetSourceExitStoreTrackID(void) {return fSourceExitStoreTrackID;}
	void AddSourceExitPassCounter(G4int num) {fSourceExitPassCounter+=num;}
	void ResetSourceExitPassCounter() {fSourceExitPassCounter=0;}
	G4int GetSourceExitPassCounter(void) {return fSourceExitPassCounter;}
	
	void SetScintStoreTrackID(G4int trid) {fScintStoreTrackID=trid;}
	G4int GetScintStoreTrackID(void) {return fScintStoreTrackID;}
	void AddScintPassCounter(G4int num) {fScintPassCounter+=num;}
	void ResetScintPassCounter() {fScintPassCounter=0;}
	G4int GetScintPassCounter(void) {return fScintPassCounter;}
	
	void SetPostAbsStoreTrackID(G4int trid) {fPostAbsStoreTrackID=trid;}
	G4int GetPostAbsStoreTrackID(void) {return fPostAbsStoreTrackID;}
	void AddPostAbsPassCounter(G4int num) {fPostAbsPassCounter+=num;}
	void ResetPostAbsPassCounter() {fPostAbsPassCounter=0;}
	G4int GetPostAbsPassCounter(void) {return fPostAbsPassCounter;}
	
	void SetPreProbeStoreTrackID(G4int trid) {fPreProbeStoreTrackID=trid;}
	G4int GetPreProbeStoreTrackID(void) {return fPreProbeStoreTrackID;}
	void AddPreProbePassCounter(G4int num) {fPreProbePassCounter+=num;}
	void ResetPreProbePassCounter() {fPreProbePassCounter=0;}
	G4int GetPreProbePassCounter(void) {return fPreProbePassCounter;}
	
	void SetEnteringParticle(G4int part) {fEnteringParticle=part;}
	G4int GetEnteringParticle(void) {return fEnteringParticle;}

	void SetEnterScintFlag(void) {fEnterScintFlag=1;}
	G4int GetEnterScintFlag(void) {return fEnterScintFlag;}

	void AddNPMT(G4int n) {fNPMT+=n;}

	
private:
	B1RunAction* fRunAction;
	G4double fEdep;
	G4double fEdepSiPM;
	G4double fEdkin;
	G4int fNumHitsDet;
	G4int fPreScintNo;
	G4int fPreProbeNo;
	G4int fPostAbsNo;

	G4double	fEdepEle;
	G4double	fEdepPos;
	G4double	fEdepFot;
	G4double  fEdepSiPMpos;
	G4double  fEdepSiPMfot;
	G4int fEnteringParticle;
		
	G4double fSourceX;
	G4double fSourceY;
	G4double fSourceZ;
	G4double fSourceEne;
	G4int fSourceIsotope;
	G4double fSourceCosX;
	G4double fSourceCosY;
	G4double fSourceCosZ;
	
	G4int fSourceExitPassCounter;
	G4int fScintPassCounter;
	G4int fPostAbsPassCounter;
	G4int fPreProbePassCounter;

	G4double fNSourceExit;
	G4int fNPMT;
	
	G4String fExitProcess;
	G4int fSourceExitStoreTrackID;
	G4int fScintStoreTrackID;
	G4int fPostAbsStoreTrackID;
	G4int fPreProbeStoreTrackID;
	G4int fEnterScintFlag;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


