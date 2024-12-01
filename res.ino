#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte pixel0[] = { B11111, B00000, B00000, B00000, B00000, B00000, B00000, B11111 };
byte pixel1[] = { B11111, B00000, B10000, B10000, B10000, B10000, B00000, B11111 };
byte pixel2[] = { B11111, B00000, B11000, B11000, B11000, B11000, B00000, B11111 };
byte pixel3[] = { B11111, B00000, B11100, B11100, B11100, B11100, B00000, B11111 };
byte pixel4[] = { B11111, B00000, B11110, B11110, B11110, B11110, B00000, B11111 };
byte pixel5[] = { B11111, B00000, B11111, B11111, B11111, B11111, B00000, B11111 };

byte customCharStart[] = { B00001, B00011, B00010, B00010, B00010, B00010, B00011, B00001 };
byte customCharEnd[] = { B10000, B11000, B01000, B01000, B01000, B01000, B11000, B10000 };

const char* daysOfTheWeek[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, pixel0);
  lcd.createChar(1, pixel1);
  lcd.createChar(2, pixel2);
  lcd.createChar(3, pixel3);
  lcd.createChar(4, pixel4);
  lcd.createChar(5, pixel5);
  lcd.createChar(6, customCharStart);
  lcd.createChar(7, customCharEnd);

  displayLoadingScreen();

  if (!rtc.begin()) {
    lcd.clear();
    lcd.print("RTC error!");
    while (1)
      ;
  }
  displayStartupMessage();
}

void loop() {
  displayDateTime();
}

void displayLoadingScreen() {
  lcd.setCursor(0, 0);
  lcd.print("Loading...");
  lcd.setCursor(0, 1);
  lcd.write(byte(6));  
  lcd.setCursor(15, 1);
  lcd.write(byte(7));  
  for (int i = 0; i <= 70; i++) {  
    drawProgressBar(i);
    if (i < 45) {
      delay(15);  
    } else {
      delay(50);  
    }
  }
  delay(1000);  
  lcd.clear();
}

void drawProgressBar(int progress) {
  lcd.setCursor(1, 1); 
  for (int i = 2; i <= 15; i++) { 
    int pixelState = progress - ((i - 2) * 5);  
    if (pixelState >= 5) {
      lcd.write(byte(5));  
    } else if (pixelState > 0) {
      lcd.write(byte(pixelState));  
    } else {
      lcd.write(byte(0));  
    }
  }
}

void displayStartupMessage() {
  lcd.setCursor(0, 0);
  lcd.print(" Nihao Thao");
  delay(2000);
  lcd.clear();
}

void displayDateTime() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print(" - ");
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  delay(1000);
}
