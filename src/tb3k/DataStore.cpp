#include "DataStore.h"
#include "EEPROM.h"
#include "Now.h"

DataStore::DataStore(int todStartByte) : _todStartByte(todStartByte) {
}

bool DataStore::isFirstRun(void) {
  return EEPROM.read(0) == 0;
}

void DataStore::setFirstRun(void) {
  EEPROM.write(0, 1);
}

int DataStore::readChannelNames(char (*channelNames)[22]) {
  int startByte = 1;
  int channelCount = EEPROM.read(startByte);
  if (channelCount > 0) {
    int offset = startByte + channelCount + 1;
    for (int i = 0; i < channelCount; i++) {
      int nameLength = EEPROM.read(i + startByte + 1);
      for (int l = 0; l < nameLength; l++) {
        char c = EEPROM.read(offset + l);
        channelNames[i][l] = c;
      }
      offset += nameLength;
    }
  }
  return channelCount;
}
int DataStore::readDates(TrashOutDate *dates, int size) {
  return readDates(dates, size, true);
}

int DataStore::readDates(TrashOutDate *dates, int size, bool futureOnly) {
  int firstByte = _todStartByte;
  int startYear =  Now::getYear2kBased();
  int currentYear = EEPROM.read(firstByte + 1);
  int numberOfDatesInYear = EEPROM.read(firstByte);
  int numberOfDates = 0;

  bool noMoreDates = false;
  while (numberOfDates < size && !noMoreDates) {
    while (currentYear < startYear || noMoreDates) {
      firstByte += (numberOfDatesInYear * 3) + 2;
      numberOfDatesInYear = EEPROM.read(firstByte);
      if (0 == numberOfDatesInYear) {
        noMoreDates = true;
        break;
      }
      currentYear = EEPROM.read(firstByte + 1);
    }

    if (!noMoreDates) {
      for (int i = 2; numberOfDates < size && i < numberOfDatesInYear * 3; i += 3) {

        int month = EEPROM.read(firstByte + i);
        int day = EEPROM.read(firstByte + i + 1);
        int channel = EEPROM.read(firstByte + i + 2);

        if (!futureOnly || Now::isFuture(currentYear, month, day, true)) { // is today or in the future?
          dates[numberOfDates].setDay(day);
          dates[numberOfDates].setMonth(month);
          dates[numberOfDates].setYear(currentYear);
          dates[numberOfDates].setChannel(channel);
          dates[numberOfDates].nack();
          numberOfDates++;
        }
      }
      startYear++;
    }
  }
  return numberOfDates;
}

int DataStore::getDateCount(void) {
  int firstByte = _todStartByte;
  int numberOfDatesInYear = EEPROM.read(firstByte);
  int numberOfDates = numberOfDatesInYear;

  bool noMoreDates = false;
  while (!noMoreDates) {
    firstByte += (numberOfDatesInYear * 3) + 2;
    numberOfDatesInYear = EEPROM.read(firstByte);
    if (0 == numberOfDatesInYear) {
      noMoreDates = true;
      break;
    } else {
      numberOfDates += numberOfDatesInYear;
    }
  }
  return numberOfDates;
}


void DataStore::eepromWrite(void) {

  static int cfg[] = { 1, 3, 9, 6, 11, 'R', 'e', 's', 't', 'm', 'u', 'e', 'l', 'l', 'P', 'a', 'p', 'i', 'e', 'r', 'G', 'e', 'l', 'b', 'e', 'r', ' ', 'S', 'a', 'c', 'k' };
  for (unsigned int i = 0; i < sizeof(cfg); i++) {
    EEPROM.write(0 + i, cfg[i]);
  }

  static int data[] = {
    // nr of dates, year
    18, 18,

    8, 24, 2,
    8, 31, 1,
    9, 3, 3,
    9, 14, 1,
    9, 21, 2,
    9, 28, 1,
    9, 28, 3,
    10, 12, 1,
    10, 26, 1,
    10, 26, 3,
    11, 9, 1,
    11, 20, 2,
    11, 23, 1,
    11, 26, 3,
    12, 7, 1,
    12, 19, 2,
    12, 21, 1,
    12, 28, 3,

    2, 19,
    8, 20, 1,
    12, 31, 2,

    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  };

  for (unsigned int i = 0; i < sizeof(data); i++) {
    EEPROM.write(_todStartByte + i, data[i]);
  }
}
