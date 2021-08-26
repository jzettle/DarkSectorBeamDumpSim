#ifndef ScintillationStore_hh
#define ScintillationStore_hh

#include "TObject.h"
#include "OpPhoton.hh"

class ScintillationStore : public TObject 
{
private:
  OpPhoton* onePhoton;
  std::vector<OpPhoton> photonVec;
  double scint_en;
  double scint_posx;
  double scint_posy;
  double scint_posz;
  ScintillationStore *store;

public:
  ScintillationStore();
  virtual ~ScintillationStore();
  void clear();
  void SetSinglePhoton(OpPhoton* photon) {onePhoton = photon;}
  OpPhoton * GetSinglePhoton() {return onePhoton;}
  void SetVectorOfPhotons(std::vector<OpPhoton> vecphoton) {photonVec = vecphoton;}
  std::vector<OpPhoton> GetVectorOfPhotons() {return photonVec;}
  void SetScintEnergy(double en) {scint_en = en;}
  double GetScintEnergy() {return scint_en;}
  void SetScintPosX(double x) {scint_posx = x;}
  double GetScintPosX() {return scint_posx;}
  void SetScintPosY(double y) {scint_posy = y;}
  double GetScintPosY() {return scint_posy;}
  void SetScintPosZ(double z) {scint_posz = z;}
  double GetScintPosZ() {return scint_posz;}

  void SetScintillationStore(ScintillationStore* st) {store = st;}
  ScintillationStore * GetScintillationStore() {return store;}

  OpPhoton* GetPhotonInVector(int photonnum)
  {
    std::vector<OpPhoton>::iterator start = photonVec.begin();
    std::vector<OpPhoton>::iterator end = photonVec.end();
    while(start!=end && start->GetID() != photonnum) {++start;}
    return (start == end ? 0 : &(*start));
  }

  void AddPhoton(OpPhoton photon)
  {
    photonVec.push_back(photon);
  }

  ClassDef(ScintillationStore, 1)
};

#endif
