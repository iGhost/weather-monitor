#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "DHT.h"

DHT dht(2, DHT22);
DHT dht11(3, DHT11);

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
 {
    Serial.begin(9600);
    Serial.println(F("DHT and DME test!"));
    delay(50);
    dht.begin();delay(30);
    dht11.begin();delay(30);
    lcd.init();
    lcd.backlight();
    lcd.clear();

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
  dhtValues(dht,   2, 0);
  dhtValues(dht11, 1, 2);
  
  lcd.setCursor(random(0, 20), 1);
  lcd.print("*");
  lcd.setCursor(random(0, 20), 3);
  lcd.print("*");
  delay(3000);
}

void dhtValues(DHT &sensor, int index, int line) {
  float h = sensor.readHumidity();
  float t = sensor.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.setCursor(6, 0);
    lcd.print(F("Failed!"));
    for (int i=0;i<4;i++) {
      lcd.setCursor(random(0, 20), i);
      lcd.print("X");
    }
    delay(2000);
    return;
  }

  float hic = sensor.computeHeatIndex(t, h, false);

  lcd.setCursor(0, line);
  lcd.print(F("D"));
  lcd.print(index);
  lcd.print(F(" "));
//  char float_str[8];
//  dtostrf(t,4,2,float_str);
//  lcd.print(sprintf("%0.1sC", float_str));
  lcd.print(t);
  lcd.print(F(" C  "));
  lcd.print(h);
  lcd.print(F("%"));

  Serial.print(F("DHT"));
  Serial.print(index);
  Serial.print(index);
  Serial.print(F(": Temperature: "));
  Serial.print(t);
  Serial.print(F("°C Humidity: "));
  Serial.print(h);
  Serial.print(F("% Heat index: "));
  Serial.print(hic);
  Serial.print(F("\n"));
}