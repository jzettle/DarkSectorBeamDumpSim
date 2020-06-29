#include "DarkSectorSimSteppingAction.hh"
#include "G4SystemOfUnits.hh"

DarkSectorSimSteppingAction::DarkSectorSimSteppingAction(){}
DarkSectorSimSteppingAction::~DarkSectorSimSteppingAction(){}

void DarkSectorSimSteppingAction::UserSteppingAction(const G4Step* step)
{
  DarkSectorSimAnalysis::GetInstance()->SteppingAction(step);
}

