#include <LiquidCrystal.h>
// "Borrowed" this code from an online guide. I will use it to test the LCD module and then fully modify for our needs later.
// Obivously this file itself won't end up being used in the final project, but some form of it will make it way into the final project where it will be combined with the other modules.

// initialize the library by associating any needed LCD interface pin

// with the arduino pin number it is connected to

const int rs = 48, en = 49, d4 = 50, d5 = 51, d6 = 52, d7 = 53;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2); // This line is neccessary, do not change

  // Print a message to the LCD.

  lcd.print("testing");

}


void loop() {

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);

  lcd.print("Test: ");

  //lcd.autoscroll();

  // print the number of seconds since reset:
  lcd.print(millis() / 1000);

}