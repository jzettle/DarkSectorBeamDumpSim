#include "DarkSectorSimAnalysis.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrackVector.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4VProcess.hh"
#include "G4TrajectoryContainer.hh"
#include "G4EventManager.hh"
#include "G4NistManager.hh"
#include "G4HadronicProcessStore.hh"
#include "TDirectory.h"
#include "TRandom3.h"
#include "Randomize.hh"
#include "DarkSectorSimTrackingAction.hh"
#include "DarkSectorSimTrajectory.hh"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <limits>

#include "DetectorChannel.hh"
#include "DetectorEvent.hh"
#include "BeamEvent.hh"
#include "ScintillationStore.hh"

DarkSectorSimAnalysisMessenger::DarkSectorSimAnalysisMessenger(DarkSectorSimAnalysis* analysis)
{
  fAnalysis = analysis;
  //Create commands to use in Geant4 run macro files
  fDirectory = new G4UIdirectory("/analysis/");
  fDirectory->SetGuidance("Control the analysis output");

  fRootFileNameCmd = new G4UIcmdWithAString("/analysis/setOutputFileName",this);
  fRootFileNameCmd->SetGuidance("Set the output ROOT file name");
  fRootFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fRootTreeNameCmd = new G4UIcmdWithAString("/analysis/setOutputTreeName",this);
  fRootTreeNameCmd->SetGuidance("Set the output ROOT tree name");
  fRootTreeNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  fUseVoxelCmd = new G4UIcmdWithABool("/analysis/useVoxel", this);
  fUseVoxelCmd->SetGuidance("Use voxelization for optical table?");
  fUseVoxelCmd->SetDefaultValue(false);
  fUseVoxelCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSaveFastOpTableCmd = new G4UIcmdWithABool("/analysis/saveFastOpTable", this);
  fSaveFastOpTableCmd->SetGuidance("Save ROOT tree for information tables for fast optical simulation (run only once!)");
  fSaveFastOpTableCmd->SetDefaultValue(false);
  fSaveFastOpTableCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DarkSectorSimAnalysisMessenger::~DarkSectorSimAnalysisMessenger()
{
  delete fDirectory;
  delete fRootFileNameCmd;
  delete fRootTreeNameCmd;
  delete fUseVoxelCmd;
  delete fSaveFastOpTableCmd;
}
void DarkSectorSimAnalysisMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if(cmd == fRootFileNameCmd)
    fAnalysis->SetROOTFileName(val);
  else if(cmd == fRootTreeNameCmd)
    fAnalysis->SetROOTTreeName(val);
  else if(cmd == fUseVoxelCmd)
    fAnalysis->SetVoxelization(val);
  else if(cmd == fSaveFastOpTableCmd)
    fAnalysis->SetSaveFastOpTable(val);
}

DarkSectorSimAnalysis* DarkSectorSimAnalysis::fInstance = 0;

DarkSectorSimAnalysis::DarkSectorSimAnalysis()
{
  fRootFile = NULL;
  fRootTree = NULL;
  fVoxel = false;
  fSaveFastOpTable = false;
  fRootFileName = "DSout.root";
  fRootTreeName = "DStree";
  fDetEvent = new DetectorEvent();
  fBeamEvent = new BeamEvent();
  ClearVariables();
}

DarkSectorSimAnalysis::~DarkSectorSimAnalysis(){;}

void DarkSectorSimAnalysis::PrepareNewRun(const G4Run* g4run)
{
  TTimeStamp start;
  G4cout << "Run " << g4run->GetRunID() << " starts at" << G4endl;
  start.Print();
  //Create ROOT file and tree for simulation output
  fRootFile = new TFile(fRootFileName, "RECREATE");
  fRootFile->cd();
  //gDirectory->WriteObject(&start, Form("Start time for Run %d", g4run->GetRunID()));
  fRootTree = new TTree(fRootTreeName, "DS Information Tree");
  fRootTree->SetDirectory(fRootFile);
  //Reset variables and add all ROOT branches to file for output
  if(!fDetEvent)
    fDetEvent = new DetectorEvent();
  if(!fBeamEvent)
    fBeamEvent = new BeamEvent();
  ClearVariables();
  SetBranches();
}

