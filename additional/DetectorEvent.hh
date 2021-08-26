#ifndef DetectorEvent_hh
#define DetectorEvent_hh

#include "TObject.h"
#include "DetectorChannel.hh"
#include "ScintillationStore.hh"

class DetectorEvent : public TObject
{
private:
  int fID;
  int fChanNum;
  double fGenPosX;
  double fGenPosY;
  double fGenPosZ;
  double fEnergy;
  double fF90;
  double fTotalPhotons;
  int fChannels;
  int fHitChannel;
  std::vector<DetectorChannel> fDetChannels;
  //ScintillationStore *store;
public:
  DetectorEvent();
  virtual ~DetectorEvent();
  void clear();
  void SetEventID(int ID) {fID = ID;}
  int GetEventID() const {return fID;}
  void SetChannelNumber(int num) {fChanNum = num;}
  int GetChannelNumber() const {return fChanNum;}
  void SetGenPosX(double posx) {fGenPosX = posx;}
  double GetGenPosX() const {return fGenPosX;}
  void SetGenPosY(double posy) {fGenPosY = posy;}
  double GetGenPosY() const {return fGenPosY;}
  void SetGenPosZ(double posz) {fGenPosZ = posz;}
  double GetGenPosZ() const {return fGenPosZ;}
  void SetF90Value(double f90) {fF90 = f90;}
  double GetF90Value() const {return fF90;}
  void SetNumChannels(int chan) {fChannels = chan;}
  int GetNumChannels() const {return fChannels;}
  void SetHitChannel(int hitchan) {fHitChannel = hitchan;}
  int GetHitChannel() const {return fHitChannel;}
  void SetEventEnergy(double energy) {fEnergy = energy;}
  double GetEventEnergy() const {return fEnergy;}
  void SetTotalPhotons(double photons) {fTotalPhotons = photons;}
  double GetTotalPhotons() const {return fTotalPhotons;}

  const std::vector<DetectorChannel>& GetChannels() const {return fDetChannels;}
  std::vector<DetectorChannel>& GetChannels() {return fDetChannels;}
  
  //work to just get channel called
  DetectorChannel *GetChannel(int channel)
  {
    //std::vector<DetectorChannel>::iterator chanstart = fDetChannels.begin();
    std::vector<DetectorChannel>::iterator chanend = fDetChannels.end();
    std::vector<DetectorChannel>::iterator changet = fDetChannels.begin() + channel;
    //while(chanstart!=chanend && chanstart->GetChannelNum() != channel) {++chanstart;}
    return (changet >= chanend ? 0 : &(*changet));
  }
  
  //void SetStore(ScintillationStore *storage) {store = storage;}
  //ScintillationStore *GetStore() {return store;}
  
  ClassDef(DetectorEvent,1)

};


#endif /* DetectorEvent.hh */

