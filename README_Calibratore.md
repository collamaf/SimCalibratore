# Full simulation of Pter beta- probe in Geant4

## HOW TO RUN:
```
cd build
cmake -DGeant4_DIR=$G4INSTALL ../
make
./exampleB1 {flags (see below)} ../run1.mac
```
Some Use cases:
- Gallium68 Campaign Gemelli measurements (late 2018) with PVC
```
./exampleb1 -GaSet 3 -CaseDepth -155 -Source 4 -AbsD 0 -AbsT 5.5 -AbsMat 4 -AppMat 1 -ZAbs 2.75 -PosAbs 1 -PterD 6 -PterT 3 -Z 5.5 -SourceT 6.4  ../run1.mac 

```
- Gallium68 Campaign Gemelli measurements (late 2018) No PVC
```
./exampleb1 -GaSet 3 -CaseDepth -155 -Source 4 -AbsD -10 -AbsT 5.5 -AbsMat 4 -AppMat 1 -ZAbs 2.75 -PosAbs 1 -PterD 6 -PterT 3 -Z 5.5 -SourceT 6.4  ../run1.mac 

```

- Electron Efficiency of laparoscopic NL-probe (Post Gallium68 Campaign Gemelli measurements (late 2018))
```
./exampleb1  -Source 6   -PterD 6 -PterT 3 -Z 0 -CaseDepth 50 -CaseLT 1 -HSBT 2  ../effEle.mac 

```

- Gamma Efficiency of laparoscopic NL-probe (Post Gallium68 Campaign Gemelli measurements (late 2018))
```
./exampleb1  -Source 7   -PterD 6 -PterT 3 -Z 0 -CaseDepth 50 -CaseLT 1 -HSBT 2  ../effGamma.mac 

```

- Electron Efficiency of "standard" (pen-like) probe (Post Gallium68 Campaign Gemelli measurements (april 2019))
```
./exampleb1  -Source 6   -PterD 6 -PterT 3 -Z 0 ../effEle.mac

```

- Gamma Efficiency of  "standard" (pen-like) probe (Post Gallium68 Campaign Gemelli measurements (april 2019))
```
./exampleb1  -Source 7   -PterD 6 -PterT 3 -Z 0 ../effEle.mac

```


- To obtain efficiency curve:
```
- With Eabs

B1->Draw("SourceEne>>num(100)","Eabs>68","")
B1->Draw("SourceEne>>denom(100)","EnterPterFlag==1","")
num->Sumw2()
num->Divide(denom)
num->Draw("E")

- With NPMT

B1->Draw("SourceEne>>num(200)","Npmt>55","")
B1->Draw("SourceEne>>denom(200)","EnterPterFlag==1","")
num->Sumw2()
num->Divide(denom)
num->Draw("E")

```



## SOURCES
1 - Pointlike Sr
2 - Extended Sr
3 - ExtY
4 - ExtGa
5 - 511KeV gamma sphere
6 - FlatEle
7 - FlatGamma



## GEOMETRY
- All sources ending at Z=0
- DummyExitSorg: after source (with possible air gap in GaSet 3)
- DummyExitAbs: after absorber (if any)
- DummyEnterProbe: before FrontShield


## FLAGS

- -GaSet is used to choose the experimental setup; if -GaSet1 we are in the case in which we have the probe without the "Catafalco" while in the case -GaSet2 we have the "Catafalco's one". The case -GaSet3 is indeed the configuration in which the "catafalco"'s GaContainer is made by PVC and not by 3D printer such -GaSet2 case.

- -Z is used to choose the distance of the frontshield from the source.

- -AbsT is used to choose the absorber's thickness. If the absorber is not placed in GaSet 2 you have by the way give this value taking in consideration that the distance of the probe's head ( the frontshield ) from the source is given by Z + AbsT/2.

