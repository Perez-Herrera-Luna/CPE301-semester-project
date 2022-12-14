#define RDA 0x80
#define TBE 0x20
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4; // Baud Rate Registers
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6; // Data Register

void setup()
{
  U0init(9600); // initialize the serial port on USART0:
}
void loop()
{
  unsigned char cs1; // Read current char in buffer
  unsigned char MSB; //
  unsigned char LSB;
  char test[11] = "Test Output";
  U0putcharArray(test, 11);
  U0putchar('a');
  /*
  while (U0kbhit()==0){}; // wait for RDA = true
  cs1 = U0getchar();    // read character
  MSB = hexMSB(cs1);
  LSB = hexLSB(cs1);
  if(MSB > '9') MSB += 7;
  if(LSB > '9') LSB += 7;

  // U0putchar() write character to data register
  // U0sendbuffer() send what is in the data register
  U0putchar('0');
  U0putchar('x');

  U0putchar(MSB);
  U0putchar(LSB);

  U0putchar('\n');

  *myUCSR0B |= 0x10; // 0b 0001 0000 enalbes USART reciever
  */
  delay(2000);
}
//
// function to initialize USART0 to "int" Baud, 8 data bits,
// no parity, and one stop bit. Assume FCPU = 16MHz.
//
void U0init(unsigned long U0baud)
{
//  Students are responsible for understanding
//  this initialization code for the ATmega2560 USART0
//  and will be expected to be able to intialize
//  the USART in differrent modes.
//
 unsigned long FCPU = 16000000;
 unsigned int tbaud;
 tbaud = (FCPU / 16 / U0baud - 1);
 // Same as (FCPU / (16 * U0baud)) - 1;
 *myUCSR0A = 0x20;
 *myUCSR0B = 0x18;
 *myUCSR0C = 0x06;
 *myUBRR0  = tbaud;
}
//
// Read USART0 RDA status bit and return non-zero true if set
//
unsigned char U0kbhit()
{
  return (*myUCSR0A & RDA) ? true : false;
}
//
// Read input character from USART0 input buffer
//
unsigned char U0getchar()
{
  return *myUDR0;
}
//
// Wait for USART0 TBE to be set then write character to
// transmit buffer
//
void U0putchar(unsigned char U0pdata)
{
  while((*myUCSR0A & TBE) == 0);
    *myUDR0 = U0pdata;
}

void U0putcharArray(char* charArray, int length)
{
  for(int i = 0; i < length; i++)
  {
    U0putchar(charArray);
  }
}

unsigned char hexMSB(unsigned char chara)
{
  unsigned char MSB = (chara / 16) + '0';
  return MSB;
}

unsigned char hexLSB(unsigned char chara)
{
  unsigned char LSB = chara % 16 + '0';
  return LSB;
}