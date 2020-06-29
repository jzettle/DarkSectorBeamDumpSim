#ifndef DarkSectorSimTrackingAction_h
#define DarkSectorSimTrackingAction_h 1

#include "globals.hh"
#include "G4Track.hh"
#include "G4UserTrackingAction.hh"
#include "DarkSectorSimAnalysis.hh"

class DarkSectorSimTrackingAction : public G4UserTrackingAction
{
public:

  DarkSectorSimTrackingAction();
  virtual ~DarkSectorSimTrackingAction();
  virtual void PreUserTrackingAction(const G4Track* g4Track);
  virtual void PostUserTrackingAction(const G4Track* g4Track);
  
private:
};

#endif
    
