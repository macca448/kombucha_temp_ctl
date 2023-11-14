
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

  //WiFi
  wifiSuccess = connectToWiFi();
    if(wifiSuccess == true){
      Serial.println("WiFi Connection Sucess..........\n");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      blynkSuccess = connectToBlynk();
        if(blynkSuccess){
          Serial.println("Ready");
        }
    }else{
      blynkSuccess = false;
      minCount = 0;
      Serial.println("WiFi Connection Failure......We will Update the data anyway\n");
  }

  // Five Second Blynk Timer function.
  timer.setInterval(5000UL, cloudPush);
  
  // Small delay so the Time Sync happens and let things settle
  delay(3000);

  prefsRead();

  lastHour = hour();
  lastMinute = minute();
  currentMillis = millis();
  
}
