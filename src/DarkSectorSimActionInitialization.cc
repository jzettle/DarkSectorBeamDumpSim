#include "DarkSectorSimPrimaryGeneratorAction.hh"
#include "DarkSectorSimSteppingAction.hh"
#include "DarkSectorSimRunAction.hh"
#include "DarkSectorSimEventAction.hh"
#include "DarkSectorSimTrackingAction.hh"
#include "DarkSectorSimSteppingAction.hh"
#include "DarkSectorSimStackingAction.hh"
#include "DarkSectorSimActionInitialization.hh"

DarkSectorSimActionInitialization::DarkSectorSimActionInitialization():
  G4VUserActionInitialization()
{}

DarkSectorSimActionInitialization::~DarkSectorSimActionInitialization()
{}

void DarkSectorSimActionInitialization::Build() const
{
  SetUserAction(new DarkSectorSimPrimaryGeneratorAction());
  DarkSectorSimRunAction* runAction = new DarkSectorSimRunAction();
  SetUserAction(runAction);
  DarkSectorSimEventAction* eventAction = new DarkSectorSimEventAction();
  SetUserAction(eventAction);
  SetUserAction(new DarkSectorSimTrackingAction());
  SetUserAction(new DarkSectorSimSteppingAction());
  SetUserAction(new DarkSectorSimStackingAction());
}
