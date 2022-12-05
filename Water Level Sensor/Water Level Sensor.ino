// Sensor pins
#define sensorPower 7
#define sensorPin A0

// Port H Register Pointers
unsigned char* port_h = (unsigned char*) 0x102; // Port H Data Register
unsigned char* ddr_h  = (unsigned char*) 0x101; // Port H Data Direction Register
unsigned char* pin_h  = (unsigned char*) 0x100; // Port H Input Pins Address

// Value for storing water level
int val = 0;

void setup() {
	// Set D7 as an OUTPUT
	//pinMode(sensorPower, OUTPUT);
  //set_PH_as_output(4);
  set_pin_direction(ddr_h, 4, OUTPUT);
	
	// Set to LOW so no power flows through the sensor
	//digitalWrite(sensorPower, LOW);
  write_ph(4, 0);
	
	Serial.begin(9600);
}

void loop() {
	//get the reading from the function below and print it
	int level = readSensor();
	
	Serial.print("Water level: ");
	Serial.println(level);
	
	delay(1000);
}
/*
* Sets the specififed pin to either input or output
* @param data_register_port - The Data Direction register of the desired port
* @param pin_num - The pin number of the port that you wish to modify
* @param mode - INPUT or OUTPUT
*/
void set_pin_direction(unsigned char* data_register_port, unsigned char pin_num, uint8_t mode)
{
  if(mode == OUTPUT) *data_register_port |= 0x01 << pin_num;
  else if(mode == INPUT) *data_register_port &= ~(0x01 << pin_num);
}

void set_PH_as_output(unsigned char pin_num)
{
    *ddr_h |= 0x01 << pin_num;
}

void write_ph(unsigned char pin_num, unsigned char state)
{
  if(state == 0)
  {
    *port_h &= ~(0x01 << pin_num);
  }
  else
  {
    *port_h |= 0x01 << pin_num;
  }
}

//This is a function used to get the reading
int readSensor() {
	//digitalWrite(sensorPower, HIGH);	// Turn the sensor ON
  write_ph(4, 1);
	delay(10);							// wait 10 milliseconds
	val = analogRead(sensorPin);		// Read the analog value form sensor
	//digitalWrite(sensorPower, LOW);		// Turn the sensor OFF
  write_ph(4, 0);
	return val;							// send current reading
}