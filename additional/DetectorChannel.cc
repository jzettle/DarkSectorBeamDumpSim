#include <cstdlib>
#include <iostream>
#include <vector>
#include "DetectorChannel.hh"
ClassImp(DetectorChannel)


DetectorChannel::DetectorChannel()
{
  clear();
}

DetectorChannel::~DetectorChannel()
{
  clear();
}

void DetectorChannel::clear()
{
  fChannel = 0;
  fHitChannel = 0;
  fOpPhotonVec.clear();
}

void DetectorChannel::print_contents()
{
  std::cout << "This is channel: " << fChannel << std::endl;
}
