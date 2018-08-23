#include "trashboy3k.h"

#define FLASH_LED_PIN 10
#define SPEAKER_PIN 9
#define OLED_DISPLAY_TYPE &SH1106_128x64
#define OLED_I2C_ADDRESS 0x3C // I2C address of OLED display
#define EEPROM_DATE_AREA_START 512
#define MAX_NUMBER_OF_DATES_IN_MEMORY 10
#define ACK_TRASHOUT_DATE_INTERRUPT_PIN 3
#define ACK_TRASHOUT_DATE_INTERRUPT_ID 1

#define BTLE_POWER_PIN 6
#define BTLE_SERIAL_RX_PIN 7
#define BTLE_SERIAL_TX_PIN 8

// TODO put into EEPROM as config
#define ALARM_ENABLED_FROM_HOUR 10
#define ALARM_ENABLED_TO_HOUR 22

DataStore dataStore = DataStore(EEPROM_DATE_AREA_START);
Display display = Display(OLED_DISPLAY_TYPE, OLED_I2C_ADDRESS);
SoftwareSerial btSerial(BTLE_SERIAL_RX_PIN, BTLE_SERIAL_TX_PIN);

int numberOfChannels = 0;
char channelNames[5][22];

int numberOfDates = 0;
TrashOutDate dates[MAX_NUMBER_OF_DATES_IN_MEMORY];
BLECommunication ble(&btSerial, &dataStore);

volatile unsigned long debounceLastAckBtnAction = 0;
volatile bool performAck = false;
volatile bool performingAck = false;

long lastAlaramMillis = 0;
bool lockMidnight = false;

void setup() {

  Now::init();
  Serial.begin(9600);

  pinMode(BTLE_POWER_PIN, OUTPUT);
  digitalWrite(BTLE_POWER_PIN, LOW);
  pinMode(ACK_TRASHOUT_DATE_INTERRUPT_PIN, INPUT);
  attachInterrupt(ACK_TRASHOUT_DATE_INTERRUPT_ID, ackButtonInterruptTriggered, RISING); // interrupt for ack button

  char lines[][22] = {
    "Hello!",
    "I'm TrashBoy 3000",
    "Happy birthday!",
  };
  if (isDanisBirthday()) {
    display.showIntro(lines, 3);
    SoundEffects::playHappyBirthday(SPEAKER_PIN);
  } else {
    display.showIntro(lines, 2);
    delay(2000);
  }

  // TODO do something on first run?

  // h min sec day mon y
  //Now::setDateTime(23, 59, 40, 8, 8, 18);
  //dataStore.eepromWrite();

  numberOfChannels = dataStore.readChannelNames(channelNames);
  numberOfDates = dataStore.readDates(dates, MAX_NUMBER_OF_DATES_IN_MEMORY);
  ble.setCurrentList(&dates[0], numberOfDates);

  // enable BTLE
  digitalWrite(BTLE_POWER_PIN, HIGH);
  ble.start();
}

void loop() {

  ble.readDataIfAvailable();

  if (true == performAck) {
    performingAck = true;
    performAck = false;
    doPerformAck();
    performingAck = false;
  }

  if (Now::isMidnight() && !lockMidnight) {
    lockMidnight = true;
    int numberOfAckDates = 0;
    TrashOutDate ackDates[MAX_NUMBER_OF_DATES_IN_MEMORY];
    for (int i = 0; i < numberOfDates; i++) {
      TrashOutDate *date = dates + i;
      if (date->isAck()) {
        memcpy(&ackDates[i], date, sizeof(TrashOutDate));
        numberOfAckDates++;
      }
    }
    numberOfDates = dataStore.readDates(dates, MAX_NUMBER_OF_DATES_IN_MEMORY);
    for (int i = 0; i < numberOfDates; i++) {
      TrashOutDate *date = dates + i;
      for (int j = 0; j < numberOfAckDates; j++) {
        TrashOutDate *ackDate = ackDates + j;
        if (date->isSameDate(ackDate)) {
          date->ack();
          break;
        }
      }
    }
  }
  if (!Now::isMidnight() && lockMidnight) {
    lockMidnight = false;
  }

  display.drawDateTimeLine();
  if (hasActiveDate() && millis() - lastAlaramMillis > 2000) {
    display.drawNextDates(dates, numberOfDates, channelNames, numberOfChannels, true);
    digitalWrite(FLASH_LED_PIN, HIGH);
    if (Now::getHour() >= ALARM_ENABLED_FROM_HOUR && Now::getHour() < ALARM_ENABLED_TO_HOUR) {
      SoundEffects::playAlarmSignal(SPEAKER_PIN);
    } else {
      delay(60);
    }
    delay(140);
    digitalWrite(FLASH_LED_PIN, LOW);
    lastAlaramMillis = millis();
  } else {
    display.drawNextDates(dates, numberOfDates, channelNames, numberOfChannels, false);
  }
}

bool isDanisBirthday(void) {
  if (24 >= Now::getDay() && 8 == Now::getMonth() && 18 == Now::getYear2kBased()) {
    return true;
  }
  return false;
}

bool hasActiveDate(void) {
  for (int i = 0; i < numberOfDates; i++) {
    TrashOutDate *date = dates + i;
    if (!date->isAck() && (Now::isToday(date) || Now::isTomorrow(date))) {
      return true;
    }
  }
  return false;
}

void ackButtonInterruptTriggered() {
  if ((millis() - debounceLastAckBtnAction) > 500) {
    debounceLastAckBtnAction = millis();
    if (!performingAck) {
      performAck = true;
    }
  }
}

void doPerformAck(void) {
  for (int i = 0; i < numberOfDates; i++) {
    TrashOutDate *date = dates + i;
    if (!date->isAck() && (Now::isToday(date) || Now::isTomorrow(date))) {
      date->ack();
      return;
    }
  }
  SoundEffects::playErrorTone(SPEAKER_PIN);
}
