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
// $Id: B1RunAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1RunAction.hh
/// \brief Definition of the B1RunAction class

#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4AccumulableManager.hh"
#include "globals.hh"
#include <vector>

#include <iostream>

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class B1RunAction : public G4UserRunAction
{
public:
	B1RunAction( G4double, G4double, G4int,G4String);
	virtual ~B1RunAction();
	
	// virtual G4Run* GenerateRun();
	virtual void BeginOfRunAction(const G4Run*);
	virtual void   EndOfRunAction(const G4Run*);
	
	void AddEdep (G4double edep);
	void AddEdepSiPM (G4double EdepSiPM);
	void AddEdkin (G4double edkin);
	
	// void AddEdepPhot (G4double edepPhot);
	// void AddEdepEl (G4double edepEl);
	
	
	std::vector<G4double>& GetPterEn() {return RunVPterEn; }
	std::vector<G4double>& GetPterEnPrim() {return RunVPterPrimEn; }
	std::vector<G4double>& GetPterPartPrim() {return RunVPterPrimPart; }
	std::vector<G4float>& GetPterTime() {return RunVPterTime; }
	std::vector<G4double>& GetPterX() {return RunVPterX; }
	std::vector<G4double>& GetPterY() {return RunVPterY; }
	std::vector<G4double>& GetPterZ() {return RunVPterZ; }
	std::vector<G4double>& GetPterPart() {return RunVPterPart; }
	
	std::vector<G4double>& GetExitEn() {return RunVExitEn; }
	std::vector<G4double>& GetExitX() {return RunVExitX; }
	std::vector<G4double>& GetExitY() {return RunVExitY; }
	std::vector<G4double>& GetExitZ() {return RunVExitZ; }
	std::vector<G4double>& GetExitCosX() {return RunVExitCosX; }
	std::vector<G4double>& GetExitCosY() {return RunVExitCosY; }
	std::vector<G4double>& GetExitCosZ() {return RunVExitCosZ; }
	std::vector<G4double>& GetExitPart() {return RunVExitPart; }
	std::vector<G4double>& GetExitParentID() {return RunVExitParentID; }
	std::vector<G4int>& GetExitProcess() {return RunExitProcess; }
	
	std::vector<G4double>& GetSourceCosX() {return RunVSourceCosX; }
	std::vector<G4double>& GetSourceCosY() {return RunVSourceCosY; }
	std::vector<G4double>& GetSourceCosZ() {return RunVSourceCosZ; }
	std::vector<G4double>& GetSourceEn() {return RunVSourceEn; }
	std::vector<G4double>& GetSourcePart() {return RunVSourcePart; }
	std::vector<G4double>& GetSourceIsotope() {return RunVSourceIsotope; }
	
	std::vector<G4double>& GetEAbsComp() {return RunVEAbsComp; }
	std::vector<G4double>& GetEAbsSiPMComp() {return RunVEAbsSiPMComp; }

	std::vector<G4double>& GetPrePterEn() {return RunVPrePterEn; }
	std::vector<G4double>& GetPrePterPart() {return RunVPrePterPart; }
	
	std::vector<G4double>& GetPreProbeEn() {return RunVPreProbeEn; }
	std::vector<G4double>& GetPreProbePart() {return RunVPreProbePart; }
	
	std::vector<G4double>& GetAnnihX() {return RunVAnnihX; }
	std::vector<G4double>& GetAnnihY() {return RunVAnnihY; }
	std::vector<G4double>& GetAnnihZ() {return RunVAnnihZ; }
	
	std::vector<G4double>& GetPostAbsEn() {return RunVPostAbsEn; }
	std::vector<G4double>& GetPostAbsPart() {return RunVPostAbsPart; }

	std::vector<G4double>& GetAnnihT() {return RunVAnnihT; }

	
	G4int GetEventNumber() {return nbEventInRun;}
	
	void SetMotherIsotope(G4double miso) {fMotherIsotope=miso;}
	G4double GetMotherIsotope() {return fMotherIsotope;}
	
	void SetMotherEnergy(G4double mene) {fMotherEnergy=mene;}
	G4double GetMotherEnergy() {return fMotherEnergy;}

	void SetMotherPID(G4double mpid) {fMotherPID=mpid;}
	G4double GetMotherPID() {return fMotherPID;}
	
	void SetMotherTime(G4double mtime) {fMotherTime=mtime;}
	G4float GetMotherTime() {return fMotherTime;}
	
private:
	G4Accumulable<G4double> fEdep;
//	G4Accumulable<G4double> fEdepSiPM;
	G4Accumulable<G4double> fEdep2;
	G4Accumulable <G4double> fEdkin;
	
	G4double fHoleZ;
	G4double fOrganZ;
	G4double fAbsHoleDiam;
	G4int nbEventInRun;
	G4double fTBR;
	G4int fSourceSelect;
	
	G4int fMotherIsotope=-10;
	G4int fSensorChoice;

	G4double fMotherEnergy=-10;
	G4double fMotherPID=999;
	G4float fMotherTime=0;
	
	//G4Accumulable <G4double> fEdepPhot;
	//G4Accumulable<G4double> fEdepEl;
	
	/////////////////
	// Histogramming
	//
	void CreateHistogram();
	void WriteHistogram();
	
	std::vector<G4double> RunVPterEn;
	std::vector<G4double>	RunVPterPrimEn;
	std::vector<G4double> RunVPterPrimPart;
	std::vector<G4float>	RunVPterTime;
	std::vector<G4double> RunVPterX;
	std::vector<G4double> RunVPterY;
	std::vector<G4double> RunVPterZ;
	std::vector<G4double> RunVPterPart;
	
	std::vector<G4double> RunVPrePterEn;
	std::vector<G4double> RunVPrePterPart;
	
	std::vector<G4double> RunVPreProbeEn;
	std::vector<G4double> RunVPreProbePart;
	
	std::vector<G4double> RunVAnnihX;
	std::vector<G4double> RunVAnnihY;
	std::vector<G4double> RunVAnnihZ;
	std::vector<G4double> RunVAnnihT;

	std::vector<G4double> RunVPostAbsEn;
	std::vector<G4double> RunVPostAbsPart;
	
	std::vector<G4double> RunVSourceCosX;
	std::vector<G4double> RunVSourceCosY;
	std::vector<G4double> RunVSourceCosZ;
	
	std::vector<G4double> RunVSourceEn;
	std::vector<G4double> RunVSourcePart;
	std::vector<G4double> RunVSourceIsotope;
	
	
	std::vector<G4double> RunVExitEn;
	std::vector<G4double> RunVExitX;
	std::vector<G4double> RunVExitY;
	std::vector<G4double> RunVExitZ;
	std::vector<G4double> RunVExitCosX;
	std::vector<G4double> RunVExitCosY;
	std::vector<G4double> RunVExitCosZ;
	
	std::vector<G4double> RunVExitPart;
	std::vector<G4double> RunVExitParentID;
	
	std::vector<G4int> RunExitProcess;
	
	std::vector<G4double> RunVEAbsComp;
	std::vector<G4double> RunVEAbsSiPMComp;


	G4String fOutFileName;
	
};

#endif

