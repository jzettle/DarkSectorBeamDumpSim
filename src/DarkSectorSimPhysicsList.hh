#ifndef __DARKSECTORSIM_PHYSICS_LIST_HH__
#define __DARKSECTORSIM_PHYSICS_LIST_HH__

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

#include "G4Cerenkov.hh" 
#include "G4Scintillation.hh"
#include "G4Scintillation.hh"
#include "G4OpWLS.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"

class G4Scintillation;
class G4Scintillation;
class G4Cerenkov;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;
class G4OpWLS;
class G4OpWLS;
class G4VPhysicsConstructor;

class DarkSectorSimPhysicsListMessenger;

class DarkSectorSimPhysicsList : public G4VModularPhysicsList
{
public:
  DarkSectorSimPhysicsList(G4String);
  virtual ~DarkSectorSimPhysicsList();
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  void SetScint(G4bool scint);
//    void SetCuts();
  
private:
  G4Scintillation* theScintillationProcessElectron;
  G4Scintillation* theScintillationProcessNeutron;
  G4Cerenkov* theCerenkovProcess;
  G4OpAbsorption* theAbsorptionProcess;
  G4OpRayleigh* theRayleighScatteringProcess;
  G4OpBoundaryProcess* theBoundaryProcess;
  G4OpWLS* theWLSProcess; 
  DarkSectorSimPhysicsListMessenger* fPhysicsListMessenger;
  G4bool fScint;
};
#endif /* __DARKSECTORSIM_PHYSICS_LIST_HH__ */
