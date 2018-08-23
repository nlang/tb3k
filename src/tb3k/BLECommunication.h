#ifndef BLE_COMMUNICATION_H
#define BLE_COMMUNICATION_H

#include "SoftwareSerial.h"
#include "DataStore.h"
#include "Now.h"

class BLECommunication {

private:
  SoftwareSerial *_btSerial;
  DataStore *_dataStore;
  TrashOutDate *_currentList;
  int _currentListLength;

public:
  BLECommunication(SoftwareSerial *btSerial, DataStore *dataStore);
  void start(void);
  void setCurrentList(TrashOutDate *currentList, int length);
  bool readDataIfAvailable(void);
};

#endif
