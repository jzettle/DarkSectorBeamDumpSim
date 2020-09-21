//Adapted from BooNETrajectory.cc (BooNE Collaboration)

#include "DarkSectorSimTrajectory.hh"
#include "G4TrajectoryPoint.hh"
#include "G4ParticleTable.hh"
#include "G4DynamicParticle.hh"
#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
#include "DarkSectorSimTrackingAction.hh"

G4ThreadLocal G4Allocator<DarkSectorSimTrajectory> * aTrajectoryAlloc = 0;

DarkSectorSimTrajectory::DarkSectorSimTrajectory()
{
  fTrackID = 0;
  fParticleDef = 0;
  fParentID = 0;
  fInitialTime = 0.0;
  fInitialTrackLength = 0;
  fFinalTime = 0.0;
  fFinalTrackLength = 0;
}

DarkSectorSimTrajectory::DarkSectorSimTrajectory(const G4Track* g4Track)
{
  posRec = new DarkSectorSimTrajectoryPointContainer();
  posRec->push_back(new G4TrajectoryPoint(g4Track->GetPosition()));
  fParticleDef = g4Track->GetDefinition();
  fTrackID = g4Track->GetTrackID();
  fParentID = g4Track->GetParentID();
}

DarkSectorSimTrajectory::DarkSectorSimTrajectory(DarkSectorSimTrajectory& right):G4VTrajectory(right)
{
  posRec = new DarkSectorSimTrajectoryPointContainer();
  for(size_t i = 0; i < right.posRec->size(); i++)
    {
      G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.posRec))[i]);
      posRec->push_back(new G4TrajectoryPoint(*rightPoint));
    }
  fParticleDef = right.fParticleDef;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
}

DarkSectorSimTrajectory::~DarkSectorSimTrajectory() 
{
  size_t i;
  for(i = 0; i < posRec->size(); i++)
    delete (*posRec)[i];
  posRec->clear();
  delete posRec;
}

void DarkSectorSimTrajectory::AppendStep(const G4Step* step)
{
  posRec->push_back(new G4TrajectoryPoint(step->GetPostStepPoint()->GetPosition()));
}

G4ParticleDefinition* DarkSectorSimTrajectory::GetDefinition()
{
  return(G4ParticleTable::GetParticleTable()->FindParticle(fParticleDef));
}

void DarkSectorSimTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  DarkSectorSimTrajectory* seco = (DarkSectorSimTrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(G4int i = 1; i < ent; i++)
    posRec->push_back((*(seco->posRec))[i]);
  delete (*seco->posRec)[0];
  seco->posRec->clear();
}

DarkSectorSimTrajectory* DarkSectorSimTrajectory::GetParentTrajectory()
{
  G4TrajectoryContainer* container = G4RunManager::GetRunManager()->GetCurrentEvent()->GetTrajectoryContainer();
  if(container == 0)
    return 0;
  TrajectoryVector* vect = container->GetVector();
  TrajectoryVector::iterator tr = vect->begin();
  for(;tr != vect->end(); tr++)
    if((*tr)->GetTrackID() == fParentID)
      return dynamic_cast<DarkSectorSimTrajectory*>(*tr);
 
  return 0;
}
