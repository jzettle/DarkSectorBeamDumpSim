#include <cmath>
#include <iomanip>


#include "G4ios.hh"
#include "G4Types.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Tubs.hh"
#include "G4Box.hh" 
#include "G4Ellipsoid.hh" 
#include "G4Sphere.hh"      
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4VisAttributes.hh"

#include "G4GDMLParser.hh"

#include "TString.h"

#include "DarkSectorSimDetectorConstruction.hh"

DarkSectorSimDetectorConstruction::DarkSectorSimDetectorConstruction():
  G4VUserDetectorConstruction()
{
  fNISTManager = G4NistManager::Instance();
}

DarkSectorSimDetectorConstruction::~DarkSectorSimDetectorConstruction()
{
}

G4LogicalVolume* DarkSectorSimDetectorConstruction::GetWorldLogical() const
{
  G4LogicalVolumeStore* logStore = G4LogicalVolumeStore::GetInstance();

  return logStore->GetVolume("World");
}

G4VPhysicalVolume* DarkSectorSimDetectorConstruction::Construct()
{
  G4bool checkOverlaps = true;
  /*
  G4String world_name = "World";
  G4double world_rmin = 0*m;
  G4double world_rmax = 40*m;
  G4double world_z   = 40*m;
  G4double world_phimin = 0*rad;
  G4double world_phimax = 2*M_PI*rad;
  G4Material *air = fNISTManager->FindOrBuildMaterial("G4_AIR");
  G4Tubs *worldCyl = new G4Tubs(world_name, world_rmin, world_rmax, world_z, world_phimin, world_phimax);
  G4LogicalVolume *worldLog = new G4LogicalVolume(worldCyl, air, world_name);
  G4VPhysicalVolume *worldPhys = new G4PVPlacement(0, G4ThreeVector(0,0,0), worldLog, world_name, 0, false, 0);
  */
  //Try GDML with current geometry
  G4GDMLParser parser;
  std::string gdml_file = "../gdml/simple_target_detector/simple.gdml";
  G4cout << "Parsing GDML file at " << gdml_file << G4endl;
  parser.Read(gdml_file.c_str());
  G4cout << "GDML: Get World" << G4endl;  
  G4VPhysicalVolume* worldPhys = parser.GetWorldVolume();
  worldPhys->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
  //ConstructMaterials();
  //ConstructTarget();
  //ConstructDetector();
  G4cout << "Setting up WLS Properties!" << G4endl;
  SetWLSProperties();
  G4cout << "Setting up Reflector Properties!" << G4endl;
  SetReflProperties();
  G4cout << "Setting up Argon Properties!" << G4endl;
  SetArgonProperties();
  
  G4PhysicalVolumeStore *physvolstore = G4PhysicalVolumeStore::GetInstance();
  G4VPhysicalVolume *reflVol = physvolstore->GetVolume("ReflectorVol");
  G4VPhysicalVolume *wlsVol = physvolstore->GetVolume("WLSVol");
  G4double reflectivity = 0.99;
  SetReflSurface(wlsVol, reflVol, reflectivity, 0.05);

  return worldPhys;
}

void DarkSectorSimDetectorConstruction::ConstructMaterials()
{
  G4Element *C = fNISTManager->FindOrBuildElement("C", false);
  G4double a, z, density;
  // 2/12/2014
  // MINOS graphite target is ZXF-5Q from POCO Graphite --> rho = 1.78 g/cc
  fGraphite = new G4Material("Graphite", z=6., a= 12.01*g/mole, density= 1.78*g/cm3, kStateSolid);
  G4double temp_LAr = 87.1*kelvin;
  fLAr = new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3, kStateLiquid, temp_LAr);
}

