// Sensor pins
#define sensorPower 7
#define sensorPin A0

// Port H Register Pointers
unsigned char* port_h = (unsigned char*) 0x102; // Port H Data Register
unsigned char* ddr_h  = (unsigned char*) 0x101; // Port H Data Direction Register
unsigned char* pin_h  = (unsigned char*) 0x100; // Port H Input Pins Address

// Value for storing water level
int waterValue = 0;

void setup() {
  // Set H4 as an OUTPUT
  set_pin_direction(ddr_h, 4, OUTPUT);
	
	// Set to LOW so no power flows through the sensor
  //write_ph(4, 0);
  write_to_pin(port_h, 4, LOW);
	
	Serial.begin(9600);
}

void loop() {
	//get the reading from the function below and print it
	int waterLevel = readSensor();
	
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
int readSensor() {
  write_to_pin(port_h, 4, HIGH); // Turn the sensor ON
	delay(10);							// wait 10 milliseconds
	waterValue = analogRead(sensorPin);		// Read the analog value form sensor
  write_to_pin(port_h, 4, LOW); // Turn the sensor OFF
	return waterValue;							// send current reading
}