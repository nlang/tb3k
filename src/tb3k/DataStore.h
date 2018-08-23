#ifndef DATA_STORE_H
#define DATA_STORE_H

#include "TrashOutDate.h"

class DataStore {

private:
  int _todStartByte;

public:
  DataStore(int todStartByte);
  bool isFirstRun();
  void setFirstRun(void);
  int readChannelNames(char (*channels)[22]);
  int readDates(TrashOutDate *dates, int size);
  int readDates(TrashOutDate *dates, int size, bool futureOnly);
  int getDateCount(void);

  void eepromWrite(void);
};

#endif
