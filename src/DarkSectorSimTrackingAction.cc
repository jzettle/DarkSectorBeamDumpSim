#include "DarkSectorSimTrackingAction.hh"

DarkSectorSimTrackingAction::DarkSectorSimTrackingAction(){}
DarkSectorSimTrackingAction::~DarkSectorSimTrackingAction(){}

void DarkSectorSimTrackingAction::PreUserTrackingAction(const G4Track* g4Track)
{
  DarkSectorSimAnalysis::GetInstance()->PreUserTrackingAction(g4Track, fpTrackingManager);
}

void DarkSectorSimTrackingAction::PostUserTrackingAction(const G4Track* g4Track)
{
  DarkSectorSimAnalysis::GetInstance()->PostUserTrackingAction(g4Track, fpTrackingManager);
}
