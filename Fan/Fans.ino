#define RDA 0x80
#define TBE 0x20  

// UART Pointers
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;
// GPIO Pointers
volatile unsigned char *portB     = 0x25; //PB6 is pin 12
volatile unsigned char *portDDRB  = 0x24;
// Timer Pointers
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;

double clk_period = 0.0000000625;
unsigned int ticks= ((1.0/100.0)/2.0)/clk_period; //fan shouldn't go too fast, so 100Hz is good

//global ticks counter
unsigned long currentTicks=0;
unsigned int timer_running=1;

void setup() 
{ 
  // set PB6 to output
  *portDDRB |= 0x40;
  // set PB6 LOW
  *portB &= 0xBF;
  // setup the Timer for Normal Mode, with the TOV interrupt enabled
  setup_timer_regs();
  // Start the UART
  U0Init(9600);
  Serial.begin(9600); //remove this
} 
 
 
void loop() 
{
  if (Serial.parseInt()==1) //change this out with the water monitor values to switch fans on
  {
     // set the ticks
     currentTicks = ticks;
     // if the timer is not already running, start it
     if(!timer_running)
     {
      // start the timer
      *myTCCR1B |= 0x01;
      // set the running flag
      timer_running = 1;
     }
    Serial.write("HIGH\n"); //remove this
  }
  if (Serial.parseInt()==2) //change this out with the water monitor values to switch fans off
    {
     // set the current ticks to the max value
      currentTicks = 65535;
      // if the timer is running
      if(timer_running)
      {
        // stop the timer
        *myTCCR1B &= 0xF8;
        // set the flag to not running
        timer_running = 0;
        // set PB6 LOW
        *portB &= 0xBF;
        Serial.write("LOW\n"); //remove this
      }
    }
}
// Timer setup function
void setup_timer_regs()
{
  // setup the timer control registers
  *myTCCR1A= 0x00;
  *myTCCR1B= 0X00;
  *myTCCR1C= 0x00;
  
  // reset the TOV flag
  *myTIFR1 |= 0x01;
  
  // enable the TOV interrupt
  *myTIMSK1 |= 0x01;
}


// TIMER OVERFLOW ISR
ISR(TIMER1_OVF_vect)
{
  // Stop the Timer
  *myTCCR1B &=0xF8;
  // Load the Count
  *myTCNT1 =  (unsigned int) (65535 -  (unsigned long) (currentTicks));
  // Start the Timer
  *myTCCR1B |=  0x01;
  // if it's not the STOP amount
  if(currentTicks != 65535)
  {
    // XOR to toggle PB6
    *portB ^= 0x40;
  }
}
/*
 * UART FUNCTIONS
 */
void U0Init(int U0baud)
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
