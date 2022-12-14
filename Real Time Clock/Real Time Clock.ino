#include <uRTCLib.h>

uRTCLib rtc(0x68);
const int MAX_DATE_STRING_SIZE = 21; // Maximum size for a date stamp
char dateString[MAX_DATE_STRING_SIZE];
const int MAX_INT_TO_CHAR_ARRAY_BUFFER_SIZE = 2; // RTC library function calls return at max a 2 digit integer
char intToCharArrayBuffer[MAX_INT_TO_CHAR_ARRAY_BUFFER_SIZE];

void setup() {
  Serial.begin(9600); // Used only for testing purposes. Not necessary
  delay(3000); // Gives the console a chance to open properly

  URTCLIB_WIRE.begin();

  // Comment out below line once you set the date & time.
  // Following line sets the RTC with an explicit date & time
  // for example to set December 13 2022 at 16:03 you would call:
  // rtc.set(0, 55, 16, 3, 13, 12, 22);
  // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
  // set day of week (1=Sunday, 7=Saturday)
}

void loop() {
  Serial.println(getTime()); // Serial output is used only for testing purpose
  delay(1000); // Delay is only for testing purpose
}

// Takes an integer and converts it to a char array of that same integer. Relies on a global char array 'intToCharArrayBuffer'
char* intToCharArray(int input)
{
  memset(&intToCharArrayBuffer[0], 0, sizeof(intToCharArrayBuffer));
  sprintf(intToCharArrayBuffer, "%d", input);
  return intToCharArrayBuffer;
}

// Returns a char array containing a time stamp containing the current time and date. Relies on a global char array 'dateString'
char* getTime() {

  rtc.refresh();
  memset(&dateString[0], 0, sizeof(dateString)); // Clears dateString char array to flush out previous time stamp
  
  int hour12 = rtc.hour() % 12;
  char tempString[6] = ""; // Temporary char array used to hold string literals required in assembling char array
  if (hour12 == 0) { // Converts the hour to the 12-hour format
    hour12 = 12; // 0 in the 24-hour format is 12 in the 12-hour format
  }

  // Assembles time information
  strcpy(tempString, "("); 
  strcat(dateString, tempString); // "("
  strcat(dateString, intToCharArray(hour12)); // "(xx"
  strcpy(tempString, ":");
  strcat(dateString, tempString); // "(xx:"
  if(rtc.minute() < 10) // Adds a leading 0 to the time if it's a one digit number
  {
    strcpy(tempString, "0");
    strcat(dateString, tempString);
  }
  strcat(dateString, intToCharArray(rtc.minute())); // "(xx:xx"

  // Determines whether the time should be displayed as AM or PM
  if(rtc.hour() < 12)
  {
    strcpy(tempString, " AM, ");
  }
  else
  {
    strcpy(tempString, " PM, ");
  }
  strcat(dateString, tempString); // "(xx:xx (AM/PM), "

  // Assemble date information
  strcat(dateString, intToCharArray(rtc.month())); // "(xx:xx (AM/PM), xx"
  strcpy(tempString, "/");
  strcat(dateString, tempString); // "(xx:xx (AM/PM), xx/"
  strcat(dateString, intToCharArray(rtc.day())); // "(xx:xx (AM/PM), xx/xx"
  strcpy(tempString, "/");
  strcat(dateString, tempString); // "(xx:xx (AM/PM), xx/xx/"
  strcat(dateString, intToCharArray(rtc.year())); // "(xx:xx (AM/PM), xx/xx/xx"
  strcpy(tempString, ")");
  strcat(dateString, tempString); // "(xx:xx (AM/PM), xx/xx/xx)"

  return dateString;
}