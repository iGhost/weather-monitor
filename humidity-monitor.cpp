#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR          0x27        //Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN      3
#define En_pin             2
#define Rw_pin             1
#define Rs_pin             0
#define D4_pin             4
#define D5_pin             5
#define D6_pin             6
#define D7_pin             7

//Initialise the LCD
LiquidCrystal_I2C      lcd(I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup()
 {
    //Define the LCD as 16 column by 2 rows 
    lcd.begin (20,4);
    
    //Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    
    //goto first column (column 0) and first line (Line 0)
    lcd.setCursor(0,0);
    
    //Print at cursor Location
    lcd.print("Initializing...");
    delay(300);
    
    //goto first column (column 0) and second line (line 1)
    lcd.setCursor(0,1);
    lcd.print("...");
    delay(300);

    lcd.setCursor(0,2);
    lcd.print("...");
    delay(300);

    lcd.setCursor(0,3);
    lcd.print("done!");
    delay(500);
}

void loop(){
  char spinner[4] = {'|', '/', '-', '\\'};
  int j = 0;
  for(int i=0; i<19; i++) {
    lcd.clear();
    lcd.setCursor(9, 0);
    lcd.print(spinner[j]);
    j++;
    if (j > 3) { j=0; }
    lcd.setCursor(random(0, 20), 1);
    lcd.print(".");
    lcd.setCursor(random(0, 20), 2);
    lcd.print(".");
    lcd.setCursor(random(0, 20), 3);
    lcd.print(".");
    delay(150);
  }
}