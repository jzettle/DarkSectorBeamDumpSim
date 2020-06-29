#ifndef DarkSectorSimSteppingAction_h
#define DarkSectorSimSteppingAction_h 1

#include "globals.hh"

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "DarkSectorSimAnalysis.hh"


class DarkSectorSimSteppingAction : public G4UserSteppingAction
{
public:
  DarkSectorSimSteppingAction();
  virtual ~DarkSectorSimSteppingAction();

  virtual void UserSteppingAction(const G4Step*);

private:

};

#endif
