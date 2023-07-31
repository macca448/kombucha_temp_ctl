## Kombucha Temperature Control Brewing with Blynk Monitoring
###  

### Items used:
  1.  ESP32
  2.  Relay Module
  3.  AC Lamp inside a tin can
  4.  Old Cooler (Esky or Chilli Bin)
  5.  2 x DS18B20 Temperature sensors
  6.  1 x 50mm Radial Fan
  7.  AC light switch for AC isolation.
  8.  misc wire and hardeware

## How it Works
`Heat ON temp is when chamber sensor temp is 26 degC.`

`Heat OFF temp is when chamber temp is 27 degC.`

`Fan HIGH speed when heater sensor temp above 30 degC else LOW speed.`

`First brew duration is typically 10 days subject to "TANG" taste test.`

`Second ferment is not less than 4 days but I liketwo weeks or more for more "FIZZ".`

`OTA has been implemented so Wireless updates are possible.`

`a 75W incandesecent lamp is all that was needed for the heater.`

#### What it looks like
![kombucha brewing chamber](https://github.com/macca448/kombucha_temp_ctl/blob/main/assets/kombucha_brewing.jpg)
