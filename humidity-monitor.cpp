#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <DHT.h>

#define HISTORY_SETS 2
#define HISTORY_SIZE 20

DHT dht22(2, DHT22);
DHT dht11(3, DHT11);
LiquidCrystal_I2C lcd(0x27, 20, 4);

int history[HISTORY_SETS][HISTORY_SIZE];
int iteration = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT temperature and humidity monitor"));
  delay(50);
  dht22.begin();delay(30);
  dht11.begin();delay(30);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(F("Initializing..."));
  delay(500);
  
  for(int i=0;i<3;i++) {
    lcd.setCursor(0,i+1);
    lcd.print(F("..."));
    delay(500);
  }

  initHistory();
}

void loop() {
  lcd.clear();
  dhtValues(dht22, 2, 0, history[0]);
  printHistory(history[0]);
  dhtValues(dht11, 1, 2, history[1]);
  printHistory(history[1]);
  
  lcd.setCursor(random(0, 20), 1);
  lcd.print("*");
  lcd.setCursor(random(0, 20), 3);
  lcd.print("*");
  delay(3000);
}

void dhtValues(DHT &sensor, int index, int line, int* history_set) {
  float h = sensor.readHumidity();
  float t = sensor.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.setCursor(6, 0);
    lcd.print(F("Failed!"));
    for (int i=0;i<4;i++) {
      lcd.setCursor(random(0, 20), i);
      lcd.print(F("X"));
    }
    delay(2000);
    return;
  }

  pushToHistory(history_set, h);

  float hic = sensor.computeHeatIndex(t, h, false);

  lcd.setCursor(0, line);
  lcd.print(F("D"));
  lcd.print(index);
  lcd.print(F("  "));
//  char float_str[8];
//  dtostrf(t,4,2,float_str);
//  lcd.print(sprintf("%0.1sC", float_str));
  lcd.print(t, 1);
  lcd.print(F(" C   "));
  lcd.print(h, 1);
  lcd.print(F("%"));

  Serial.print(F("DHT"));
  Serial.print(index);
  Serial.print(index);
  Serial.print(F(": Temperature: "));
  Serial.print(t, 1);
  Serial.print(F("°C Humidity: "));
  Serial.print(h, 1);
  Serial.print(F("% Heat index: "));
  Serial.print(hic);
  Serial.print(F("\n"));
}

void initHistory() {
  for(int j=0; j<HISTORY_SETS;j++) {
    for(int i=0; i<HISTORY_SIZE;i++) {
      history[j][i] = 0;
    }
  }
}

void printHistory(int* history_set) {
  Serial.print(F("History: "));
  for(int i=0; i<HISTORY_SIZE;i++) {
    Serial.print(history_set[i]);
    Serial.print(F(" "));
  }
  Serial.println("");
}

void pushToHistory(int* &history_set, int value) {
  for(int i=0; i<HISTORY_SIZE;i++) {
    if(i < HISTORY_SIZE-1) {
      history_set[i] = history_set[i+1];
    }
  }
  history_set[HISTORY_SIZE-1] = value;
}
