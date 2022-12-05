// Port F pointers
volatile unsigned char *portF =    (unsigned char *) 0x31;
volatile unsigned char *portDDRF = (unsigned char *) 0x30;
volatile unsigned char* pinF  = (unsigned char*) 0x2F; //ccw adjuster
// Port K pointers
volatile unsigned char *portK =    (unsigned char *) 0x107;
volatile unsigned char *portDDRK = (unsigned char *) 0x108;
volatile unsigned char* pinK  = (unsigned char*) 0x106; //cw adjuster

//setup the stepper
#include <Stepper.h>
const int stepsPerRev=200;
Stepper myStepper(stepsPerRev, 24, 25, 26, 27); //setup on PA2-PA5

void setup() {
  myStepper.setSpeed(60); //set stepper to 60 rpm
  *portDDRF &= 0xFE; //1111 1110 set PF0 to input
  *portDDRK &= 0xFE; //1111 1110 set PK0 to input
  Serial.begin(9600);
}

void loop() {
    
  if( ((*pinF==0)&&(*pinK==1))||((*pinF==1)&&(*pinK==0)) ) //does nothing if both buttons are pressed at the same time
  {
    if(*pinK==1)
    {
      myStepper.step(stepsPerRev); //do one rotation cw if button connected to pin48 is pressed
    }
    else
    {
      myStepper.step(-stepsPerRev); //do one rotation ccw if button connected to pin50 is pressed
    }
  }
}
