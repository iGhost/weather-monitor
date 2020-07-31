#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

#define I2C_ADDR          0x27        //Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN      3
#define En_pin             2
#define Rw_pin             1
#define Rs_pin             0
#define D4_pin             4
#define D5_pin             5
#define D6_pin             6
#define D7_pin             7
#define DHTPIN             2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


//Initialise the LCD
LiquidCrystal_I2C      lcd(I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup()
 {
    Serial.begin(9600);
    Serial.println(F("DHT and DME test!"));
    delay(50);
    dht.begin();delay(30);
    bme.begin();delay(30); // 0x76, &Wire
    lcd.begin(20,4);
    
    //Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    
    lcd.setCursor(0,0);
    
    lcd.print(F("Initializing..."));
    delay(500);
    
    lcd.setCursor(0,1);
    lcd.print(F("..."));
    delay(500);

    lcd.setCursor(0,2);
    lcd.print(F("..."));
    delay(500);

    lcd.setCursor(0,3);
    lcd.print(F("done!"));
    delay(500);
}

void loop(){
  lcd.clear();
  dhtValues();
  bmeValues();
  
  lcd.setCursor(random(0, 20), 3);
  lcd.print("*");
  delay(3000);
}

void dhtValues() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.setCursor(6, 0);
    lcd.print(F("Failed!"));
    delay(2000);
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  lcd.setCursor(0, 0);
  lcd.print(F("D "));
//  char float_str[8];
//  dtostrf(t,4,2,float_str);
//  lcd.print(sprintf("%0.1sC", float_str));
  lcd.print(t);
  lcd.print(F(" C  "));
  lcd.print(h);
  lcd.print(F("%"));

  Serial.print(F("DHT: Temperature: "));
  Serial.print(t);
  Serial.print(F("°C Humidity: "));
  Serial.print(h);
  Serial.print(F("% Heat index: "));
  Serial.print(hic);
  Serial.print(F("\n"));
}

void bmeValues() {
  float bt = bme.readTemperature();
  float bh = bme.readHumidity();
  float bp = bme.readPressure() / 100.0F;
  float ba = bme.readAltitude(SEALEVELPRESSURE_HPA);

  lcd.setCursor(0, 1);
  lcd.print(F("B "));
  lcd.print(bt);
  lcd.print(F("C  "));
  lcd.print(bh);
  lcd.print(F("% "));

  lcd.setCursor(2, 2);
  lcd.print(bp);
  lcd.print(F("hPa  "));
  lcd.print(ba);
  lcd.print(F("m"));
  
  Serial.print(F("BME: Temperature = "));
  Serial.print(bt);

  Serial.print(F("°C Humidity = "));
  Serial.print(bh);

  Serial.print(F("% Pressure = "));
  Serial.print(bp);
  Serial.print(F(" hPa"));
  Serial.println("");

  Serial.print(F("Approx. Altitude = "));
  Serial.print(ba);
  Serial.println(F(" m"));
}
