
#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#include "tb3k/DataStore.h"
#include "tb3k/Date.h"
#include "tb3k/Display.h"
#include "tb3k/Now.h"
#include "tb3k/TrashOutDate.h"
#include "tb3k/SoundEffects.h"
#include "tb3k/BLECommunication.h"

void ackButtonInterruptTriggered(void);
void doPerformAck(void);
bool hasActiveDate(void);
bool isDanisBirthday(void);