void DarkSectorSimDetectorConstruction::ConstructTarget()
{
  //Set beam dump front face at (0,0,0)
  //make Box for now
  G4String target_name = "TargetVol";
  //G4double target_rmin = 0*cm;
  //G4double target_rmax = 13.5*cm;
  //G4double target_z = 3.6*m;
  //G4double target_phimin = 0*rad;
  //G4double target_phimax = 2*M_PI*rad;

  G4double target_x = 3.6*m;
  G4double target_y = 27.0*cm;
  G4double target_z = 27.0*cm;
  
  //G4Tubs *targetCyl = new G4Tubs(target_name, target_rmin, target_rmax, target_z/2.0, target_phimin, target_phimax);
  G4Box *targetBox = new G4Box(target_name, target_x/2.0, target_y/2.0, target_z/2.0);
  G4LogicalVolume *targetLog = new G4LogicalVolume(targetBox, fGraphite, target_name);
  G4RotationMatrix *rot = new G4RotationMatrix();
  rot->rotateX(-(M_PI/2.0)*rad);

  G4double dumpShift = target_x/2.0;
  G4VPhysicalVolume *targetPhys = new G4PVPlacement(0, G4ThreeVector(-dumpShift,0,0), targetLog,target_name,GetWorldLogical(), false, 0, true);
  
}

void DarkSectorSimDetectorConstruction::ConstructDetector()
{
  G4double det_face_distance = 18.0*m;

  G4String det_name = "FullDetectorVol";
  G4double det_x = 6*m;
  G4double det_y = 6*m;
  G4double det_z = 6*m;
  G4Box *detBox = new G4Box(det_name, det_x/2.0, det_y/2.0, det_z/2.0);
  G4LogicalVolume *detLog = new G4LogicalVolume(detBox, fLAr, det_name);
  G4VPhysicalVolume *detPhys = new G4PVPlacement(0, G4ThreeVector(det_face_distance+det_x,0,0), detLog,det_name, GetWorldLogical(), false, 0, true);

  G4String LArCyl_name = "LArCylVol";
  G4double LArCyl_rmin = 0*m;
  G4double LArCyl_rmax = 2.5*m;
  G4double LArCyl_z = 5.0*m;
  G4double LArCyl_phimin = 0*rad;
  G4double LArCyl_phimax = 2*M_PI*rad;

  G4Tubs *LArCyl = new G4Tubs(LArCyl_name,LArCyl_rmin, LArCyl_rmax, LArCyl_z/2.0, LArCyl_phimin, LArCyl_phimax);
  G4LogicalVolume *LArCylLog = new G4LogicalVolume(LArCyl, fLAr, LArCyl_name);
  G4VPhysicalVolume *LArCylPhys = new G4PVPlacement(0, G4ThreeVector(0,0,0), LArCylLog, LArCyl_name, detLog, false, 0, true);
  
}

void DarkSectorSimDetectorConstruction::SetWLSProperties()
{
  //Set Properties of wavelength shifter TPB
  G4Material *WLShifter = G4Material::GetMaterial("TPB");
  const G4int numWLSBins = 6;
  G4double WLSEnergy[numWLSBins] = {0.602*eV, 5.474*eV, 9.5*eV, 9.6*eV, 9.7*eV, 9.8*eV};
  G4double WLSAbsorption[numWLSBins] = {1000*m, 1000*m, 0.001*nm, 0.001*nm, 0.001*nm, 0.001*nm};
  G4double WLSTransmittance[numWLSBins] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  G4double WLSVisAbsorption[numWLSBins] = {1000.*m, 1000.*m, 1000.*m, 1000.*m, 1000.*m, 1000.*m};
  G4double WLSRefIndex[numWLSBins] = {1.38, 1.38, 1.38, 1.38, 1.38, 1.38}; //Make same as Reflector, Geant requires separate WLS-specific volume to perform WLS process, can adjust as needed

  //adapted from NIM A 654, 1, 2011, Pages 116-121                                                                                
  const G4int WLSEmitEntries = 25;
  G4double WLSEmitEnergy[WLSEmitEntries] =
    {0.602*eV, 0.689*eV, 1.03*eV,  1.926*eV, 2.138*eV, 2.25*eV, 2.38*eV,
     2.48*eV, 2.583*eV, 2.80*eV, 2.88*eV, 2.98*eV, 3.124*eV,
     3.457*eV, 3.643*eV, 3.812*eV, 4.086*eV,
     4.511*eV, 4.953*eV, 5.474*eV, 6.262*eV,
     7.000*eV, 8.300*eV, 10.00*eV, 12.60*eV };
  G4double WLSEmission[WLSEmitEntries] =
    { 0, 0, 0, 0, 0.0005, 0.0015, 0.003, 0.005,
      0.007, 0.011, 0.011, 0.006, 0.002,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0 }; //TPB Emission spectrum, recreates spectrum well

  G4MaterialPropertiesTable *WLSProp = new G4MaterialPropertiesTable();
  WLSProp->AddProperty("WLSABSLENGTH", WLSEnergy, WLSAbsorption, numWLSBins);
  WLSProp->AddProperty("WLSCOMPONENT", WLSEmitEnergy, WLSEmission, WLSEmitEntries);
  WLSProp->AddConstProperty("WLSTIMECONSTANT", 0.1*ns);
  WLSProp->AddConstProperty("WLSMEANNUMBERPHOTONS", 1.0);
  WLSProp->AddProperty("RINDEX", WLSEnergy, WLSRefIndex, numWLSBins);
  WLSProp->AddProperty("ABSLENGTH", WLSEnergy, WLSVisAbsorption, numWLSBins);
  WLSProp->AddProperty("TRANSMITTANCE", WLSEnergy, WLSTransmittance, numWLSBins);
  WLShifter->SetMaterialPropertiesTable(WLSProp);
}