- -AbsD is used to choose the diameter of the absorber's hole in the center of the absorber itself; In -GaSet2 it must be = 0 (not >0 because in this setup the absorber isn't drilled in the midle) if you want to place the absorber and must be < 0 otherwise.                   

- -ZAbs is used to choose the position of the center of the absorber with respect to the source.

- -AbsMat  is used to choose the absorber's material; if AbsMat1->Cu, else if AbsMat2->Pb, else if AbsMat3->Alu, else if AbsMat4->PVC.

- -PosAbs (PosAbsorber) is used to choose absorber width in -GaSet2; if -PosAbs == 1 it will have the same diameter of the hole just near the source, that has 2mm depth, while if -PosAbs == 2 it will have the same diameter of the hole that has 6mm depth, just after the first hole.

- -Source is used to choose the source type: if Source=1 or Source=2 we have respectively point-like or extended Sr source, else if Source=3 we have Y extended source and if Source=4 we have Ga-68 extended source.

- -X is used to change the position of the probe along the x asse. N.B. must be 0 in GaSet2.

- -SourceD is used to choose the diameter of the source; this is preseted to 10mm, the diameter of the Ga container in GaSet2.

- -SourceT is used to choose the thickness of the source; this is preseted to 7mm, the thickness of the Ga container in GaSet2.

- -PterD is used to choose the diameter of the P-Terfenile; this is preseted to 6mm.

- -PterT is used tochoose the thickness of the P-Terfenile; this is preseted to 5mm.

- -CaseLT is used to set the lateral thickness of the probe's case.

- -CaseBT is used to set the back thickness of the probe's case ( the case is a sort of horseshoe and this flag set the thickness of the back part ).

- -HSLT is used to set the lateral thickness of the structure inside the case (this structure looks like an horseshoe too).

- -HBLT is used to set the back thickness of the structure inside the case (this structure looks like an horseshoe too).

- -CaseDepth is used to set the lenght of the probe's case. In GaSet2 if it's >0 the simulation will place inside the case the internal structure while if it's < 0 it'll not. If 0 the "bare probe" is placed (just PTER+Delrin+PVC)

- -HSMat is used to choose the material of the structure inside the case.                                                                                                    If HSMat1 (default) the inner material inside the inner horseshoe is made of PVC, the horseshoe itself is made of Pb and the externale case is made of PVC.                                                                                                                                                              Else if HSMat2 the inner material inside the inner horseshoe is made of air, the horseshoe itself is made of Pb and the externale case is made of air.                                                                                                                                                                                                              Else if HSMat3 all is made of air

- -AppMat is used to choose the "catafalco's" materials.



## PHYSICS
Process				Type		SubType
RadioActiveDecay	6			210
eIoni				2			2
eBrem				2			3

CPU TIMES NEEDED FOR 1e5 PRIMARIES:

## SCORING:
- Primary paricles generated (PrimAction);
- Decay products (StackingAction);
- What exits source (!->DummyExitSorg) (StepAction);
- What exit Absorber (if any) (Abs->DummyExitAbs) (StepAction);
- What enters Probe (DummyEnterProbe->FrontShield) (StepAction);
- What enters Pter (!->Pter) (StepAction)

## OUTPUT:
A root file named MCsondaGEANT_{Relevant sim info}.root is created, in which on an event (i.e. a primary particle) by event basis it is stored:

