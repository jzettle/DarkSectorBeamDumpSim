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
#include <fstream>

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
  //setupDMFile("../splitDMResults_ArRecoils.txt");
  
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

void DarkSectorSimPrimaryGeneratorAction::SetDarkMatterFile(G4String dmfile)
{
  dm_events_file = dmfile;
  G4cout << dm_events_file << G4endl;
  setupDMFile(dm_events_file);
}

void DarkSectorSimPrimaryGeneratorAction::setupDMFile(std::string filename)
{
  //open DM file and store components in vectors together, check this works the way I expect (it does) 
  std::ifstream infile;
  infile.open(filename.c_str());
  std::string line = "";
  double E = 0;
  double px = 0;
  double py = 0;
  double pz = 0;
  double m = 0;
  double x0 = 0;
  double x1 = 0;
  double x2 = 0;
  double x3 = 0;
  double end0 = 0;
  double end1 = 0;
  double end2 = 0;
  double end3 = 0;
  double recE = 0;
  double p0 = 0;
  double p1 = 0;
  std::ifstream ins(filename.c_str());

  if(!infile.good())
  {
    std::cout << "File not found!" << std::endl;
  }

  while(getline(infile, line))
    {
      if(infile.eof()) {break;}
      infile >> E >> px >> py >> pz >> m >> x0 >> x1 >> x2 >> x3 >> end0 >> end1 >> end2 >> end3 >> recE >> p0 >> p1;
      fDMposX.push_back(x0);
      fDMposY.push_back(x1);
      fDMposZ.push_back(x2);
      fDMtime.push_back(x3);
      fDMrecE.push_back(recE);
      fDMmomX.push_back(px);
      fDMmomY.push_back(py);
      fDMmomZ.push_back(pz);
      line = ""; 
      E=px=py=px=x0=x1=x2=x3=end0=end1=end2=end3=recE=p0=p1=0;
    }
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
  if(fGenerator == "BoostedDM")
  {
    //G4cout << "entering DM generator" << G4endl;
    GenerateDM(event);
  }
  
}

void DarkSectorSimPrimaryGeneratorAction::SetGenerator(G4String generator)
{
  if(generator == "gps" || generator == "VoxelPhoton" || generator == "BoostedDM")
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
  //G4double x = 21.0 - voxeldist*voxelR*cos(angle);
  G4double x = 21.0 - voxeldist*voxelR;
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

void DarkSectorSimPrimaryGeneratorAction::GenerateDM(G4Event *event)
{
  int selval = 0;                                                    
  G4double px = 0;
  G4double py = 0;
  G4double pz = 0;
  G4double x = 0;
  G4double y = 0;
  G4double z = 0;
  G4double energy = 0;
  G4double time = 0;
  G4int Z = 18;
  G4int A = 40;
  while(true)
  {
    selval = G4UniformRand()*fDMposX.size(); // proxy for number of lines in the file...this should work (it does)
    px = fDMmomX[selval];
    py = fDMmomY[selval];
    pz = fDMmomZ[selval];
    x = (fDMposX[selval] + 3.0);
    y = (fDMposY[selval]);
    z = (fDMposZ[selval]);
    energy = (fDMrecE[selval]*1e6);
    time = (fDMtime[selval]);
    if((x > 19.0 && x < 23.0) && (y > -2.0 && y < 2.0) && (z > -2.0 && z < 2.0))
    {
      break;
    }
  }
  G4ParticleDefinition* Argon = G4IonTable::GetIonTable()->GetIon(Z, A, 0.0*keV);
  fPartGenerator->SetParticleDefinition(Argon);
  fPartGenerator->SetParticleEnergy(energy*keV);
  fPartGenerator->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
  fPartGenerator->SetParticlePosition(G4ThreeVector(x*m,y*m,z*m));
  fPartGenerator->SetParticleTime(0.0);
  //G4cout << "Values are: " << selval << " " << fPartGenerator->GetParticleEnergy() << " " << fPartGenerator->GetParticleDefinition()->GetParticleName() << " " << x << " " << y << " " << z << G4endl;
  fPartGenerator->GeneratePrimaryVertex(event);
}

