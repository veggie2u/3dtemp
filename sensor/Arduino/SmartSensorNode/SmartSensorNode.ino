// Smart Sensor Node

#include <Wire.h>             // handle I2C comms
#include "wiring_private.h"   // needed to rewire the 2nd I2C port
#include "Adafruit_Si7021.h"  // access the temp sensor
#include <GOFi2cOLED.h>       // shrunk down library to access lcd
#include "TimedAction.h"      // allow "async" (sort of) actions

// ID of this sensor
const int ID = 2;
const byte LCD_ID = 0x3C;  // default address
// const byte LCD_ID = 0x3D;     // address 2

char temperatureUnits = 'C';  // temperature units 'C' or 'F'

TimedAction displayAction;    // updates display on a timer
TimedAction sensorAction;     // read from sensor
TimedAction indicatorAction;  // briefly show the indicator

GOFi2cOLED GOFoled;           // minimal display library
Adafruit_Si7021 sensor = Adafruit_Si7021(); // temperature/humidity sensor

// Set up myWire to communicate with RasPi
// Changing 2 pins to a second I2C port
// TwoWire myWire(&sercom1, 11, 13);
TwoWire myWire(&sercom4, 15, 16);

typedef struct {
  int temperature;
  int humidity;
} SensorSend;

typedef struct {
  float temperature;
  float humidity;
} SensorReading;

