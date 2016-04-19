#include <OneWire.h>
#include <DallasTemperature.h>

/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%**%*%*%*%*%*%*%*%*%*%/
//Configuration Settings
//Board: Arduino Genuino Micro
//Programmer: AVR ISP
//Pins:
//  13 - SSR for Heat
//  12 - SSR for Fan
//   2 - OneWire Bus
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%**%*%*%*%*%*%*%*%*%*%*/

// Define some global constants
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9
const int SSRPin = 13;
const int FanPin = 12;
const int LCD_TEMPERATURE_DECIMALS = 2;
const int logInterval = 1000; // In ms, set to 0 for lots of logs

// These globals can be modified in the code
// They affect the duty cycles of the fan and the heat SSRs
boolean fanPowered = false;
int fanDutyOn = 0; // How many milliseconds to keep the fan on in the duty cycle
int fanDutyOff = 0; // How many milliseconds to keep the fan off in the duty cycle

boolean heatPowered = false;
int heatDutyOn = 0; // How many milliseconds to keep the heat on in the duty cycle
int heatDutyOff = 0; // How many milliseconds to keep the heat off in the duty cycle

// This global can be used throuought the code as the most recent temperature reading 
double avTemp; // Stored in C

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Arrays to hold device addresses
DeviceAddress insideThermometer, outsideThermometer;

////////// Setup

// The setup code
void setup() {
  // Initialize the serial connection to a connected computer
  Serial.begin(9600);
  Serial.println("Let's Get Toasty!");

  // Intialize the serial connnection to the LCD
  Serial1.begin(19200);
  Serial1.write(17);
  Serial1.write(12);
  Serial1.write("Let's Get Toasty!");

  // Set up pinmodes for used pins
  pinMode(SSRPin, OUTPUT);
  pinMode(FanPin, OUTPUT);

  // Start up the OneWire library
  sensors.begin();

  //Set up addresses for the two thermometers
  sensors.getAddress(insideThermometer, 0);
  sensors.getAddress(outsideThermometer, 1);

  // set the resolution to 9 bit
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
}

////////// Collect Data

// A function to collect thermometer readings and update the global variables
// Set globals: avTemp
void collectData() {
  sensors.requestTemperatures();
  avTemp = (sensors.getTempC(insideThermometer) + sensors.getTempC(outsideThermometer))/2;
}

////////// Apply Profile

// Only touch this in applyProfile
unsigned int profileStage = 1;
unsigned long stageStartTime = 0;

