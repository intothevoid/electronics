#include <Arduino.h>
#include <Wire.h>  // for I2C communication with RTC module
#include <RTClib.h>  // for date and time handling

RTC_DS3231 rtc;  // define the RTC object
time_t time_t_values[] = {Time(07, 30, 0), Time(09, 30, 0), Time(12,0,0), Time(14,0,0)};  // example time values

void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("RTC not connected!");
  } else {
    Serial.println("RTC connected!");
  }
}

void loop() {
  Time now = rtc.now();

  for (int i = 0; i < sizeof(time_t_values) / sizeof(time_t); i++) {
    Time compareTime = time_t_values[i];
    if (now == compareTime) {
      Serial.println("Matched time!");
      // Activate servo and move it from 0-180 degrees
      analogWrite(SERVO_PIN, 0);  // initialize servo to 0 degrees
      delay(2000);
      for (int deg = 1; deg <= 180; deg++) {
        analogWrite(SERVO_PIN, map(deg, 0, 180, 0, 255));
        delay(10);
      }
      break;
    }
  }
}

