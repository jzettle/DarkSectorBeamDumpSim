#include "DarkSectorSimRunActionMessenger.hh"

#include "DarkSectorSimRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

DarkSectorSimRunActionMessenger::DarkSectorSimRunActionMessenger(DarkSectorSimRunAction* action) : fRunAction(action){}
DarkSectorSimRunActionMessenger::~DarkSectorSimRunActionMessenger(){}
void DarkSectorSimRunActionMessenger::SetNewValue(G4UIcommand* /*command*/, G4String /*newValues*/){}
