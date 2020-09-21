//Adapted from BooNETrajectory.hh (BooNE Collaboration)

//class DarkSectorSimTrajectory;

#ifndef DarkSectorSimTrajectory_h
#define DarkSectorSimTrajectory_h 1

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleTable.hh"

class G4Polyline;
typedef std::vector<G4VTrajectoryPoint*> DarkSectorSimTrajectoryPointContainer;

class DarkSectorSimTrajectory : public G4VTrajectory
{
public:
  DarkSectorSimTrajectory();
  DarkSectorSimTrajectory(const G4Track* g4Track);
  DarkSectorSimTrajectory(DarkSectorSimTrajectory &);
  virtual ~DarkSectorSimTrajectory();
  
  inline void* operator new(size_t);
  inline void operator delete(void*);
  inline int operator == (const DarkSectorSimTrajectory& right) const
  {return(this==&right);}
  
  inline G4int GetTrackID() const {return fTrackID;}
  inline void SetTrackID(const G4int val) {fTrackID = val;}
  inline G4int GetParentID() const {return fParentID;}
  inline void SetParentID(const G4int val) {fParentID = val;}
  inline G4String GetParticleName() const {return fParticleName;}
  inline void SetParticleName(const G4String val) {fParticleName = val;}
  inline G4int GetPDGEncoding() const {return fPDGEncoding;}
  inline void SetPDGEncoding(const G4int val) {fPDGEncoding = val;}
  inline G4double GetCharge() const {return fPDGCharge;}
  inline void SetPDGCharge(const G4double val) {fPDGCharge = val;}
  inline G4double GetInitialEnergy() const {return fInitialEnergy;}
  inline void SetInitialEnergy(const G4double val) {fInitialEnergy = val;}
  inline G4ThreeVector GetInitialMomentum() const {return fInitialMomentum;}
  inline void SetInitialMomentum(const G4ThreeVector val) {fInitialMomentum = val;}
  inline G4double GetInitialTime() const {return fInitialTime;}
  inline void SetInitialTime(const G4double val) {fInitialTime = val;}
  inline G4int GetInitialStepNumber() const {return fInitialStepNumber;}
  inline void SetInitialStepNumber(const G4int val) {fInitialStepNumber = val;}
  inline G4int GetInitialTrackLength() const {return fInitialTrackLength;}
  inline void SetInitialTrackLength(const G4int val) {fInitialTrackLength = val;} 

  
  inline G4double GetFinalEnergy() const {return fFinalEnergy;}
  inline void SetFinalEnergy(const G4double val) {fFinalEnergy = val;}
  inline G4double GetFinalTime() const {return fFinalTime;}
  inline void SetFinalTime(const G4double val) {fFinalTime = val;}
  inline G4double GetFinalStepNumber() const {return fFinalStepNumber;}
  inline void SetFinalStepNumber(const G4int val) {fFinalStepNumber = val;}
  inline G4double GetFinalTrackLength() const {return fFinalTrackLength;}
  inline void SetFinalTrackLength(const G4int val) {fFinalTrackLength = val;}

  DarkSectorSimTrajectory* GetParentTrajectory();
  G4ParticleDefinition* GetDefinition();

  virtual int GetPointEntries() const {return posRec->size();}
  virtual G4VTrajectoryPoint* GetPoint(G4int i) const {return (*posRec)[i];}
  virtual void AppendStep(const G4Step* step);
  virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

private:

  DarkSectorSimTrajectoryPointContainer* posRec;

  G4ParticleDefinition* fParticleDef;
  G4int fTrackID;
  G4int fParentID;
  G4String fParticleName;
  G4int fPDGEncoding;
  G4double fPDGCharge;
  G4double fInitialEnergy;
  G4double fInitialTime;
  G4ThreeVector fInitialMomentum;
  G4int fInitialStepNumber;
  G4double fInitialTrackLength;
  G4double fFinalEnergy;
  G4double fFinalTime;
  G4int fFinalStepNumber;
  G4double fFinalTrackLength;
};

//extern G4Allocator<DarkSectorSimTrajectory> aTrajectoryAlloc;
extern G4ThreadLocal G4Allocator<DarkSectorSimTrajectory> * aTrajectoryAlloc;

inline void* DarkSectorSimTrajectory::operator new(size_t)
{
  //void* aTrajectory;
  //aTrajectory = (void*)aTrajectoryAlloc.MallocSingle();
  if(!aTrajectoryAlloc) aTrajectoryAlloc = new G4Allocator<DarkSectorSimTrajectory>;
  //return aTrajectory;
  return (void*)aTrajectoryAlloc->MallocSingle();
} 

inline void DarkSectorSimTrajectory::operator delete(void* aTrajectory)
{
  //aTrajectoryAlloc.FreeSingle((DarkSectorSimTrajectory*)aTrajectory);
  aTrajectoryAlloc->FreeSingle((DarkSectorSimTrajectory*)aTrajectory);
}

#endif
