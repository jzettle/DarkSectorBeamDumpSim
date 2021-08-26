#ifndef __DARKSECTORSIM_PRIMARY_GENERATOR_ACTION_HH__
#define __DARKSECTORSIM_PRIMARY_GENERATOR_ACTION_HH__

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

//#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "globals.hh"
#include <vector>

class G4ParticleGun;
class G4Event;
class DarkSectorSimPrimaryGeneratorMessenger;
class G4ParticleTable;

class DarkSectorSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  DarkSectorSimPrimaryGeneratorAction();
  virtual ~DarkSectorSimPrimaryGeneratorAction();

  void GeneratePrimaries(G4Event *event);
  void SetGenerator(G4String);
  void SetVoxelRNum(G4double voxelR);
  void SetVoxelZNum(G4double voxelZ);
  void setupDMFile(std::string filename);
  void GenerateOptPhotonVoxel(G4Event *event);
  void GetPositioninVoxel(G4ThreeVector &pos, G4double voxelR, G4double voxelZ);
  void GenerateDM(G4Event *event);

private:
  G4GeneralParticleSource* fParticleGun;    // For normal work need using gps
  DarkSectorSimPrimaryGeneratorMessenger* fGunMessenger;
  G4String fGenerator;
  G4ParticleGun* fPartGenerator;
  G4double voxelRNum;
  G4double voxelZNum;
  std::vector<G4double> fDMposX;
  std::vector<G4double> fDMposY;
  std::vector<G4double> fDMposZ;
  std::vector<G4double> fDMmomX;
  std::vector<G4double> fDMmomY;
  std::vector<G4double> fDMmomZ;
  std::vector<G4double> fDMtime;
  std::vector<G4double> fDMrecE;
  
};
#endif /* __DARKSECTORSIM_PRIMARY_GENERATOR_ACTION_HH__ */