// Sets the globals related to outputting on the SSRs
// depending on the temperature to create a heating curve
// Used globals: avgTemp
// Set globals: fanPowered, fanDutyOn, fanDutyOff, heatPowered, heatDutyOn, and heatDutyOff
void applyProfile() {
  unsigned long now = millis();
  long stageElapsedTime = now - stageStartTime;
  switch (profileStage) {
    case 1:
      if (avTemp < 100) { // While temp is less than 100 C
        heatPowered = true;
        heatDutyOn = 2000;
        heatDutyOff = 5; // Duty cycle of [2000 ms on | 5 ms off]
        fanPowered = false;
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 1
    case 2:
      if (avTemp < 150) { // While temp is less than 150 C 
        heatPowered = true;
        heatDutyOn = 2000;
        heatDutyOff = 10; // Duty cycle of [2000 ms on | 10 ms off]
        fanPowered = false;
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 2
    case 3:
      if (avTemp < 210) { // While temp is less than 210 C
        heatPowered = true;
        heatDutyOn = 100;
        heatDutyOff = 0; // Duty cycle of [100 ms on | 0 ms off] (never turns off)
        fanPowered = false;
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 3
    case 4:
      if (avTemp < 220) { // While temp is less than 220 C
        heatPowered = true;
        heatDutyOn = 1700;
        heatDutyOff = 100; // Duty cycle of [1700 ms on | 100 ms off]
        fanPowered = true;
        fanDutyOn = 100;
        fanDutyOff = 0; // Duty cycle of [100 ms on | 0 ms off] (never turns off)
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 4
    case 5: // Venting
      if (avTemp > 50) { // While temp is more than 50 C
        heatPowered = false;
        fanPowered = true;
        fanDutyOn = 100;
        fanDutyOff = 0; // Duty cycle of [100 ms on | 0 ms off] (never turns off)
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 
    default: 
      // Invalid profile stage selected, disabling SSR and FAN
      fanPowered = false;
      heatPowered = false;
    break;
  }
}

////////// Display to LCD

// Display the current temperature in Fahrenheit to the LCD
// Used globals: avgTemp
void displayToLCD() {
  Serial1.write(12);
  Serial1.print(String(DallasTemperature::toFahrenheit(avTemp), LCD_TEMPERATURE_DECIMALS));
  Serial1.print(" F");
}

////////// Log to USB Serial

// Only touch this in logToUSBSerial
unsigned long lastLogTime = 0;

// Log data to the USB serial port
// Used globals: avgTemp
void logToUSBSerial() {
  unsigned long now = millis();
  if (lastLogTime + logInterval < now){
    Serial.print(now/1000.0);
    Serial.print(" ");
    Serial.print(avTemp);
    Serial.println();
    lastLogTime = now;
  }
}

////////// Handle SSRs

// Don't touch these (Except in handleSSRs)
unsigned long fanPhaseStartTime;
unsigned long heatPhaseStartTime;
boolean fanInOnPhase = false;
boolean heatInOnPhase = true;

// Using the duty cycle global variables, pulse the 2 SSRs.
// Used globals: fanPowered, fanDutyOn, fanDutyOff, heatPowered, heatDutyOn, and heatDutyOff
void handleSSRs() {
  if (fanPowered) {
    if (fanInOnPhase) {
      // Fan is on, maybe needs to turn off?
      if (fanPhaseStartTime + fanDutyOn < millis()){
        // Time to turn off
        digitalWrite(FanPin, LOW); // Remove this line to stop miniscule offs at 100% cycle
        fanPhaseStartTime = millis();
        fanInOnPhase = false;
      } else {
        // Fan can stay on
        digitalWrite(FanPin, HIGH);
      }
    } else {
      // Fan is off, maybe needs to turn on?
      if (fanPhaseStartTime + fanDutyOff < millis()){
        // Time to turn on
        digitalWrite(FanPin, HIGH); // Remove this line to stop miniscule ons at 0% cycle
        fanPhaseStartTime = millis();
        fanInOnPhase = true;
      } else {
        // Fan can stay off
        digitalWrite(FanPin, LOW);
      }
    }
  } else {
    digitalWrite(FanPin, LOW);
  }

  if (heatPowered) {
    if (heatInOnPhase) {
      // Heat is on, maybe needs to turn off?
      if (heatPhaseStartTime + heatDutyOn < millis()){
        // Time to turn off
        digitalWrite(SSRPin, LOW); // Remove this line to stop miniscule offs at 100% cycle
        heatPhaseStartTime = millis();
        heatInOnPhase = false;
      } else {
        // Heat can stay on
        digitalWrite(SSRPin, HIGH);
      }
    } else {
      // Heat is off, maybe needs to turn on?
      if (heatPhaseStartTime + heatDutyOff < millis()){
        // Time to turn on
        digitalWrite(SSRPin, HIGH); // Remove this line to stop miniscule ons at 0% cycle
        heatPhaseStartTime = millis();
        heatInOnPhase = true;
      } else {
        // Heat can stay off
        digitalWrite(SSRPin, LOW);
      }
    }
  } else {
    digitalWrite(SSRPin, LOW);
  }
}

////////// Loop

// The main loop
void loop() {
  // put your main code here, to run repeatedly:
  collectData();    // Refreshes temperature readings
  applyProfile();   // Sets global variables for handleSSRs;
  displayToLCD();   // Puts the current temperature on the LCD
  logToUSBSerial(); // Logs data to the USB serial conneciton
  handleSSRs();     // Pulses the fan and heat SSRs according to global variables
}