void DarkSectorSimAnalysis::EndofRun(const G4Run* g4run)
{
  //Last place Geant4 goes after all events are generated and tracked
  //TTimeStamp end;
  G4cout << "Run " << g4run->GetRunID() << " ends at" <<G4endl;
  //end.Print();
  G4int nEvents = g4run->GetNumberOfEvent();

  TParameter<G4int> nMCEvents(Form("Run%d", g4run->GetRunID()), nEvents);

  //TTimeStamp* start = (TTimeStamp*)fRootFile->Get(Form("StartTime_Run%d", g4run->GetRunID()));
  /*if(start)
    {
    time_t SimTime = end.GetSec() - start->GetSec();
    G4cout << "Took " << SimTime << "seconds to simulate " << nEvents << "Average = " << nEvents*1.0/SimTime << "event/second" << G4endl;
    }
    else 
    G4cout << "Could not compute simulation time" << G4endl;
    */
  fRootFile->cd();
  //gDirectory->WriteObject(&end,Form("EndTime_Run%d",g4run->GetRunID()));

  G4cout << "Saving tree: " << fRootTreeName << " to output ROOT file: " << fRootFileName << G4endl;

  G4String elementName = "C";
  G4String particleName = "pi+";
  const G4Element* el = G4NistManager::Instance()->FindOrBuildElement(elementName);
  const G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
  if(!particle || ! el)
    G4cout << "Particle or element not found!" << G4endl;
  else
  {
    G4int prec = G4cout.precision();
    G4cout.precision(4);
    G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();
    G4double minE = 10*MeV;
    G4double maxE = 2000*MeV;
    G4double nbins = 2000;
    G4double e1 = std::log10(minE/MeV);
    G4double e2 = std::log10(maxE/MeV);
    G4double dE = (e2-e1)/G4double(nbins);
    G4double x = e1 - dE*0.5;
    G4double e, xs, xstot;
    G4int i;
    for(i = 0; i<nbins; ++i)
    {
      x += dE;
      e = std::pow(10., x)*MeV;
      xs = store->GetInelasticCrossSectionPerAtom(particle, e, el);
      //G4cout << e << " " << std::setw(12) << xs/barn << G4endl;
    }
    G4cout.precision(prec);
  }

  //Finish writing ROOT file and close
  fRootTree->Write();
  fRootFile->Close();
  delete fRootFile;
  fRootFile = 0;
  fDetEvent->clear();
  delete fDetEvent;
  fDetEvent = 0;
  fBeamEvent->clear();
  delete fBeamEvent;
  fBeamEvent = 0;
}

void DarkSectorSimAnalysis::PrepareNewEvent(const G4Event* event)
{
  //After beginning of run, when a new event is created 
  ClearVariables();
  fEventNumber = event->GetEventID();
  if(fEventNumber % 10 == 0) {
    G4cout << "Event # "<<fEventNumber<<G4endl;
  }
  fSinglePhoton = new OpPhoton();
  //G4cout << "New Event: " << overallStore.GetScintEnergy() << G4endl;
}

