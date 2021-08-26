#include <cstdlib>
#include <iostream>
#include <vector>

#include "ScintillationStore.hh"

ClassImp(ScintillationStore)

ScintillationStore::ScintillationStore()
{
  clear();
}

ScintillationStore::~ScintillationStore()
{
  clear();
}

void ScintillationStore::clear()
{
  if(onePhoton)
    delete onePhoton;
  photonVec.clear();
  scint_en = 0.0;
  scint_posx = 0.0;
  scint_posy = 0.0;
  scint_posz = 0.0;
  if(store)
    delete store;
}

