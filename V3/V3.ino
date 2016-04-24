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
//   4 - Up Button
//   5 - Down Button
//   6 - Left Button
//   7 - Right Button
//   8 - Select Button
/*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%*%**%*%*%*%*%*%*%*%*%*%*/

// Define some global constants
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 11
#define UP_PIN 4
#define DOWN_PIN 5
#define LEFT_PIN 6
#define RIGHT_PIN 7
#define SELECT_PIN 8
const int SSRPin = 13;
const int FanPin = 12;
const int LCD_TEMPERATURE_DECIMALS = 2;
const int logInterval = 1000; // In ms, set to 0 for lots of logs
const int tempInterval = 1000;
const int maxAllowedTempDiff = 30;

#define PRHT 1
#define SOAK 2
#define RAMP 3
#define COOL 4

String runType; // One of "Lead" "Lead-Free" "Custom"

enum MainState { menu, customTemplateMenu, customMenu, run } ;
MainState mainState = menu;
MainState lastState = menu;

// Some constants and global variables for the custom menu
#define CUSTOM_MENU_SIZE 14
#define eCANCEL_A 0
#define ePRHT_H_DUTY_ON 1
#define ePRHT_H_DUTY_OFF 2
#define eSOAK_H_DUTY_ON 3
#define eSOAK_H_DUTY_OFF 4
#define eRAMP_H_DUTY_ON 5
#define eRAMP_H_DUTY_OFF 6
#define eCOOL_F_DUTY_ON 7
#define eCOOL_F_DUTY_OFF 8
#define eSOAK_START_TEMP 9
#define eRAMP_START_TEMP 10
#define eCOOL_START_TEMP 11
#define eRUN 12
#define eCANCEL_B 13

int customMenuState = ePRHT_H_DUTY_ON;

// These globals can be modified in the code
// They affect the duty cycles of the fan and the heat SSRs
boolean fanPowered = false;
int fanDutyOn = 0; // How many milliseconds to keep the fan on in the duty cycle
int fanDutyOff = 0; // How many milliseconds to keep the fan off in the duty cycle

boolean heatPowered = false;
int heatDutyOn = 0; // How many milliseconds to keep the heat on in the duty cycle
int heatDutyOff = 0; // How many milliseconds to keep the heat off in the duty cycle

boolean drawAgain = false;

// This global can be used throuought the code as the most recent temperature reading 
double avTemp = 0; // Stored in C
double tempI  = 0; // Stored in C
double tempO  = 0; // Stored in C

// This global stores the current stage of the curve
String stage;
unsigned int profileStage = PRHT;

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

  // Set up pinmodes for used pins
  pinMode(SSRPin, OUTPUT);
  pinMode(FanPin, OUTPUT);

  // Start up the OneWire library
  sensors.begin();

  // Don't wait when we request temperatures
  sensors.setWaitForConversion(false);

  //Set up addresses for the two thermometers
  sensors.getAddress(insideThermometer, 0);
  sensors.getAddress(outsideThermometer, 1);

  // set the resolution to 9 bit
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);

  buttonSetup();

  mainMenuInit();
  customMenuState = ePRHT_H_DUTY_ON;

  // Intialize the serial connnection to the LCD
  // Do this last because the LCD needs to power on
  Serial1.begin(19200);
  Serial1.write(17); // Turn backlight on
  Serial1.write(22); // Turn cursor display off
  Serial1.write(12); // Return to first character
  Serial1.write("Let's Get Toasty!");
}

////////// Collect Data
long lastTempTime = -tempInterval;
String err;

// A function to collect thermometer readings and update the global variables
// Set globals: avTemp
void collectData() {
  unsigned long now = millis();
  double lastAvg = avTemp;
  if (lastTempTime + tempInterval < now) {
    sensors.requestTemperatures();
    lastTempTime = now;
  }
  if (sensors.isConversionAvailable(insideThermometer) && sensors.isConversionAvailable(outsideThermometer)){
    tempI = sensors.getTempC(insideThermometer);
    tempO = sensors.getTempC(outsideThermometer);
    avTemp = (tempI + tempO)/2;
    if ((tempI - tempO > maxAllowedTempDiff) || (tempI - tempO < -maxAllowedTempDiff)) {
      err = "ERROR";
    } else {
      err = "";
    }
    if (lastAvg != avTemp){
      drawAgain = true;
    }
  }
}

////////// Load Profile

int PRHT_H_DUTY_ON;
int PRHT_H_DUTY_OFF;
int SOAK_H_DUTY_ON;
int SOAK_H_DUTY_OFF;
int RAMP_H_DUTY_ON;
int RAMP_H_DUTY_OFF;
int COOL_F_DUTY_ON;
int COOL_F_DUTY_OFF;

int SOAK_START_TEMP;
int RAMP_START_TEMP;
int COOL_START_TEMP;

void loadCustomProfile() {
  Serial.println("Loading Custom Profile");
  runType = "Custom";
  mainState = customMenu;
  customMenuState = ePRHT_H_DUTY_ON;
  profileStage = PRHT;
  CM_init();
}

void loadLeadProfile() {
  Serial.println("Loading Lead Profile");
  PRHT_H_DUTY_ON = 2000;
  PRHT_H_DUTY_OFF = 505;
  SOAK_H_DUTY_ON = 2000;
  SOAK_H_DUTY_OFF = 510;
  RAMP_H_DUTY_ON = 100;
  RAMP_H_DUTY_OFF = 0;
  COOL_F_DUTY_ON = 100;
  COOL_F_DUTY_OFF = 0;

  SOAK_START_TEMP = 100;
  RAMP_START_TEMP = 150;
  COOL_START_TEMP = 230;

  runType = "Lead";
  mainState = run;
  profileStage = PRHT;
}

