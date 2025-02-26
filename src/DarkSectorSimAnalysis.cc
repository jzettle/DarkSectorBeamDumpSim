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
#include "G4IonTable.hh"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <limits>

#include "DetectorChannel.hh"
#include "DetectorEvent.hh"
#include "BeamEvent.hh"
#include "ScintillationStore.hh"

std::ofstream pioninfo_file;
std::ofstream etainfo_file;
std::ofstream ALP_el_infofile;
std::ofstream ALP_gamma_infofile;
std::ofstream piplusminusinfo_file;
std::ofstream kaoninfo_file;
std::ofstream beamtargetinfo_file;
std::ofstream ex_gamma_file;


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
  
  fSaveBeamPionCmd = new G4UIcmdWithABool("/analysis/saveBeamPion", this);
  fSaveBeamPionCmd->SetGuidance("Save beam pion information?");
  fSaveBeamPionCmd->SetDefaultValue(false);
  fSaveBeamPionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSaveALPProdCmd = new G4UIcmdWithABool("/analysis/saveALPProd", this);
  fSaveALPProdCmd->SetGuidance("Save ALP Products (e+,e-, gammas) information?");
  fSaveALPProdCmd->SetDefaultValue(false);
  fSaveALPProdCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fBeamPiZeroFileCmd = new G4UIcmdWithAString("/analysis/setBeamPiZeroFile",this);
  fBeamPiZeroFileCmd->SetGuidance("Set the output beam pi0 text file name");
  fBeamPiZeroFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fBeamPiPlusMinusFileCmd = new G4UIcmdWithAString("/analysis/setBeamPiPlusMinusFile",this);
  fBeamPiPlusMinusFileCmd->SetGuidance("Set the output beam pi+/- text file name");
  fBeamPiPlusMinusFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fBeamEtaFileCmd = new G4UIcmdWithAString("/analysis/setBeamEtaFile",this);
  fBeamEtaFileCmd->SetGuidance("Set the output beam eta text file name");
  fBeamEtaFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fBeamKaonFileCmd = new G4UIcmdWithAString("/analysis/setBeamKaonFile", this);
  fBeamKaonFileCmd->SetGuidance("Set the output beam charged kaon text file name");
  fBeamKaonFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fALPProdElFileCmd = new G4UIcmdWithAString("/analysis/setALPProdElFile", this);
  fALPProdElFileCmd->SetGuidance("Set the output ALP electron product text file name");
  fALPProdElFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fALPProdGammaFileCmd = new G4UIcmdWithAString("/analysis/setALPProdGammaFile", this);
  fALPProdGammaFileCmd->SetGuidance("Set the output ALP gamma product text file name");
  fALPProdGammaFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fBeamTargetFileCmd = new G4UIcmdWithAString("/analysis/setBeamTargetFile", this);
  fBeamTargetFileCmd->SetGuidance("Set the output beam target material product text file name");
  fBeamTargetFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSaveBeamTargetCmd = new G4UIcmdWithABool("/analysis/saveBeamTarget", this);
  fSaveBeamTargetCmd->SetGuidance("Save products from beam target material information?");
  fSaveBeamTargetCmd->SetDefaultValue(false);
  fSaveBeamTargetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSaveStepGammaCmd = new G4UIcmdWithAString("/analysis/setExGammaFile", this);
  fSaveStepGammaCmd->SetGuidance("Set the excited state gamma text file name");
  fSaveStepGammaCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}