// We store multiple readings and then average them so the numbers don't bounce
const int RAW_DATA_SIZE = 10;
SensorReading rawData[RAW_DATA_SIZE] = {
  {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, 
  {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}
};
int dataIndex = 0;          // position in raw data array
boolean dataFull = false;   // have we filled raw data?
boolean ledState = false;   // should the led show?
boolean indicator = false;  // should the 'connected to pi' indicator show?

void setup()
{
  Serial.begin(115200);
  delay(2000);              // give serial a bit to start up
  Serial.println("Starting...");
  GOFoled.init(LCD_ID);     //initialze  OLED display
  GOFoled.display();        // show splashscreen
  delay(2000);
  GOFoled.clearDisplay();
  Serial.println(A1);

  // check if we have a sensor and stop if we don't
  Serial.println("Locating Sensor");
  if (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    GOFoled.clearDisplay();
    GOFoled.setTextSize(1);
    GOFoled.setTextColor(WHITE);
    GOFoled.setCursor(0,0);
    GOFoled.print("Sensor ID ");
    GOFoled.print(ID);
    GOFoled.println("Humidity / Temp"); 
    GOFoled.setTextSize(3);
    GOFoled.setTextColor(WHITE);
    GOFoled.println("Bad");
    GOFoled.println("Sensor");
    GOFoled.display();
    while (true)
      delay(100); // no sensor = stop
  }

  Serial.print("Found model ");
  switch(sensor.getModel()) {
    case SI_Engineering_Samples:
      Serial.print("SI engineering samples"); break;
    case SI_7013:
      Serial.print("Si7013"); break;
    case SI_7020:
      Serial.print("Si7020"); break;
    case SI_7021:
      Serial.print("Si7021"); break;
    case SI_UNKNOWN:
    default:
      Serial.print("Unknown");
  }
  Serial.print(" Rev(");
  Serial.print(sensor.getRevision());
  Serial.print(")");
  Serial.print(" Serial #"); Serial.print(sensor.sernum_a, HEX); Serial.println(sensor.sernum_b, HEX);
  
  myWire.begin(ID);                  // join i2c bus
  pinPeripheral(15, PIO_SERCOM_ALT); // reassign the pins
  pinPeripheral(16, PIO_SERCOM_ALT);
  myWire.onRequest(requestEvent);    // register event from RasPi

  // Print top section once
  GOFoled.setTextSize(1);
  GOFoled.setTextColor(WHITE);
  GOFoled.setCursor(0,0);
  GOFoled.print("Sensor ID ");
  GOFoled.println(ID);
  GOFoled.println("Humidity / Temp"); 
  GOFoled.display();
  Serial.println("Listening for requests");
  pinMode(LED_BUILTIN, OUTPUT); // blinks when reading data

  displayAction = TimedAction(4000, updateDisplay); // update display every 4 sec
  sensorAction = TimedAction(1000, updateSensor);   // update sensor readings every 1 sec
  indicatorAction = TimedAction(750, handleIndicatorOff); // show indicator for 750ms
}

// Needed for Arduino, like you put in a .h file (because of returning a struct)
SensorReading getAverageReading();

// Takes all the readings from the rawData and returns the average
SensorReading getAverageReading()
{
  float temperatureTotal = rawData[0].temperature;
  float humidityTotal = rawData[0].humidity;
  for (int i=1; i < RAW_DATA_SIZE; i++) {
    temperatureTotal += rawData[i].temperature;
    humidityTotal += rawData[i].humidity;
  }
  SensorReading averageReading;
  // if we get called before we are ready, just return an error value
  if (!dataFull) {
    averageReading.temperature = 999.0;
    averageReading.humidity = 999.0;
  } else {
    averageReading.temperature = temperatureTotal / RAW_DATA_SIZE;
    averageReading.humidity = humidityTotal / RAW_DATA_SIZE;
  }
  return averageReading;
}

// Display sensor data on the LCD
void updateDisplay() {
  SensorReading average = getAverageReading();
  // Switch between C and F
  if (temperatureUnits == 'C') {
    display(average.humidity, average.temperature, 'C');
    temperatureUnits = 'F';
  } else {
    float t = average.temperature * 9 / 5 + 32;
    display(average.humidity, t, 'F');
    temperatureUnits = 'C';
  }
}

// Display the temperature, humidity, and temp unit
void display(float humidity, float temp, char unit) {
  clear();
  GOFoled.setCursor(0,18);
  GOFoled.setTextSize(3);
  if (!dataFull) {
    GOFoled.println("Reading...");
  } else {
    GOFoled.print(humidity, 1);
    GOFoled.setTextSize(2);
    GOFoled.print(" Rh");
    GOFoled.setTextSize(3);
    GOFoled.println(); 
    GOFoled.print(temp, 1);
    GOFoled.setTextSize(2);
    GOFoled.print(" ");
    GOFoled.print(unit);
  }
  GOFoled.display();
  Serial.print("Display Humidity: ");
  Serial.print(humidity, 1);
  Serial.print("\tTemperature: ");
  Serial.print(temp, 1);
  Serial.println(unit);
}

// Clear the main section of the screen
void clear() {
  GOFoled.fillRect(0, 16, 160, 80, 0);
}

// Turn off the indicator and turn off the timer
void handleIndicatorOff() {
  indicator = false;
  indicatorAction.disable();
  showIndicator(' ');
}

// Display the 'connected to pi' indicator
void showIndicator(char indicator)
{
  GOFoled.fillRect(120, 0, 130, 10, 0);
  GOFoled.setTextSize(1);
  GOFoled.setTextColor(WHITE);
  GOFoled.setCursor(120,0);
  GOFoled.print(indicator);
  GOFoled.display();
}

// Needed for Arduino, like you put in a .h file (because of returning a struct)
SensorReading getSensorReading();
// Read data from the sensor and return in a struct
SensorReading getSensorReading()
{
  SensorReading reading;
  reading.temperature = sensor.readTemperature();
  reading.humidity = sensor.readHumidity();
  return reading;
}

// Gets one sensor reading and puts it into the array
void updateSensor() {
  // turn led on then off for each reading
  digitalWrite(LED_BUILTIN, (ledState = !ledState));
  SensorReading reading = getSensorReading();
  rawData[dataIndex] = reading;
  if (dataIndex++ >= RAW_DATA_SIZE) {
    dataIndex = 0;
    dataFull = true;
  }
  Serial.print("Read Humidity: ");
  Serial.print(reading.humidity, 2);
  Serial.print("\tTemperature: ");
  Serial.println(reading.temperature, 2);
}

// Very little is needed in the the loop. The display is updated, and the sensor
// readings taken when the check on the action is true.
void loop()
{
  if (indicator) {
    indicator = false;
    indicatorAction.reset();
    indicatorAction.enable();
    showIndicator('*');
  }
  indicatorAction.check();
  
  // check to see if time to update the display
  displayAction.check();

  // check to see if time to pull new data from the sensor
  sensorAction.check();
}

// function that executes whenever data is requested by RasPi
void requestEvent()
{
  indicator = true;
  SensorReading average = getAverageReading();
  SensorSend sensorData;
  sensorData.temperature = int(average.temperature * 10 + 0.5);
  sensorData.humidity = int(average.humidity * 10 + 0.5);
  myWire.write((byte *)&sensorData, sizeof sensorData);
}

// needed to handle interupts on new I2C port
void SERCOM4_Handler(void) {
  myWire.onService();
}
