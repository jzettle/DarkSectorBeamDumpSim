#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"
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
  voxelRNum = 0;
  voxelZNum = 0;
}

DarkSectorSimPrimaryGeneratorAction::~DarkSectorSimPrimaryGeneratorAction()
{ 
  delete fParticleGun; 
}

void DarkSectorSimPrimaryGeneratorAction::SetVoxelRNum(G4double voxelR)
{
  voxelRNum = voxelR;
}

void DarkSectorSimPrimaryGeneratorAction::SetVoxelZNum(G4double voxelZ)
{
  voxelZNum = voxelZ;
}


void DarkSectorSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{ 
  //Use standard Geant4 gps commands to create particle sources
  //G4cout << fGenerator << G4endl;
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
  if(fGenerator == "VoxelPhoton")
  {
    GenerateOptPhotonVoxel(event);
  }
}

void DarkSectorSimPrimaryGeneratorAction::SetGenerator(G4String generator)
{
  if(generator == "gps" || generator == "VoxelPhoton")
    fGenerator = generator;
}

void DarkSectorSimPrimaryGeneratorAction::GenerateOptPhotonVoxel(G4Event *event)
{
  G4double phi = 2*M_PI*G4UniformRand();
  G4double costheta = 1.0 - 2.0*G4UniformRand();
  G4double sintheta = sqrt(1.0 - costheta*costheta);
  G4double px = sintheta*cos(phi);
  G4double py = sintheta*sin(phi);
  G4double pz = costheta;
  G4ThreeVector p(px,py,pz);
  fPartGenerator->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
  fPartGenerator->SetParticleEnergy(9.686*eV);
  fPartGenerator->SetParticleTime(0.0*ns);
  fPartGenerator->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));

  //Include random photon polarization in generation, maybe matters for truly random travel direction
  G4double polx = costheta*cos(phi);
  G4double poly = costheta*sin(phi);
  G4double polz = -sintheta;
  G4ThreeVector pol(polx,poly,polz);
  G4ThreeVector perp = p.cross(pol);
  G4double phi_2 = 2*M_PI*G4UniformRand();
  G4double sinp = std::sin(phi_2);
  G4double cosp = std::cos(phi_2);
  pol = cosp*pol + sinp*perp;
  pol = pol.unit();
  fPartGenerator->SetParticlePolarization(G4ThreeVector(pol.x(), pol.y(), pol.z()));

  G4ThreeVector pos;
  GetPositioninVoxel(pos, voxelRNum, voxelZNum);
  fPartGenerator->SetParticlePosition(pos);
  fPartGenerator->GeneratePrimaryVertex(event);
					      
}

void DarkSectorSimPrimaryGeneratorAction::GetPositioninVoxel(G4ThreeVector &pos, G4double voxelR, G4double voxelZ)
{
  G4Navigator *nav = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4double voxeldist = 0.02;
  double angle = 45.0*(M_PI/180.0);
  G4double x = 21.0 - voxeldist*voxelR*cos(angle);
  //G4double y = voxeldist*voxelR*sin(angle);
  G4double y = 0.0;
  G4double z = -voxeldist*voxelZ;
  if(x > 23.25)
    x = 23.25;
  if(y > 2.5) 
    y = 2.5;
  if(z > 2.5)
    z = 2.5;
  pos.set(x*m, y*m, z*m);
  return;
}

