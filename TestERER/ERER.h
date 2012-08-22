#ifndef ERER_h
#define ERER_h

#include "WProgram.h"

class ERER {
  public:
    ERER(byte, byte, byte);
    long getRawDistance();
  private:
    byte getBCDpin(byte);
    void setStatus(byte, byte, boolean);
  };

#endif
