#ifndef TRASH_OUT_DATE_H
#define TRASH_OUT_DATE_H

#include "Date.h"
#include <Arduino.h>

class TrashOutDate : public Date {

private:
  int _channel;
  bool _ack;

public:
  TrashOutDate();
  TrashOutDate(int year, int month, int day);
  TrashOutDate(int year, int month, int day, int channel);

  void setChannel(int channel);
  int getChannel(void);
  void getAsString(char* buf);

  void ack(void);
  void nack(void);
  bool isAck(void);
  bool isSameDate(TrashOutDate * date);
};

#endif
