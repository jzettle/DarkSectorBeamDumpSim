#ifndef DarkSectorSimRunAction_h
#define DarkSectorSimRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "DarkSectorSimAnalysis.hh"

class DarkSectorSimRunActionMessenger;
class G4Run;

class DarkSectorSimRunAction : public G4UserRunAction
{
public:
  DarkSectorSimRunAction();
  virtual ~DarkSectorSimRunAction();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  DarkSectorSimRunActionMessenger* fMessenger;
  DarkSectorSimAnalysisMessenger* fAnalysisMessenger;
};

#endif
