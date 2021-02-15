#include "DarkSectorSimDetectorMessenger.hh"
#include "DarkSectorSimDetectorConstruction.hh"

DarkSectorSimDetectorMessenger::DarkSectorSimDetectorMessenger(DarkSectorSimDetectorConstruction *det) : G4UImessenger(), fDetConstruct(det)
{
  fDirectory = new G4UIdirectory("/inputmodel/");
  fDirectory->SetGuidance("Control input model (beam or detector simulation)");
  
  fInputModelCmd = new G4UIcmdWithAString("/inputmodel/inputGDML", this);
  fInputModelCmd->SetGuidance("Set the input GDML file (full path, default detector simulation)");
  fInputModelCmd->SetDefaultValue("../gdml/simple_target_detector/simple.gdml");
  fInputModelCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  
}

DarkSectorSimDetectorMessenger::~DarkSectorSimDetectorMessenger()
{
  delete fDirectory;
  delete fInputModelCmd;
}

void DarkSectorSimDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String val)
{
  
  if(cmd == fInputModelCmd)
  { 
    fDetConstruct->SetInputModel(val);
  }
}
