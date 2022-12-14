// Port F pointers
volatile unsigned char *portF     = (unsigned char*) 0x31;
unsigned char *portDDRF  = (unsigned char*) 0x30;
volatile unsigned char* pinF      = (unsigned char*) 0x2F; //ccw adjuster
// Port K pointers
volatile unsigned char *portK     = (unsigned char*) 0x107;
unsigned char *portDDRK  = (unsigned char*) 0x108;
volatile unsigned char *pinK      = (unsigned char*) 0x106; //cw adjuster

//setup the stepper
#include <Stepper.h>
const int stepsPerRev=200;
Stepper myStepper(stepsPerRev, 24, 25, 26, 27); //setup on PA2-PA5

void setup() {
  myStepper.setSpeed(60); //set stepper to 60 rpm
  set_pin_direction(portDDRF, 0, INPUT);
  set_pin_direction(portDDRK, 0, INPUT);
  //*portDDRF &= 0xFE; //1111 1110 set PF0 to input
  //*portDDRK &= 0xFE; //1111 1110 set PK0 to input
  //pinMode(A0, INPUT_PULLUP);
  //pinMode(A8, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  //Serial.println(*pinF);
  //Serial.println(*pinK);
  myStepper.step(stepsPerRev);
  if( ((*pinF==0)&&(*pinK>=1))||((*pinF>=1)&&(*pinK==0)) ) //does nothing if both buttons are pressed at the same time
  {
    //Serial.println("Test1");
    if(*pinK>=1)
    {
      //Serial.println("Test2");
      //myStepper.step(stepsPerRev); //do one rotation cw if button connected to pin48 is pressed
    }
    else
    {
      //Serial.println("Test3");
      //myStepper.step(-stepsPerRev); //do one rotation ccw if button connected to pin50 is pressed
    }
  }
}

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