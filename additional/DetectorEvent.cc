#include <cstdlib>
#include <iostream>
#include <vector>
#include "DetectorEvent.hh"
#include "DetectorChannel.hh"
#include "ScintillationStore.hh"

ClassImp(DetectorEvent)

DetectorEvent::DetectorEvent()
{
  clear();
}

DetectorEvent::~DetectorEvent()
{
  clear();
}

void DetectorEvent::clear()
{
  fID = -1;
  fChanNum = -1;
  fGenPosX = 0.0;
  fGenPosY = 0.0;
  fGenPosZ = 0.0;
  fEnergy = 0.0;
  fF90 = 0.0;
  fChannels = 0;
  fDetChannels.clear();
  for(int i=0; i<1300;++i)
  {
    DetectorChannel chan;
    chan.SetChannelNum(i);
    fDetChannels.push_back(chan);
  }
  fChannels = fDetChannels.size();
}
