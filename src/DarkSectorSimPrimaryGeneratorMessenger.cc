#include "DarkSectorSimPrimaryGeneratorMessenger.hh"
#include "DarkSectorSimPrimaryGeneratorAction.hh"

DarkSectorSimPrimaryGeneratorMessenger::DarkSectorSimPrimaryGeneratorMessenger(DarkSectorSimPrimaryGeneratorAction* DSGun):fDSAction(DSGun)
{
  fDirectory = new G4UIdirectory("/generator/");
  fDirectory->SetGuidance("Generator commands");
  
  fGenerateCmd = new G4UIcmdWithAString("/generator/setGenerator", this);
  fGenerateCmd->SetGuidance("Choose the primary event generator");
  fGenerateCmd->SetGuidance("gps (default)");
  fGenerateCmd->SetDefaultValue("gps");
  fGenerateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DarkSectorSimPrimaryGeneratorMessenger::~DarkSectorSimPrimaryGeneratorMessenger()
{
  delete fDirectory;
  delete fGenerateCmd;
}

void DarkSectorSimPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String value)
{
  if(cmd == fGenerateCmd)
    fDSAction->SetGenerator(value);
}
