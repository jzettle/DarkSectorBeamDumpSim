#ifndef DarkSectorSimDetectorMessenger_h
#define DarkSectorSimDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

class DarkSectorSimDetectorConstruction;

class DarkSectorSimDetectorMessenger : public G4UImessenger
{
public:
  DarkSectorSimDetectorMessenger(DarkSectorSimDetectorConstruction*);
  virtual ~DarkSectorSimDetectorMessenger();
  void SetNewValue(G4UIcommand*, G4String); 
private:
  DarkSectorSimDetectorConstruction* fDetConstruct;
  G4UIdirectory *fDirectory;
  G4UIcmdWithAString *fInputModelCmd;
};

#endif
  
