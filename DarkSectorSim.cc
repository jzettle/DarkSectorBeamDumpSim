#include <time.h>
//#include <iostream>
//#include <memory>

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

#include "G4UImanager.hh"
#include "Randomize.hh"

#include "G4PhysListFactory.hh"

#include "src/DarkSectorSimActionInitialization.hh"
#include "src/DarkSectorSimPrimaryGeneratorAction.hh"
#include "src/DarkSectorSimDetectorConstruction.hh"
#include "src/DarkSectorSimPhysicsList.hh"

int main(int argc,char** argv)
{

  G4String macro;
  G4String DarkSectorSimPhysListmacro = "";
  //G4int seed = 123;
  G4int seed = time(0); // returns time in seconds as an int
  time_t start_time = seed;

  for(G4int i = 1; i < argc; i = i + 2)
  {
    if(G4String(argv[i]) == "-m")
      macro = argv[i+1];
    if(G4String(argv[i]) == "-s")
      seed = atoi(argv[i+1]);
  }
  // choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(seed);
  G4cout << "SEED: "<<seed << G4endl;
  // Construct the default run manager
  G4RunManager *runManager = new G4RunManager();

  G4cout << "STAGE" << G4endl;
  //G4PhysListFactory *physListFactory = new G4PhysListFactory();
   
  //G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("LBE");//"QGSP_BIC_HP");

  // set mandatory initialization classes
  runManager->SetUserInitialization(new DarkSectorSimDetectorConstruction());
  runManager->SetUserInitialization(new DarkSectorSimPhysicsList(DarkSectorSimPhysListmacro));
  runManager->SetUserInitialization(new DarkSectorSimActionInitialization());

 
  //Initialize G4 kernel                            
  //runManager->Initialize();


  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
    
  // get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  // Batch mode                             
  if(macro.size())
  {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  //Define UI session for interactive mode
  else
  {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute run.mac");   
    ui->SessionStart();
    delete ui;
  }
//    }

  if(visManager) delete visManager;
  
  delete runManager;
  G4cout<<"Simulation took "<<time(0) - start_time<<" seconds.\n";

  return 0;
}

