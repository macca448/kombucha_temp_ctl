#define BLYNK_FIRMWARE_VERSION        "1.2.0"
#define BLYNK_PRINT Serial
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

#include "credientials.h"

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
#define FACTOR_L  4
#define FACTOR_H  6
#define TEN_DAYS  0xD2F00
#define RELAY_OFF 1
#define RELAY_ON  0
#define FAN_OFF   0
#define FAN_ON    255
#define BLINK     10

//Class Objects
OneWire oneWire(ONE_WIRE);
DallasTemperature sensors(&oneWire);
DeviceAddress heatProbe, chamberProbe;
BlynkTimer timer;
WidgetRTC rtc;

//Variables
bool     btnPress = false, btnState, lastBtnState = 1, relayState, blink,
         blynkLed, lastRelayState, start, ledState, lastLedState;
double   heaterTemp, chamberTemp;
uint8_t  fanSpeed, lastFanSpeed, lastSecond, blinkCount, factor;
uint32_t totalHeatTime = 0, lastHeatTime = 0, currentMillis = 0, lastMillis = 0,
         previousTime = 0;
char     scobyEnd[18], ht_buff[10], startStamp[18];

//My Headers (Tabs)
#include "the_calls.h"
#include "the_setup.h"
#include "main_loop.h"
