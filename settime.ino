#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool isTimeSet = false;

// Cài đặt sẵn ngày, tháng, năm
const int presetYear = 2024;  // Năm mặc định
const int presetMonth = 12;   // Tháng mặc định
const int presetDay = 1;      // Ngày mặc định

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("RTC Error!");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("Set time: ...");
  Serial.println("Nhập thời gian theo định dạng HHMMSS:");
}

void loop() {
  if (!isTimeSet) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      input.trim();

      if (input.length() == 6 && input.toInt() > 0) {
        int hour = input.substring(0, 2).toInt();
        int minute = input.substring(2, 4).toInt();
        int second = input.substring(4, 6).toInt();

        if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60) {
          rtc.adjust(DateTime(presetYear, presetMonth, presetDay, hour, minute, second));
          isTimeSet = true;

          lcd.clear();
          lcd.print("Time Set Done!");
          delay(2000);
          lcd.clear();
        } else {
          Serial.println("Sai thời gian! Nhập lại (HHMMSS).");
        }
      } else {
        Serial.println("Sai định dạng! Nhập lại (HHMMSS).");
      }
    }
  } else {
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
    lcd.setCursor(0, 1);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    delay(1000);
  }
}