DarkSectorSimAnalysisMessenger::~DarkSectorSimAnalysisMessenger()
{
  delete fDirectory;
  delete fRootFileNameCmd;
  delete fRootTreeNameCmd;
  delete fUseVoxelCmd;
  delete fSaveFastOpTableCmd;
  delete fSaveBeamPionCmd;
  delete fBeamPiZeroFileCmd;
  delete fBeamPiPlusMinusFileCmd;
  delete fBeamKaonFileCmd;
  delete fBeamEtaFileCmd;
  delete fSaveALPProdCmd;
  delete fALPProdElFileCmd;
  delete fALPProdGammaFileCmd;
  delete fSaveBeamTargetCmd;
  delete fBeamTargetFileCmd;
  delete fSaveStepGammaCmd;
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
  else if(cmd == fSaveBeamPionCmd)
    fAnalysis->SetSaveBeamPion(val);
  else if(cmd == fBeamPiZeroFileCmd)
    fAnalysis->SetBeamPiZeroFile(val);
  else if(cmd == fBeamPiPlusMinusFileCmd)
    fAnalysis->SetBeamPiPlusMinusFile(val);
  else if(cmd == fBeamKaonFileCmd)
    fAnalysis->SetBeamKaonFile(val);
  else if(cmd == fBeamEtaFileCmd)
    fAnalysis->SetBeamEtaFile(val);
  else if(cmd == fSaveALPProdCmd)
    fAnalysis->SetSaveALPProducts(val);
  else if(cmd == fALPProdElFileCmd)
    fAnalysis->SetALPProductElFile(val);
  else if(cmd == fALPProdGammaFileCmd)
    fAnalysis->SetALPProductGammaFile(val);
  else if(cmd == fSaveBeamTargetCmd)
    fAnalysis->SetSaveBeamTargetProducts(val);
  else if(cmd == fBeamTargetFileCmd)
    fAnalysis->SetBeamTargetProductFile(val);
  else if(cmd == fSaveStepGammaCmd)
    fAnalysis->SetStepGammaFile(val);
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
  fSaveBeamPion = false;
  fBeamPiZeroFile = "fermilab_pizero.txt";
  fBeamPiPlusMinusFile = "fermilab_piplusminus.txt";
  fBeamEtaFile = "fermilab_eta.txt";
  fBeamKaonFile = "fermilab_kaon.txt";
  fSaveBeamTarget = false;
  fBeamTargetFile = "fermilab_beamtarget.txt";
  fExGammaFile = "fermilab_gammaline.txt";
  fSaveALP = false;
  fALPElFile = "fermilab_elprod.txt";
  fALPGammaFile = "fermilab_gammaprod.txt";
  //fStore = new ScintillationStore();
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
  //if(!fStore)
  //fStore = new ScintillationStore();
  if(fSaveBeamPion)
  {
    G4cout << "pi0 going to: " << fBeamPiZeroFile << " ,pi+/- going to: " << fBeamPiPlusMinusFile << " , and eta going to: " << fBeamEtaFile << G4endl;
    pioninfo_file.open(fBeamPiZeroFile);
    piplusminusinfo_file.open(fBeamPiPlusMinusFile);
    etainfo_file.open(fBeamEtaFile);
    kaoninfo_file.open(fBeamKaonFile);
  }
  if(fSaveALP)
  {
    G4cout << fALPElFile << " " << " " << fALPGammaFile << G4endl;
    ALP_el_infofile.open(fALPElFile);
    ALP_gamma_infofile.open(fALPGammaFile);
  }
  if(fSaveBeamTarget)
  {
    G4cout << fBeamTargetFile << G4endl;
    beamtargetinfo_file.open(fBeamTargetFile);
    G4cout << fExGammaFile << G4endl;
    ex_gamma_file.open(fExGammaFile);
  }
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
  if(!particle || !el)
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
}