void DarkSectorSimAnalysis::EndOfEvent(const G4Event* /* event */)
{
  //Compute F90 here
  G4double fast = 0.0;
  G4double slow = 0.0;
  //start with earliest, make calculation relative to that, removes effects of decay (still need to factor into global times as for some decays cannot then see effect of F90 on new global timescale
  G4double earliesthit = 100000000000000.0;

  
  for(unsigned int i = 0; i<fPhotonHitTimes.size(); ++i)
  {
    earliesthit = std::min(earliesthit, fPhotonHitTimes[i]);
  }
  for(unsigned int i = 0; i<fPhotonHitTimes.size(); ++i)
  {
    G4double relativeval = fPhotonHitTimes[i]-earliesthit;
    if(relativeval < 90.0)
      ++fast;
    else
      ++slow;
  }
  fF90 = fast/(fast+slow);

  //Photon Storage
  fSinglePhoton->SetID(fEventNumber);
  fSinglePhoton->SetDetectionTime(fPhotonHitTime);
  fSinglePhoton->SetHitPosX(fPMTHitPosX);
  fSinglePhoton->SetHitPosY(fPMTHitPosY);
  fSinglePhoton->SetHitPosZ(fPMTHitPosZ);
  fSinglePhoton->SetHitChannel(fPMTHit);
  fSinglePhoton->SetNumReflections(fReflectTeflon);
  fPhotonStore.push_back(*(fSinglePhoton));
  //G4cout << "Here" << G4endl;
  overallStore.AddPhoton(*(fSinglePhoton));
  //G4cout << "Here" << G4endl;
  //Detector Event Storage
  fDetEvent->SetEventID(fEventNumber);
  fDetEvent->SetGenPosX(fGenXPhoton);
  fDetEvent->SetGenPosY(fGenYPhoton);
  fDetEvent->SetGenPosZ(fGenZPhoton);
  fDetEvent->SetNumChannels(1284);
  fDetEvent->SetF90Value(fF90);
  fDetEvent->SetHitChannel(fPMTHit);
  for(int i = 0; i < fDetEvent->GetNumChannels(); ++i)
  {
    DetectorChannel &chan = *fDetEvent->GetChannel(i);
    chan.SetChannelNum(i);
    if(i == fSinglePhoton->GetHitChannel())
      chan.SetDetectedPhotonsStore(fPhotonStore);
  }
  //fDetEvent->SetStore(overallStore);

  //Beam Event Storage
  fBeamEvent->SetEventID(fEventNumber);
  fBeamEvent->SetProtonGenPosX(fGenXProton);
  fBeamEvent->SetProtonGenPosY(fGenYProton);
  fBeamEvent->SetProtonGenPosZ(fGenZProton);
  fBeamEvent->SetNumPiPlus(fGenPiPlus);
  fBeamEvent->SetPiPlusGenPosX(fGenXPiPlus);
  fBeamEvent->SetPiPlusGenPosY(fGenYPiPlus);
  fBeamEvent->SetPiPlusGenPosZ(fGenZPiPlus);
  fBeamEvent->SetNumPiZero(fGenPiZero);
  fBeamEvent->SetPiZeroGenPosX(fGenXPiZero);
  fBeamEvent->SetPiZeroGenPosY(fGenYPiZero);
  fBeamEvent->SetPiZeroGenPosZ(fGenZPiZero);
  fBeamEvent->SetNumPiMinus(fGenPiMinus);
  fBeamEvent->SetNumDAR(fNumDAR);
  fBeamEvent->SetNumDIF(fNumDIF);
  fBeamEvent->SetNumNuMu(fNum_numu);
  fBeamEvent->SetNuMuGenPosX(fnumu_gen_posx);
  fBeamEvent->SetNuMuGenPosY(fnumu_gen_posy);
  fBeamEvent->SetNuMuGenPosZ(fnumu_gen_posz);
  fBeamEvent->SetNuMuTime(fnumu_time);
  fBeamEvent->SetNuMuEnergy(fnumu_energy);
  fBeamEvent->SetNumNuE(fNum_nue);
  fBeamEvent->SetNuEGenPosX(fnue_gen_posx);
  fBeamEvent->SetNuEGenPosY(fnue_gen_posy);
  fBeamEvent->SetNuEGenPosZ(fnue_gen_posz);
  fBeamEvent->SetNuETime(fnue_time);
  fBeamEvent->SetNuEEnergy(fnue_energy);
  fBeamEvent->SetNumAntiNuMu(fNum_antinumu);
  fBeamEvent->SetAntiNuMuGenPosX(fantinumu_gen_posx);
  fBeamEvent->SetAntiNuMuGenPosY(fantinumu_gen_posy);
  fBeamEvent->SetAntiNuMuGenPosZ(fantinumu_gen_posz);
  fBeamEvent->SetAntiNuMuTime(fantinumu_time);
  fBeamEvent->SetAntiNuMuEnergy(fantinumu_energy);
  fBeamEvent->SetNuMuDAREnergy(fnumu_darE);
  fBeamEvent->SetNuEDAREnergy(fnue_darE);
  fBeamEvent->SetAntiNuMuDAREnergy(fantinumu_darE);
  fBeamEvent->SetNuMuDIFEnergy(fnumu_difE);
  fBeamEvent->SetNuEDIFEnergy(fnue_difE);
  fBeamEvent->SetAntiNuMuDIFEnergy(fantinumu_difE);


  fRootTree->Fill();
}

