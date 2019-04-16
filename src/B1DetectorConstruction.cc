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

B1DetectorConstruction::B1DetectorConstruction(G4double x0, G4double ZValue, G4double AbsHoleDiam, G4int SourceSelect, G4int AbsorberMaterial,G4double PterDiameter, G4double PterThickness,G4double SourceDiameter,G4double SourceThickness, G4double AbsorberThickness, G4double ProbeCaseDepth, G4double ProbeCaseLateralThickness, G4double ProbeCaseBackThickness, G4double HSLateralThickness, G4double HSBackThickness, G4int HousingCase, G4bool ScintFlag, G4int GaSet, G4int ApparatusMat,G4int PosAbsorber,G4double AbsCenter)
: G4VUserDetectorConstruction(),
fScoringVolume(0), fX0Scan(x0), fZValue(ZValue), fAbsHoleDiam(AbsHoleDiam), fSourceSelect(SourceSelect), fAbsorberMaterial(AbsorberMaterial), fPterDiameter(PterDiameter), fPterThickness(PterThickness), fSourceDiameter(SourceDiameter), fSourceThickness(SourceThickness), fAbsorberThickness(AbsorberThickness),fCaseDepth(ProbeCaseDepth),fLateralCaseThickness(ProbeCaseLateralThickness), fBackCaseThickness(ProbeCaseBackThickness), fHorsesShoeLateralThickness(HSLateralThickness),fHorsesShoeBackThickness(HSBackThickness), fHousingCase(HousingCase), fScintFlag(ScintFlag), fGaSet(GaSet), fApparatusMat (ApparatusMat), fPosAbsorber (PosAbsorber), fAbsCenter (AbsCenter)
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
	
	//###################################################
	// AGAR AGAR Source - AgarAgar should be C14 H24 O9
	//##########################
	G4double Agardensity = 1.030*g/cm3;
	G4Material* AgarAgar = new G4Material (name="AgarAgar", Agardensity, ncomponents=3);
	AgarAgar->AddElement (elH, natoms=24);
	AgarAgar->AddElement (elC, natoms=14);
	AgarAgar->AddElement (elO, natoms=9);
	
	//###################################################
	// ABS material - ABS should be C15 H17 N
	//##########################
	G4double ABSdensity = 0.7*g/cm3;
	G4Material* ABS = new G4Material (name="ABS", ABSdensity, ncomponents=3);
	ABS->AddElement (elH, natoms=17);
	ABS->AddElement (elC, natoms=15);
	ABS->AddElement (elN, natoms=1);
	
	//###################################################
	// P-Terphenyl Material
	//##########################
	G4double PTerphenyldensity = 1.23*g/cm3;
	G4Material* PTerphenyl= new G4Material (name="PTerphenyl", PTerphenyldensity, ncomponents=2);
	PTerphenyl->AddElement (elC, natoms=18);
	PTerphenyl->AddElement (elH, natoms=14);
	
	//###################################################
	// Delrin Material      C H2 O
	//##########################
	G4double Delrindensity = 1.41*g/cm3;
	G4Material* Delrin= new G4Material (name="Delrin", Delrindensity, ncomponents=3);
	Delrin->AddElement (elC, natoms=1);
	Delrin->AddElement (elH, natoms=2);
	Delrin->AddElement (elO, natoms=1);
	
	//###################################################
	// GaContainer2 Material (ABS)
	//##########################
	G4double GaContainer2Matdensity = 0.43*g/cm3; //mean measured density
	G4Material* GaContainer2Mat = new G4Material (name="GaContainer2Mat", GaContainer2Matdensity, ncomponents=3);
	GaContainer2Mat->AddElement (elH, natoms=17);
	GaContainer2Mat->AddElement (elC, natoms=15);
	GaContainer2Mat->AddElement (elN, natoms=1);
	
	//##########################
	//###################################################
	//###################################################################
	
