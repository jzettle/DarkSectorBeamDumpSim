#ifndef DarkSectorSimEventAction_h
#define DarkSectorSimEventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"
#include "DarkSectorSimAnalysis.hh"

class DarkSectorSimEventAction : public G4UserEventAction
{
public:
  DarkSectorSimEventAction();
  virtual ~DarkSectorSimEventAction();
private:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
};

#endif
