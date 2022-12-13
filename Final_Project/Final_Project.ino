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

unsigned int systemState = 0;
// 0 = off
// 1 = Running
// 2 = Idle
// 3 = Disabled
// 4 = Error

unsigned int waterValue = 0; // Value for storing water level

void setup() 
{
  U0init(9600); // initialize the serial port on USART0:
}
void loop() 
{

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