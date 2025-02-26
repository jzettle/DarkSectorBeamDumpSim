#ifndef BeamEvent_hh
#define BeamEvent_hh

#include "TObject.h"

class BeamEvent : public TObject
{
private:
  int fEventID;
  double fProtonGenPosX;
  double fProtonGenPosY;
  double fProtonGenPosZ;
  double fProtonStopPosX;
  double fProtonStopPosY;
  double fProtonStopPosZ;
  int fGenPiPlus;
  double fPiPlusGenPosX;
  double fPiPlusGenPosY;
  double fPiPlusGenPosZ;
  int fGenPiZero;
  double fPiZeroGenPosX;
  double fPiZeroGenPosY;
  double fPiZeroGenPosZ;
  int fNumPiMinus;
  int fNumDAR;
  int fNumDIF;
  int fnumu_gen;
  double fnumu_genposx;
  double fnumu_genposy;
  double fnumu_genposz;
  double fnumu_time;
  int fnue_gen;
  double fnue_genposx;
  double fnue_genposy;
  double fnue_genposz;
  double fnue_time;
  int fantinumu_gen;
  double fantinumu_genposx;
  double fantinumu_genposy;
  double fantinumu_genposz;
  double fantinumu_time;
  int fantinue_gen;
  double fantinue_genposx;
  double fantinue_genposy;
  double fantinue_genposz;
  double fantinue_time;
  double fnumu_energy;
  double fnue_energy;
  double fantinumu_energy;
  double fnumu_darE;
  double fnue_darE;
  double fantinumu_darE;
  double fnumu_difE;
  double fnue_difE;
  double fantinumu_difE;

public:
  BeamEvent();
  virtual ~BeamEvent();
  void clear();
  void SetEventID(int ID) {fEventID = ID;}
  int GetEventID() const {return fEventID;}
  void SetProtonGenPosX(double protonx) {fProtonGenPosX = protonx;}
  double GetProtonGenPosX() const {return fProtonGenPosX;}
  void SetProtonGenPosY(double protony) {fProtonGenPosY = protony;}
  double GetProtonGenPosY() const {return fProtonGenPosY;}
  void SetProtonGenPosZ(double protonz) {fProtonGenPosZ = protonz;}
  double GetProtonGenPosZ() const {return fProtonGenPosZ;} 
  void SetProtonStopPosX(double protonx) {fProtonStopPosX = protonx;}
  double GetProtonStopPosX() const {return fProtonStopPosX;}
  void SetProtonStopPosY(double protony) {fProtonStopPosY = protony;}
  double GetProtonStopPosY() const {return fProtonStopPosY;}
  void SetProtonStopPosZ(double protonz) {fProtonStopPosZ = protonz;}
  double GetProtonStopPosZ() const {return fProtonStopPosZ;}
  void SetNumPiPlus(int piplus) {fGenPiPlus = piplus;}
  int GetNumPiPlus() const {return fGenPiPlus;}
  void SetPiPlusGenPosX(double piplusx) {fPiPlusGenPosX = piplusx;}
  double GetPiPlusGenPosX() const {return fPiPlusGenPosX;}
  void SetPiPlusGenPosY(double piplusy) {fPiPlusGenPosY = piplusy;}
  double GetPiPlusGenPosY() const {return fPiPlusGenPosY;}
  void SetPiPlusGenPosZ(double piplusz) {fPiPlusGenPosZ = piplusz;}
  double GetPiPlusGenPosZ() const {return fPiPlusGenPosZ;}
  void SetNumPiZero(int pizero) {fGenPiZero = pizero;}
  int GetNumPiZero() const {return fGenPiZero;}
  void SetPiZeroGenPosX(double pizerox) {fPiZeroGenPosX = pizerox;}
  double GetPiZeroGenPosX() const {return fPiZeroGenPosX;}
  void SetPiZeroGenPosY(double pizeroy) {fPiZeroGenPosY = pizeroy;}
  double GetPiZeroGenPosY() const {return fPiZeroGenPosY;}
  void SetPiZeroGenPosZ(double pizeroz) {fPiZeroGenPosZ = pizeroz;}
  double GetPiZeroGenPosZ() const {return fPiZeroGenPosZ;}
  void SetNumPiMinus(int piminus) {fNumPiMinus = piminus;}
  int GetNumPiMinus() const {return fNumPiMinus;}
  void SetNumDAR(int numdar) {fNumDAR = numdar;}
  int GetNumDAR() const {return fNumDAR;}
  void SetNumDIF(int numdif) {fNumDIF = numdif;}
  int GetNumDIF() const {return fNumDIF;}
  void SetNumNuMu(int num_numu) {fnumu_gen = num_numu;}
  int GetNumNuMu() const {return fnumu_gen;}
  void SetNuMuGenPosX(double numu_genposx) {fnumu_genposx = numu_genposx;}
  double GetNuMuGenPosX() const {return fnumu_genposx;}
  void SetNuMuGenPosY(double numu_genposy) {fnumu_genposy = numu_genposy;}
  double GetNuMuGenPosY() const {return fnumu_genposy;}
  void SetNuMuGenPosZ(double numu_genposz) {fnumu_genposz = numu_genposz;}
  double GetNuMuGenPosZ() const {return fnumu_genposz;}
  void SetNuMuTime(double numu_time) {fnumu_time = numu_time;}
  double GetNuMuTime() const {return fnumu_time;}
  void SetNumNuE(int num_numu) {fnumu_gen = num_numu;}
  int GetNumNuE() const {return fnumu_gen;}
  void SetNuEGenPosX(double nue_genposx) {fnue_genposx = nue_genposx;}
  double GetNuEGenPosX() const {return fnue_genposx;}
  void SetNuEGenPosY(double nue_genposy) {fnue_genposy = nue_genposy;}
  double GetNuEGenPosY() const {return fnue_genposy;}
  void SetNuEGenPosZ(double nue_genposz) {fnue_genposz = nue_genposz;}
  double GetNuEGenPosZ() const {return fnue_genposz;}
  void SetNuETime(double nue_time) {fnue_time = nue_time;}
  double GetNuETime() const {return fnue_time;}
  void SetNumAntiNuMu(int num_antinumu) {fantinumu_gen = num_antinumu;}
  int GetNumAntiNuMu() const {return fantinumu_gen;}
  void SetAntiNuMuGenPosX(double antinumu_genposx) {fantinumu_genposx = antinumu_genposx;}
  double GetAntiNuMuGenPosX() const {return fantinumu_genposx;}
  void SetAntiNuMuGenPosY(double antinumu_genposy) {fantinumu_genposy = antinumu_genposy;}
  double GetAntiNuMuGenPosY() const {return fantinumu_genposy;}
  void SetAntiNuMuGenPosZ(double antinumu_genposz) {fantinumu_genposz = antinumu_genposz;}
  double GetAntiNuMuGenPosZ() const {return fantinumu_genposz;}
  void SetAntiNuMuTime(double antinumu_time) {fantinumu_time = antinumu_time;}
  double GetAntiNuMuTime() const {return fantinumu_time;}
  void SetNumAntiNuE(int num_antinue) {fantinue_gen = num_antinue;}
  int GetNumAntiNuE() const {return fantinue_gen;}
  void SetAntiNuEGenPosX(double antinue_genposx) {fantinue_genposx = antinue_genposx;}
  double GetAntiNuEGenPosX() const {return fantinue_genposx;}
  void SetAntiNuEGenPosY(double antinue_genposy) {fantinue_genposy = antinue_genposy;}
  double GetAntiNuEGenPosY() const {return fantinue_genposy;}
  void SetAntiNuEGenPosZ(double antinue_genposz) {fantinue_genposz = antinue_genposz;}
  double GetAntiNuEGenPosZ() const {return fantinue_genposz;}
  void SetAntiNuETime(double antinue_time) {fantinue_time = antinue_time;}
  double GetAntiNuETime() const {return fantinue_time;}
  void SetNuMuEnergy(double numu_E) {fnumu_energy = numu_E;}
  double GetNuMuEnergy() const {return fnumu_energy;}
  void SetNuEEnergy(double nue_E) {fnue_energy = nue_E;}
  double GetNuEEnergy() const {return fnue_energy;}
  void SetAntiNuMuEnergy(double antinumu_E) {fantinumu_energy = antinumu_E;}
  double GetAntiNuMuEnergy() const {return fantinumu_energy;}
  void SetNuMuDAREnergy(double numu_darE) {fnumu_darE = numu_darE;}
  double GetNuMuDAREnergy() const {return fnumu_darE;}
  void SetNuEDAREnergy(double nue_darE) {fnue_darE = nue_darE;}
  double GetNuEDAREnergy() const {return fnue_darE;}
  void SetAntiNuMuDAREnergy(double antinumu_darE) {fantinumu_darE = antinumu_darE;}
  double GetAntiNuMuDAREnergy() const {return fantinumu_darE;}
  void SetNuMuDIFEnergy(double numu_difE) {fnumu_difE = numu_difE;}
  double GetNuMuDIFEnergy() const {return fnumu_difE;}
  void SetNuEDIFEnergy(double nue_difE) {fnue_difE = nue_difE;}
  double GetNuEDIFEnergy() const {return fnue_difE;}
  void SetAntiNuMuDIFEnergy(double antinumu_difE) {fantinumu_difE = antinumu_difE;}
  double GetAntiNuMuDIFEnergy() const {return fantinumu_difE;}
  
  ClassDef(BeamEvent,1)
};

#endif /* BeamEvent_hh */
