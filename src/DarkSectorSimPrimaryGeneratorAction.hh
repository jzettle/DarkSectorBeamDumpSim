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

private:
  G4GeneralParticleSource* fParticleGun;    // For normal work need using gps
  DarkSectorSimPrimaryGeneratorMessenger* fGunMessenger;
  G4String fGenerator;
  G4ParticleGun* fPartGenerator;
};
#endif /* __DARKSECTORSIM_PRIMARY_GENERATOR_ACTION_HH__ */
