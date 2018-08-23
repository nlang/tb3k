#ifndef DISPLAY_H
#define DISPLAY_H

#include "Now.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "TrashOutDate.h"

class Display {

private:
  SSD1306AsciiAvrI2c _display;
  void skipLine();
  void printEmptyLine(int skipLines);
  void drawChannelLine(TrashOutDate *date, char (*channelNames)[22], int daysFromToday);
  void fillEmptyString(char *string, int len);

public:
  Display(const DevType *displayType, int I2CAddress);
  void drawFrame(char frameChar);
  void drawFrame(char frameChar, int delayMsec);
  //void printLine(char *c);
  //void printLine(char *line, bool centered);
  void drawDateTimeLine(void);
  void drawNoMoreDatesScreen(void);
  void drawNextDates(TrashOutDate *dates, int numberOfDates, char (*channelNames)[22], int numberOfChannels, bool blink);
  void showIntro(char (*lines)[22], int lineCount);
};

#endif
