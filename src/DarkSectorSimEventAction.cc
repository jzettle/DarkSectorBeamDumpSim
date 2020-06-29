#include "DarkSectorSimEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"

DarkSectorSimEventAction::DarkSectorSimEventAction(){}
DarkSectorSimEventAction::~DarkSectorSimEventAction(){}

void DarkSectorSimEventAction::BeginOfEventAction(const G4Event* event)
{
  DarkSectorSimAnalysis::GetInstance()->PrepareNewEvent(event);
}

void DarkSectorSimEventAction::EndOfEventAction(const G4Event* event)
{
  DarkSectorSimAnalysis::GetInstance()->EndOfEvent(event);
}
