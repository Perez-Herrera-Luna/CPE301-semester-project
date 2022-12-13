#include <LiquidCrystal.h>

const int rs = 48, en = 49, d4 = 50, d5 = 51, d6 = 52, d7 = 53;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Initialize the library with the neccessary pins

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2); // This line is neccessary, do not change
  // Print a message to the LCD.
  lcd.print("testing"); // This can be changed

}

void loop() {
  lcd.setCursor(0, 1); // set the cursor to column 0, line 1
  lcd.print("Test: ");
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}