// This code simulates a button pusher that activates a servo motor at specific times of the day.
// The times are defined in the time_t_values array, and the code compares the current time with these values.
// If a match is found, the servo is activated and moves from 0 to 180 degrees.

#include <Arduino.h>
#include <Wire.h>   // for I2C communication with RTC module
#include <RTClib.h> // for date and time handling
#include <time.h>

RTC_DS3231 rtc;          // define the RTC object
const int SERVO_PIN = 8; // define the pin for the servo motor

// The times at which the servo should be activated i.e. button pushed
// The year, month and day are set to the current date, and the time is set to the desired time
DateTime time_t_values[] = {
    DateTime(2021, 1, 1, 7, 30, 0),
    DateTime(2021, 1, 1, 9, 0, 0),
    DateTime(2021, 1, 1, 12, 0, 0),
    DateTime(2021, 1, 1, 14, 0, 0),
};

void toggleButton()
{
  // Set servo to 127 degrees and then back to 0 degrees
  analogWrite(SERVO_PIN, 127);
  delay(100);
  analogWrite(SERVO_PIN, 0); // initialize servo to 0 degrees
  delay(100);
}

void setup()
{
  Serial.begin(9600);
  if (!rtc.begin())
  {
    Serial.println("RTC not connected!");
  }
  else
  {
    Serial.println("RTC connected!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Iterate through the time_t_values array and update the year, month and day to the current date
  // This is done to ensure that the comparison is accurate
  // Leave the time values as they are
  DateTime now = rtc.now();
  for (size_t i = 0; i < sizeof(time_t_values) / sizeof(time_t); i++)
  {
    time_t_values[i] = DateTime(now.year(), now.month(), now.day(), time_t_values[i].hour(), time_t_values[i].minute(), time_t_values[i].second());
  }

  // Initialize the servo motor
  pinMode(SERVO_PIN, OUTPUT);

  delay(100);
  analogWrite(SERVO_PIN, 0); // initialize servo to 0 degrees
  Serial.println("Setup complete!");
}

void loop()
{
  DateTime now = rtc.now();

  for (size_t i = 0; i < sizeof(time_t_values) / sizeof(time_t); i++)
  {
    DateTime compareTime = time_t_values[i];
    if (now == compareTime)
    {
      Serial.println("Matched time!");
      toggleButton();
      break;
    }
  }
}

