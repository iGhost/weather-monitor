#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

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
    Serial.println(F("DHTxx test!"));
    dht.begin();
    //Define the LCD as 16 column by 2 rows 
    lcd.begin(20,4);
    
    //Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    
    lcd.setCursor(0,0);
    
    lcd.print("Initializing...");
    delay(500);
    
    lcd.setCursor(0,1);
    lcd.print("...");
    delay(500);

    lcd.setCursor(0,2);
    lcd.print("...");
    delay(500);

    lcd.setCursor(0,3);
    lcd.print("done!");
    delay(500);
}

void loop(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.setCursor(6, 0);
    lcd.print("Failed!");
    delay(2000);
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C Heat index: "));
  Serial.print(hic);
  Serial.print(F("\n"));

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(t);
  lcd.print(" C  ");
  lcd.print(h);
  lcd.print("%");

  lcd.setCursor(random(0, 20), 1);
  lcd.print(".");
  lcd.setCursor(random(0, 20), 2);
  lcd.print(".");
  lcd.setCursor(random(0, 20), 3);
  lcd.print(".");
  delay(3000);
}
