#ifndef DetectorChannel_hh
#define DetectorChannel_hh

#include "TObject.h"
#include "OpPhoton.hh"

class DetectorChannel : public TObject 
{
private:
  int fChannel;
  int fHitChannel;
  std::vector<OpPhoton> fOpPhotonVec;

public:
  DetectorChannel();
  virtual ~DetectorChannel();
  void clear();
  void print_contents();

  void SetChannelNum(int channum) {fChannel = channum;}
  int GetChannelNum() {return fChannel;}
  
  void SetHitChannelNum(int num) {fHitChannel = num;}
  int GetHitChannelNum() {return fHitChannel;}

  void SetDetectedPhotonsStore(std::vector<OpPhoton> photons) {fOpPhotonVec = photons;}
  const std::vector<OpPhoton>& GetDetectedPhotonsStore() {return fOpPhotonVec;}
  double GetChannelDetectedPhotons() {return fOpPhotonVec.size();}
  
  ClassDef(DetectorChannel, 1)
};

#endif /* DetectorChannel_hh */
  
