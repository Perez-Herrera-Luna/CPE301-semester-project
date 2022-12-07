// Sensor pins
#define sensorPower 4
#define sensorPin 2

// ADC Registers
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;

// Port H Register Pointers
unsigned char* port_h = (unsigned char*) 0x102; // Port H Data Register
unsigned char* ddr_h  = (unsigned char*) 0x101; // Port H Data Direction Register
unsigned char* pin_h  = (unsigned char*) 0x100; // Port H Input Pins Address

// Value for storing water level
unsigned int waterValue = 0;

void setup() {
  // Set H4 as an OUTPUT
  set_pin_direction(ddr_h, sensorPower, OUTPUT);
	
	// Set to LOW so no power flows through the sensor
  //write_ph(4, 0);
  write_to_pin(port_h, sensorPower, LOW);

  // setup the ADC
  adc_init();
	
	Serial.begin(9600);
}

void loop() {
	//get the reading from the function below and print it
	unsigned int waterLevel = readSensor();
	
	Serial.print("Water level: ");
	Serial.println(waterLevel);
	
	delay(1000);
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
unsigned int readSensor() {
  write_to_pin(port_h, sensorPower, HIGH); // Turn the sensor ON
	delay(10);							// wait 10 milliseconds
  waterValue = adc_read(3); // Read the analog value form sensor
  // 3 should probably be replaced with sensorPin but not neccessary
  // Whenever I read on an analog port that isn't A0 (such as A3) the lowest value for waterValue ends up being 7 instead of the epexected 0. This can proably be worked around but to fix I'd proably have to chnage something witha  register.
  write_to_pin(port_h, sensorPower, LOW); // Turn the sensor OFF
	return waterValue;							// send current reading
}

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