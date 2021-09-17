
// this is better alternative to software serial
#include <NeoSWSerial.h>

// Neo GPS library
#include <NMEAGPS.h>

static const int RXPin = 4, TXPin = 3;
NeoSWSerial gpsPort(RXPin, TXPin);
static const uint32_t GPSBaud = 9600;

NMEAGPS gps;
gps_fix fix;
uint8_t fixCount = 0;

//Trigger setup
unsigned int ADCValue;
double Voltage;
double Vcc;


#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR   0x3C

// reset pin not used on 4-pin OLED module
Adafruit_SSD1306 display(-1);  // -1 = no reset pin


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gpsPort.begin(GPSBaud);

  Serial.println(F("GPS Loading....."));
  Serial.println(F("Obtain and print lati,logi,speed,date and time"));
  Serial.println();
  
  // initialize and clear 1306 OLED display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

}

void loop() {
  // put your main code here, to run repeatedly
  // 1306 SSD
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(13, 1);
  display.print("M + P INT");
  display.setTextSize(1);
  display.setCursor(15, 10);
  display.print("SPEED (MPH)");
  display.display();

  // main GPS code
  while (gps.available( gpsPort )) {
    fix = gps.read();
    // once every 1 seconds ...
    if (++fixCount >=1) {
      displayInfo();
      // I added this function to read trigger voltage
      // trigger_volt();
      displayspeed();
      fixCount = 0;
    }
  }

  if ((gps.statistics.chars < 10) && (millis() > 5000)) {
    Serial.println( F("NO GPS detected: check wiring.") );
    while(true);
  }

}

void displayInfo()
{

  Serial.print(F("  Latitude = ")); 
  if (fix.valid.location) {
    Serial.println( fix.latitude(), 5 );
    //Serial.print( ',' );
    //Serial.print( fix.longitude(), 5 );
  } else {
    Serial.println(F("INVALID"));
  }
  
  Serial.print(F("  Longitude = ")); 
  if (fix.valid.location) {
    //Serial.print( fix.latitude(), 5 );
    //Serial.print( ',' );
    Serial.println( fix.longitude(), 5 );
  } else {
    Serial.println(F("INVALID"));
  }

  Serial.print(F("  Speed = ")); 
  if (fix.valid.speed) {
    Serial.println(fix.speed_mph());
    //Serial.print(F(" MPH "));
  } else {
    Serial.println(F("INVALID"));
  }

  // Shift the date/time to local time
  NeoGPS::clock_t localSeconds;
  NeoGPS::time_t  localTime;
  if (fix.valid.date && fix.valid.time) {
    using namespace NeoGPS; // save a little typing below...

    localSeconds = (clock_t) fix.dateTime; // convert structure to a second count
    localSeconds -= 5 * SECONDS_PER_HOUR; //+ 30 * SECONDS_PER_MINUTE; // shift timezone
    localTime = localSeconds;              // convert back to a structure
  }

  Serial.print(F("  Date = "));
  if (fix.valid.date) {
    Serial.print(localTime.month);
    Serial.print('/');
    Serial.print(localTime.date);
    Serial.print('/');
    Serial.print(localTime.year);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.println();
  Serial.print(F("  Time = "));
  if (fix.valid.time) {
    Serial.print(localTime.hours);
    Serial.print(':');
    if (localTime.minutes < 10) Serial.print('0');
    Serial.print(localTime.minutes);
    Serial.print(':');
    if (localTime.seconds < 10) Serial.print(F("0"));
    Serial.print(localTime.seconds);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.println();
  trigger_volt();
  Serial.println();
  
}

long readVcc() 
{
  // this is a function defintion - outside of void loop
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

void trigger_volt()
{
// corrected analog input based on power supply voltage correction
  Vcc = readVcc()/1000.0;
  ADCValue = analogRead(0);
  Voltage = (ADCValue / 1024.0) * Vcc; //For accurate measurements, divide the ADC value by the ADC maximum value and multiply by the resultant ADV value.
  Serial.print("  GPS Trigger volt = "); Serial.println(Voltage);
}


  void displayspeed()
{
  if (fix.valid.speed)
  //if (gps.speed.isValid())
  {
    display.setTextSize(1.75);
    display.setCursor(20, 20);
    display.print(fix.speed_mph());
    display.print(F(" MPH "));
    //display.print(gps.speed.mph());
    display.display();
  }
  else
  {
    display.setTextSize(1);
    display.setCursor(35, 40);
    display.print("No Data!!!");
    display.display();
  }
  delay(100);
}
