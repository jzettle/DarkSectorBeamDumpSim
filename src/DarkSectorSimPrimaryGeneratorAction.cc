#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Proton.hh"
#include "G4IonTable.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "DarkSectorSimPrimaryGeneratorAction.hh"
#include "DarkSectorSimPrimaryGeneratorMessenger.hh"

DarkSectorSimPrimaryGeneratorAction::DarkSectorSimPrimaryGeneratorAction():
  G4VUserPrimaryGeneratorAction(), fParticleGun()
{ 
  fParticleGun = new G4GeneralParticleSource();  
  fGunMessenger = new DarkSectorSimPrimaryGeneratorMessenger(this);
  fGenerator = " ";
  fPartGenerator = new G4ParticleGun();
}

DarkSectorSimPrimaryGeneratorAction::~DarkSectorSimPrimaryGeneratorAction()
{ 
  delete fParticleGun; 
}

void DarkSectorSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{ 
  //Use standard Geant4 gps commands to create particle sources
  if(fGenerator == "gps" || fGenerator == "GeneralParticleSource")
  {
    if(fParticleGun->GetParticleDefinition() == G4Proton::ProtonDefinition())
    {
      G4double random = G4UniformRand();
      G4double pottime = random*320.0 + 400; //add 400 ns offset to better see results for now
      fParticleGun->SetParticleTime(pottime);
      fParticleGun->GeneratePrimaryVertex(event);
    }
    else
      fParticleGun->GeneratePrimaryVertex(event);
  }
}
      

void DarkSectorSimPrimaryGeneratorAction::SetGenerator(G4String generator)
{
  if(generator == "gps")
    fGenerator = generator;
}