G4ClassificationOfNewTrack DarkSectorSimAnalysis::ClassifyNewTrack(const G4Track* g4Track)
{
  /*
  if(g4Track->GetDefinition()->GetParticleName() == "pi+" || g4Track->GetDefinition()->GetParticleName() == "pi-" || g4Track->GetDefinition()->GetParticleName() == "pi0" || g4Track->GetDefinition()->GetParticleName() == "mu+" || g4Track->GetDefinition()->GetParticleName() == "mu-" || g4Track->GetDefinition()->GetParticleName() == "nu_mu" || g4Track->GetDefinition()->GetParticleName() == "nu_e" || g4Track->GetDefinition()->GetParticleName() == "anti_nu_mu")
  {
    G4cout << g4Track->GetDefinition()->GetParticleName() << G4endl;
    if(g4Track->GetParentID() != 0)
      G4cout << g4Track->GetParentID() << G4endl;
  }
  */
  if(g4Track->GetParentID() == 0) //primary proton
  {
    fGenXProton = g4Track->GetPosition().getX()/mm;
    fGenYProton = g4Track->GetPosition().getY()/mm;
    fGenZProton = g4Track->GetPosition().getZ()/mm;
  }  
  if(g4Track->GetParentID() == 1 && g4Track->GetDefinition()->GetParticleName() == "pi+")
  {
    ++fGenPiPlus;
    fGenXPiPlus = g4Track->GetPosition().getX()/mm;
    fGenYPiPlus = g4Track->GetPosition().getY()/mm;
    fGenZPiPlus = g4Track->GetPosition().getZ()/mm;
    //G4cout << g4Track->GetCreatorProcess()->GetProcessName() << G4endl;
  }
  if(g4Track->GetParentID() == 1 && g4Track->GetDefinition()->GetParticleName() == "pi0")
  {
    ++fGenPiZero;
    fGenXPiZero = g4Track->GetPosition().getX()/mm;
    fGenYPiZero = g4Track->GetPosition().getY()/mm;
    fGenZPiZero = g4Track->GetPosition().getZ()/mm;
  }
  if(g4Track->GetParentID() == 1 && g4Track->GetDefinition()->GetParticleName() == "pi-")
    ++fGenPiMinus;
  
  if(g4Track->GetParentID() != 1 && g4Track->GetDefinition()->GetParticleName() == "pi+")
  {
    G4cout << "Event #: " << fEventNumber << G4endl;
    G4cout << "Parent ID is: " << g4Track->GetParentID() << G4endl;
    G4cout << g4Track->GetCreatorProcess()->GetProcessName() << G4endl;
  }
  if(g4Track->GetParentID() != 0 && g4Track->GetCreatorProcess()->GetProcessName() == "pi+Inelastic")
  {
    G4cout << "Event #: " << fEventNumber << G4endl;
    G4cout << g4Track->GetDefinition()->GetParticleName() <<  G4endl;
  }
  if(fVoxel)
  {
    fGenXPhoton = g4Track->GetPosition().getX()/mm;
    fGenYPhoton = g4Track->GetPosition().getY()/mm;
    fGenZPhoton = g4Track->GetPosition().getZ()/mm;
  }
  //G4cout << g4Track->GetDefinition()->GetParticleName() <<  G4endl;
  //if(g4Track->GetParentID() != 0)
  //{ 
      //Reset time for rad decay (e.g. Cs137 decay gives 1e10 yrs)
      //if(g4Track->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay" && g4Track->GetGlobalTime() > 1*second)
      //(const_cast<G4Track*>(g4Track))->SetGlobalTime(fInitialtime);
      //if(g4Track->GetCreatorProcess()->GetProcessName() == "nCapture")
      //G4cout << g4Track->GetDefinition()->GetParticleName() << " " << g4Track->GetKineticEnergy()/MeV << G4endl;
  //}
  //if (fEventNumber%1000 == 0) {
  //  G4cout << g4Track->GetDefinition()->GetParticleName() << " " << g4Track->GetKineticEnergy()/MeV << " " << g4Track->GetGlobalTime() << G4endl;
  //}
    //G4cout << g4Track->GetCreatorProcess()->GetProcessName() << G4endl;
    
  //}
  //if(g4Track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() && g4Track->GetKineticEnergy()/eV > 6.0)
  //fTotalLength = 0;
  return fUrgent;
}

