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

#include "DarkSectorSimDetectorConstruction.hh"

DarkSectorSimDetectorConstruction::DarkSectorSimDetectorConstruction():
  G4VUserDetectorConstruction()
{
  fNISTManager = G4NistManager::Instance();
  fGDML_geom = "../gdml/simple_target_detector/simple.gdml";
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
  std::string gdml_file = fGDML_geom;
  G4cout << "Parsing GDML file at " << gdml_file << G4endl;
  parser.Read(gdml_file.c_str());
  G4cout << "GDML: Get World" << G4endl;  
  G4VPhysicalVolume* worldPhys = parser.GetWorldVolume();
  worldPhys->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
 
  //ConstructMaterials();
  //ConstructTarget();
  //ConstructDetector();
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