### SOURCE vector (one entry per primary particle, only for first 100k primaries if more primaries are requested):
- 0: AllX: X coordinate of primary particle [mm];
- 1: AllY: Y coordinate of primary particle [mm];
- 2: AllZ: Z coordinate of primary particle [mm];
- 3: AllCosX[1+]: X directive cosine of produced particle;
- 4: AllCosY[1+]: Y directive cosine of produced particle;
- 5: AllCosZ[1+]: Z directive cosine of produced particle;
- 6: AllEne[1+]: kinetic energy of produced particle [keV];
- 7: AllPart[1+]: PID of produced particle;
- 8: AllIsotope[1+]: isotope of primary particle (0=Sr, 1=Y ??, ParentID - 1);
- 9: ExitX: X coordinate of primary particle exiting the source volume (-> DummyExitSorg) [mm];
- 10: ExitY: Y coordinate of primary particle exiting the source volume [mm];
- 11: ExitZ: Z coordinate of primary particle exiting the source volume [mm];
- 12: ExitCosX[2+]: X directive cosine of primary particle exiting the source volume;
- 13: ExitCosY[2+]: Y directive cosine of primary particle exiting the source volume;
- 14: ExitCosZ[2+]: Z directive cosine of primary particle exiting the source volume;
- 15: ExitEne[2+]: kinetic energy of primary particle exiting the source volume [keV];
- 16: ExitPart[2+]: kind of primary particle (11=e-, -11=e+, 22=gamma, 13=mu-...) exiting the source volume;
- 17: ExitParentID[2+]: partent-id of particle exiting the source
- 18: ExitProcess[2+]: process that created the particles that exits the source (see table above)
- 19: ExitTrackN: number of different tracks exiting the source per event
- 20: AnnihilationX: X coord of annihilation point of beta+ source particle [mm]
- 21: AnnihilationY: Y coord of annihilation point of beta+ source particle [mm]
- 22: AnnihilationZ: Z coord of annihilation point of beta+ source particle [mm]

