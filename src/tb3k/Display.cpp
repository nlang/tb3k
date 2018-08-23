#include "Display.h"


Display::Display(const DevType *displayType, int I2CAddress) {
  _display.setFont(Stang5x7);
  _display.begin(displayType, I2CAddress);
  _display.clear();
}

void Display::skipLine(void) {
  _display.println();
}

void Display::printEmptyLine(int skipLines) {
  for (int i = 0; i < skipLines; i++) {
    skipLine();
  }
  _display.println(F("                     "));
}

void Display::drawNextDates(TrashOutDate *dates, int numberOfDates, char (*channelNames)[22], int numberOfChannels, bool blink) {

  bool channelHasOutput[numberOfChannels];
  for (int i = 0; i < numberOfChannels; i++) {
    channelHasOutput[i] = false;
  }

  int lines = 1;
  bool hasTodayOrTomorrow = false;
  bool newLinePrinted = false;

  printEmptyLine(0);
  lines++;
  for (int i = 0; i < numberOfDates; i++) {

    TrashOutDate *date = (dates + i);

    if (true == date->isAck()) continue;
    if (true == channelHasOutput[date->getChannel() - 1]) continue;
    if (!Now::isFuture(date, true)) continue;

    if (true == Now::isToday(date) || true == Now::isTomorrow(date)) {
      hasTodayOrTomorrow = true;
      if (blink) {
        printEmptyLine(0);
        lines++;
      } else {
        drawChannelLine(date, channelNames, Now::isToday(date) ? 0 : 1);
        lines++;
      }
    } else {
      if (hasTodayOrTomorrow && !newLinePrinted) {
        newLinePrinted = true;
        printEmptyLine(0);
        lines++;
      }
      drawChannelLine(date, channelNames, Now::isToday(date) ? 0 : -1);
      lines++;
    }
    channelHasOutput[date->getChannel() - 1] = true;
  }
  for (int i = lines; i < 8; i++) {
    printEmptyLine(0);
  }
}

void Display::drawChannelLine(TrashOutDate *date, char (*channelNames)[22], int daysFromToday) {

  char buf[30] = "";
  char *channelName = channelNames[date->getChannel() - 1];
  int len = strlen(channelName);
  char spaces[20] = "";
  fillEmptyString(&spaces[0], (21 - (len + 6)));

  if (0 == daysFromToday) {
    sprintf(buf, "%s%sHeute!", channelName, spaces);
  } else if (1 == daysFromToday) {
    sprintf(buf, "%s%sMorgen", channelName, spaces);
  } else {
    sprintf(buf, "%s%s %02d.%02d", channelName, spaces, date->getDay(), date->getMonth());
  }
  _display.println(buf);
}

void Display::fillEmptyString(char *string, int len) {
  for (int i = 0; i < len; i++) {
    string[i] = ' ';
  }
}

void Display::drawDateTimeLine() {
  _display.setCursor(0, 0);
  int secondFraction = millis() % 2000;
  char buf[22] = "";
  sprintf(buf, "%02d.%02d.%02d        %02d%s%02d",
    Now::getDay(),
    Now::getMonth(),
    Now::getYear2kBased(),
    Now::getHour(),
    secondFraction > 1000 ? " " : ":",
    Now::getMinute()
  );
  _display.println(buf);
}

void Display::drawNoMoreDatesScreen(void) {
  _display.setCursor(0, 0);
  drawDateTimeLine();
  printEmptyLine(1);
  _display.println(F("   Keine weiteren    "));
  _display.println(F("      Termine.       "));
  printEmptyLine(4);
  printEmptyLine(5);
  printEmptyLine(6);
  printEmptyLine(7);
}

void Display::showIntro(char (*lines)[22], int lineCount) {
  drawFrame('*');
  delay(900);

  for (int i = 0; i < lineCount; i++) {
    if (i > 0) {
      delay(2200);
    }
    char *line = lines[i];
    int len = strlen(line);
    int spaces = floor(((float)(21 - len)) / 2.0f);
    char centeredLine[22] = "";
    for (int ws = 0; ws <= 21; ws++) {
      if (ws < spaces || ws >= spaces + len) {
        if (ws == 0 || ws == 20) {
          centeredLine[ws] = '*';
        } else {
          centeredLine[ws] = ' ';
        }
      } else {
        centeredLine[ws] = line[ws - spaces];
      }
    }
    _display.setCursor(0, 0);
    skipLine();
    skipLine();
    skipLine();
    _display.println(centeredLine);
  }
}

void Display::drawFrame(char frameChar) {
  return drawFrame(frameChar, 0);
}
void Display::drawFrame(char frameChar, int delayMsec) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 21; col++) {
      if ((row == 0 || row == 7) || (col == 0 || col == 20)) {
        _display.print(frameChar);
      } else {
        _display.print(' ');
      }
      delay(delayMsec);
    }
    _display.print('\n');
  }
}
