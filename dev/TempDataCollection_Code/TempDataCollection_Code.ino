#include <OneWire.h>
#include <DallasTemperature.h>

/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%**%*%*%*%*%*%*%*%*%*%/
//Settings For Arduino IDE
//Board: Arduino Micro
//Programmer: AVR ISP
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%**%*%*%*%*%*%*%*%*%*%*/

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9
const int SSRPin =  12;  
const int LEDPin = 6;
const int FanPin = 11;
double time;
double avTemp;
boolean UP;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer, outsideThermometer;

void setup(void)
{
  // start serial port
  UP = true;
  avTemp = 0;
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  pinMode(SSRPin, OUTPUT);
  pinMode(FanPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(outsideThermometer, 1)) Serial.println("Unable to find address for Device 1"); 

  
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(outsideThermometer);
  Serial.println();

  // set the resolution to 9 bit
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();

  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(outsideThermometer), DEC); 
  Serial.println();
  
  Serial1.begin(19200);
  Serial1.write(17);
  
  Serial1.write(12);
  Serial1.write("Starting Up");
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  //Serial.print("Temp C: ");
  Serial.print(tempC);
  // Serial.print(" Temp F: ");
  Serial.print(" ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial1.write(12);
  Serial1.write("Test");//String(DallasTemperature::toFahrenheit(tempC), 2));
  
}
//function to define profile
void checktemp()//values for run from cold oven
{
    avTemp = (sensors.getTempC(insideThermometer) + sensors.getTempC(outsideThermometer))/2;
    if (avTemp < 100)
    {
     if(UP)
    { 
      digitalWrite(SSRPin, HIGH);
      delay(2000);
      digitalWrite(SSRPin, LOW);
      delay(5);
    }
     if(!UP)
    {
      digitalWrite(FanPin, HIGH);
      digitalWrite(SSRPin, LOW);
    }
    }
   if (avTemp >= 100 && avTemp< 150)
   {    
    if(UP)
    {
      digitalWrite(SSRPin, HIGH);//153 seconds
      delay(2000);
      digitalWrite(SSRPin, LOW);
      delay(10); 
    }
     if(!UP)
    {
      digitalWrite(FanPin, HIGH);
      digitalWrite(SSRPin, LOW);
      delay(100);
    }
 }
  if (avTemp >= 150 && avTemp < 210)
  {
     if(UP)
     {
      digitalWrite(SSRPin, HIGH);
      delay(100);
     }
    if(!UP)
    {
      digitalWrite(FanPin, HIGH);
      digitalWrite(SSRPin, LOW);
      delay(100);
    }    
  }
  if (avTemp >= 210 && avTemp < 220)
  {
    if(UP)
    {
      digitalWrite(SSRPin, HIGH);
      digitalWrite(FanPin, HIGH);
      delay(1700);
      digitalWrite(SSRPin, LOW);
      delay(100);
      if(avTemp == 210)
      UP = false;
    }
    if(!UP)
    {
      digitalWrite(FanPin, HIGH);
      digitalWrite(SSRPin, LOW);
      delay(100);
    }    
  }
   if (avTemp >= 240)
  {
    digitalWrite(FanPin, HIGH);
    digitalWrite(SSRPin, LOW);
    delay(100);
  }
}


void checkTempTest()
{
    avTemp = (sensors.getTempC(insideThermometer) + sensors.getTempC(outsideThermometer))/2;
    if (avTemp < 90)
    {
     if(UP)
    { 
      digitalWrite(SSRPin, HIGH);
      delay(1500);
      digitalWrite(SSRPin, LOW);
      delay(5);
      if (avTemp == 90)
     {
       UP = false;
       Serial.write("False");
       Serial.write(UP);
     }
    }
     if(!UP)
    {
      digitalWrite(FanPin, HIGH);
    }
    }
}
// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
 // Serial.print("Resolution: ");
 // Serial.print(sensors.getResolution(deviceAddress));
  //Serial.println();    
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
 // Serial.print("Device Address: ");
 // printAddress(deviceAddress);
 // Serial.print(" ");
  printTemperature(deviceAddress);
  
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  //Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  //Serial.println("DONE");
  
checktemp();


  Serial1.write(12);
  Serial1.print(String(avTemp, 2));
  
  // print the device information
  time = millis()/1000.0;
  Serial.print(time);
  Serial.print(" ");
/*  printData(insideThermometer);
  Serial.print(" ");
  printData(outsideThermometer);
  Serial.print(" ");*/
  Serial.print(avTemp);
  Serial.print(" ");
  if(sensors.getTempC(insideThermometer) - sensors.getTempC(outsideThermometer)>15.0||sensors.getTempC(insideThermometer) - sensors.getTempC(outsideThermometer)<-15.0)
  Serial.print("ERROR ");
  Serial.print(UP);
  Serial.println();
}