void DarkSectorSimDetectorConstruction::SetReflProperties()
{
  G4Material *Reflector = G4Material::GetMaterial("PTFE"); //Use Teflon for now, can change if something else is preferred
  const G4int numReflBins = 2;
  G4double ReflEnergy[numReflBins] = {0.1*eV, 1600*eV};
  G4double ReflRefIndex[numReflBins] = {1.38, 1.38};
  G4double ReflReflectivity[numReflBins] = {0.99, 0.99}; //99% chance to reflect at all wavelengths (WAG)
  G4double ReflTransmittance[numReflBins] = {0.0,0.0}; //0% chance to transmit, then 1% chance to just be absorbed
  
  G4MaterialPropertiesTable *ReflProp =new G4MaterialPropertiesTable();
  ReflProp->AddProperty("RINDEX", ReflEnergy, ReflRefIndex, numReflBins);
  ReflProp->AddProperty("REFLECTIVITY", ReflEnergy, ReflReflectivity, numReflBins);
  Reflector->SetMaterialPropertiesTable(ReflProp);
}

void DarkSectorSimDetectorConstruction::SetReflSurface(G4VPhysicalVolume *exitVol, G4VPhysicalVolume *enterVol, G4double reflectivity, G4double sigmaAlpha)
{
  G4OpticalSurface *SurfRefl = new G4OpticalSurface("Reflector");
  SurfRefl->SetType(dielectric_dielectric);
  SurfRefl->SetModel(unified);
  if(sigmaAlpha <= 0.0)
     SurfRefl->SetFinish(polished);
  else
  {
    SurfRefl->SetFinish(ground);
    SurfRefl->SetSigmaAlpha(sigmaAlpha);
  }
  const G4int numReflBins = 2;
  G4double ReflEnergy[numReflBins] = {0.1*eV, 1600*eV};
  G4double ReflReflectivity[numReflBins] = {0.99, 0.99};
  G4MaterialPropertiesTable *ReflSurfProp = new G4MaterialPropertiesTable();
  ReflSurfProp->AddProperty("REFLECTIVITY", ReflEnergy, ReflReflectivity, numReflBins); 
  SurfRefl->SetMaterialPropertiesTable(ReflSurfProp);
  G4String surfName(Form("surf_%s_%s", exitVol->GetName().data(), enterVol->GetName().data()));
  new G4LogicalBorderSurface(surfName, exitVol, enterVol, SurfRefl);

}

