#include <cstdlib>
#include <iostream>
#include "OpPhoton.hh"
ClassImp(OpPhoton)

OpPhoton::OpPhoton()
{
  fID = 0;
  fStartTime = 0.0;
  fVertexPosX = 0.0;
  fVertexPosY = 0.0;
  fVertexPosZ = 0.0;
  fHitPosX = 0.0;
  fHitPosY = 0.0;
  fHitPosZ = 0.0;
  fHitChannel = -1;
  fDirect = 0;
  fRefl = 0;
  fNumRefl = 0;
  fDetected = 0;
  fTrackLength = 0.0;
  fDetectionTime = 0.0;
}


