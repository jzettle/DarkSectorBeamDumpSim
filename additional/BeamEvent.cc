#include <cstdlib>
#include <iostream>
#include <vector>

#include "BeamEvent.hh"
ClassImp(BeamEvent)

BeamEvent::BeamEvent()
{
  clear();
}

BeamEvent::~BeamEvent()
{
  clear();
}

void BeamEvent::clear()
{
  fProtonGenPosX = 0.0;
  fProtonGenPosY = 0.0;
  fProtonGenPosZ = 0.0;
  fGenPiPlus = 0;
  fPiPlusGenPosX = 0.0;
  fPiPlusGenPosY = 0.0;
  fPiPlusGenPosZ = 0.0;
  fGenPiZero = 0;
  fPiZeroGenPosX = 0.0;
  fPiZeroGenPosY = 0.0;
  fPiZeroGenPosZ = 0.0;
  fNumPiMinus = 0;
  fNumDAR = 0;
  fNumDIF = 0;
  fnumu_gen = 0;
  fnumu_genposx = 0.0;
  fnumu_genposy = 0.0;
  fnumu_genposz = 0.0;
  fnumu_time = 0.0;
  fnue_gen = 0;
  fnue_genposx = 0.0;
  fnue_genposy = 0.0;
  fnue_genposz = 0.0;
  fnue_time = 0.0;
  fantinumu_gen = 0;
  fantinumu_genposx = 0.0;
  fantinumu_genposy = 0.0;
  fantinumu_genposz = 0.0;
  fantinumu_time = 0.0;
  fantinue_gen = 0;
  fantinue_genposx = 0.0;
  fantinue_genposy = 0.0;
  fantinue_genposz = 0.0;
  fantinue_time = 0.0;
  fnumu_darE = 0.0;
  fnue_darE = 0.0;
  fantinumu_darE = 0.0;
  fnumu_difE = 0.0;
  fnue_difE = 0.0;
  fantinumu_difE = 0.0;
}
