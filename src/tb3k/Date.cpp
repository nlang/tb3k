#include "Date.h"

Date::Date() {
  Date(-1, -1, -1);
}
Date::Date(int year, int month, int day): _year(year), _month(month), _day(day) {
}

void Date::setYear(int year) {
  _year = year;
}

void Date::setMonth(int month) {
  _month = month;
}

void Date::setDay(int day) {
  _day = day;
}

int Date::getYear(void) {
  if (_year >= 2000) {
    return _year;
  } else {
    return _year + 2000;
  }
}

int Date::getYear2kBased(void) {
  return getYear() - 2000;
}

int Date::getMonth(void) {
  return _month;
}

int Date::getDay(void) {
  return _day;
}

bool Date::isEmpty(void) {
  return (_year == -1 && _month == -1 && _day == -1);
}

bool Date::equals(Date *date) {
  return date->getYear() == getYear() && date->getMonth() == getMonth() && date->getDay() == getDay();
}
