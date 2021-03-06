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
// $Id: B1DetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class
///
///
///

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4StepLimiter.hh"
#include "G4UserLimits.hh"
#include "G4Region.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction(G4double HoleZ, G4double OrganZ,G4double OrganR, G4int SourceSelect)
: G4VUserDetectorConstruction(),
fScoringVolume(0), fHoleZ(HoleZ), fOrganZ(OrganZ),fOrganR(OrganR),fSourceSelect(SourceSelect)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();
	
	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;
	
	//
	// World
	//
	G4double world_sizeXY = 0.5*m;
	G4double world_sizeZ  = 0.5*m;
	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* acqua = nist->FindOrBuildMaterial("G4_WATER");
	//	G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
	
	G4Box* solidWorld =
	new G4Box("World",                       //its name
						0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
	
	G4LogicalVolume* logicWorld =
	new G4LogicalVolume(solidWorld,          //its solid
											world_mat,           //its material
											"World");            //its name
	
	G4VPhysicalVolume* physWorld =
	new G4PVPlacement(0,                     //no rotation
										G4ThreeVector(),       //at (0,0,0)
										logicWorld,            //its logical volume
										"World",               //its name
										0,                     //its mother  volume
										false,                 //no boolean operation
										0,                     //copy number
										checkOverlaps);        //overlaps checking
	
#pragma mark Definition of Materials
	//###################################################################
	//###################################################
	// Definitions of materials
	//##########################
	
	G4double z, a;
	G4String name, symbol;
	G4int ncomponents, natoms;
	
	a = 1.01*g/mole;
	G4Element* elH = new G4Element (name="Hydrogen", symbol="H", z=1.,a );
	a = 12.01*g/mole;
	G4Element* elC = new G4Element (name="Carbon", symbol="C", z=6.,a );
	a = 16.00*g/mole;
	G4Element* elO = new G4Element (name="Oxygen", symbol="O", z=8.,a );
	a = 14.00*g/mole;
	G4Element* elN = new G4Element (name="Nitrogen", symbol="N", z=7.,a );
	
	G4double densityAlu = 2.600*g/cm3;
	G4NistManager::Instance()->BuildMaterialWithNewDensity("MyAlu","G4_Al",densityAlu);
	
	G4Material* SourceSR_mat = nist->FindOrBuildMaterial("MyAlu");

	G4Material* PVC_mat= nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	// Plastic scintillator tiles (used both in CMS hadron calorimeter
	// and ATLAS hadron barrel calorimeter):
	//     X0 = 42.4 cm  and  lambda_I = 79.360 cm.
	G4double density = 1.032*g/cm3;
	G4Material* Polystyrene = new G4Material(name="Polystyrene", density, 2);
	Polystyrene->AddElement(elC, natoms=19);
	Polystyrene->AddElement(elH, natoms=21);
	
	// Polypropelene
	
	G4Material* Polyethilene = new G4Material ("Polypropelene" , 0.92*g/cm3, 2);
	Polyethilene->AddElement(elH,2);
	Polyethilene->AddElement(elC,1);
	
	
	G4Material* Scint_mat = Polystyrene;
	G4Material* Vial_mat = Polyethilene;

	//##########################
	//###################################################
	//###################################################################
	
#pragma mark Assignment of Materials
	//###################################################################
	//###################################################
	// Assignment of materials
	//##########################
	
	//##########################
	//###################################################
	//###################################################################
	
	
#pragma mark Definitions of Dimensions and Sizes
	
	//###################################################################
	//###################################################
	// Definitions of dimensions and sizes
	//##########################
	
	G4double Ang0 = 0.*deg;
	G4double Ang2Pi = 360.*deg;
	

	
	//### Sr Source
	G4double RminSourceSR = 0.*mm;
	G4double RmaxSourceSR = 12.5*mm;
	G4double DzSourceSR= 3*mm;
	//###
	

	
	//##########################
	//###################################################
	
#pragma mark Definitions of Volumes
	//###################################################################
	//###################################################
	// Definitions of volumes
	//##########################
	//###################################################################
	
	//###################################################
	// Sr90 lab Source
	//##########################
	
	
	G4Tubs* solidSourceSR =
	new G4Tubs("Source",                       //its name
						 RminSourceSR,
						 RmaxSourceSR,
						 0.5*DzSourceSR,
						 Ang0,
						 Ang2Pi);     //its size
	
	G4LogicalVolume* logicSourceSR =
	new G4LogicalVolume(solidSourceSR,          //its solid
											SourceSR_mat,           //its material
											"Source");            //its name
	
	//################################################### END SR SOURCE
	
	
	//###################################################
	// 	SCINTILLATOR
	//##########################
	
#pragma mark Logic SCINT Definition
	G4double ScintX=22*mm;
	G4double ScintY=22*mm;
	G4double ScintZ=20*mm;

	G4double ScintScassoR=5*mm;
	G4double ScintScassoZ=1.5*mm;

	G4double ScintBucoR=15*mm/2.;
	G4double ScintBucoZ=fHoleZ;
	
	G4double VialR=14*mm/2.;
	G4double VialDR=0.83*mm;
	G4double VialZ=46*mm;
	G4double VialCapZ=VialDR;

	G4double SourceOrganR=VialR-VialDR;
	if (fOrganR!=0) SourceOrganR=fOrganR;
	G4double SourceOrganZ=fOrganZ;
	
	G4VisAttributes* ScintVisAtt=new G4VisAttributes(G4Colour(0,0,1));
	G4VisAttributes* VialVisAtt=new G4VisAttributes(G4Colour(0,1,0));
	G4VisAttributes* SourceOrganVisAtt=new G4VisAttributes(G4Colour(1,0,0));


	G4Box* solidScintBulk =	new G4Box("solidScintBulk",ScintX*0.5,ScintY*0.5,ScintZ*0.5);
	G4Tubs* solidScintScasso =	new G4Tubs("ScintScasso",0, ScintScassoR,ScintScassoZ*0.5, Ang0, Ang2Pi);
	G4Tubs* solidScintBuco =	new G4Tubs("ScintBuco",0, ScintBucoR,ScintBucoZ*0.5, Ang0, Ang2Pi);

	
	G4VSolid* solidScintTemp= new G4UnionSolid("ScintTemp",solidScintBulk,solidScintScasso,0,G4ThreeVector(0.,0.,ScintZ/2.));
	G4VSolid* solidScintAll= new G4SubtractionSolid("ScintAll",solidScintTemp,solidScintBuco,0,G4ThreeVector(0.,0.,-ScintZ/2.+ScintBucoZ/2.));

	G4LogicalVolume* logicScint =	new G4LogicalVolume(solidScintAll,Scint_mat,"Scint");
	G4VPhysicalVolume* physScint=new G4PVPlacement(0,G4ThreeVector(0,0,0),logicScint,"Scint",logicWorld,false,0, checkOverlaps);
	logicScint->SetVisAttributes(ScintVisAtt);

//
//	logicScint->SetRegion(pterreg);
//	pterreg->AddRootLogicalVolume(logicScint);
//
//	Vial_mat=world_mat; //to temporarly remove Vial
	
	G4ThreeVector posVial= G4ThreeVector(0,0,-VialZ/2.-VialCapZ-ScintZ/2.+ScintBucoZ);
	G4ThreeVector posSourceOrgan= G4ThreeVector(0,0,-SourceOrganZ/2.-VialCapZ-ScintZ/2.+ScintBucoZ);

	G4Tubs* solidVialBody =	new G4Tubs("VialBody",VialR-VialDR, VialR,VialZ*0.5, Ang0, Ang2Pi);
	G4Tubs* solidVialCap =	new G4Tubs("VialCap",0, VialR,VialCapZ*0.5, Ang0, Ang2Pi);
	G4VSolid* solidVial= new G4UnionSolid("Vial",solidVialBody,solidVialCap,0,G4ThreeVector(0.,0.,VialZ/2.+VialCapZ/2.));
	
	G4LogicalVolume* logicVial =	new G4LogicalVolume(solidVial,Vial_mat,"Vial");

	
	
	G4Tubs* solidSourceOrgan =	new G4Tubs("SourceOrgan",0, SourceOrganR,SourceOrganZ*0.5, Ang0, Ang2Pi);
	G4LogicalVolume* logicSourceOrgan =	new G4LogicalVolume(solidSourceOrgan,acqua,"Vial");
	logicSourceOrgan->SetVisAttributes(SourceOrganVisAtt);

	G4ThreeVector posPistone= posVial;
G4Tubs* solidPistone =	new G4Tubs("Pistone",0, VialR-VialDR,VialZ*0.5-fOrganZ, Ang0, Ang2Pi);
	G4LogicalVolume* logicPistone =	new G4LogicalVolume(solidPistone,PVC_mat,"Vial");
	
	
	
	
	if (fSourceSelect==0) { //Vial Y source
	G4VPhysicalVolume* physVial=new G4PVPlacement(0,posVial,logicVial,"Vial",logicWorld,false,0, checkOverlaps);
		new G4PVPlacement(0,posSourceOrgan,logicSourceOrgan,"Source",logicWorld,false,0,checkOverlaps);
		new G4PVPlacement(0,posPistone,logicPistone,"Pistone",logicWorld,false,0,checkOverlaps);

	logicVial->SetVisAttributes(VialVisAtt);
	} else if (fSourceSelect==1) { //Sr Lab Source
		G4ThreeVector posSourceSR = G4ThreeVector(0, 0, -ScintZ*0.5-DzSourceSR/2.);
		new G4PVPlacement(0,posSourceSR,logicSourceSR,"Source",logicWorld,false,0,checkOverlaps);
	}
	
	

	
	
	
	
	fScoringVolume = logicScint;

	return physWorld;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