void DarkSectorSimDetectorConstruction::SetArgonProperties()
{
  G4Material *LAr = G4Material::GetMaterial("liquidArgon");
  
  const G4int numScintBins = 3;
  G4double scintEn[numScintBins] = {9.6*eV,9.7*eV,9.8*eV};
  G4double scintSpectrum[numScintBins] = {0.0, 1.0, 0.0};

  G4double actualSY = 40000; //40k photons/MeV
  G4double photoneff = 1.0; //Right now, can just plug in constant PMT QE, could fold in real spectrum of visible light PMT QE but would increase sim time, for now do this
  G4double effectiveSY = actualSY*photoneff;

  G4MaterialPropertiesTable *LArProp = new G4MaterialPropertiesTable();
  LArProp->AddProperty("FASTCOMPONENT", scintEn, scintSpectrum, numScintBins);
  LArProp->AddProperty("SLOWCOMPONENT", scintEn, scintSpectrum,numScintBins);
  LArProp->AddConstProperty("SCINTILLATIONYIELD", effectiveSY/MeV);
  LArProp->AddConstProperty("FASTTIMECONSTANT", 6*ns);
  LArProp->AddConstProperty("SLOWTIMECONSTANT", 1600*ns);
  LArProp->AddConstProperty("RESOLUTIONSCALE", 0.1);
  LArProp->AddConstProperty("YIELDRATIO", 0.3); //fast/slow ratio -- make particle/energy dependent can use Regenfus et al. (2012) paper when necessary

  const G4int RIndexbins = 21;
  G4double RIndexEnergies[RIndexbins] =  {1.900*eV, 2.934*eV, 3.592*eV, 5.566*eV, 6.694*eV, 7.540*eV, 8.574*eV, 9.044*eV, 9.232*eV, 9.420*eV, 9.514*eV, 9.608*eV, 9.702*eV, 9.796*eV, 9.890*eV, 9.984*eV, 10.08*eV, 10.27*eV, 10.45*eV, 10.74*eV, 10.92*eV };
  G4double RIndexSpectrum[RIndexbins] = { 1.232,  1.236,  1.240,  1.261,  1.282,  1.306,  1.353,  1.387,  1.404,  1.423,  1.434,  1.446,  1.459,  1.473,  1.488,  1.505,  1.524,  1.569,  1.627,  1.751,  1.879 };
  const G4int AbsBins = 11;
  G4double AbsLengthEnergies[AbsBins] = {1.*eV, 2.*eV, 3.*eV, 4.*eV, 5.*eV, 6.*eV, 7.*eV, 8.*eV, 9.*eV, 10.*eV, 11.*eV};
  G4double AbsLengthSpectrum[AbsBins] = {2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm, 2000.*cm};

  const G4int RayleighBins = 22;
  //Rayleigh scattering length (cm) @ 90K as a function of energy (eV) from arXiv:1502.04213                                         
  G4double RayleighEnergies[RayleighBins] = {2.80*eV, 3.00*eV, 3.50*eV, 4.00*eV, 5.00*eV, 6.00*eV, 7.00*eV, 8.00*eV, 8.50*eV, 9.00*eV, 9.20*eV, 9.40*eV, 9.50*eV, 9.60*eV, 9.70*eV, 9.80*eV, 9.90*eV, 10.0*eV, 10.2*eV, 10.4*eV, 10.6*eV, 10.8*eV};
  G4double RayleighSpectrum[RayleighBins] = {47923.*cm, 35981.*cm, 18825.*cm, 10653.*cm, 3972.*cm, 1681.*cm, 750.9*cm, 334.7*cm, 216.8*cm, 135.0*cm, 109.7*cm, 88.06*cm, 78.32*cm, 69.34*cm, 61.06*cm, 53.46*cm, 46.50*cm, 40.13*cm, 28.91*cm, 19.81*cm, 12.61*cm, 7.20*cm};

  LArProp->AddProperty("RINDEX", RIndexEnergies, RIndexSpectrum, RIndexbins);
  LArProp->AddProperty("ABSLENGTH", AbsLengthEnergies, AbsLengthSpectrum, AbsBins);
  LArProp->AddProperty("RAYLEIGH", RayleighEnergies, RayleighSpectrum, RayleighBins); 
  LAr->SetMaterialPropertiesTable(LArProp);

}



