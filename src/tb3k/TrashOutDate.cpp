#include "TrashOutDate.h"

TrashOutDate::TrashOutDate() : Date() {
  _channel = -1;
  _ack = false;
}
TrashOutDate::TrashOutDate(int year, int month, int day, int channel) : Date(year, month, day) {
  _channel = channel;
  _ack = false;
}
TrashOutDate::TrashOutDate(int year, int month, int day) : Date(year, month, day) {
  _channel = -1;
  _ack = false;
}

void TrashOutDate::setChannel(int channel) {
  _channel = channel;
}

int TrashOutDate::getChannel() {
  return _channel;
}

void TrashOutDate::getAsString(char *buf) {
  sprintf(buf, "%02d%02d%02d%d", getYear2kBased(), getMonth(), getDay(), _channel);
}

void TrashOutDate::ack(void) {
  _ack = true;
}
void TrashOutDate::nack(void) {
  _ack = false;
}
bool TrashOutDate::isAck(void) {
  return _ack;
}

bool TrashOutDate::isSameDate(TrashOutDate * date) {
  return date->getYear() == getYear() && date->getMonth() == getMonth() && date->getDay() == getDay() && date->getChannel() == getChannel();
}