void DarkSectorSimAnalysis::SteppingAction(const G4Step* step)
{
  //While particle is being tracked, it takes a step (particle travels until it reaches a volume boundary or interacts)
  G4StepPoint* preStepPoint = step->GetPreStepPoint(); //position of particle pre step
  G4StepPoint* postStepPoint = step->GetPostStepPoint(); //position of particle post step
  G4int collected = 0;
  const G4VPhysicalVolume* preStepVolume = preStepPoint->GetPhysicalVolume(); //volume particle is in pre step
  const G4VPhysicalVolume* postStepVolume = postStepPoint->GetPhysicalVolume(); //volume particle is in post step

  const G4String preVolName = preStepVolume->GetName();
  G4Track* stepTrack = step->GetTrack();
  G4String partname = stepTrack->GetDefinition()->GetParticleName();
  //step->GetTrack()->GetTrackStatus() == fStopAndKill &&
  
  if(step->GetTrack()->GetTrackStatus() == fStopAndKill && partname == "pi+")
    {
      G4cout << "Event Number #: " << fEventNumber << G4endl;
      G4cout << "pi+ Kill Process: " << postStepPoint->GetProcessDefinedStep()->GetProcessName() << G4endl;
      G4cout << "Energy: " << postStepPoint->GetKineticEnergy()/MeV << G4endl;
      G4cout << "Position (x,y,z): " << postStepPoint->GetPosition().getX()/mm << " " << postStepPoint->GetPosition().getY()/mm << " " << postStepPoint->GetPosition().getZ()/mm << G4endl;  
    }
  
  if(postStepVolume)
  {
    G4String postVolName = postStepVolume->GetName();
    G4double edep = step->GetTotalEnergyDeposit()/MeV; //Energy deposit in this step
    if(postStepVolume->GetName() == "LArCylVol")
      if(partname == "nu_mu" || partname == "nu_e" || partname == "anti_nu_mu" || partname == "anti_nu_e")
	++fNuInDetector;
    if(stepTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
      G4int numPMTendcap = 305;
      G4int numPMTside = 610;
      G4int pmtnum = 0;
      //G4cout << "Here" << G4endl;
      G4bool cont = postVolName.contains("topcapPMT");
      G4bool cont2 = postVolName.contains("bottomcapPMT");
      G4bool cont3 = postVolName.contains("sidePMT");
      fTotalLength += step->GetStepLength();
      if(step->GetTrack()->GetKineticEnergy()/eV < 6.0)
      {
	if(cont)
	{
	  const char* numstring = postVolName.remove(0,9).c_str();
	  std::istringstream iss(numstring);
	  iss >> pmtnum;        
	  ++fPMTHitWLS[pmtnum];
	  ++fTotalPMTHitWLS;
	  ++fTotalPMTHit;
	  //fTrackLengthWLS.push_back(fTotalLength);
	  /*
	  if(fSaveFastOpTable)
	  {
	    if(fReflectTeflon < 5)
	      ++fPMTReflEffVec[fReflectTeflon][pmtnum];
	    else
	      ++fPMTReflEffVec[5][pmtnum];
	    //fPMTReflTimeMap[pmtnum].push_back(std::make_pair(fReflectTeflon, postStepPoint->GetGlobalTime()));
	  }
	  */
	  if(fVoxel)
	  {
	    fPMTHitPosX = postStepPoint->GetPosition().getX()/mm;
	    fPMTHitPosY = postStepPoint->GetPosition().getY()/mm;
	    fPMTHitPosZ = postStepPoint->GetPosition().getZ()/mm;
	    fPhotonHitTime = postStepPoint->GetGlobalTime()/ns;
	    fPMTHit = pmtnum;
	  }
	}
	if(cont2)
	{
	  const char* numstring = postVolName.remove(0,12).c_str();
	  std::istringstream iss(numstring);
	  iss >> pmtnum;
	  pmtnum += numPMTendcap;
	  ++fPMTHitWLS[pmtnum];
	  ++fTotalPMTHitWLS;
	  ++fTotalPMTHit;
	  /*
	  if(fSaveFastOpTable)
	    {
	      if(fReflectTeflon < 5)
		++fPMTReflEffVec[fReflectTeflon][pmtnum];
	      else
		++fPMTReflEffVec[5][pmtnum];
	      //fPMTReflTimeMap[pmtnum].push_back(std::make_pair(fReflectTeflon, postStepPoint->GetGlobalTime()));
	    }
	  */
	  //fTrackLengthWLS.push_back(fTotalLength);
	  if(fVoxel)
	  {
	    fPMTHitPosX = postStepPoint->GetPosition().getX()/mm;
	    fPMTHitPosY = postStepPoint->GetPosition().getY()/mm;
	    fPMTHitPosZ = postStepPoint->GetPosition().getZ()/mm;
	    fPhotonHitTime = postStepPoint->GetGlobalTime()/ns;
	    fPMTHit = pmtnum;
	  }
	}
	if(cont3)
	{
	  const char* numstring = postVolName.remove(0,7).c_str();
	  std::istringstream iss(numstring);
	  iss >> pmtnum; 
	  pmtnum += numPMTendcap*2.0;
	  ++fPMTHitWLS[pmtnum];
	  ++fTotalPMTHitWLS;
	  ++fTotalPMTHit;
	  /*
	  if(fSaveFastOpTable)
	    {
	      G4cout << fReflectTeflon << " " << pmtnum << G4endl;
	      if(fReflectTeflon < 5)
		++fPMTReflEffVec[fReflectTeflon][pmtnum];
	      else
		++fPMTReflEffVec[5][pmtnum];
	      //fPMTReflTimeMap[pmtnum].push_back(std::make_pair(fReflectTeflon, postStepPoint->GetGlobalTime()));
	    }
	  */
	  //fTrackLengthWLS.push_back(fTotalLength);
	  if(fVoxel)
	  {
	    fPMTHitPosX = postStepPoint->GetPosition().getX()/mm;
	    fPMTHitPosY = postStepPoint->GetPosition().getY()/mm;
	    fPMTHitPosZ = postStepPoint->GetPosition().getZ()/mm;
	    fPhotonHitTime = postStepPoint->GetGlobalTime()/ns;
	    fPMTHit = pmtnum;
	  }
	}
      } 	
     
      G4OpBoundaryProcessStatus boundaryStatus = Undefined;
      G4OpBoundaryProcess* boundary = NULL;
      if(!boundary) {
	G4ProcessManager* pm = step->GetTrack()->GetDefinition()->GetProcessManager();
	G4int nprocesses = pm->GetProcessListLength();
	G4ProcessVector *pv = pm->GetProcessList();
	G4int i;
	for(i=0;i<nprocesses;i++)
	  if((*pv)[i]->GetProcessName()=="OpBoundary")
	    {
	      boundary = (G4OpBoundaryProcess*)(*pv)[i];
	      break;
	    }
      }
      boundaryStatus=boundary->GetStatus();
      if(postStepPoint->GetStepStatus()==fGeomBoundary)
	switch(boundaryStatus){
	case LambertianReflection:
	  fReflectTeflon++;
	  break;
	default:
	  break;
	}
     
    }
  }
}

