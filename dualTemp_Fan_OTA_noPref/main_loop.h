//Main Loop
void loop() {
    ArduinoOTA.handle();
    Blynk.run();
    timer.run();

    currentMillis = millis();

    btnState = digitalRead(BUTTON);
    if(btnState != lastBtnState){
      btnPress = 1;
      previousTime = millis();
    }
    if(millis() - previousTime >= DB){
      if(btnPress){
        if(btnState == LOW){
          start = !start;
            if(start){              
              blink = false;
              updateStamps();
            }else{
              zeroData();
              blink = true;
            }
        }
        btnPress = 0;
      }
      if(blink){
        blinkCount++;
          if(blinkCount >= BLINK){
            ledState = !ledState;
            blinkCount = 0;
          }
      }
      previousTime = millis();
    }

    // Using the RTC every second we update our Temperature readings
    if(second() != lastSecond){
        sensors.requestTemperatures();          // Send the command to get temperatures
        heaterTemp = getTemp(heatProbe);        // Use a simple function to print out the data
        chamberTemp = getTemp(chamberProbe);    // Use a simple function to print out the data
        setSpeedFactor();    
        lastSecond = second();
    }

    if(start){
      heaterSwitching();
      
      //Fan Speed Control
      if(fanSpeed != lastFanSpeed){
          analogWrite(FAN, fanSpeed);
      }    
      //Relay Switching
      if(relayState != lastRelayState){
          digitalWrite(RELAY, relayState);
              if(blynkLed){
                  lastMillis = millis();
              }
      }
    }

    //Local LED State Indicator
    if(ledState != lastLedState){
        digitalWrite(LED, ledState);
    }

    //State Engine Updates
    lastBtnState = btnState;
    lastLedState = ledState;
    lastFanSpeed = fanSpeed;
    lastRelayState = relayState;
}
