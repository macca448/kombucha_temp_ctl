#define BLYNK_FIRMWARE_VERSION        "1.3.0"
#define BLYNK_PRINT Serial
#include "credientials.h"
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp32_SSL.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Blynk/BlynkDateTime.h>
#include <OneWire.h>
#include <DallasTemperature.h> 

// Constants
#define LED       2
#define ONE_WIRE  14
#define RELAY     13
#define FAN       27 
#define BUTTON    0
#define DB        50
#define MIN_TEMP  26.00
#define MAX_TEMP  27.00
#define FAN_MID_T 30.00
#define FACTOR_L  5
#define FACTOR_H  7
#define TEN_DAYS  0xD2F00
#define RELAY_OFF 1
#define RELAY_ON  0
#define FAN_OFF   0
#define FAN_SLOW  120
#define FAN_ON    255
#define BLINK     10

//Class Objects
Preferences prefs;
OneWire oneWire(ONE_WIRE);
DallasTemperature sensors(&oneWire);
DeviceAddress heatProbe, chamberProbe;
BlynkTimer timer;
WidgetRTC rtc;

//Variables
bool     btnPress = false, btnState, lastBtnState = 1, relayState, blink,
         blynkLed, lastRelayState, start, ledState, lastLedState,
         blynkSuccess = 0, wifiSuccess = 0;
double   heaterTemp, chamberTemp;
uint8_t  lastFanSpeed, lastSecond, lastMinute, lastHour, blinkCount, minCount, factor = FACTOR_L;
int      fanSpeed;
uint32_t totalHeatTime = 0, lastHeatTime = 0, currentMillis = 0, lastMillis = 0,
         previousTime = 0;
char     scobyEnd[18], ht_buff[10], startStamp[18];

//My Headers (Tabs)
#include "the_calls.h"
#include "network.h"
#include "the_setup.h"
#include "main_loop.h"