void DarkSectorSimAnalysis::PreUserTrackingAction(const G4Track* g4Track,G4TrackingManager* fpTrackingManager)
{
  DarkSectorSimTrajectory* traj = NULL;
  if(!fpTrackingManager->GimmeTrajectory())
  {
    traj = new DarkSectorSimTrajectory(g4Track);
    //fpTrackingManager->SetTrajectory(traj);
    fpTrackingManager->SetStoreTrajectory(true);
    fpTrackingManager->SetTrajectory(traj);
  }
  else
    fpTrackingManager->SetStoreTrajectory(false);
    
}

void DarkSectorSimAnalysis::PostUserTrackingAction(const G4Track* g4Track, G4TrackingManager* fpTrackingManager)
{

  if(fpTrackingManager->GimmeTrajectory() && fpTrackingManager->GetStoreTrajectory())
  {
    DarkSectorSimTrajectory* traj = (DarkSectorSimTrajectory*)fpTrackingManager->GimmeTrajectory();
    traj->SetFinalTrackLength(g4Track->GetTrackLength());
    if(g4Track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() && g4Track->GetKineticEnergy()/eV < 6.0)
    {
      //DarkSectorSimTrajectory* traj = (DarkSectorSimTrajectory*)fpTrackingManager->GimmeTrajectory();
      G4double totTrackLength = g4Track->GetTrackLength() + traj->GetParentTrajectory()->GetFinalTrackLength();
      fTrackLengthWLS.push_back(totTrackLength);
    }
    
    if(g4Track->GetDefinition()->GetParticleName() == "nu_mu")
    {
      G4String parentname = traj->GetParentTrajectory()->GetDefinition()->GetParticleName();
      G4cout << "Event Number: " << fEventNumber << G4endl;
      G4cout << "Muon Neutrino Parent: " << parentname << G4endl;
      G4cout << "Energy: " << g4Track->GetKineticEnergy()/MeV << G4endl;
    } 
    if(g4Track->GetDefinition()->GetParticleName() == "nu_e")
    {
      G4String parentname = traj->GetParentTrajectory()->GetDefinition()->GetParticleName();
      G4cout <<"Electron Neutrino Parent: " << parentname << G4endl;
      G4cout <<"Energy: " << g4Track->GetKineticEnergy()/MeV << G4endl;
    }
    if(g4Track->GetDefinition()->GetParticleName() == "anti_nu_mu")
      {
	G4String parentname = traj->GetParentTrajectory()->GetDefinition()->GetParticleName();
	G4cout <<"Nu-mu-bar Parent: " << parentname << G4endl;
	G4cout <<"Energy: " << g4Track->GetKineticEnergy()/MeV << G4endl;
      }
    



  }

  G4String name = g4Track->GetDefinition()->GetParticleName();
  G4double finalkineticenergy = g4Track->GetKineticEnergy();
  G4double time = g4Track->GetGlobalTime();

  if(g4Track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  {
    fNumReflections.push_back(fReflectTeflon);
  }    
  if(name=="pi0" || name=="pi+" || name=="pi-" || name=="mu+" || name=="mu-") {
  //if(name == "pi+" || name == "mu+") {
    G4TrackVector *children = fpTrackingManager->GimmeSecondaries();
    for(size_t i = 0; i<children->size(); i++) {
      G4Track *child = children->at(i);
      G4String cname = child->GetDefinition()->GetParticleName();
      if(cname == "nu_mu" || cname == "nu_e" ||
	 cname == "anti_nu_e" || cname == "anti_nu_mu") {
	
	const G4VProcess* ccreatorproc = child->GetCreatorProcess();
	G4String cproc;
	if(ccreatorproc!=0)
	  cproc = ccreatorproc->GetProcessName();
	else
	  cproc = G4String("NULL");
	G4double ctime = child->GetGlobalTime();
	G4double dectime = ctime-time;
	if(name == "pi+")
	  fPiDecayTime = dectime;
	if(name == "mu+")
	  fMuDecayTime = dectime;
	G4ThreeVector ccreationpos = child->GetPosition();
	G4ThreeVector ccreationmom = child->GetMomentumDirection();
	G4double ccreationkineticenergy = child->GetTotalEnergy();
	if(cname == "nu_mu")
	  {
	    ++fNum_numu;
	    fnumu_energy = ccreationkineticenergy;
	    fnumu_gen_posx = child->GetPosition().getX()/mm;
	    fnumu_gen_posy = child->GetPosition().getY()/mm;
	    fnumu_gen_posz = child->GetPosition().getZ()/mm;
	    fnumu_time = child->GetGlobalTime()/ns;
	  }
	if(cname == "nu_e")
	  {
	    ++fNum_nue;
	    fnue_energy = ccreationkineticenergy;
	    fnue_gen_posx = child->GetPosition().getX()/mm;
            fnue_gen_posy = child->GetPosition().getY()/mm;
            fnue_gen_posz = child->GetPosition().getZ()/mm;
	    fnue_time = child->GetGlobalTime()/ns;
	  }
	if(cname == "anti_nu_e")
	  {
	    ++fNum_antinue;
	    fantinue_energy = ccreationkineticenergy;
	  }
	if(cname == "anti_nu_mu")
	  {
	    ++fNum_antinumu;
	    fantinumu_energy = ccreationkineticenergy;
            fantinumu_gen_posx = child->GetPosition().getX()/mm;
            fantinumu_gen_posy = child->GetPosition().getY()/mm;
            fantinumu_gen_posz = child->GetPosition().getZ()/mm;
	    fantinumu_time = child->GetGlobalTime()/ns;
	  }
	G4int ctid = child->GetTrackID();
	G4int cpid = child->GetParentID();
	if(cproc == "Decay" || cproc == "Scintillation") {
	  if(finalkineticenergy < 0.1) {
	    cproc = name + "_DAR";
	    fNumDAR = 1;
	    if(cname == "nu_mu")
	      fnumu_darE = ccreationkineticenergy;
	    if(cname == "nu_e")
	      fnue_darE = ccreationkineticenergy;
	    if(cname == "anti_nu_mu")
	      fantinumu_darE = ccreationkineticenergy;
	  } else {
	    fNumDIF = 1;
	    cproc = name + "_DIF";
	    if(cname =="nu_mu")
              fnumu_difE = ccreationkineticenergy;
            if(cname =="nu_e")
              fnue_difE= ccreationkineticenergy;
            if(cname =="anti_nu_mu")
	      fantinumu_difE = ccreationkineticenergy;
	  }
	}
      }
    }
    /*
    if(fGenPiPlus > 0)
      G4cout << fNum_numu << G4endl;
    */
  }
}

void DarkSectorSimAnalysis::ClearVariables(void)
{
  //Reset variables at the end of an event}
  fDetEvent->clear();
  fBeamEvent->clear();
  fGenPiPlus = 0;
  fGenXPiPlus = 0;
  fGenYPiPlus = 0;
  fGenZPiPlus = 0;
  fGenPiZero = 0;
  fGenXPiZero = 0;
  fGenYPiZero = 0;
  fGenZPiZero = 0;
  fGenPiMinus = 0;
  fNuInDetector = 0;
  fPiDecayTime = 0;
  fMuDecayTime = 0;
  fnumu_energy = 0;
  fnue_energy = 0;
  fantinumu_energy = 0;
  fantinue_energy = 0;
  fNum_numu = 0;
  fnumu_gen_posx = 0;
  fnumu_gen_posy = 0;
  fnumu_gen_posz = 0;
  fnumu_time = 0;
  fNum_nue = 0;
  fnue_gen_posx = 0;
  fnue_gen_posy = 0;
  fnue_gen_posz = 0;
  fnue_time = 0;
  fNum_antinumu = 0;
  fantinumu_gen_posx = 0;
  fantinumu_gen_posy = 0;
  fantinumu_gen_posz = 0;
  fantinumu_time = 0;
  fNum_antinue = 0;
  fNumDAR = 0;
  fnumu_darE = 0;
  fnue_darE = 0;
  fantinumu_darE = 0;
  fnumu_difE = 0;
  fnue_difE = 0;
  fantinumu_difE = 0;
  fNumDIF = 0;
  G4int nChans = 610+684;
  fTotalPMTHit = 0;
  fTotalPMTHitWLS = 0;
  fTotalPMTHitVUV = 0;
  fNumReflections.clear();
  fReflectTeflon = 0;
  fTrackLength.clear();
  fTotalLength = 0;
  fPMTHitPosX = 0;
  fPMTHitPosY = 0;
  fPMTHitPosZ = 0;
  fPMTHit = -1;
  fPhotonHitTime = -1;
  fTrackLengthWLS.clear();
  fPMTHitWLS.clear();
  for (int i=0; i<nChans; ++i) {
    fPMTHitWLS.push_back(0.0);
  }
  /*
  for(int j=0; j<6; ++j) {
    std::vector<G4double> container;
    for (int i=0; i<nChans; ++i) {
      container.push_back(0.0);
    }
    fPMTReflEffVec.push_back(container);
  }
  */
  fF90 = 0.0;
  fPhotonHitTimes.clear();
  fPhotonStore.clear();
  //fPMTReflTimeMap.clear();
}

void DarkSectorSimAnalysis::SetBranches(void)
{
  fRootTree->Branch("DetectorEvent", &fDetEvent);
  fRootTree->Branch("BeamEvent", &fBeamEvent);
}
