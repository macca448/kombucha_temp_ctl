
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  analogWrite(FAN, FAN_OFF);
  digitalWrite(RELAY, RELAY_OFF);

  //Start Dallas Sensors
  Serial.print("Locating devices....");
  sensors.begin();
  Serial.print(" Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" device");
  Serial.print("Parasite power: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  if (!sensors.getAddress(chamberProbe, 0)) Serial.println("Unable to find address for Device 1");
  if (!sensors.getAddress(heatProbe, 1)) Serial.print("Can't find Device 0 ");
  sensors.setResolution(chamberProbe, 9);
  Serial.print("Dev 0 Resolution: ");
  Serial.println(sensors.getResolution(chamberProbe), DEC);
  sensors.setResolution(heatProbe, 9);
  Serial.print("Dev 1 Resolution: ");
  Serial.println(sensors.getResolution(heatProbe), DEC);
  Serial.println();
  
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

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

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Start Blynk Connection
  Blynk.config(BLYNK_AUTH_TOKEN);
  bool result = Blynk.connect();
    if(result){
      Serial.println("Blynk Connected............\n");
    }else{
      Serial.println("Blynk Failed to connect........!!!!!!!!\n");
      delay(2000);
      ESP.restart();
    }
    
  // Sync RTC interval in seconds (30 minutes)
  setSyncInterval(30 * 60);
  
  // Five Second Blynk Timer function.
  timer.setInterval(5000UL, cloudPush);

  // Small delay so the Time Sync happens and let things settle
  delay(3000);

  currentMillis = millis();
  zeroData();
}
