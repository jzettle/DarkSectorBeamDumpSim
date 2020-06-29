#ifndef __DARKSECTORSIM_ACTION_INITIALIZATION_HH__
#define __DARKSECTORSIM_ACTION_INITIALIZATION_HH__

#include "G4VUserActionInitialization.hh"

class DarkSectorSimActionInitialization : public G4VUserActionInitialization
{
  public:
    
    DarkSectorSimActionInitialization();
    virtual ~DarkSectorSimActionInitialization();

    virtual void Build() const;
};
#endif /* __DARKSECTORSIM_ACTION_INITIALIZATION_HH__*/