### B1 vector (one entry per primary particle that gives a >0 energy deposition):
- 0: Eabs: energy absorbed in Pter [keV];
- 1: EAbsComp[3]: vector containing energy absorbed in Pter [keV] due to Electrons (EAbsComp[0]), Positrons (EAbsComp[1]) and photons (EAbsComp[2])
- 2: InPterTrackN: number of hits inside Pter (it's the length of the following vector);
- 3: InPterPart[InPterTrackN]: kind of particle of hit inside Pter;
- 4: InPterEn[InPterTrackN]: energy deposit of single hit of particle inside Pter;
- 5: InPterPrimEn[InPterTrackN]: energy of the primary particle (decay product, or fresh primary e/gamma) that origined the hit of particle inside Pter [keV];
- 6: InPterPrimPart[InPterTrackN]: PID of the primary particle (decay product, or fresh primary e/gamma) that origined the hit of particle inside Pter [keV];
- 7: InPterTime[InPterTrackN]: time of interaction of hit inside Pter [ns] (To be really undersood);
- 8: InPterX[InPterTrackN]: X position of hit inside Pter [mm];
- 9: InPterY[InPterTrackN]: Y position of hit inside Pter [mm];
- 10: InPterZ[InPterTrackN]: Z position of hit inside Pter [mm];
- 11: PrePterTrackN: number of tracks entering Pter (from wherever) per primary (it's the length of the following vector);
- 12: PrePterPart[PrePterTrackN]: kind of particle of each track entering Pter (from wherever);
- 13: PrePterEn[PrePterTrackN]: kinetic energy of particle of each tracks entering Pter (from wherever) [keV];
- 14: PreProbeTrackN: number of tracks entering Probe (DummyEnterProbe->FrontShield) per primary (it's the length of the following vector);
- 15: PreProbePart[PreProbeTrackN]: kind of particle of each track entering Probe (DummyEnterProbe->FrontShield);
- 16: PreProbeEn[PreProbeTrackN]: kinetic energy of particle of each tracks entering Probe (DummyEnterProbe->FrontShield) [keV];
- 17: PostAbsTrackN: number of tracks exiting Absorber (Abs->DummyExitAbs) (if any, otherwise 0) per primary (it's the length of the following vector);
- 18: PostAbsPart[PrePterTrackN]: kind of particle of each track entering  Absorber (Abs->DummyExitAbs) (if any, otherwise empty);
- 19: PostAbsEn[PrePterTrackN]: kinetic energy of particle of each tracks entering  Absorber (Abs->DummyExitAbs) (if any, otherwise empty)  [keV];
- 20:  ExitEne[2+]: kinetic energy of primary particle exiting the source volume (-> DummyExitSorg) [keV];
- 21: SourceX: X coordinate of primary particle (isotope) giving a signal in Pter [mm];
- 22: SourceY: Y coordinate of primary particle (isotope) giving a signal in Pter [mm];
- 23: SourceZ: Z coordinate of primary particle (isotope) giving a signal in Pter [mm];
- 24: SourceCosX[2]: X directive cosine of decay electron(s) giving a signal in Pter;
- 25: SourceCosY[2]: Y directive cosine of  decay electron(s) giving a signal in Pter;
- 26: SourceCosZ[2]: Z directive cosine of decay electron(s) giving a signal in Pter;
- 27: SourceEne[2]: kinetic energy of  decay product(s)  giving a signal in Pter [keV];
- 28: SourcePart[2]: PID of  decay product(s)  giving a signal in Pter [keV];
- 29: SourceIsotope: isotope of primary particle (0=Sr, 1=Y) giving a signal in Pter;
- 30: Npmt: number of optical photons entering SiPm per event;
- 31: EnterPterFlag: bool flag to save if the primary particle had a secondary track entering PTER;
- 32: AnnihilationX: X coord of annihilation point of beta+ source particle [mm]
- 33: AnnihilationY: Y coord of annihilation point of beta+ source particle [mm]
- 34: AnnihilationZ: Z coord of annihilation point of beta+ source particle [mm]
- 35: AnnihilationTime: time of annihilation point of beta+ source particle [ns]
- 36: Nev: storing number of events generated


```


file=$(ls -t Primaries_*.dat | head -n1); tail -f $file


```
ELE
POS
FOT


## TO MERGE IN CASE OF MT

TString nomefile="PTERmc_PDiam6_PDz2_NoAbs_CaseDepth40_CaseLT1_CaseBT5_HSLT4_HSBT3_HSMat3_X0_Z2_Sphere511";
TChain * chain = new TChain("B1")
chain->Add(Form("%s_t*.root",nomefile.Data()))
TChain * chain2 = new TChain("Source")
chain2->Add(Form("%s_t*.root",nomefile.Data()))
TFile *file = TFile::Open(Form("%s.root",nomefile.Data()),"RECREATE");
chain->CloneTree(-1,"fast");
chain2->CloneTree(-1,"fast");
file->Write();


## TO ANALYZE HOUSING BACK MATERIAL

_file2->cd()
B1->Draw("EabsComp[2]","EabsComp[2]>0")
B1->Draw("EabsComp[0]","EabsComp[0]>0")
_file1->cd()
B1->Draw("EabsComp[2]","EabsComp[2]>0")
B1->Draw("EabsComp[0]","EabsComp[0]>0")
_file0->cd()
B1->Draw("EabsComp[2]","EabsComp[2]>0")
B1->Draw("EabsComp[0]","EabsComp[0]>0")

_file2->cd()
B1->Draw("PrePterEn","PrePterPart==22")
B1->Draw("PrePterEn","PrePterPart==11")
_file1->cd()
B1->Draw("PrePterEn","PrePterPart==22")
B1->Draw("PrePterEn","PrePterPart==11")
_file0->cd()
B1->Draw("PrePterEn","PrePterPart==22")
B1->Draw("PrePterEn","PrePterPart==11")




to see energy spectrum of electrons created by Sr/Y that exit the source

Per disegnare contributi Sr e Y:
```
B1->Draw("Eabs")
B1->SetLineColor(kBlue)
B1->Draw("InPterEnSr","","same")
B1->SetLineColor(kRed)
B1->Draw("InPterEnY","","same")
````

## CHANGELOG
2017.12.1 by collamaf
- Try to fix problem of primary particles double counting by putting a check in Stepping Action. Seems to reduce by about ~9% the number of exiting particles (NoCudZ2 test)

2017.12.4 by collamaf
- Added reset of pass counter in stacking action. Seems to increase back of about another 5% the number of exiting particles. now we have about 96% of the beginning. Verified this should be the right approach.

2017.12.12 by collamaf
- Corrected z_resin:  no need to add 1mm of copper since distance is always from source top

2018.01.17 by collamaf
- Deep reorganization of DetConstr, much clearer now
- Code extended to both sensors smoothly (new argument to be passed by terminal, default sensor 1).
- Fixed ExtY source problem

2018.01.30 by collamaf
- Quite deep reorganization of output root file, cleaned a little in ordering and name (and updated readme)
- Changed "ExtY" to "ExtY" in output file naming

2018.04.23 by collamaf
- Now resin is always present in front of Pter, if flag not selected made of air (useful for scoring)
- Added double crossing check also for particles entering Pter

2018.04.26 by collamaf
- Added InPterEnPrim to bring primary particle info to Riduzione and DataAnalysis

2018.05.7 by collamaf
- first implementation of storage of time of interection. Still not clear which time to save...
- Introduced possibility to simulate bare SiPm (assumed to be a particular version of Pter detector): SensorChoice=3


2018.05.31 by collamaf
- Now arguments are taken with labels, not necessary to give them all! If a macro is provided no visualization is init.. cool! (removed useless FilterFlag)

2018.06.05 by collamaf
- Added structure to classify energy release due to e-/e+/gamma

2018.06.11 by MorettiR
- Added Probe's Casing in Geometry with parameters from line comand.

2018.06.11 by collamaf
- Fixed spotted error in fSourceSelect==4 condition in PrimAct
- Added fSourceSelect==5 to generate a sphere (now with R=10cm) around origin of isotropic 511keV gammas to simulate the "far bacgkround" in a Ga68 environment
- Simplification of materials for the probe laparo housing: 3 materials: "Ext" for external housing, "Metal" for metal covering inside and "Inner" for inner part (maybe air)

2018.06.13 by MorettiR
- Probe Casing modified with boolean solids and possibility to choose the presence/absence of the casing, from line comand, simply putting respectively the CaseDepth > 0 or <=0 ( this is true only in the case -GaSet 1 ). 

2018.06.13 by collamaf
- Now when requesting Sphere Source the sphere is centered on the center of the probe case to gain in isotropy
- Changed "entering PTER" condition: now is "from Not PTER to PTER" to consider also tracks from behind (before was "from frontshield")
- Added in output file name also info about the probe case

2018.06.14 by collamaf
- Substituted "Midle" with "Middle"
- Fix sphere source offset when no case is requested

2018.06.20 by collamaf
- Added new argument from command line to choose material for inner probe case: "HSMat": 1 (default) is Pb+plastic, 2 is Pb+Air, 3 is Air+Air
- Added possibility to run in Multi Thread

2018.06.21 by collamaf
- Added scintillation for PTER (with ScintFlag from terminal, default NO). Actually addedd all Optical Physics but disabled for now Cerenkov directly in PhysList
- Added SiPm volume behind pter to score optical photons entering it
- Added scoring of "Npmt" in root file and "_Scint" to filename
- Now the progress status is printed on screen (since on Primaries file sometimes does not work) and every 10% of evts
- Changed condition to score primary decay product: added requeste StepN==0 to avoid double counting particles interacting via optical processes. Should not have any other undesired effect
- Excluded OptFot from InPter scoring to avoid huge size of root file
- Changed X and Z in filename to be in 0.1mm

2018.06.22 by collamaf
- Corrected error in FrontShield Thickness: was 5um is now 15

2018.06.27 by MorettiR
- Boolean Geometry Implemented for SiPM + TopCase solids

2018.07.06 by MorettiR
- Added G4ScoringManager in exampleB1.cc line 263 (Problem to be fixed).
- Addition of "GaSetting" flag to choiche experimental setup in Ga-Source Case.
- Addition of the 3D-Printed expreimental setup.
- In -GaSet 2  -CaseDepth>0 case, every distances are setted and can't be modified by line comand exept the ProbeCase Depth.
- In -GaSet 2 case exists only 2 configurations: if CaseDepth>0 we have the one with ProbeCase while if CaseDepth<0 we have the classic beta minus probe (that is an aluminum case behind the Pter).
- In -GaSet 2 case if CaseDepth<0 his absolute value correspondes to the aluminum case's length.
- In -GaSet 1 case we have the usual simulation (the one before this upgrade).
- To must be add, 3D-Printed expreimental setup's material.

2018.07.15 by MorettiR
- Added Absorber in -GaSet 2.
- Added flag PosAbsorber to choose absorber position in -GaSet2; if -PosAbs == 1 it will be placed in the hole just near the source, that has 2mm depth, while if -PosAbs == 2 it will be placed  in the hole that has 6mm depth.
- The Absorber thickness could be choosen by AbsT flag while the diameter is fixed in both PosAbsorber cases.
- The flag AbsD must be >=0.


2018.07.17 by MorettiR
- Added opportunity to select placement position of the absorber; the PosAbsorber now is used only to fix absorber diameter, in fact this depends in which hole I'll place the absorber.
- The -Z flag is used to choice the absorber's placement in -GaSet2 case.
- Example of a line comad for the optimal choice of the absorber type: ./exampleb1 -GaSet 2 -CaseDepth -40 -Source 4 -AbsD 0 -AbsT 1 -AbsMat 2 -AppMat 1 -Z 7 -PosAbs 2 ../run1.mac
- Stepping Action fixed when fCudiam>=0.

2018.07.17 by MorettiR
- Fixed bug in geometry relative to Dummy placement.
- Addition of another solid Dummy2 for stepping action in -GaSet 2 case.
- Example of a line comad for the optimal choice of the absorber type: ./exampleb1 -GaSet 2 -CaseDepth -40 -Source 4 -AbsD 0 -AbsT 1 -AbsMat 2 -AppMat 1 -Z 0.5 -PosAbs 1 -PterD 5 -PterT 3 ../run1.mac

2018.07.19 by MorettiR
- File name fixed.

2018.07.20 by Collamaf + MorettiR
- Fixed scoring macro
- Fixed source thickness in PrimGenAct
- Added keep event for interesting annihilation events

2018.07.20 by MorettiR
- Addition of annihilation coordinates of positrons in the .root file

2018.07.25 by MorettiR
- Addition of a leaf in B1 branch about energy of particles arriving on Absorber from source  called "PreDummy2En"
- Addition of a leaf in B1 branch about energy of particles arriving on Dummy2 from source  called "InDummy2En"
- Change in stepping action's condition relative to positrons annihilation region; now I'm looking annihilations in all over the world.

2018.07.26 by MorettiR
- New leafs added in B1 and Source Branches 

2018.09.19 by MorettiR
- The position of the Probe in GaSet2 has been modified; now the top of the probe is placed at 5.5mm from the source (before at 8mm).
This because the configuration used at Gemelli's hospital in which the probe was placed on the absrober's surface (We used absorber of 5.5mm thickness). In the configuration without the absorber the probe was placed at the same distance of 5.5mm from the source.
- Fixed the position of the probe, now is take in consideration the dummy2's thickness.
- SourceDiameter now fixed at 10.*mm and SourceThickness at 7.*mm

2018.10.2 by collamaf
- Fixed error in StackingAction: we were writing info on produced particles only if PDGcode>0, thus cutting all anti particles

2018.11.9 by MorettiR
- README file uploaded with flags specifications.
- Fixing of line 1639 and 1640 of DetectorConstruction.cc .

2018.11.11 by MorettiR
- Now the position of the probe's head ( the frontshield ) varies automatically with the placement of the absorber in GaSet 2.  
  N.B. in this way you have always to give AbsT and Z even if you do not want to place the absorber, for more details take a look to the flags section in this ReadMe file. 
- Name of the file with no abs in GaSet 2 has been modified. Now there is inside the name the distance of the probe from the source.


2018.11.14 by MorettiR
- A new leaf called "AnnihilationTime" has been added to B1 vector. This gives you the time elapsed since the positron's creation and his annihilation. 
- Now we can count the number of hits in the SiPM thanks to the new leaf "EabsSiPM" in B1 Vector.

2018.11.23 by MorettiR
- Added new configuration GaSet3; in this one the GaContainer is obtained by PVC and not printed by 3D printer. 
- File name modified to take into account GaSet3 configuration.
- New stepping action condition in source's branch (to be cheked). Take a look to line 144 an line 76 in stepping action to see changes. 

2018.12.03 by MorettiR
- Table added in GaSet1 configuration.

2018.12.05 by MorettiR
- Added new flag for absorber placement -ZAbs. This corresponds to the center of the absorber on z axes.
- Now in GaSet 2 and GaSet3, the flag -Z corresponds to the distance of the frontshield from the source on z axes (-Z must be always >= -AbsT when the absorber is placed to avoid superposition of volumes).
- Fixed superposistion of absorber in GaSet1
- Added study of Catafalco's material in GaSet3

2018.12.07 by MorettiR (morning)
- Fixed GaSet3 dimension of catafalco's Gacontainer.

2018.12.07 by MorettiR (afternoon)
- Fixed enumeration in B1RunAction (take a look also to B1EventAction).

2018.12.08 by MorettiR
- Fixed line 99 of B1RunAction.

2018.12.11 by MorettiR
- Fixed error in geometry relative to the source.
- Fixed error in PrimaryGeneratorAction relativo to GaSet 3.

2018.12.12 by MorettiR
- Fixed error in geometry relative to the dimension of the dummy2 in GaSet3.
- Fixed error in geometry relative to the dimension of the absorber in GaSet3.

2018.12.12 by MorettiR (afternoon)
- Now you can choose di thickness of the source in GaSet3.

2018.12.12 by collamaf
- Added flat sources (ele-6, gamma-7) for efficiency studies

2019.01.09 by collamaf
- First attempt of deep reorganization and cleaning of DetConst: seems to work fine at least up to GaSet 1. Please note that in this case the absorber is always ontop of source (no need of AbsorberPos) and the Z distance of the probe is always from source surface

2019.01.10 by collamaf
- Dummy reorganisation seems to work at least for Gaset 1 and 3. Need to fix DummyExitSource for GaSet 3, but first need to fix ExtGa z position in this case to start from the bottom of the hole

2019.01.14 by collamaf
- Finally got rid of all old "dummies". New 3 dummy scheme seems to work for all 3 gaset!
- Removed all references to FilePrim

2019.01.16 by collamaf
- Deep Cleaning of scoring, root file, PrimAction etc
- Now primary particle position in GaSet 2/3 is based on "Source" volume in DetConst
- Still need to clean OutFIleName creation in main

2019.01.17 by collamaf
- Cleaned OutFileName generation in main
- Fixed InPterPrimPart (was same as SourcePart)
- Updated readme about scoring

2019.04.01 by collamaf
- Updated readme about Efficiency (also for stanard pen like probe)

## TO DO's

- flag per non piazzare la struttura di supporto se uno dei valori passati da terminale a riguardo è negativo
- Sistemare l'overlap fra il volume SiPm e il tappo presente dietro il PTER quando c'è il case
- capire perche quando si accende la scintillazione poi guardando il verbose dell'evento al posto di RadioactiveDecay compare sempre "Scintillation" (ma lo scoring della sorgente lo risonosce uguale..)
- Sostituire fCuDiam e CuDiam
- Correggere sovrapposizione tra top case e SiPM in GaSet 2 quando DepthCase > 0
