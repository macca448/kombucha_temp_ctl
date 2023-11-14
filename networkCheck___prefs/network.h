//WiFi and Blynk connect and status checking to reconnect if timed out.
/*
• 0 : WL_IDLE_STATUS when Wi-Fi is in process of changing between statuses
• 1 : WL_NO_SSID_AVAILin case configured SSID cannot be reached
• 3 : WL_CONNECTED after successful connection is established
• 4 : WL_CONNECT_FAILED if connection failed
• 6 : WL_CONNECT_WRONG_PASSWORD if password is incorrect
• 7 : WL_DISCONNECTED if module is not configured in station mode */

void setOTA(void){
    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
      ESP.restart();
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
  return;
}

bool connectToWiFi(){
    WiFi.mode(WIFI_STA);
    Serial.println("Connecting to WiFi..........\n");
    WiFi.begin(ssid, password);
    int i = 0;
    while(WiFi.status() != WL_CONNECTED){
        i++;
        delay(500);
        if(i >= 60){
            return false;
        }
    }
    setOTA();
    return true;
}

bool connectToBlynk(){
  Blynk.config(auth);
  bool result = Blynk.connect();
    if(result){
      Serial.println("\n");
      Serial.println("Blynk Connected............\n");
      // Sync RTC interval in seconds (30 minutes)
      setSyncInterval(30 * 60); 
      return true;
    }else{
      Serial.println("Blynk Failed to connect but we will Update the data anyway\n");
      return false;
    }
}

void checkNetworkStatus(void){
  if(WiFi.status() != WL_CONNECTED){
     wifiSuccess == false;
     minCount = 0; 
  }
  if(Blynk.connected() == false){
    bool result = connectToBlynk();
     if(result){
        blynkSuccess = true; 
     }else{
        blynkSuccess = false;
     }
  }
  return;  
}