void DarkSectorSimAnalysis::EndOfEvent(const G4Event* /* event */)
{
  //Compute F90 here
  G4double fast = 0.0;
  G4double slow = 0.0;

  //G4cout << "End Event: " << fStore->GetScintEnergy() << G4endl;
  //start with earliest, make calculation relative to that, removes effects of decay (still need to factor into global times as for some decays cannot then see effect of F90 on new global timescale
  G4double earliesthit = 100000000000000.0;

  
  for(unsigned int i = 0; i<fPhotonHitTimes.size(); ++i)
  {
    earliesthit = std::min(earliesthit, fPhotonHitTimes[i]);
  }
  //G4cout << earliesthit << G4endl;
  for(unsigned int i = 0; i<fPhotonHitTimes.size(); ++i)
  {
    G4double relativeval = fPhotonHitTimes[i]-earliesthit;
    //G4cout << fPhotonHitTimes[i] << " " << relativeval << G4endl;
    if(relativeval > 0.0 && relativeval < 90.0)
      ++fast;
    else
      ++slow;
  }
  fF90 = fast/(fast+slow);
  //G4cout << fF90 << G4endl;
  //Photon Storage
  //fSinglePhoton->SetID(fEventNumber);
  //fSinglePhoton->SetDetectionTime(fPhotonHitTime);
  //fSinglePhoton->SetHitPosX(fPMTHitPosX);
  //fSinglePhoton->SetHitPosY(fPMTHitPosY);
  //fSinglePhoton->SetHitPosZ(fPMTHitPosZ);
  //fSinglePhoton->SetHitChannel(fPMTHit);
  //fSinglePhoton->SetNumReflections(fReflectTeflon);
  //fPhotonStore.push_back(*(fSinglePhoton));
  //fStore->AddPhoton(*(fSinglePhoton));
  //Detector Event Storage
  fDetEvent->SetEventID(fEventNumber);
  fDetEvent->SetGenPosX(fGenXProton);
  fDetEvent->SetGenPosY(fGenYProton);
  fDetEvent->SetGenPosZ(fGenZProton);
  fDetEvent->SetNumChannels(1284);
  fDetEvent->SetF90Value(fF90);
  fDetEvent->SetHitChannel(fPMTHit);
  fDetEvent->SetEventEnergy(fParentEnergy);
  fDetEvent->SetTotalPhotons(fTotalPMTHit);
  /*
  for(int i = 0; i < fDetEvent->GetNumChannels(); ++i)
  {
    DetectorChannel &chan = *fDetEvent->GetChannel(i);
    chan.SetChannelNum(i);
    if(i == fSinglePhoton->GetHitChannel())
      chan.SetDetectedPhotonsStore(fPhotonStore);
  }
  */
  //fDetEvent->SetStore(fStore);
  //Beam Event Storage
  fBeamEvent->SetEventID(fEventNumber);
  fBeamEvent->SetProtonGenPosX(fGenXProton);
  fBeamEvent->SetProtonGenPosY(fGenYProton);
  fBeamEvent->SetProtonGenPosZ(fGenZProton);
  fBeamEvent->SetProtonStopPosX(fStopXProton);
  fBeamEvent->SetProtonStopPosY(fStopYProton);
  fBeamEvent->SetProtonStopPosZ(fStopZProton);
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
 
  if(fSaveBeamPion && fGenPiZero > 0)
  {
    pioninfo_file << fpizero_xmom << " " << fpizero_ymom << " " << fpizero_zmom << " " << fpizero_E << " " << fGenXPiZero << " " << fGenYPiZero << " " << fGenZPiZero << " " << fpizero_time << "\n";
  }

  if(fSaveBeamPion && fGenPiPlus > 0)
  {
    piplusminusinfo_file << fpiplus_name <<  " " <<  fpiplus_xmom << " " << fpiplus_ymom << " " << fpiplus_zmom << " " << fpiplus_E << " " << fGenXPiPlus << " " << fGenYPiPlus << " " << fGenZPiPlus << " " << fpiplus_time << G4endl;
  }
  
  if(fSaveBeamPion && fGenPiMinus > 0)
  {
    piplusminusinfo_file << fpiminus_name << " " <<  fpiminus_xmom << " " << fpiminus_ymom << " " << fpiminus_zmom << " " << fpiminus_E << " " << fGenXPiMinus << " " << fGenYPiMinus << " " << fGenZPiMinus << " " << fpiminus_time << G4endl;
  }

  if(fSaveBeamPion && fGenKaon > 0)
  {
    kaoninfo_file << fkaon_name << " " <<  fkaon_xmom << " " << fkaon_ymom << " " << fkaon_zmom << " " << fkaon_E << " " << fGenXKaon << " " << fGenYKaon << " " << fGenZKaon << " " << fkaon_time << G4endl;
  }
 
  if(fSaveBeamPion && fGenEta > 0)
  {
    etainfo_file << feta_xmom << " " << feta_ymom << " " << feta_zmom << " " << feta_E << " " << fGenXEta << " " << fGenYEta << " " << fGenZEta << " " << feta_time << "\n";
  }

  if(fSaveALP && fGenElectron > 0)
    for(unsigned int i = 0; i < fGenElEnergy.size(); ++i)
      if(fGenElEnergy[i] > 0.1)
	ALP_el_infofile << fGenElName[i] << " " << fGenElEnergy[i] << " " << fGenElPx[i] << " " << fGenElPy[i] << " " << fGenElPz[i] << "\n";
  if(fSaveALP && fGenGamma > 0)
    for(unsigned int i = 0; i < fGenGammaEnergy.size(); ++i)
      if(fGenGammaEnergy[i] > 0.1)
	ALP_gamma_infofile << fGenGammaEnergy[i] << " " << fGenGammaPx[i] << " " << fGenGammaPy[i] << " " << fGenGammaPz[i] << "\n";

  if(fSaveBeamTarget && fGenGamma > 0)
    for(unsigned int i = 0; i < fGenGammaEnergy.size(); ++i)
      if(fGenGammaEnergy[i] > 0.02)
	beamtargetinfo_file << fGenGammaEnergy[i] << " " << fGenGammaPx[i] << " " << fGenGammaPy[i] << " " << fGenGammaPz[i] << " " << fGenGammaCreatorProc[i] << " " << fbeamprod_parentname[i] << " " << fGenGammaParentofParent[i] << "\n";

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
    fParentEnergy = g4Track->GetKineticEnergy()/MeV;
    //G4cout << g4Track->GetDefinition()->GetParticleName() << " " << g4Track->GetKineticEnergy()/keV << " " << fGenXProton << " " << fGenYProton << " " << fGenZProton << G4endl;
  }  
  if(g4Track->GetParentID() == 1 && g4Track->GetDefinition()->GetParticleName() == "pi+")
  {
    ++fGenPiPlus;
    fGenXPiPlus = g4Track->GetPosition().getX()/m;
    fGenYPiPlus = g4Track->GetPosition().getY()/m;
    fGenZPiPlus = g4Track->GetPosition().getZ()/m;
    fpiplus_name = g4Track->GetDefinition()->GetParticleName();
    fpiplus_E = g4Track->GetTotalEnergy()/GeV;
    fpiplus_xmom = g4Track->GetMomentum().getX()/GeV;
    fpiplus_ymom = g4Track->GetMomentum().getY()/GeV;
    fpiplus_zmom = g4Track->GetMomentum().getZ()/GeV;
    fpiplus_time = g4Track->GetGlobalTime()/s;
  }
  if(g4Track->GetParentID() == 1 && g4Track->GetDefinition()->GetParticleName() == "pi-")
  {
    ++fGenPiMinus;
    fGenXPiMinus = g4Track->GetPosition().getX()/m;
    fGenYPiMinus = g4Track->GetPosition().getY()/m;
    fGenZPiMinus = g4Track->GetPosition().getZ()/m;
    fpiminus_name = g4Track->GetDefinition()->GetParticleName();
    fpiminus_E = g4Track->GetTotalEnergy()/GeV;
    fpiminus_xmom = g4Track->GetMomentum().getX()/GeV;
    fpiminus_ymom = g4Track->GetMomentum().getY()/GeV;
    fpiminus_zmom = g4Track->GetMomentum().getZ()/GeV;
    fpiminus_time = g4Track->GetGlobalTime()/s;
  }
  if(g4Track->GetParentID() == 1 && g4Track->GetDefinition()->GetParticleName() == "pi0")
  {
    ++fGenPiZero;
    fGenXPiZero = g4Track->GetPosition().getX()/m;
    fGenYPiZero = g4Track->GetPosition().getY()/m;
    fGenZPiZero = g4Track->GetPosition().getZ()/m;
    fpizero_E = g4Track->GetTotalEnergy()/GeV;
    fpizero_xmom = g4Track->GetMomentum().getX()/GeV;
    fpizero_ymom = g4Track->GetMomentum().getY()/GeV;
    fpizero_zmom = g4Track->GetMomentum().getZ()/GeV;
    fpizero_time = g4Track->GetGlobalTime()/s;
  }
  if(g4Track->GetParentID() == 1 && (g4Track->GetDefinition()->GetParticleName() == "kaon+" || g4Track->GetDefinition()->GetParticleName() == "kaon-") )
  {
    ++fGenKaon;
    fGenXKaon = g4Track->GetPosition().getX()/m;
    fGenYKaon = g4Track->GetPosition().getY()/m;
    fGenZKaon = g4Track->GetPosition().getZ()/m;
    fkaon_name = g4Track->GetDefinition()->GetParticleName();
    fkaon_E = g4Track->GetTotalEnergy()/GeV;
    fkaon_xmom = g4Track->GetMomentum().getX()/GeV;
    fkaon_ymom = g4Track->GetMomentum().getY()/GeV;
    fkaon_zmom = g4Track->GetMomentum().getZ()/GeV;
    fkaon_time = g4Track->GetGlobalTime()/s;
  }

  if(g4Track->GetParentID() == 1 && (g4Track->GetDefinition()->GetParticleName() == "eta" || g4Track->GetDefinition()->GetParticleName() == "eta_prime") )
  {
    ++fGenEta;
    fGenXEta = g4Track->GetPosition().getX()/m;
    fGenYEta = g4Track->GetPosition().getY()/m;
    fGenZEta = g4Track->GetPosition().getZ()/m;
    feta_E = g4Track->GetTotalEnergy()/GeV;
    feta_xmom = g4Track->GetMomentum().getX()/GeV;
    feta_ymom = g4Track->GetMomentum().getY()/GeV;
    feta_zmom = g4Track->GetMomentum().getZ()/GeV;
    feta_time = g4Track->GetGlobalTime()/s;
  }
  if(g4Track->GetParentID() == 1 && g4Track->GetDefinition()->GetParticleName() == "pi-")
    ++fGenPiMinus;

  if(g4Track->GetDefinition()->GetParticleName() == "e-" || g4Track->GetDefinition()->GetParticleName() == "e+")
  {
    //G4cout << "Electron!" << G4endl;
    ++fGenElectron;
    fGenElEnergy.push_back(g4Track->GetKineticEnergy()/MeV);
    fGenElPx.push_back(g4Track->GetMomentum().getX()/MeV);
    fGenElPy.push_back(g4Track->GetMomentum().getY()/MeV);
    fGenElPz.push_back(g4Track->GetMomentum().getZ()/MeV);
    fGenElName.push_back(g4Track->GetDefinition()->GetParticleName());
  }
  if(g4Track->GetDefinition()->GetParticleName() == "gamma")
  {
    //G4cout << "Gamma!" << G4endl;
    ++fGenGamma;
    fGenGammaEnergy.push_back(g4Track->GetKineticEnergy()/MeV);
    fGenGammaPx.push_back(g4Track->GetMomentum().getX()/MeV);
    fGenGammaPy.push_back(g4Track->GetMomentum().getY()/MeV);
    fGenGammaPz.push_back(g4Track->GetMomentum().getZ()/MeV);
    if(g4Track->GetCreatorProcess())
    {
      fGenGammaCreatorProc.push_back(g4Track->GetCreatorProcess()->GetProcessName());
    }
  }
 
  
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
  if(g4Track->GetParentID() != 0)
    {
      //Reset time for rad decay (e.g. Cs137 decay gives 1e10 yrs) 
      if(g4Track->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay" && g4Track->GetGlobalTime() > 1*second)
        (const_cast<G4Track*>(g4Track))->SetGlobalTime(fInitialtime);
    }
  if(fVoxel)
  {
    fGenXPhoton = g4Track->GetPosition().getX()/mm;
    fGenYPhoton = g4Track->GetPosition().getY()/mm;
    fGenZPhoton = g4Track->GetPosition().getZ()/mm;
  }
  //if(g4Track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition() && g4Track->GetParentID() == 1)
  //G4cout << "New Particle: " << g4Track->GetDefinition()->GetParticleName() << "!" << G4endl;
  if(g4Track->GetDefinition() == G4Electron::ElectronDefinition() && g4Track->GetParentID() == 1)
    if(g4Track->GetKineticEnergy()/MeV > fParentEnergy)
      fParentEnergy = g4Track->GetKineticEnergy()/MeV;
  fReflectTeflon = 0;
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

    //add in a fStopAndKill condition for protons here and then store that value to check where the protons themselves stop
    //look only at the primary proton here
    if(step->GetTrack()->GetParentID() == 0 && step->GetTrack()->GetTrackStatus() == fStopAndKill)
    {
      fStopXProton = postStepPoint->GetPosition().getX()/m; 
      fStopYProton = postStepPoint->GetPosition().getY()/m; 
      fStopZProton = postStepPoint->GetPosition().getZ()/m;
    }

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
	  //if(fVoxel)
	  //{
	  fPMTHitPosX = postStepPoint->GetPosition().getX()/mm;
	  fPMTHitPosY = postStepPoint->GetPosition().getY()/mm;
	  fPMTHitPosZ = postStepPoint->GetPosition().getZ()/mm;
	  fPhotonHitTime = postStepPoint->GetGlobalTime()/ns;
	  fPhotonHitTimes.push_back(postStepPoint->GetGlobalTime()/ns);
	  fPMTHit = pmtnum;
	  //}
	  fSinglePhoton = new OpPhoton();
	  fSinglePhoton->SetID(fEventNumber);
	  fSinglePhoton->SetDetectionTime(fPhotonHitTime);
	  fSinglePhoton->SetHitPosX(fPMTHitPosX);
	  fSinglePhoton->SetHitPosY(fPMTHitPosY);
	  fSinglePhoton->SetHitPosZ(fPMTHitPosZ);
	  fSinglePhoton->SetHitChannel(fPMTHit);
	  fSinglePhoton->SetNumReflections(fReflectTeflon);
	  DetectorChannel &chan = *fDetEvent->GetChannel(pmtnum);
	  chan.AddSinglePhoton(*(fSinglePhoton));
	  //fPhotonStore.push_back(*(fSinglePhoton));
	  
	}
	if(cont2)
	{
	  const char* numstring = postVolName.remove(0,12).c_str();
	  std::istringstream iss(numstring);
	  iss >> pmtnum;
	  //pmtnum += numPMTendcap/2.0;
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
	  //if(fVoxel)
	  //{
	    fPMTHitPosX = postStepPoint->GetPosition().getX()/mm;
	    fPMTHitPosY = postStepPoint->GetPosition().getY()/mm;
	    fPMTHitPosZ = postStepPoint->GetPosition().getZ()/mm;
	    fPhotonHitTime = postStepPoint->GetGlobalTime()/ns;
	    fPhotonHitTimes.push_back(postStepPoint->GetGlobalTime()/ns);
	    fPMTHit = pmtnum;
	    fSinglePhoton = new OpPhoton();
	    fSinglePhoton->SetID(fEventNumber);
	    fSinglePhoton->SetDetectionTime(fPhotonHitTime);
	    fSinglePhoton->SetHitPosX(fPMTHitPosX);
	    fSinglePhoton->SetHitPosY(fPMTHitPosY);
	    fSinglePhoton->SetHitPosZ(fPMTHitPosZ);
	    fSinglePhoton->SetHitChannel(fPMTHit);
	    fSinglePhoton->SetNumReflections(fReflectTeflon);
	    DetectorChannel &chan = *fDetEvent->GetChannel(pmtnum);
	    chan.AddSinglePhoton(*(fSinglePhoton));
	    //fPhotonStore.push_back(*(fSinglePhoton));
	    //}
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
	  //if(fVoxel)
	  //{
	  fPMTHitPosX = postStepPoint->GetPosition().getX()/mm;
	  fPMTHitPosY = postStepPoint->GetPosition().getY()/mm;
	  fPMTHitPosZ = postStepPoint->GetPosition().getZ()/mm;
	  fPhotonHitTime = postStepPoint->GetGlobalTime()/ns;
	  fPhotonHitTimes.push_back(postStepPoint->GetGlobalTime()/ns);
	  fPMTHit = pmtnum;
	  //}
	  fSinglePhoton = new OpPhoton();
	  fSinglePhoton->SetID(fEventNumber);
	  fSinglePhoton->SetDetectionTime(fPhotonHitTime);
	  fSinglePhoton->SetHitPosX(fPMTHitPosX);
	  fSinglePhoton->SetHitPosY(fPMTHitPosY);
	  fSinglePhoton->SetHitPosZ(fPMTHitPosZ);
	  fSinglePhoton->SetHitChannel(fPMTHit);
	  fSinglePhoton->SetNumReflections(fReflectTeflon);
	  DetectorChannel &chan = *fDetEvent->GetChannel(pmtnum);
          chan.AddSinglePhoton(*(fSinglePhoton));
	  //fPhotonStore.push_back(*(fSinglePhoton));
	    
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
  //alternative method to extract secondary gammas similar to method used for CCM explorations on excited state gammas coming from the target. We can compare to what I already have produced and plot the different gamma lines

  bool ph = false;
  const G4VProcess* process = step->GetPostStepPoint()->GetProcessDefinedStep();
  G4String processName = "UserLimit";
  if (process) processName = process->GetProcessName();
  {
    const std::vector<const G4Track*>* secondary = step->GetSecondaryInCurrentStep();
    size_t nbtrk = (*secondary).size();
    if(nbtrk)
    {
      for(size_t sec = 0; sec < (*secondary).size(); ++sec)
      {
	if(((*secondary)[sec]->GetDefinition()->GetParticleName() == "gamma"))
	{
	  ph = true;
	  ex_gamma_file << (*secondary)[sec]->GetKineticEnergy() << " ";
	  ex_gamma_file << step->GetTrack()->GetPosition().x() << " ";
	  ex_gamma_file << step->GetTrack()->GetPosition().y() << " ";
	  ex_gamma_file << step->GetTrack()->GetPosition().z() << " ";
	  ex_gamma_file << process->GetProcessName() << " ";
	  ex_gamma_file << step->GetTrack()->GetVolume()->GetName() << " ";
	  ex_gamma_file << step->GetTrack()->GetDefinition()->GetParticleName() << " ";
	  ex_gamma_file << step->GetTrack()->GetKineticEnergy() << " ";
	
	  if(!ph)
	    ex_gamma_file << std::endl;
	  if(ph)
	  {
	    ph = false;
	    for(size_t seco = 0; seco < (*secondary).size(); ++seco)
	      ex_gamma_file << " " << (*secondary)[seco]->GetDefinition()->GetParticleName();
	    ex_gamma_file << std::endl;
	  }
	}
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

  if(g4Track->GetParentID() != 0)
  {
    if(g4Track->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay" && g4Track->GetGlobalTime() > 1.0*second)
    {
      fInitialtime = traj->GetParentTrajectory()->GetFinalTime();
      G4EventManager::GetEventManager()->GetStackManager()->ReClassify();
    }
  }

    
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
    
    if(g4Track->GetDefinition()->GetParticleName() == "gamma")
    {
      if(g4Track->GetParentID() != 0)
      {
	G4String parentname = traj->GetParentTrajectory()->GetDefinition()->GetParticleName();
	if(traj->GetParentTrajectory()->GetParentID() != 0)
	{
	  G4String parentofparent = traj->GetParentTrajectory()->GetParentTrajectory()->GetDefinition()->GetParticleName();
	  fGenGammaParentofParent.push_back(parentofparent);
	}
	else
	{
	  G4String parentofparent = "primary_proton";
	  fGenGammaParentofParent.push_back(parentofparent);
	}
	fbeamprod_parentname.push_back(parentname);                                      
	/*
	if(traj->GetParentTrajectory()->GetDefinition()->GetParticleType() == "nucleus")
	{
	  ++fGenGamma;
	  fGenGammaEnergy.push_back(g4Track->GetKineticEnergy()/MeV);
	  fGenGammaPx.push_back(g4Track->GetMomentum().getX()/MeV);
	  fGenGammaPy.push_back(g4Track->GetMomentum().getY()/MeV);
	  fGenGammaPz.push_back(g4Track->GetMomentum().getZ()/MeV);
	  G4String parentname = traj->GetParentTrajectory()->GetDefinition()->GetParticleName();
	  fbeamprod_parentname.push_back(parentname);
	  G4cout << g4Track->GetDefinition()->GetParticleName() << " " << parentname << G4endl;
	}
	*/
      }
    }
    /*
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
    */



  }

  G4String name = g4Track->GetDefinition()->GetParticleName();
  G4double finalkineticenergy = g4Track->GetKineticEnergy();
  G4double time = g4Track->GetGlobalTime();

  if(g4Track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  {
    fNumReflections.push_back(fReflectTeflon);
  }    
  if((name=="pi+" || name=="pi-" || name=="mu+" || name=="mu-" || name=="kaon+" || name=="kaon-")) { //&& g4Track->GetParentID() == 1) {
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
  //fStore->clear();
  fGenPiPlus = 0;
  fGenXPiPlus = 0;
  fGenYPiPlus = 0;
  fGenZPiPlus = 0;
  fGenPiZero = 0;
  fGenXPiZero = 0;
  fGenYPiZero = 0;
  fGenZPiZero = 0;
  fStopXProton = 0;
  fStopYProton = 0;
  fStopZProton = 0;
  fGenPiMinus = 0;
  fGenXPiMinus = 0;
  fGenYPiMinus = 0;
  fGenZPiMinus = 0;
  fGenKaon = 0;
  fGenXKaon = 0;
  fGenYKaon = 0;
  fGenZKaon = 0;
  fGenEta = 0;
  fGenXEta = 0;
  fGenYEta = 0;
  fGenZEta = 0;
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
  fInitialtime = 0;
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
  fpiplus_name = "";
  fpiplus_xmom = 0;
  fpiplus_ymom = 0;
  fpiplus_zmom = 0;
  fpiplus_E = 0;
  fpiplus_time = 0;

  fpizero_xmom = 0;
  fpizero_ymom = 0;
  fpizero_zmom = 0;
  fpizero_E = 0;
  fpizero_time = 0;
  
  fpiplus_name = "";
  fpiminus_xmom = 0;
  fpiminus_ymom = 0;
  fpiminus_zmom = 0;
  fpiminus_E = 0;
  fpiminus_time = 0;

  fkaon_name = "";
  fkaon_xmom = 0;
  fkaon_ymom = 0;
  fkaon_zmom = 0;
  fkaon_E = 0;
  fkaon_time = 0;

  feta_xmom = 0;
  feta_ymom = 0;
  feta_zmom = 0;
  feta_E = 0;
  feta_time = 0;
  
  fGenElectron = 0;
  fGenElEnergy.clear();
  fGenElPx.clear();
  fGenElPy.clear();
  fGenElPz.clear();
  fGenElName.clear();

  fbeamprod_parentname.clear();
  
  fGenGamma = 0;
  fGenGammaEnergy.clear();
  fGenGammaPx.clear();
  fGenGammaPy.clear();
  fGenGammaPz.clear();
  fGenGammaCreatorProc.clear();
  fGenGammaParentofParent.clear();
}

void DarkSectorSimAnalysis::SetBranches(void)
{
  fRootTree->Branch("DetectorEvent", &fDetEvent);
  fRootTree->Branch("BeamEvent", &fBeamEvent);
}
