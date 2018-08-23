#ifndef TB3K_DATE_H
#define TB3K_DATE_H

class Date {

private:
  int _year;
  int _month;
  int _day;

public:
  Date();
  Date(int year, int month, int day);

  void setYear(int year);
  void setMonth(int month);
  void setDay(int day);

  int getYear(void);
  int getYear2kBased(void);
  int getMonth(void);
  int getDay(void);
  bool isEmpty(void);
  bool equals(Date * date);
};
#endif
