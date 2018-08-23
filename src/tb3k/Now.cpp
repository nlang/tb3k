#include "Now.h"

void Now::init(void) {
  setSyncProvider(RTC.get);
  // TODO display time input notice?
  if(timeStatus() != timeSet) {
    //Serial.println("Unable to sync with the RTC");
  } else {
    //Serial.println("RTC has set the system time");
  }
}

int Now::getYear(void) {
   return year();
}
int Now::getYear2kBased(void) {
   return year() - 2000;
}
int Now::getMonth(void) {
  return month();
}
int Now::getDay(void) {
  return day();
}
int Now::getHour(void) {
  return hour();
}
int Now::getMinute(void) {
  return minute();
}
int Now::getSecond(void) {
  return second();
}

Date Now::getToday(void) {
  return Date(year(), month(), day());
}

void Now::setDateTime(int hour, int minute, int second, int day, int month, int year) {
  setTime(hour, minute, second, day, month, year);
  RTC.set(now());
}

int Now::getDaysInMonth(int month, bool isLeapYear = false) {
  if (month > 0 && month <= 12) {
    int daysInMonth[] = {31, isLeapYear ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return daysInMonth[month - 1];
  }
  return 0;
}

int Now::getDayOfYear(void) {
  Date d = getToday();
  return getDayOfYear(&d);
}

int Now::getDayOfYear(Date *date) {
  int doy = 0;
  bool isLeapYear = Now::isLeapYear(date->getYear());

  for (int i = 0; i < date->getMonth() - 1; i++) {
    doy += getDaysInMonth(date->getMonth(), isLeapYear);
  }
  doy += date->getDay();
  return doy;
}

bool Now::isLeapYear(int year) {
  return (year % 4  == 0) && (year % 100 != 0 || year % 400 == 0);
}

bool Now::isToday(Date *date) {
  const int year = getYear();
  if (date->getYear() == year) {
    int doy = getDayOfYear(date);
    if (doy == getDayOfYear()) {
      return true;
    }
  }
  return false;
}

bool Now::isTomorrow(Date * date) {
  const int year = getYear();
  if (date->getYear() == year) {
    // same year, same day of year?
    int doy = getDayOfYear(date);
    if (doy == getDayOfYear() + 1) {
      return true;
    }
  } else if (year + 1 == date->getYear()) {
      // next year, is 31.12 -> 1.1.?
      if (date->getMonth() == 1 && date->getDay() == 1) {
        if (12 == getMonth() && 31 == getDay()) {
          return true;
        }
      }
  }
  return false;
}

bool Now::isFuture(int year, int month, int day, bool todayIsFuture) {
    Date date = Date(year, month, day);
    return isFuture(&date, todayIsFuture);
}

bool Now::isFuture(Date *date) {
  return isFuture(date, false);
}

bool Now::isFuture(Date *date, bool todayIsFuture) {
  const int year = getYear();
  if (date->getYear() > year) {
    return true;
  }
  if (date->getYear() == year) {
    int doy = getDayOfYear(date);
    int todayDoy = getDayOfYear();
    if (todayIsFuture && doy == todayDoy) {
      return true;
    } else if (doy > todayDoy) {
      return true;
    }
  }
  return false;
}

bool Now::isPast(Date * date) {
  return !isFuture(date);
}

bool Now::isMidnight(void) {
  if (0 == getHour() && 0 == getMinute()) {
    return true;
  }
  return false;
}
