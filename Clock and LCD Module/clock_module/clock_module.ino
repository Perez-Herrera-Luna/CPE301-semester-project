#include <uRTCLib.h>

// uRTCLib rtc;
uRTCLib rtc(0x68);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char dateString[20] = {"1"};

void setup() {
  Serial.begin(9600);
  delay(3000); // wait for console opening

  URTCLIB_WIRE.begin();

  // Comment out below line once you set the date & time.
  // Following line sets the RTC with an explicit date & time
  // for example to set January 13 2022 at 12:56 you would call:
  // rtc.set(0, 16, 19, 1, 12, 4, 22);
  // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
  // set day of week (1=Sunday, 7=Saturday)
}

void loop() {
  rtc.refresh();
  /*
  Serial.print("Current Date & Time: ");
  Serial.print(rtc.year());
  Serial.print('/');
  Serial.print(rtc.month());
  Serial.print('/');
  Serial.print(rtc.day());

  Serial.print(" (");
  Serial.print(daysOfTheWeek[rtc.dayOfWeek()-1]);
  Serial.print(") ");

  Serial.print(rtc.hour());
  Serial.print(':');
  Serial.print(rtc.minute());
  Serial.print(':');
  Serial.println(rtc.second());
  */
  Serial.print("Function time: ");
  Serial.print(getTime(dateString));
  Serial.print(" Library time: ");
  Serial.println(rtc.hour());
  delay(1000);
}

// Define getTime() function
// No arguments
// Return current date and time as string
char* getTime(char* dateString) {
  // = "" + (rtc.hour() % 12) + ":" + rtc.minute() + "(AM/PM)" + ", " + rtc.month() + "/" + rtc.day() + "/" + rtc.year();
  uint8_t hourInt = rtc.hour();
  char hour = (char *) hourInt;
  //char hour[] = "big test string";
  //strcat(dateString, (const char *) rtc.hour());
  return strcat(dateString, hour);
}
// Need to clear string after we have written to it or else it will just append
// Calculate size of final string
// Assemble final string
// Timestamp format:
// "x:xx (pm/am), 00/00/0000"