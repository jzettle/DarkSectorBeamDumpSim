#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4GenericPhysicsList.hh"
#include "G4OpticalPhysics.hh"
#include "G4Neutron.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Gamma.hh"
#include "G4MuonPlus.hh"
#include "G4GenericIon.hh"
#include "G4HadronElasticProcess.hh"
#include "G4HadronElastic.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4RadioactiveDecay.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "G4PAIModel.hh"
#include "G4PAIPhotModel.hh"
#include "G4BraggIonGasModel.hh"
#include "G4BetheBlochIonGasModel.hh"
#include "G4IonFluctuations.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"
#include "G4UniversalFluctuation.hh"
#include "G4EmConfigurator.hh"
#include "G4EmProcessOptions.hh"
#include "G4PhysicsListHelper.hh"
#include "G4PionDecayMakeSpin.hh"
#include "G4DecayWithSpin.hh"
#include "G4DecayTable.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4EmSaturation.hh"
#include "G4ParticleDefinition.hh"
#include "G4EmLivermorePhysics.hh"

#include "DarkSectorSimPhysicsList.hh"
//#include "DarkSectorSimPhysicsListMessenger.hh"

DarkSectorSimPhysicsList::DarkSectorSimPhysicsList(G4String name): G4VModularPhysicsList()
{
  //fPhysicsListMessenger = new DarkSectorSimPhysicsListMessenger(this);
  G4LossTableManager::Instance();
  SetDefaultCutValue(1.*mm);
  
  G4VModularPhysicsList* physList = NULL; //initialize list
  std::vector<G4String>* construct = new std::vector<G4String>; //Create vector to hold options

  //electromagnetic Physics
  //Set as UI option for EM physics?
  construct->push_back("G4EmLivermorePhysics");
  G4EmProcessOptions EMOpt;
  EMOpt.SetFluo(true); //deexcitation and flourescence

  construct->push_back("G4EmExtraPhysics"); //gamma-nuc and muon-nuc
  construct->push_back("G4DecayPhysics");  //decays (for cal source simulations)
  construct->push_back("G4HadronElasticPhysicsHP"); //self-explanatory
  construct->push_back("G4HadronPhysicsQGSP_BERT_HP"); 
  construct->push_back("G4StoppingPhysics");
  construct->push_back("G4IonPhysics");
  //construct->push_back("G4NeutronTrackingCut");

  physList = new G4GenericPhysicsList(construct);

  //Load Default Physics  
  for(G4int i = 0; ;i++)
  {
    G4VPhysicsConstructor* part = const_cast<G4VPhysicsConstructor*> (physList->GetPhysics(i));
    if(part == NULL) 
      break;
    G4cout << "Physics Added: " << part->GetPhysicsName() << G4endl;
    RegisterPhysics(part);
  }
  /*
  G4OpticalPhysics* optPhys = new G4OpticalPhysics();
  optPhys->SetScintillationYieldFactor(1.0);                
  optPhys->SetScintillationExcitationRatio(0.5);              
  optPhys->SetTrackSecondariesFirst(kScintillation, true);

  RegisterPhysics(optPhys);
  */
}

DarkSectorSimPhysicsList::~DarkSectorSimPhysicsList(){
  //delete fPhysicsListMessenger;
}

void DarkSectorSimPhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
  G4OpticalPhoton::OpticalPhotonDefinition();
  G4Neutron::NeutronDefinition();
  G4Electron::ElectronDefinition();
  G4Proton::ProtonDefinition();
  G4Gamma::GammaDefinition();
  G4GenericIon::GenericIonDefinition();
  G4MuonPlus::MuonPlusDefinition();
}

void DarkSectorSimPhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();

  //Radioactive Decay

  G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();

  radioactiveDecay->SetICM(true);                //Internal Conversion       
  radioactiveDecay->SetARM(false);               //Atomic Rearrangement       
  //radioactiveDecay->SetVerboseLevel(1);
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIonDefinition());

  //Use low energy neutron (more precise) cross sections instead of standard neutron cross sections
  G4HadronElasticProcess *theNeutronElasticProcess = new G4HadronElasticProcess();
  G4HadronElastic *theHElasticModel = new G4HadronElastic();
  theHElasticModel->SetMinEnergy(19*MeV);
  G4NeutronHPElastic *theLElasticModel = new G4NeutronHPElastic();
  theLElasticModel->SetMaxEnergy(19*MeV);
  theNeutronElasticProcess->RegisterMe(theHElasticModel);
  theNeutronElasticProcess->RegisterMe(theLElasticModel);
  theNeutronElasticProcess->AddDataSet(new G4NeutronHPElasticData());
  
  //Add Optical Processes
  //different scintillation yields for electron/neutron
  theScintillationProcessElectron = new G4Scintillation("Scintillation");
  theScintillationProcessElectron->SetScintillationYieldFactor(1.0);
  theScintillationProcessElectron->SetScintillationExcitationRatio(0.3);
  theScintillationProcessNeutron = new G4Scintillation("Scintillation");
  theScintillationProcessNeutron->SetScintillationYieldFactor(1.0);
  theScintillationProcessNeutron->SetScintillationExcitationRatio(0.7);
  //Other optical processes: Cherenkov, Absorption, Rayleigh Scattering, wavelength shifting
  theCerenkovProcess = new G4Cerenkov();
  theAbsorptionProcess = new G4OpAbsorption();
  theRayleighScatteringProcess = new G4OpRayleigh();
  theBoundaryProcess = new G4OpBoundaryProcess();
  theWLSProcess = new G4OpWLS();
  theWLSProcess->UseTimeProfile("exponential");
  theBoundaryProcess->SetVerboseLevel(0);
  //theWLSProcess->SetVerboseLevel(1);
  //Have Geant4 track the secondary optical photons as soon as they are created
  theScintillationProcessElectron->SetTrackSecondariesFirst(true);
  theScintillationProcessNeutron->SetTrackSecondariesFirst(true);
  
  //theScintillationProcess->SetVerboseLevel(1);
  theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
  theCerenkovProcess->SetTrackSecondariesFirst(true);
  
  G4EmSaturation* theSaturation = new G4EmSaturation(1);
  theSaturation->DumpBirksCoefficients();
  theSaturation->DumpG4BirksCoefficients();
  theScintillationProcessElectron->AddSaturation(theSaturation);

  G4ParticleDefinition *muplus = G4MuonPlus::MuonPlusDefinition();
  G4ProcessManager *manager = muplus->GetProcessManager();
  manager->AddProcess(new G4MuMultipleScattering,-1, 1, 1);
  manager->AddProcess(new G4MuIonisation,       -1, 2, 2);
  manager->AddProcess(new G4MuBremsstrahlung,   -1, 3, 3);
  manager->AddProcess(new G4MuPairProduction,   -1, 4, 4);

  //G4OpticalSurfaceModel Model = unified;
  //theBoundaryProcess->SetModel(Model);
  //auto particleIterator = GetParticleIterator();

  GetParticleIterator()->reset();
  //Add scintillation and cherenkov processes to particles
  while((*GetParticleIterator())()) 
  {
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* manager = particle->GetProcessManager();
    G4String partName = particle->GetParticleName();
    
    if(theCerenkovProcess->IsApplicable(*particle))
    {
      //manager->AddProcess(theCerenkovProcess);
      //manager->SetProcessOrdering(theCerenkovProcess, idxPostStep);
    }
    
    if(partName == "neutron")
    {
      //manager->AddDiscreteProcess(theNeutronElasticProcess);
      //if(fScint)
      manager->AddProcess(theScintillationProcessNeutron);
      manager->SetProcessOrderingToLast(theScintillationProcessNeutron, idxPostStep);
      manager->SetProcessOrderingToLast(theScintillationProcessNeutron, idxAtRest);

    }
    
    else if(partName == "e-")
    {
      //if(fScint)
      manager->AddProcess(theScintillationProcessElectron);
      manager->SetProcessOrderingToLast(theScintillationProcessElectron, idxPostStep);
      manager->SetProcessOrderingToLast(theScintillationProcessElectron, idxAtRest);
      //G4EmSaturation* emSaturation = theScintillationProcess->GetSaturation();
      //G4cout << "Saturation" << G4endl;
    }
    
    else if(partName == "GenericIon" || particle->GetParticleType() == "nucleus")
    {
      
      ph->RegisterProcess(radioactiveDecay, particle);
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 1*um);
      //ph->RegisterProcess(ionIoni, particle);
      G4NuclearStopping* ionnuc = new G4NuclearStopping();
      //ph->RegisterProcess(ionnuc, particle);
      //if(fScint)
      manager->AddProcess(theScintillationProcessNeutron);
      manager->SetProcessOrderingToLast(theScintillationProcessNeutron, idxPostStep);
      manager->SetProcessOrderingToLast(theScintillationProcessNeutron, idxAtRest);
    }
    
    else if(partName == "alpha" || partName == "proton")
    {
      //if(fScint)
      manager->AddProcess(theScintillationProcessNeutron);
      manager->SetProcessOrderingToLast(theScintillationProcessNeutron, idxPostStep);
      manager->SetProcessOrderingToLast(theScintillationProcessNeutron, idxAtRest);

    }
      
    else if(partName == "opticalphoton")
    {
      manager->AddDiscreteProcess(theAbsorptionProcess);
      manager->AddDiscreteProcess(theRayleighScatteringProcess);
      manager->AddDiscreteProcess(theBoundaryProcess);
      manager->AddDiscreteProcess(theWLSProcess);
    }
    
    else
    {
      if(theScintillationProcessElectron->IsApplicable(*particle))
      {
	//if(fScint)
	manager->AddProcess(theScintillationProcessElectron);
	manager->SetProcessOrderingToLast(theScintillationProcessElectron, idxPostStep);
	manager->SetProcessOrderingToLast(theScintillationProcessElectron, idxAtRest);
      }
    }
    
  }
 
}
