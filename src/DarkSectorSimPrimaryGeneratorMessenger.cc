#include "DarkSectorSimPrimaryGeneratorMessenger.hh"
#include "DarkSectorSimPrimaryGeneratorAction.hh"

DarkSectorSimPrimaryGeneratorMessenger::DarkSectorSimPrimaryGeneratorMessenger(DarkSectorSimPrimaryGeneratorAction* DSGun):fDSAction(DSGun)
{
  fDirectory = new G4UIdirectory("/generator/");
  fDirectory->SetGuidance("Generator commands");
  
  fGenerateCmd = new G4UIcmdWithAString("/generator/setGenerator", this);
  fGenerateCmd->SetGuidance("Choose the primary event generator");
  fGenerateCmd->SetGuidance("gps (default), VoxelPhoton");
  fGenerateCmd->SetDefaultValue("gps");
  fGenerateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fVoxelRNumCmd = new G4UIcmdWithADouble("/generator/voxelNumR", this);
  fVoxelRNumCmd->SetGuidance("Choose the radial voxel number to generate in (either radial or z)");
  fVoxelRNumCmd->SetDefaultValue(0.0);
  fVoxelRNumCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fVoxelZNumCmd = new G4UIcmdWithADouble("/generator/voxelNumZ", this);
  fVoxelZNumCmd->SetGuidance("Choose the z-position voxel number to generate in (either radial or z)");
  fVoxelZNumCmd->SetDefaultValue(0.0);
  fVoxelZNumCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}

DarkSectorSimPrimaryGeneratorMessenger::~DarkSectorSimPrimaryGeneratorMessenger()
{
  delete fDirectory;
  delete fGenerateCmd;
  delete fVoxelRNumCmd;
  delete fVoxelZNumCmd;
}

void DarkSectorSimPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String value)
{
  if(cmd == fGenerateCmd)
    fDSAction->SetGenerator(value);

  if(cmd == fVoxelRNumCmd)
    fDSAction->SetVoxelRNum(fVoxelRNumCmd->GetNewDoubleValue(value));
  
  if(cmd == fVoxelZNumCmd)
    fDSAction->SetVoxelZNum(fVoxelZNumCmd->GetNewDoubleValue(value));
}
