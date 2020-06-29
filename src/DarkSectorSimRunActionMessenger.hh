#ifndef DarkSectorSimRunActionMessenger_h
#define DarkSectorSimRunActionMessenger_h 1

#include "globals.hh"

#include "G4UImessenger.hh"

class DarkSectorSimRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class DarkSectorSimRunActionMessenger: public G4UImessenger
{
public:
  DarkSectorSimRunActionMessenger(DarkSectorSimRunAction*);
  ~DarkSectorSimRunActionMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  DarkSectorSimRunAction* fRunAction;
};

#endif
