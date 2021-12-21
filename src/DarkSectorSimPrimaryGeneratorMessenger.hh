#ifndef DarkSectorSimPrimaryGeneratorMessenger_h
#define DarkSectorSimPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"

class DarkSectorSimPrimaryGeneratorAction;

class DarkSectorSimPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  DarkSectorSimPrimaryGeneratorMessenger(DarkSectorSimPrimaryGeneratorAction*);
  virtual ~DarkSectorSimPrimaryGeneratorMessenger();
  void SetNewValue(G4UIcommand*, G4String);

private:
  DarkSectorSimPrimaryGeneratorAction* fDSAction;
  G4UIdirectory* fDirectory;
  G4UIcmdWithAString* fGenerateCmd;
  G4UIcmdWithADouble* fVoxelRNumCmd;
  G4UIcmdWithADouble* fVoxelZNumCmd;
  G4UIcmdWithAString* fDarkMatterFileCmd;
};

#endif
