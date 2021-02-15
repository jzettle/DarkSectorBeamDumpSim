#ifndef DARKSECTORSIM_DETECTOR_CONSTRUCTION_H
#define DARKSECTORSIM_DETECTOR_CONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"

#include <map>
#include "G4String.hh"
#include "globals.hh"

class G4NistManager;
class G4Material;
class G4LogicalVolume;
class DarkSectorSimDetectorMessenger;

class DarkSectorSimDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DarkSectorSimDetectorConstruction();
  ~DarkSectorSimDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();
  virtual DarkSectorSimDetectorMessenger* GetMessenger(void) {return fDetMessenger;}

  void ConstructTarget();
  void ConstructDetector();
  void SetInputModel(G4String);
private:
  std::map<G4String, G4String> fVolNames;
  DarkSectorSimDetectorMessenger *fDetMessenger;
  G4String fInputModel;
  G4NistManager *fNISTManager;
  G4Material *fGraphite;
  G4Material *fLAr;
  G4String fGDML_geom = "";
  void ConstructMaterials();
  G4LogicalVolume* GetWorldLogical() const;
  void SetWLSProperties();
  void SetReflProperties();
  void SetArgonProperties();
  void SetReflSurface(G4LogicalVolume* exitVol, G4LogicalVolume* enterVol, G4double reflectivity, G4double sigmaAlpha);
  void SetReflBorderSurface(G4VPhysicalVolume *exitVol, G4VPhysicalVolume *enterVol, G4double reflectivity, G4double sigmaAlpha);
};

#endif /* DARKSECTORSIM_DETECTOR_CONSTRUCTION_H */
