#ifndef DarkSectorSimAnalysis_h
#define DarkSectorSimAnalysis_h 1

#include "globals.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SteppingManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4Neutron.hh"
#include "G4GenericIon.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "G4TrackingManager.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include "TTimeStamp.h"
#include "TVector3.h"
#include "TParameter.h"
#include "TH1.h"
#include "TPad.h"
#include "DarkSectorSimTrajectory.hh"
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "DetectorEvent.hh"
#include "BeamEvent.hh"
#include "OpPhoton.hh"

class DetectorEvent;
class BeamEvent;
class OpPhoton;

class DarkSectorSimAnalysis
{
public: 
  static DarkSectorSimAnalysis* GetInstance()
  {
    if(DarkSectorSimAnalysis::fInstance == NULL)
      DarkSectorSimAnalysis::fInstance = new DarkSectorSimAnalysis();
    return DarkSectorSimAnalysis::fInstance;
  }

  virtual ~DarkSectorSimAnalysis();
  void SetROOTFileName(G4String& name) {fRootFileName = name;}
  void SetROOTTreeName(G4String& name) {fRootTreeName = name;}
  void SetVoxelization(G4bool voxel) {fVoxel = voxel;}
  void SetSaveFastOpTable(G4bool savefastop) {fSaveFastOpTable = savefastop;}
  void SetSaveBeamPion(G4bool savepion) {fSaveBeamPion = savepion;}
  void SetBeamPionFile(G4String pionfile) {fBeamPionFile = pionfile;}
  void PrepareNewRun(const G4Run* g4run);
  void EndofRun(const G4Run* g4run);
  void PrepareNewEvent(const G4Event* event);
  void EndOfEvent(const G4Event* event);
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* g4Track);
  void PreUserTrackingAction(const G4Track* g4Track, G4TrackingManager* fpTrackingManager);
  void PostUserTrackingAction(const G4Track* g4Track, G4TrackingManager* fpTrackingManager);
  void SteppingAction(const G4Step* step);

private:
  DarkSectorSimAnalysis();
  static DarkSectorSimAnalysis* fInstance;
  
  void ClearVariables();
  void SetBranches();
  G4String fRootFileName;
  G4String fRootTreeName;
  G4bool fVoxel;
  G4bool fSaveFastOpTable;
  G4bool fSaveBeamPion;
  std::string fBeamPionFile;
  TFile* fRootFile;
  TTree* fRootTree;
private:

  DetectorEvent *fDetEvent;
  BeamEvent *fBeamEvent;
  OpPhoton *fSinglePhoton;
  std::vector<OpPhoton> fPhotonStore;
  //ScintillationStore *fStore;
  G4int fEventNumber;
  G4double fParentEnergy;
  G4double fGenXProton;
  G4double fGenYProton;
  G4double fGenZProton;
  G4double fGenPiPlus;
  G4double fGenXPiPlus;
  G4double fGenYPiPlus;
  G4double fGenZPiPlus;
  G4double fGenPiZero;
  G4double fGenXPiZero;
  G4double fGenYPiZero;
  G4double fGenZPiZero;
  G4double fGenPiMinus;
  G4double fNumDAR;
  G4double fNumDIF;
  G4double fNum_numu;
  G4double fnumu_gen_posx;
  G4double fnumu_gen_posy;
  G4double fnumu_gen_posz;
  G4double fnumu_time;
  G4double fNum_nue;
  G4double fnue_gen_posx;
  G4double fnue_gen_posy;
  G4double fnue_gen_posz;
  G4double fnue_time;
  G4double fNum_antinue;
  G4double fNum_antinumu;
  G4double fantinumu_gen_posx;
  G4double fantinumu_gen_posy;
  G4double fantinumu_gen_posz;
  G4double fantinumu_time;
  G4double fnumu_energy;
  G4double fnue_energy;
  G4double fantinue_energy;
  G4double fantinumu_energy;
  G4double fnumu_darE;
  G4double fnue_darE;
  G4double fantinumu_darE;
  G4double fnumu_difE;
  G4double fnue_difE;
  G4double fantinumu_difE;
  G4double fNuInDetector;
  G4double fPiDecayTime;
  G4double fMuDecayTime;
  G4double fReflectTeflon;
  std::vector<G4double> fNumReflections;
  std::vector<G4double> fTrackLength;
  G4double fTotalPMTHit;
  G4double fTotalPMTHitVUV;
  G4int fPMTHit;
  G4double fPMTHitPosX;
  G4double fPMTHitPosY;
  G4double fPMTHitPosZ;
  std::vector<G4double> fTrackLengthWLS;
  G4double fTotalPMTHitWLS;
  G4double fTotalLength;
  std::vector<G4double> fPMTHitWLS;
  std::vector<G4double> fPhotonHitTimes;
  G4double fPhotonHitTime;
  G4double fF90;
  G4double fGenXPhoton;
  G4double fGenYPhoton;
  G4double fGenZPhoton;
  G4double fInitialtime;
  std::vector< std::vector <std::pair<G4double, G4double> > > fPMTReflTimeMap;
  std::vector< std::vector<G4double> > fPMTReflEffVec;

  G4double fpi_xmom;
  G4double fpi_ymom;
  G4double fpi_zmom;
  G4double fpi_time;
  G4double fpi_E;
};

class DarkSectorSimAnalysisMessenger : public G4UImessenger
{
public: 
  DarkSectorSimAnalysisMessenger(DarkSectorSimAnalysis* analysis);
  virtual ~DarkSectorSimAnalysisMessenger();
  virtual void SetNewValue(G4UIcommand* cmd, G4String val);

private:
  DarkSectorSimAnalysis* fAnalysis;
  G4UIdirectory* fDirectory;
  G4UIcmdWithAString* fRootFileNameCmd;
  G4UIcmdWithAString* fRootTreeNameCmd;  
  G4UIcmdWithABool* fUseVoxelCmd;
  G4UIcmdWithABool* fSaveFastOpTableCmd;
  G4UIcmdWithABool* fSaveBeamPionCmd;
  G4UIcmdWithAString* fBeamPionFileCmd;
};

#endif
