#include "DarkSectorSimRunAction.hh"
#include "DarkSectorSimRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "Randomize.hh"

DarkSectorSimRunAction::DarkSectorSimRunAction()
{
  fMessenger = new DarkSectorSimRunActionMessenger(this);
  fAnalysisMessenger = new DarkSectorSimAnalysisMessenger(DarkSectorSimAnalysis::GetInstance());
}

DarkSectorSimRunAction::~DarkSectorSimRunAction()
{
  delete fMessenger;
  //delete fAnalysisMessenger;
}

void DarkSectorSimRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "In Begin Run Action" << G4endl;
  DarkSectorSimAnalysis::GetInstance()->PrepareNewRun(aRun);
  
#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance()) {
    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/clear/view");
    UI->ApplyCommand("/vis/draw/current");
  }
#endif
  
}

void DarkSectorSimRunAction::EndOfRunAction(const G4Run* aRun)
{
  DarkSectorSimAnalysis::GetInstance()->EndofRun(aRun);
  
#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance())
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/show/view");
#endif
  
}
