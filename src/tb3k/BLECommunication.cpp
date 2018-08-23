#include "BLECommunication.h"

BLECommunication::BLECommunication(SoftwareSerial *btSerial, DataStore *dataStore): _btSerial(btSerial), _dataStore(dataStore) {
}

void BLECommunication::setCurrentList(TrashOutDate *currentList, int length) {
  _currentList = currentList;
  _currentListLength = length;
}

void BLECommunication::start(void) {
  _btSerial->begin(9600);

  delay(100);
  _btSerial->println("AT+BAUD0");
  delay(100);
  _btSerial->println("AT+NAMETrashBoy3000");
  delay(100);
  _btSerial->println("AT+POWE3");
  delay(100);

  /*
  if (_btSerial->available()) {
    while (_btSerial->available()) {
      Serial.print((char)_btSerial->read());
    }
    Serial.println();
  }*/
}

bool BLECommunication::readDataIfAvailable(void) {
  if (_btSerial->available()) {
    int c = 0;
    char in[20] = "";
    while (_btSerial->available()) {
      in[c] = _btSerial->read();
      c++;

      if (0 == strcmp(in, "TB+EVENTS?")) {
        char buf[8] = "";
        int dateCount = _dataStore->getDateCount();
        TrashOutDate dates[dateCount];
        _dataStore->readDates(dates, dateCount, false);
        for (int i = 0; i < dateCount; i++) {
          TrashOutDate *date = dates + i;
          date->getAsString(buf);
          _btSerial->print(buf);
        }
        _btSerial->println("/TB");

      } else if (0 == strcmp(in, "TB+ACKEVENTS?")) {
        char buf[8] = "";
        for (int i = 0; i < _currentListLength; i++) {
          TrashOutDate *date = _currentList + i;
          if (date->isAck()) {
            date->getAsString(buf);
            _btSerial->print(buf);
          }
        }
        _btSerial->println("/TB");
      } else if (0 == strcmp(in, "TB+ACK=")) {

        char ackDate[8] = "";
        int index = 0;
        while (_btSerial->available() && index < 8) {
          ackDate[index] = _btSerial->read();
          index++;
        }

        int dateParts[4]; // year, month, day, channel
        char chrParts[3] = "";

        for (int i = 0; i < 8; i += 2) {
          memcpy(chrParts, ackDate + i, 2 * sizeof(char));
          dateParts[i/2] = atoi(chrParts);
        }

        for (int i = 0; i < _currentListLength; i++) {
          TrashOutDate *date = _currentList + i;
          if (
            date->getYear2kBased() == dateParts[0] &&
            date->getMonth() == dateParts[1] &&
            date->getDay() == dateParts[2] &&
            date->getChannel() == dateParts[3]
          ) {
            date->ack();
            break;
          }
        }
        _btSerial->println("/TB");
      } else if (0 == strcmp(in, "TB+DATETIME=")) {

        char dateTime[13] = "";
        int index = 0;
        while (_btSerial->available() && index < 13) {
          dateTime[index] = _btSerial->read();
          index++;
        }

        int newDateTime[6]; // year, month, day, hour, min, sec
        char chrParts[3] = "";

        int i;
        for (i = 0; i < 13; i += 2) {
          memcpy(chrParts, dateTime + i, 2 * sizeof(char));
          newDateTime[i/2] = atoi(chrParts);
        }
        if (i >= 12) {
          Now::setDateTime(newDateTime[3], newDateTime[4], newDateTime[5], newDateTime[2], newDateTime[1], newDateTime[0]);
          _btSerial->println("/TB");
        }
      }
    }
  }

  return false;
}
