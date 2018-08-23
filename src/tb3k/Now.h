#ifndef TB3K_NOW_H
#define TB3K_NOW_H

#include "DS3232RTC.h"
#include "Date.h"

class Now {

public:
  static void init(void);
  static int getYear(void);
  static int getYear2kBased(void);
  static int getMonth(void);
  static int getDay(void);
  static int getHour(void);
  static int getMinute(void);
  static int getSecond(void);
  static Date getToday(void);
  static void setDateTime(int hour, int minute, int second, int day, int month, int year);

  static int getDaysInMonth(int month, bool isLeapYear);
  static int getDayOfYear(void);
  static int getDayOfYear(Date * date);
  static bool isLeapYear(int year);
  static bool isToday(Date *date);
  static bool isTomorrow(Date * date);
  static bool isFuture(int year, int month, int day, bool todayIsFuture);
  static bool isFuture(Date *date);
  static bool isFuture(Date *date, bool todayIsFuture);
  static bool isPast(Date * date);
  static bool isMidnight(void);
};

#endif