void loadLeadFreeProfile() {
  Serial.println("Loading Lead Free Profile");
  PRHT_H_DUTY_ON = 2000;
  PRHT_H_DUTY_OFF = 505;
  SOAK_H_DUTY_ON = 2000;
  SOAK_H_DUTY_OFF = 510;
  RAMP_H_DUTY_ON = 100;
  RAMP_H_DUTY_OFF = 0;
  COOL_F_DUTY_ON = 100;
  COOL_F_DUTY_OFF = 0;

  SOAK_START_TEMP = 100;
  RAMP_START_TEMP = 150;
  COOL_START_TEMP = 230;
  
  runType = "Lead Free";
  mainState = run;
  profileStage = PRHT;
}

////////// Apply Profile

// Only touch this in applyProfile
unsigned long stageStartTime = 0;

// Sets the globals related to outputting on the SSRs
// depending on the temperature to create a heating curve
// Used globals: avgTemp
// Set globals: fanPowered, fanDutyOn, fanDutyOff, heatPowered, heatDutyOn, and heatDutyOff
void applyProfile() {
  unsigned long now = millis();
  long stageElapsedTime = now - stageStartTime;

  if (err != "") {
    profileStage = COOL;
  }
  
  switch (profileStage) {
    case PRHT:
      if (avTemp < SOAK_START_TEMP) { // While temp is less than 100 C
        heatPowered = true;
        heatDutyOn = PRHT_H_DUTY_ON;
        heatDutyOff = PRHT_H_DUTY_OFF; // Duty cycle of [2000 ms on | 505 ms off]
        fanPowered = false;
        stage = "PRHT";
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 1
    case SOAK:
      if (avTemp < RAMP_START_TEMP) { // While temp is less than 150 C 
        heatPowered = true;
        heatDutyOn = SOAK_H_DUTY_ON;
        heatDutyOff = SOAK_H_DUTY_OFF; // Duty cycle of [2000 ms on | 510 ms off]
        fanPowered = false;
        stage = "SOAK";
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 2
    case RAMP:
      if (avTemp < COOL_START_TEMP) { // While temp is less than 230 C
        heatPowered = true;
        heatDutyOn = RAMP_H_DUTY_ON;
        heatDutyOff = RAMP_H_DUTY_OFF; // Duty cycle of [100 ms on | 0 ms off] (never turns off)
        fanPowered = false;
        stage = "RAMP";
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 3
    case COOL: // Venting
      if (avTemp > 50) { // While temp is more than 50 C
        heatPowered = false;
        fanPowered = true;
        fanDutyOn = COOL_F_DUTY_ON;
        fanDutyOff = COOL_F_DUTY_OFF; // Duty cycle of [100 ms on | 0 ms off] (never turns off)
        stage = "COOL";
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 4 
    default: 
      // Invalid profile stage selected, disabling SSR and FAN
      fanPowered = false;
      heatPowered = false;
      mainMenuInit();
    break;
  }
}

////////// Display to LCD

// Display the current temperature in Fahrenheit to the LCD
// Used globals: avgTemp, stage
void displayToLCD() {
  if (drawAgain) {
    Serial1.write(12);
    Serial1.print(String(avTemp, LCD_TEMPERATURE_DECIMALS));
    Serial1.print(" C");
    if (avTemp < 100) {
      Serial1.print(" ");
    } 
    Serial1.print("    ");
    Serial1.print(stage);
    if (err != "") {
      Serial1.print(err);
      Serial1.print(" ");
    }
    Serial1.print(runType);
    drawAgain = false;
  }
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
    Serial.print(" | ");
    Serial.print(avTemp);
    Serial.print(" | ");
    Serial.print(tempI);
    Serial.print(" ");
    Serial.print(tempO);
    Serial.print(" | ");
    Serial.print(tempI - tempO);
    if (err != "") {
      Serial.print(" | ");
      Serial.print(err);
    }
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
        //digitalWrite(FanPin, LOW); // Remove this line to stop miniscule offs at 100% cycle
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
        //digitalWrite(FanPin, HIGH); // Remove this line to stop miniscule ons at 0% cycle
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


void watchTempDuringMenu() {
  digitalWrite(SSRPin, LOW); // Heater is off
  collectData();
  if (avTemp > 50) { // We're back in menu mode, but we're still too hot, run the fans
    digitalWrite(FanPin, HIGH);
  } else {
    digitalWrite(FanPin, LOW);
  }
}

////////// Loop

// The main loop
void loop() {
  // put your main code here, to run repeatedly:
  buttonRefresh();
  if (mainState == menu) {
    watchTempDuringMenu();
    reactToButtons();
    displayMenu();
  } else if (mainState == customTemplateMenu) {
    watchTempDuringMenu();
    CTMreactToButtons();
    CTMdisplayMenu();
  } else if (mainState == customMenu) {
    watchTempDuringMenu();
    CM_reactToButtons();
    CM_displayMenu();
  } else {
    collectData();    // Refreshes temperature readings
    if (wasSelectPressed()) { // Go back to menu mode
      mainMenuInit();
    }
    applyProfile();   // Sets global variables for handleSSRs;
    displayToLCD();   // Puts the current temperature on the LCD
    logToUSBSerial(); // Logs data to the USB serial conneciton
    handleSSRs();     // Pulses the fan and heat SSRs according to global variables
  }
  if (lastState != mainState) {
    drawAgain = true;
    lastState = mainState;
  }
  clearPresses();
}

