#ifndef DarkSectorSimStackingAction_h
#define DarkSectorSimStackingAction_h 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"
#include "DarkSectorSimAnalysis.hh"

class DarkSectorSimStackingAction : public G4UserStackingAction
{
public:
  DarkSectorSimStackingAction();
  virtual ~DarkSectorSimStackingAction();

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

private:
};

#endif
