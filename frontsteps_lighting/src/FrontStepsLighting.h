/*
Philip Barclay
26 July 2016
*/

#include <Arduino.h>
#include <Timezone.h>

#define DEVICE_FRIENDLY_NAME  ("Frontsteps lighting")
#define FIRMWARE_VERSION      ("1.0.1")

// Logic of motion sensors
#define MOTION_SENSOR       (HIGH)
#define MOTION_YES          (true)
#define MOTION_NO           (false)
#define ON                  (true)
#define OFF                 (false)

// Error detection
#define MQTT_TX_FAIL_LIMIT  (10)
#define WDT                 (WDTO_8S)

enum partsOfDay_t {
  daytime, // main portion of day (lots of light)
  evening, // transition into night (dim light)
  night,   // main portion of night (no light)
  morning  // transition into day (dim light)
};

enum lightsLevel_t {
  off,
  ambient,
  bright
};

TimeChangeRule nzdt = NZDT_RULE;
TimeChangeRule nzst = NZST_RULE;

int serial_putchar(char c, FILE* f);
byte readI2CRegister(byte i2c_address, byte reg);
int percent2LEDInt(int p);
void printTime(byte h, byte m);
void printDateTime(time_t t);

void timeToTmpBuf(uint16_t minsAfterMidnight);
void uint16ToTmpBuf(uint16_t value);
void uint32ToTmpBuf(uint32_t value);

uint16_t hourMinToMinAfterMidnight(uint16_t t);
//uint16_t minAfterMidnightToHourMin(uint16_t m);

time_t getUtcFromNtp();
void sendNTPpacket(IPAddress &address);

void timeOfDayAlarm();
void sunriseSunsetAlarm();
void updateLights();

boolean mqttConnect();
void mqttSubscribe(const char* name);
void mqttPublish(const char* name, const char* payload);
void mqttPublish(const char* name, const char* payload, bool retained);
void mqttCallback(char* topic, byte* payload, unsigned int length);

void publishConfigAndSettings();
void publishSunriseSunset();
void publishPortionOfDay(const char* portion);
