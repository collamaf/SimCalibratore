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
// $Id: B1PrimaryGeneratorAction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

#include "G4IonTable.hh"
#include "G4ChargedGeantino.hh"

#include "B1RunAction.hh"
#include "B1Analysis.hh"

#include "G4Event.hh"

#include <iostream>
#include <fstream>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using std::ofstream;
using std::ios;
using std::endl;


B1PrimaryGeneratorAction::B1PrimaryGeneratorAction(B1EventAction* eventAction, G4int SourceSelect)
: G4VUserPrimaryGeneratorAction(),
fParticleGun(0) ,
evtPrimAction(eventAction),fSourceSelect(SourceSelect)

{
	G4int n_particle = 1;
	fParticleGun  = new G4ParticleGun(n_particle);
	
	
	
	switch (fSourceSelect) {
		case 0: //Vial Y
			fRadiusInt=0*mm;
			fDZInt=0*mm;
			fRadiusExt=0*mm;
			fDZExt=0*mm;
			break;

		case 1: //ExtSr
			fRadiusInt=8*mm;  //8 for RM, 10.5mm PG source
			fDZInt=0*mm;
			fRadiusExt=8*mm;
			fDZExt=0*mm;
			break;
			
		default:
			break;
	}
	
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("geantino");
	
	fParticleGun->SetParticleDefinition(particle);
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
	delete fParticleGun;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries (G4Event* anEvent)
{
	G4int Z = 39, A = 90;
	if (fSourceSelect==1) Z=38;
	
	G4double ionCharge   = 0.*eplus;
	G4double excitEnergy = 0.*keV;
	
	
	G4ParticleDefinition* ion
	= G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
	fParticleGun->SetParticleDefinition(ion);
	fParticleGun->SetParticleCharge(ionCharge);
	

	G4double zSource=0;
	G4double zSourceOffset=2e-5*mm; //to avoid generating particles at the very boundary of source!
	
	//	if (fRadiusExt==fRadiusInt) { //se ho un solo raggio ignoro il TBR e faccio la pasticca di sorgente
	if (fSourceSelect==0) { //se ho una delle due pasticche di Sr ignoro il TBR e faccio la pasticca di sorgente
		G4Tubs* 	SorgVol=	(G4Tubs*) G4PhysicalVolumeStore::GetInstance()->GetVolume("Source")->GetLogicalVolume()->GetSolid();
		
		G4double OrganSourceOffsetZ=(G4PhysicalVolumeStore::GetInstance()->GetVolume("Source")->GetTranslation().z())*mm;
		
		fRadiusMax=SorgVol->GetOuterRadius();
		fRadiusMin=0*mm;

		fZ=SorgVol->GetZHalfLength()*2*mm;
//		zSource = -(G4UniformRand()*fZ+zSourceOffset)-OrganSourceOffsetZ;
		zSource=(G4UniformRand()*fZ-1)+OrganSourceOffsetZ;
//		G4cout<<"fZ= "<<fZ<<" zSourceOffset= "<<zSourceOffset << " OrganSourceOffsetZ= "<<OrganSourceOffsetZ<<" zSource= " <<zSource<<G4endl;
		
		
	} else if (fSourceSelect==1) {
		fRadiusMax=fRadiusInt;
		fRadiusMin=0*mm;
		zSource = -zSourceOffset;
	}
	
//	G4double Sphere_Theta=G4UniformRand()*CLHEP::pi*2.;
//	G4double Sphere_Phi=acos(2*G4UniformRand()-1);
//	G4double Sphere_U=cos(Sphere_Phi);
//	G4double Sphere_X=sqrt(1-Sphere_U*Sphere_U)*cos(Sphere_Theta);
//	G4double Sphere_Y=sqrt(1-Sphere_U*Sphere_U)*sin(Sphere_Theta);
//	G4double Sphere_Z=Sphere_U;
//	G4double Sphere_Radius=5*cm;
	
	fParticleGun->SetParticleEnergy(0*MeV); //SetParticleEnergy uses kinetic energy
	
	G4double rho = sqrt(fRadiusMin*fRadiusMin + G4UniformRand()*(fRadiusMax*fRadiusMax-fRadiusMin*fRadiusMin));   //fixed square problem by collamaf with internal radius!
	G4double alpha = G4UniformRand()*CLHEP::pi*2.;
	
	G4double Source_X=rho*cos(alpha);
	G4double Source_Y=rho*sin(alpha);
	G4double Source_Z=zSource;
	
	G4double Sphere_ZOffset=0;
	
	G4double xDirection=0;
	G4double yDirection=0;
	G4double zDirection=0;

	const	G4ThreeVector SourcePosition = G4ThreeVector(Source_X, Source_Y, Source_Z);
	

	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xDirection,yDirection,zDirection));
	fParticleGun->SetParticlePosition(SourcePosition);
	
	//	Save source info to root file
	evtPrimAction->SetSourceX((SourcePosition.x())/mm);
	evtPrimAction->SetSourceY((SourcePosition.y())/mm);
	evtPrimAction->SetSourceZ((SourcePosition.z())/mm);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
	
	if(anEvent->GetEventID()==1) {  //stampo informazioni sorgente
		G4cout<<"############# SORGENTE RICHIESTA: = "<<fSourceSelect<<"##############"<<G4endl;
		G4cout<<"Dimensioni sorgente: Raggio interno = "<<fRadiusMin<<", Raggio esterno = "<<fRadiusMax<<", H = "<<fZ<<G4endl;
	}
}



G4double  B1PrimaryGeneratorAction::BetaDecaySpectrum(G4double Ek, G4double EndPoint)
{
	G4double ElMassMev = 0.510998928*MeV;
	
	G4double res=0.;
	
	G4double omega= Ek /ElMassMev  +1.;
	G4double omegamax= EndPoint /ElMassMev +1.;
	if(omega>1 && omega<omegamax)
	{
		res=(omegamax-omega)*(omegamax-omega) *Ek*sqrt(omega*omega-1.);
	}
	return res;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

