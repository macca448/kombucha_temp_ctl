BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}

void heaterSwitching(void){
  //LAMP Heater Switching - One Degree Chamber range
  if(chamberTemp < MIN_TEMP){
    relayState = RELAY_ON;
    blynkLed = 1;
    ledState = 1;
  }else if(chamberTemp >= MAX_TEMP){
    relayState = RELAY_OFF;
    blynkLed = 0;
    ledState = 0;
  }
  return;
}

void setSpeedFactor(void){
  if(heaterTemp >= FAN_MID_T){
    factor = FACTOR_H;
  }else{
    factor = FACTOR_L;
  }
  fanSpeed = (((double)(heaterTemp * factor) + 0.5));
    if(fanSpeed > FAN_ON){
      fanSpeed = FAN_ON;
    }else if(fanSpeed < FAN_SLOW){
      fanSpeed = FAN_SLOW;
    }
  return;
}

void getTimeNow(char* startStamp, int32_t start_epoch_val){
  time_t start_Now = time(NULL);
  start_Now = start_epoch_val;
  struct tm *start_tm;
  start_tm = localtime(&start_Now);
  char buff[20];
  strftime (buff,20,"%a %d-%b %H:%M", start_tm);
  strcpy(startStamp, buff);
}
void getScobyEnd(char* scobyEnd, int32_t start_epoch_val){
  time_t sNow = time(NULL);
  sNow = start_epoch_val;
  sNow += TEN_DAYS;
  struct tm *scoby_tm_end;
  scoby_tm_end = localtime(&sNow);
  char buff[18];
  strftime (buff,18,"%a %d-%b %H:%M", scoby_tm_end);
  strcpy(scobyEnd, buff);
}

double getTemp(DeviceAddress probe){
  double t = sensors.getTempC(probe);
  t = t + 0.05;                                 //Because we will round to one decimal place
  return t;
}

//Zero All Data
void zeroData(void){
  digitalWrite(RELAY, RELAY_OFF);
  blynkLed = 0;
  blink = true;
  start = false;
  lastHour = hour();
  lastMinute = minute();
  analogWrite(FAN, FAN_OFF);
  fanSpeed = FAN_OFF;
  totalHeatTime = 0;
  char buff[18] = " ";
  strcpy(scobyEnd, buff);
  strcpy(startStamp, buff);
  char buff1[10] = " ";
  strcpy(ht_buff, buff1);
  prefs.putLong("start_epoch_val", 0);
  prefs.end();
  return;
}

void prefsRead(void){
  prefs.begin("my-app", false);
  int32_t start_epoch_val = prefs.getLong("start_epoch_val", 0);
    if(start_epoch_val > 0){
      getTimeNow(startStamp, start_epoch_val);
      getScobyEnd(scobyEnd, start_epoch_val);
    }else{
      zeroData();
    }
    prefs.end();
    return;
}
void prefsWrite(int32_t start_epoch_val){
  prefs.begin("my-app", false);
  prefs.putLong("start_epoch_val", start_epoch_val);
  prefs.end();
  return;
}

void updateStamps(void){
  //Create Start and End Time Stamps 
  int32_t start_epoch_val = now();
  getTimeNow(startStamp, start_epoch_val);
  getScobyEnd(scobyEnd, start_epoch_val);
  prefsWrite(start_epoch_val);
  return;
}

void cloudPush(void){
   
   //Heat Time is 10sec increments when ON
    if(blynkLed){
      totalHeatTime += (currentMillis - lastMillis);
      uint32_t allSeconds = totalHeatTime / 1000;
      uint8_t heatHours   = allSeconds / 3600;
      uint16_t secsRemain = allSeconds % 3600;
      uint8_t heatMinutes = secsRemain / 60;
      uint8_t heatSeconds = secsRemain % 60;
      sprintf(ht_buff, "%02u:%02u:%02u", heatHours, heatMinutes, heatSeconds);
      lastMillis = currentMillis;
    }
  if(blynkSuccess){
    Blynk.virtualWrite(V0, ht_buff);
    Blynk.virtualWrite(V1, chamberTemp);
    Blynk.virtualWrite(V2, scobyEnd);
    Blynk.virtualWrite(V3, fanSpeed);
    Blynk.virtualWrite(V4, startStamp);
    Blynk.virtualWrite(V5, blynkLed);
    Blynk.virtualWrite(V8, heaterTemp);
  }
}

BLYNK_WRITE(V6){      //Scoby Start
  uint8_t pinValue = param.asInt();
    if(pinValue == 1){
      updateStamps();
    }
}

BLYNK_WRITE(V7){      //Zero All Data Streams
  uint8_t pinValue = param.asInt();
    if(pinValue == 1){
      prefs.begin("my-app", false);
      zeroData();
    }
}