#pragma mark Assignment of Materials
	//###################################################################
	//###################################################
	// Assignment of materials
	//##########################
	
	G4Material* SourceExtY_mat = AgarAgar;
	G4Material* ABSaround_mat = ABS;
	G4Material* ABSbehind_mat = ABS;
	G4Material* GaContainer_mat=nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	G4Material* GaContainer2_mat = GaContainer2Mat;
	G4Material* GaContainer3_mat =nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	G4Material* ProbeContainer_mat=nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	G4Material* SourceExtGa_mat=nist->FindOrBuildMaterial("G4_WATER");
	G4Material* SourceSR_mat = nist->FindOrBuildMaterial("MyAlu");
	G4Material* FrontShield_mat = nist->FindOrBuildMaterial("MyAlu");
	G4Material* shapeDummy_mat = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* Pter_mat = PTerphenyl;
	G4Material* PVC_mat= nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	G4Material* Delrin_mat=Delrin;
	G4Material* Absorber_mat = nist->FindOrBuildMaterial("G4_Cu");
	G4Material* SiPM_mat = nist->FindOrBuildMaterial("G4_Si");
	G4Material* Table_mat = nist->FindOrBuildMaterial("MyAlu");
	
	//Materials for NL probe case
	G4Material* PlasticCase_mat = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	G4Material* HorsesShoe_mat= nist->FindOrBuildMaterial("G4_Pb");
	G4Material* CaseInner_mat=nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");
	
	
	
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
	G4ThreeVector posSourceSR = G4ThreeVector(0, 0, -DzSourceSR*0.5);
	
	
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
	
//	if(fGaSet==1 && (fSourceSelect==1 || fSourceSelect==2 || fSourceSelect==6)) { //If I requested the Sr source (or the flat electron one for efficiencies)
//		G4cout<<"GEOMETRY DEBUG - Sr(-like) Source has been placed!!"<<G4endl;
//
//		new G4PVPlacement(0,                     //no rotation
//											posSourceSR,       //at (0,0,0)
//											logicSourceSR,            //its logical volume
//											"Source",               //its name
//											logicWorld,            //its mother  volume
//											false,                 //no boolean operation
//											0,                     //copy number
//											checkOverlaps);        //overlaps checking
//
//		logicSourceSR->SetRegion(sorgente);
//		sorgente->AddRootLogicalVolume(logicSourceSR);
//	}
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
	G4double ScintBucoZ=5*mm;
	
	G4double VialR=14.15*mm/2.;
	G4double VialDR=1*mm;
	G4double VialZ=46*mm;
	G4double VialCapZ=VialDR;

	G4VisAttributes* ScintVisAtt=new G4VisAttributes(G4Colour(0,0,1));
	G4VisAttributes* VialVisAtt=new G4VisAttributes(G4Colour(0,1,0));


	G4Box* solidScintBulk =	new G4Box("solidScintBulk",ScintX*0.5,ScintY*0.5,ScintZ*0.5);
	G4Tubs* solidScintScasso =	new G4Tubs("ScintScasso",0, ScintScassoR,ScintScassoZ*0.5, Ang0, Ang2Pi);
	G4Tubs* solidScintBuco =	new G4Tubs("ScintBuco",0, ScintBucoR,ScintBucoZ*0.5, Ang0, Ang2Pi);

	
	G4VSolid* solidScintTemp= new G4UnionSolid("ScintTemp",solidScintBulk,solidScintScasso,0,G4ThreeVector(0.,0.,ScintZ/2.));
	G4VSolid* solidScintAll= new G4SubtractionSolid("ScintAll",solidScintTemp,solidScintBuco,0,G4ThreeVector(0.,0.,-ScintZ/2.+ScintBucoZ/2.));

	G4LogicalVolume* logicScint =	new G4LogicalVolume(solidScintAll,Pter_mat,"Scint");
	G4VPhysicalVolume* physScint=new G4PVPlacement(0,G4ThreeVector(0,0,0),logicScint,"Scint",logicWorld,false,0, checkOverlaps);
	logicScint->SetVisAttributes(ScintVisAtt);

//
//	logicScint->SetRegion(pterreg);
//	pterreg->AddRootLogicalVolume(logicScint);
//
	
	
	G4ThreeVector posVial= G4ThreeVector(0,0,-VialZ/2.-VialCapZ-ScintZ/2.+ScintBucoZ);
//	G4ThreeVector posVial= G4ThreeVector(0,0,-29.5*mm);

	G4Tubs* solidVialBody =	new G4Tubs("VialBody",VialR-VialDR, VialR,VialZ*0.5, Ang0, Ang2Pi);
	G4Tubs* solidVialCap =	new G4Tubs("VialCap",0, VialR,VialCapZ*0.5, Ang0, Ang2Pi);
	G4VSolid* solidVial= new G4UnionSolid("Vial",solidVialBody,solidVialCap,0,G4ThreeVector(0.,0.,VialZ/2.+VialCapZ/2.));
	
	G4LogicalVolume* logicVial =	new G4LogicalVolume(solidVial,Pter_mat,"Vial");

	G4VPhysicalVolume* physVial=new G4PVPlacement(0,posVial,logicVial,"Vial",logicWorld,false,0, checkOverlaps);

	logicVial->SetVisAttributes(VialVisAtt);

	
	

	
	
	
	
	fScoringVolume = logicScint;

	return physWorld;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
