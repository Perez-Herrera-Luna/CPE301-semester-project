#include <LiquidCrystal.h> // For LCD Module
// Holdovers from Labs, best not to touch
#define RDA 0x80
#define TBE 0x20
// Sensor pins
#define waterSensorPower 4
#define waterSensorPin 3
// Timer Pointers
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;
// GPIO Pointers
volatile unsigned char *portDDRB = (unsigned char *) 0x24;
volatile unsigned char *portB =    (unsigned char *) 0x25;
// UART Pointers
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1; //
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4; // Baud Rate Registers
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6; // Data Register
// Define Port B Register Pointers
volatile unsigned char* port_b = (unsigned char*) 0x25; 
volatile unsigned char* ddr_b  = (unsigned char*) 0x24; 
volatile unsigned char* pin_b  = (unsigned char*) 0x23;
// ADC Registers
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;
// Port H Register Pointers
unsigned char* port_h = (unsigned char*) 0x102; // Port H Data Register
unsigned char* ddr_h  = (unsigned char*) 0x101; // Port H Data Direction Register
unsigned char* pin_h  = (unsigned char*) 0x100; // Port H Input Pins Address

//Initialize Modules
const int rs = 48, en = 49, d4 = 50, d5 = 51, d6 = 52, d7 = 53;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Initialize the library with the neccessary pins

// Our 'Global' Varibales
unsigned int systemState = 0;
// 0 = off
// 1 = Running
// 2 = Idle
// 3 = Disabled
// 4 = Error
unsigned int waterValue = 0; // Value for storing water level
unsigned int previousWaterValue = waterValue;
bool tester = false;

void setup() 
{
  lcd.begin(16, 2); // This line is neccessary, do not change
  // This can be changed
  //lcd.print("testing");
  // Water Level Sensor setup
  set_pin_direction(ddr_h, waterSensorPower, OUTPUT); // Set H4 as an OUTPUT
  write_to_pin(port_h, waterSensorPower, LOW); // Set to LOW so no power flows through the sensor
  adc_init(); // setup the ADC
  
  U0init(9600); // initialize the serial port on USART0:
}
void loop() 
{
	//get the reading from the function below and print it
	unsigned int waterLevel = readSensor();
  if(previousWaterValue != waterValue)
  {
    previousWaterValue = waterValue;
  }
	
	Serial.print("Water level: ");
	Serial.println(waterValue);
  Serial.print("Previous-Water level: ");
	Serial.println(previousWaterValue);
	
	
  //lcd.setCursor(0, 0);
  lcd.print("testing");
  lcd.setCursor(0, 1); // set the cursor to column 0, line 1
  lcd.print("WL: ");
  lcd.print(waterLevel); // print the water level
  delay(100);
  lcd.clear();
}

void U0init(unsigned long U0baud)
{
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / U0baud - 1);
 // Same as (FCPU / (16 * U0baud)) - 1;
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}

// Read USART0 RDA status bit and return non-zero true if set
unsigned char kbhit()
{
  return *myUCSR0A & RDA;
}
// Read input character from USART0 input buffer
unsigned char U0getchar()
{
  return *myUDR0;
}
// Wait for USART0 TBE to be set then write character to transmit buffer
void U0putchar(unsigned char U0pdata)
{
  while((*myUCSR0A & TBE) == 0);
    *myUDR0 = U0pdata;
}
// These two functions probably aren't needed for anything we're doing here but better safe than sorry.
void set_PB_as_output(unsigned char pin_num)
{
    *ddr_b |= 0x01 << pin_num;
}
void write_pb(unsigned char pin_num, unsigned char state)
{
  if(state == 0)
  {
    *port_b &= ~(0x01 << pin_num);
  }
  else
  {
    *port_b |= 0x01 << pin_num;
  }
}

/*
* Sets the specififed pin to either input or output
* @param data_direction_register - The Data Direction Register of the desired port
* @param pin_num - The pin number of the port that you wish to modify
* @param mode - INPUT or OUTPUT
*/
void set_pin_direction(unsigned char* data_direction_register, unsigned char pin_num, uint8_t mode)
{
  if(mode == OUTPUT) *data_direction_register |= 0x01 << pin_num;
  else if(mode == INPUT) *data_direction_register &= ~(0x01 << pin_num);
}
/*
* Write a HIGH or LOW value to a digital pin
* @param data_register - The Data Register of the desired port
* @param pin_num - The pin number of the port that you wish to modify
* @param mode - LOW or HIGH
*/
void write_to_pin(unsigned char* data_register, unsigned char pin_num, uint8_t state)
{
  if(state == LOW) *data_register &= ~(0x01 << pin_num);
  else if(state == HIGH) *data_register |= 0x01 << pin_num;
}
/*
* Function that gets a reading from the water level sensor
*/

// ADC Functions
void adc_init()
{
  // setup the A register
  *my_ADCSRA |= 0b10000000; // set bit   7 to 1 to enable the ADC                              1000 0000
  *my_ADCSRA &= 0b11011111; // clear bit 6 to 0 to disable the ADC trigger mode                0100 0000
  *my_ADCSRA &= 0b11110111; // clear bit 5 to 0 to disable the ADC interrupt                   0010 0000
  *my_ADCSRA &= 0b11111000; // clear bit 0-2 to 0 to set prescaler selection to slow reading   0000 0111
  // setup the B register
  *my_ADCSRB &= 0b11110111; // clear bit 3 to 0 to reset the channel and gain bits             0000 1000
  *my_ADCSRB &= 0b11111000; // clear bit 2-0 to 0 to set free running mode                     0000 0111
  // setup the MUX Register
  *my_ADMUX  &= 0b01111111; // clear bit 7 to 0 for AVCC analog reference                      1000 0000
  *my_ADMUX  |= 0b01000000; // set bit   6 to 1 for AVCC analog reference                      0100 0000
  *my_ADMUX  &= 0b11011111; // clear bit 5 to 0 for right adjust result                        0010 0000
  *my_ADMUX  &= 0b11100000; // clear bit 4-0 to 0 to reset the channel and gain bits           0001 1111
}
unsigned int adc_read(unsigned char adc_channel_num)
{
  // clear the channel selection bits (MUX 4:0)                                               0001 1111
  *my_ADMUX  &= 0b11100000;
  // clear the channel selection bits (MUX 5)                                                 0001 0000
  *my_ADCSRB &= 0b11110111;
  // set the channel number
  if(adc_channel_num > 7)
  {
    // set the channel selection bits, but remove the most significant bit (bit 3)
    adc_channel_num -= 8;
    // set MUX bit 5                                                                          0010 0000
    *my_ADCSRB |= 0b00001000;
  }
  // set the channel selection bits
  *my_ADMUX += adc_channel_num;
  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0x40;
  // wait for the conversion to complete
  while((*my_ADCSRA & 0x40) != 0);
  // return the result in the ADC data register
  return *my_ADC_DATA;
}

// Water Level Function
unsigned int readSensor() {
  write_to_pin(port_h, waterSensorPower, HIGH); // Turn the sensor ON
	delay(10);							// wait 10 milliseconds
  waterValue = adc_read(3); // Read the analog value form sensor
  // 3 should probably be replaced with waterSensorPin but not neccessary
  // Whenever I read on an analog port that isn't A0 (such as A3) the lowest value for waterValue ends up being 7 instead of the epexected 0. This can proably be worked around but to fix I'd proably have to chnage something witha  register.
  write_to_pin(port_h, waterSensorPower, LOW); // Turn the sensor OFF
	return waterValue;							// send current reading
}