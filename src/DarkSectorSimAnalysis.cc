#include "DarkSectorSimAnalysis.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrackVector.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4VProcess.hh"
#include "G4TrajectoryContainer.hh"
#include "G4EventManager.hh"
#include "TDirectory.h"
#include "TRandom3.h"
#include "Randomize.hh"
#include "DarkSectorSimTrackingAction.hh"
#include <limits>

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

}

DarkSectorSimAnalysisMessenger::~DarkSectorSimAnalysisMessenger()
{
  delete fDirectory;
  delete fRootFileNameCmd;
  delete fRootTreeNameCmd;
}
void DarkSectorSimAnalysisMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if(cmd == fRootFileNameCmd)
    fAnalysis->SetROOTFileName(val);
  else if(cmd == fRootTreeNameCmd)
    fAnalysis->SetROOTTreeName(val);
}

DarkSectorSimAnalysis* DarkSectorSimAnalysis::fInstance = 0;

DarkSectorSimAnalysis::DarkSectorSimAnalysis()
{
  fRootFile = NULL;
  fRootTree = NULL;
  fRootFileName = "DSout.root";
  fRootTreeName = "DStree";
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
  //Finish writing ROOT file and close
  fRootTree->Write();
  fRootFile->Close();
  delete fRootFile;
  fRootFile = 0;
}

void DarkSectorSimAnalysis::PrepareNewEvent(const G4Event* event)
{
  //After beginning of run, when a new event is created 
  ClearVariables();
  fEventNumber = event->GetEventID();
  if(fEventNumber % 1000 == 0) {
    G4cout << "Event # "<<fEventNumber<<G4endl;
  }
}

void DarkSectorSimAnalysis::EndOfEvent(const G4Event* /* event */)
{
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
  if(postStepVolume)
  {
    G4String postVolName = postStepVolume->GetName();
    G4double edep = step->GetTotalEnergyDeposit()/MeV; //Energy deposit in this step
    if(postStepVolume->GetName() == "LArCylVol")
      if(partname == "nu_mu" || partname == "nu_e" || partname == "anti_nu_mu" || partname == "anti_nu_e")
	++fNuInDetector;
    
    
    //G4OpBoundaryProcessStatus boundaryStatus = Undefined;
    //G4OpBoundaryProcess* boundary = NULL;
    //if(!boundary) {
    //G4ProcessManager* pm = step->GetTrack()->GetDefinition()->GetProcessManager();
    //G4int nprocesses = pm->GetProcessListLength();
    //G4ProcessVector *pv = pm->GetProcessList();
    //G4int i;
    //for(i=0;i<nprocesses;i++)
    //	if((*pv)[i]->GetProcessName()=="OpBoundary")
    //{
    //	  boundary = (G4OpBoundaryProcess*)(*pv)[i];
    //	  break;
    //	}
    //}
    //boundaryStatus=boundary->GetStatus();
    //if(postStepPoint->GetStepStatus()==fGeomBoundary)
    //  switch(boundaryStatus){
    //case LambertianReflection:
    //	break;
    //default:
    //	break;
    //}
	//if(preStepPoint->GetProcessDefinedStep()->GetProcessName() == "LambertianReflection")
	//fReflectTeflon++;
	//
  }
}

void DarkSectorSimAnalysis::PreUserTrackingAction(const G4Track* g4Track,G4TrackingManager* fpTrackingManager)
{;}

void DarkSectorSimAnalysis::PostUserTrackingAction(const G4Track* g4Track, G4TrackingManager* fpTrackingManager)
{
  G4String name = g4Track->GetDefinition()->GetParticleName();
  G4double finalkineticenergy = g4Track->GetKineticEnergy();
  G4double time = g4Track->GetGlobalTime();
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
	  }
	if(cname == "nu_e")
	  {
	    ++fNum_nue;
	    fnue_energy = ccreationkineticenergy;
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
	  }
	G4int ctid = child->GetTrackID();
	G4int cpid = child->GetParentID();
	if(cproc == "Decay") {
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
  }
}

void DarkSectorSimAnalysis::ClearVariables(void)
{
  //Reset variables at the end of an event}
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
  fNum_nue = 0;
  fNum_antinumu = 0;
  fNum_antinue = 0;
  fNumDAR = 0;
  fnumu_darE = 0;
  fnue_darE = 0;
  fantinumu_darE = 0;
  fnumu_difE = 0;
  fnue_difE = 0;
  fantinumu_difE = 0;
  fNumDIF = 0;
  
}

void DarkSectorSimAnalysis::SetBranches(void)
{
  fRootTree->Branch("GenProtonXPos", &fGenXProton);
  fRootTree->Branch("GenProtonYPos", &fGenYProton);
  fRootTree->Branch("GenProtonZPos", &fGenZProton);
  fRootTree->Branch("NumPiPlus", &fGenPiPlus);
  fRootTree->Branch("GenPiPlusXPos", &fGenXPiPlus);
  fRootTree->Branch("GenPiPlusYPos", &fGenYPiPlus);
  fRootTree->Branch("GenPiPlusZPos", &fGenZPiPlus);
  fRootTree->Branch("NumPiZero", &fGenPiZero);
  fRootTree->Branch("GenPiZeroXPos", &fGenXPiZero);
  fRootTree->Branch("GenPiZeroYPos", &fGenYPiZero);
  fRootTree->Branch("GenPiZeroZPos", &fGenZPiZero);
  fRootTree->Branch("NumPiMinus", &fGenPiMinus);
  fRootTree->Branch("NumDAR", &fNumDAR);
  fRootTree->Branch("NumDIF", &fNumDIF);
  fRootTree->Branch("nu_mu_gen", &fNum_numu);
  fRootTree->Branch("nu_e_gen", &fNum_nue);
  fRootTree->Branch("antinu_e_gen", &fNum_antinue);
  fRootTree->Branch("antinu_mu_gen", &fNum_antinumu);
  fRootTree->Branch("nu_mu_energy", &fnumu_energy);
  fRootTree->Branch("nu_e_energy", &fnue_energy);
  fRootTree->Branch("antinu_e_energy", &fantinue_energy);
  fRootTree->Branch("antinu_mu_energy", &fantinumu_energy);
  fRootTree->Branch("nu_mu_darE", &fnumu_darE);
  fRootTree->Branch("nu_e_darE", &fnue_darE);
  fRootTree->Branch("antinu_mu_darE", &fantinumu_darE);
  fRootTree->Branch("nu_mu_difE", &fnumu_difE);
  fRootTree->Branch("nu_e_difE", &fnue_difE);
  fRootTree->Branch("antinumu_difE", &fantinumu_difE);
  fRootTree->Branch("pi_decay", &fPiDecayTime);
  fRootTree->Branch("mu_decay", &fMuDecayTime);
  fRootTree->Branch("nu_in_detector", &fNuInDetector);
}
