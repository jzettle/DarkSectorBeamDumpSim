#ifndef OpPhoton_hh
#define OpPhoton_hh

#include "TObject.h"
#include <vector>

class OpPhoton : public TObject
{

private:
  int fID; 
  double fStartTime;
  double fVertexPosX;
  double fVertexPosY;
  double fVertexPosZ;
  double fHitPosX;
  double fHitPosY;
  double fHitPosZ;
  int fHitChannel;
  int fDirect;
  int fRefl;
  int fNumRefl;
  int fDetected;
  double fTrackLength;
  double fDetectionTime;

public:
  OpPhoton();
  virtual ~OpPhoton() {;}
  void SetID(int ID) {fID = ID;}
  int GetID() {return fID;}
  void SetStartTime(double stime) {fStartTime = stime;}
  double GetStartTime() {return fStartTime;}
  void SetVertexPosX(double vposx) {fVertexPosX = vposx;}
  double GetVertexPosX() {return fVertexPosX;}
  void SetVertexPosY(double vposy) {fVertexPosY = vposy;}
  double GetVertexPosY() {return fVertexPosY;}
  void SetVertexPosZ(double vposz) {fVertexPosZ = vposz;}
  double GetVertexPosZ() {return fVertexPosZ;}
  void SetHitPosX(double hposx) {fHitPosX = hposx;}
  double GetHitPosX() {return fHitPosX;}
  void SetHitPosY(double hposy) {fHitPosY = hposy;}
  double GetHitPosY() {return fHitPosY;}
  void SetHitPosZ(double hposz) {fHitPosZ = hposz;}
  double GetHitPosZ() {return fHitPosZ;}
  void SetHitChannel(int hit) {fHitChannel = hit;}
  int GetHitChannel() {return fHitChannel;}
  void SetIsDirect(int isdirect) {fDirect = isdirect;}
  int GetIsDirect() {return fDirect;}
  void SetIsReflected(int isrefl) {fRefl = isrefl;}
  int GetIsReflected() {return fRefl;}
  void SetNumReflections(int numrefl) {fNumRefl = numrefl;}
  int GetNumReflections() {return fNumRefl;}
  void SetIsDetected(int isdet) {fDetected = isdet;}
  int GetIsDetected() {return fDetected;}
  void SetTrackLength(double tl) {fTrackLength = tl;}
  double GetTrackLength() {return fTrackLength;}
  void SetDetectionTime(double dettime) {fDetectionTime = dettime;}
  double GetDetectionTime() {return fDetectionTime;}

  ClassDef(OpPhoton,1)


};

#endif /* OpPhoton_hh */
