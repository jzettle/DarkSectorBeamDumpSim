#include "DarkSectorSimStackingAction.hh"

DarkSectorSimStackingAction::DarkSectorSimStackingAction(){}
DarkSectorSimStackingAction::~DarkSectorSimStackingAction(){}

G4ClassificationOfNewTrack DarkSectorSimStackingAction::ClassifyNewTrack(const G4Track* g4Track)
{
  return DarkSectorSimAnalysis::GetInstance()->ClassifyNewTrack(g4Track);
}
