#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <DHT.h>

#define HISTORY_SETS 2
#define HISTORY_SIZE 20

DHT dht22(2, DHT22);
DHT dht11(3, DHT11);
LiquidCrystal_I2C lcd(0x27, 20, 4);

int history[HISTORY_SETS][HISTORY_SIZE];
int plot_array[20];
unsigned int iteration = 0;

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
  initPlot();
}

void loop() {
  int h;
  int max_value;
  int min_value;
  ++iteration;
  lcd.clear();
  dhtValues(dht22, 2, 0, history[0]);
  printHistory(history[0]);
  max_value = -32000;
  min_value = 32000;
  for (byte i = 0; i < HISTORY_SIZE; i++) {
    if (history[0][i] > max_value) max_value = history[0][i];
    if (history[0][i] < min_value) min_value = history[0][i];
  }
  drawPlot(history[0], 0, 1, 20, 1, min_value, max_value, 0);


  h = dhtValues(dht11, 1, 2, history[1]);
  printHistory(history[1]);
  max_value = -32000;
  min_value = 32000;
  for (byte i = 0; i < HISTORY_SIZE; i++) {
    if (history[1][i] > max_value) max_value = history[1][i];
    if (history[1][i] < min_value) min_value = history[1][i];
  }
  drawPlot(history[1], 0, 3, 20, 1, min_value, max_value, h);
  
  delay(3000);
}

int dhtValues(DHT &sensor, int index, int lcdLine, int* history_set) {
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

  if(iteration % 1 == 0) {
    pushToHistory(history_set, h);
  }

  float hic = sensor.computeHeatIndex(t, h, false);

  lcd.setCursor(0, lcdLine);
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
  return h;
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

void initPlot() {
  byte row8[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row7[8] = {0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row6[8] = {0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row5[8] = {0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row4[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row3[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
  byte row2[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
  byte row1[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
  lcd.createChar(0, row8);
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
}

void drawPlot(int* plot_array, byte pos, byte row, byte width, byte height, int min_val, int max_val, int fill_val) {
//  for (byte i = 0; i < width; i++) {
//    plot_array[i] = plot_array[i + 1];
//  }
//  fill_val = constrain(fill_val, min_val, max_val);
//  plot_array[width] = fill_val;
  for (byte i = 0; i < width; i++) {                  // каждый столбец параметров
    byte infill, fract;
    // найти количество целых блоков с учётом минимума и максимума для отображения на графике
    infill = floor((float)(plot_array[i] - min_val) / (max_val - min_val) * height * 10);
    fract = (infill % 10) * 8 / 10;                   // найти количество оставшихся полосок
    infill = infill / 10;
    for (byte n = 0; n < height; n++) {     // для всех строк графика
      if (n < infill && infill > 0) {       // пока мы ниже уровня
        lcd.setCursor(pos + i, (row - n));        // заполняем полными ячейками
        lcd.write(0);
      }
      if (n >= infill) {                    // если достигли уровня
        lcd.setCursor(pos + i, (row - n));
        if (fract > 0) lcd.write(fract);          // заполняем дробные ячейки
        else lcd.write(16);                       // если дробные == 0, заливаем пустой
        for (byte k = n + 1; k < height; k++) {   // всё что сверху заливаем пустыми
          lcd.setCursor(pos + i, (row - k));
          lcd.write(16);
        }
        break;
      }
    }
  }
}
