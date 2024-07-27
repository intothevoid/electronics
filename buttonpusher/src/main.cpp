// This code simulates a button pusher that activates a servo motor at specific times of the day.
// The times are defined in the time_t_values array, and the code compares the current time with these values.
// If a match is found, the servo is activated and moves from 0 to 127 degrees.
// Your servo angle may vary, so you can adjust the angle in the toggleButton function.

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <time.h>
#include <Servo.h>

RTC_DS3231 rtc;          // define the RTC object
Servo servo;             // define the servo object

const int SERVO_PIN = 8; // define the pin for the servo motor
void toggleButton();     // function to toggle the button

// The times at which the servo should be activated i.e. button pushed
DateTime time_t_values[] = {
    DateTime(2021, 1, 1, 7, 30, 0),
    DateTime(2021, 1, 1, 9, 0, 0),
    DateTime(2021, 1, 1, 12, 0, 0),
    DateTime(2021, 1, 1, 14, 0, 0),
};

void setup()
{
  Serial.begin(9600);
  Serial.println("Initialising Arduino...");

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  servo.attach(SERVO_PIN); // attach the servo to the pin
  servo.write(0);          // initialize servo to 0 degrees
}

void loop()
{
  DateTime now = rtc.now();

  // Set time to 9.5 hours ahead of UTC
  time_t t = now.unixtime() + 34200;

  // Update now to the new time
  now = DateTime(t);

  Serial.print("Current time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  for (DateTime time_t : time_t_values)
  {
    Serial.print("Checking time: ");
    Serial.print(time_t.hour(), DEC);
    Serial.print(':');
    Serial.print(time_t.minute(), DEC);
    Serial.print(':');
    Serial.print(time_t.second(), DEC);
    Serial.print("\t");

    if (now.hour() == time_t.hour() && now.minute() == time_t.minute())
    {
      toggleButton();
    }
  }
  Serial.println("");

  delay(60000); // wait for a minute before checking again
}

void toggleButton()
{
  // Set servo to 127 degrees and then back to 0 degrees
  Serial.println("\nActivating servo...");
  servo.write(127);
  delay(1000);
  servo.write(0); // initialize servo to 0 degrees
  delay(1000);
}