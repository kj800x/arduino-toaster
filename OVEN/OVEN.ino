// ============================================================ 
//                                                              
// Wireless Club - Reflow Oven - V4                        
//                                                            
// ============================================================ 

// Include libraries
#include <Wire.h> 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <LCDMenuLib.h>

// Configure libraries
#define _LCDML_DISP_cfg_button_press_time          200    // button press time in ms
#define _LCDML_DISP_cfg_scrollbar                  1      // enable a scrollbar
#define _LCDML_DISP_cfg_cursor                     0x7E   // cursor Symbol 

// Define pins
#define UP_PIN 4
#define DOWN_PIN 5
#define LEFT_PIN 6
#define RIGHT_PIN 7
#define SELECT_PIN 8
#define ONE_WIRE_BUS 9
#define TEMPERATURE_PRECISION 11
#define FAN_PIN 12
#define SSR_PIN 13

// Define core values
const int LCD_TEMPERATURE_DECIMALS = 2; // How many decimals use when displaying to the LCD
const int logInterval = 1000; // In ms, set to 0 for lots of logs
const int tempInterval = 1000; // In ms, time between temperature readings (Values too short will adversly affect slope calculations)
const int MAX_ALLOWED_TEMP_DIFF = 30; // in C, The temperature difference that is allowed before an error is triggered
const int MENU_FAN_TEMPERATURE = 50; // in C, The temperature that the fan will continue to run and cool until during menus
const double DOOR_OPEN_TRIGGER_SLOPE = -2.0; // in C, The temperature slope that will trigger the door open flag

// Should we display "Open the door" instead of our usual run data
boolean displayDoorMessage = false;

// Some easy way to refer to the different stages of the run
#define PRHT 1
#define SOAK 2
#define RAMP 3
#define COOL 4
#define OPEN 5


// Data storing the current overall state of the program
enum MainState { menu, customTemplateMenu, customMenu, run } ;
MainState mainState = menu;
MainState lastState = menu;

// A string storing the type of run we are currently doing
String runType; // One of "Lead" "Lead-Free" "Custom"

// These globals can be modified in the code
// They affect the duty cycles of the fan and the heat SSRs
boolean fanPowered = false;
int fanDutyOn = 0; // How many milliseconds to keep the fan on in the duty cycle
int fanDutyOff = 0; // How many milliseconds to keep the fan off in the duty cycle

boolean heatPowered = false;
int heatDutyOn = 0; // How many milliseconds to keep the heat on in the duty cycle
int heatDutyOff = 0; // How many milliseconds to keep the heat off in the duty cycle

// Do we have new information? Should we redraw the LCD?
boolean drawAgain = false;

// This global can be used throuought the code as the most recent temperature reading
double avTemp = 0; // Stored in C
double tempI  = 0; // Stored in C
double tempO  = 0; // Stored in C

// Have we detected that the door was opened this run?
boolean doorOpenDetected = false;

// This global refers to the previous temperature reading (used to calculate the slope)
double lastTempReading = 0.0;

// This global can be used to refer to the most recent temperature slope reading (updates every tempInterval)
double tempSlope = 0.0;

// This global stores the current stage of the curve
String stage;
unsigned int profileStage = PRHT;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Arrays to hold device addresses
DeviceAddress insideThermometer, outsideThermometer;


// ********************************************************************* 
// LCDML TYPE SELECT
// *********************************************************************

// settings for lcd 
#define _LCDML_DISP_cols             20
#define _LCDML_DISP_rows             4  

// lcd object
//LiquidCrystal_I2C lcd(0x27);  // Set the LCD I2C address
//LiquidCrystal_I2C lcd(0x27, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

const uint8_t scroll_bar[5][8] = {
    {B10001, B10001, B10001, B10001, B10001, B10001, B10001, B10001}, // scrollbar top
    {B11111, B11111, B10001, B10001, B10001, B10001, B10001, B10001}, // scroll state 1 
    {B10001, B10001, B11111, B11111, B10001, B10001, B10001, B10001}, // scroll state 2
    {B10001, B10001, B10001, B10001, B11111, B11111, B10001, B10001}, // scroll state 3
    {B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111}  // scrollbar bottom
}; 

// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************

// create menu
// menu element count - last element id
// this value must be the same as the last menu element
#define _LCDML_DISP_cnt    12

// LCDML_root        => layer 0 
// LCDML_root_X      => layer 1 
// LCDML_root_X_X    => layer 2 
// LCDML_root_X_X_X  => layer 3 
// LCDML_root_... 	 => layer ... 

// LCDMenuLib_add(id, group, prev_layer_element, new_element_num, lang_char_array, callback_function)
LCDML_DISP_init(_LCDML_DISP_cnt);
LCDML_DISP_add      (0  , _LCDML_G1  , LCDML_root        , 1  , "Reflow Info"        , LCDML_FUNC_information);
LCDML_DISP_add      (1  , _LCDML_G1  , LCDML_root        , 2  , "Lead Profile"       , LCDML_FUNC_lead);
LCDML_DISP_add      (2  , _LCDML_G1  , LCDML_root        , 3  , "Lead-free Profile"  , LCDML_FUNC);
LCDML_DISP_add      (3  , _LCDML_G1  , LCDML_root        , 4  , "Custom Profile"     , LCDML_FUNC);
LCDML_DISP_add      (4  , _LCDML_G1  , LCDML_root_3      , 1  , "CompoundA"          , LCDML_FUNC);
LCDML_DISP_add      (5  , _LCDML_G1  , LCDML_root_3      , 2  , "CompoundB"          , LCDML_FUNC);
LCDML_DISP_add      (6  , _LCDML_G1  , LCDML_root_4      , 1  , "Program 1"          , LCDML_FUNC);
LCDML_DISP_add      (7  , _LCDML_G1  , LCDML_root_4_1    , 1  , "P1 start"           , LCDML_FUNC);
LCDML_DISP_add      (8  , _LCDML_G1  , LCDML_root_4_1    , 2  , "Settings"           , LCDML_FUNC);
LCDML_DISP_add      (9  , _LCDML_G1  , LCDML_root_4_1_2  , 1  , "Warm"               , LCDML_FUNC);
LCDML_DISP_add      (10 , _LCDML_G1  , LCDML_root_4_1_2  , 2  , "Long"               , LCDML_FUNC);
LCDML_DISP_add      (11 , _LCDML_G1  , LCDML_root_4      , 2  , "Program 2"          , LCDML_FUNC_p2);
LCDML_DISP_add      (12 , _LCDML_G1  , LCDML_root        , 5  , "Null"               , LCDML_FUNC);
LCDML_DISP_createMenu(_LCDML_DISP_cnt);


// ********************************************************************* 
// LCDML BACKEND (core of the menu)
// ********************************************************************* 

// define backend function  
#define _LCDML_BACK_cnt    1  // last backend function id

LCDML_BACK_init(_LCDML_BACK_cnt);
LCDML_BACK_new_timebased_dynamic (0  , ( 20UL )         , _LCDML_start  , LCDML_BACKEND_control);
LCDML_BACK_new_timebased_dynamic (1  , ( 1000UL )       , _LCDML_stop   , LCDML_BACKEND_menu);
LCDML_BACK_create();


// *********************************************************************
// SETUP
// *********************************************************************

void setup() {  
  // Set up pinmodes for used pins
  pinMode(SSR_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  // Start up the OneWire library
  sensors.begin();
  
  // Don't wait when we request temperatures
  sensors.setWaitForConversion(false);

  //Set up addresses for the two thermometers
  sensors.getAddress(insideThermometer, 0);
  sensors.getAddress(outsideThermometer, 1);

  // Set the resolution to 9 bit
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
    
  // LCD Begin
  lcd.begin(_LCDML_DISP_cols,_LCDML_DISP_rows);  
  lcd.setCursor(0,1);
  lcd.print("Starting Reflow Oven");
  delay(500);
  lcd.home ();      
  // set special chars for scrollbar
  lcd.createChar(0, (uint8_t*)scroll_bar[0]);
  lcd.createChar(1, (uint8_t*)scroll_bar[1]);
  lcd.createChar(2, (uint8_t*)scroll_bar[2]);
  lcd.createChar(3, (uint8_t*)scroll_bar[3]);
  lcd.createChar(4, (uint8_t*)scroll_bar[4]);
  lcd.setCursor(0,0);
  lcd.print(F("booting"));
  
  // Enable all items with _LCDML_G1
  LCDML_DISP_groupEnable(_LCDML_G1); // enable group 1
  
  // LCDMenu Setup
  LCDML_setup(_LCDML_BACK_cnt);

  //buttonSetup();
  // Initialize the menus, the last one to be initialized will be the one we start at
  //customTemplateMenuInit();
  //customMenuInit();
  //mainMenuInit();
}


////////// Collect Data
long lastTempTime = -tempInterval;
boolean err;

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
    lastTempReading = avTemp;
    tempI = sensors.getTempC(insideThermometer);
    tempO = sensors.getTempC(outsideThermometer);
    avTemp = (tempI + tempO)/2;
    tempSlope = (avTemp - lastTempReading) / (tempInterval / 1000.0);
    if (tempSlope < DOOR_OPEN_TRIGGER_SLOPE) {
      doorOpenDetected = true;
    }
    if ((tempI - tempO > MAX_ALLOWED_TEMP_DIFF) || (tempI - tempO < -MAX_ALLOWED_TEMP_DIFF)) {
      err = true;
    } else {
      err = false;
    }
    if (lastAvg != avTemp){
      drawAgain = true;
    }
  }
}

////////// Load Profile

// Variables to store the profile: (1000ms = 1s)
int PRHT_H_DUTY_ON; // In ms, how long is the heater on during a PRHT duty cycle
int PRHT_H_DUTY_OFF; // In ms, how long is the heater off during a PRHT duty cycle
int SOAK_H_DUTY_ON; // In ms, how long is the heater on during a SOAK duty cycle
int SOAK_H_DUTY_OFF; // In ms, how long is the heater off during a SOAK duty cycle
int RAMP_H_DUTY_ON; // In ms, how long is the heater on during a RAMP duty cycle
int RAMP_H_DUTY_OFF; // In ms, how long is the heater off during a RAMP duty cycle
int COOL_F_DUTY_ON; // In ms, how long is the fan on during a COOL duty cycle
int COOL_F_DUTY_OFF; // In ms, how long is the fan off during a COOL duty cycle
int SOAK_START_TEMP; // In C, the temperature where PRHT turns into SOAK
int RAMP_START_TEMP; // In C, the temperature where SOAK turns into RAMP
int COOL_START_TEMP; // In C, the temperature where RAMP turns into COOL
int OPEN_START_TIME; // Number of seconds after which COOL turns into OPEN

// Load the Lead profile
void loadLeadProfile() {
  lcd.setCursor(0,0);
  lcd.print("Loading Lead Profile");
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
  OPEN_START_TIME = 120;

  runType = "Lead";
  mainState = run;
  profileStage = PRHT;
}

// Load the Lead Free profile
// TODO: Values need to be determined
void loadLeadFreeProfile() {
  lcd.print("Loading Lead Free Profile");
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
  OPEN_START_TIME = 120;

  runType = "Lead Free";
  mainState = run;
  profileStage = PRHT;
}


/*
////////// Beep

// Beeps using the speaker on the LCD
// Piezoelectric speaker no-longer on serial.
void beep() {
  Serial1.write(214); // Set beep time for 2 sec
  Serial1.write(217); // Set octave to 5
  Serial1.write(220); // Play A
}
*/



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

  if (err) { // If we've detected an error, throw us into open immediately
    profileStage = OPEN;
  }

  switch (profileStage) {
    case PRHT:
      if (avTemp < SOAK_START_TEMP) {
        heatPowered = true;
        heatDutyOn = PRHT_H_DUTY_ON;
        heatDutyOff = PRHT_H_DUTY_OFF;
        fanPowered = false;
        stage = "PRHT";
        doorOpenDetected = false;
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
    case COOL: // Venting (Door closed)
      if ((now - stageStartTime) / 1000 > OPEN_START_TIME) { // Go to OPEN
        profileStage++;
        stageStartTime = now;
        doorOpenDetected = false;
      }
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
    case OPEN: // Venting (Door open)
      if (avTemp > 50) { // While temp is more than 50 C
        if (doorOpenDetected) {
          displayDoorMessage = false;
        } else {
          //beep();
          displayDoorMessage = true;
        }
        heatPowered = false;
        fanPowered = true;
        fanDutyOn = COOL_F_DUTY_ON;
        fanDutyOff = COOL_F_DUTY_OFF; // Duty cycle of [100 ms on | 0 ms off] (never turns off)
        stage = "OPEN";
        displayDoorMessage = true;
      } else {
        profileStage++;
        stageStartTime = now;
      }
      break; // End profileStage 5
    default:
      // Profile stage fell off the end of the sequence, go back to main menu
      fanPowered = false;
      heatPowered = false;
      //mainMenuInit();
    break;
  }
}

////////// Display to LCD

// Display run information (temp, stage, run type) to the LCD
// Used globals: avgTemp, stage, runType


void displayToLCD() {
  if (drawAgain) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (displayDoorMessage) {
      lcd.print("OPEN THE DOOR");
      //Serial1.write(13);
      lcd.print(String(avTemp, LCD_TEMPERATURE_DECIMALS));
      lcd.print(" C");
    } else {
      lcd.print(String(avTemp, LCD_TEMPERATURE_DECIMALS));
      lcd.print(" C");
      if (avTemp < 100) {
        lcd.print(" ");
      }
      lcd.print("    ");
      lcd.print(stage);
      if (err) {
        lcd.print("ERROR");
        lcd.print(" ");
      }
      lcd.print(runType);
    }
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
    Serial.print(" | ");
    Serial.print(tempSlope);
    if (err) {
      Serial.print(" | ");
      Serial.print("ERROR");
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
        //digitalWrite(, LOW); // Remove this line to stop miniscule offs at 100% cycle
        fanPhaseStartTime = millis();
        fanInOnPhase = false;
      } else {
        // Fan can stay on
        digitalWrite(FAN_PIN, HIGH);
      }
    } else {
      // Fan is off, maybe needs to turn on?
      if (fanPhaseStartTime + fanDutyOff < millis()){
        // Time to turn on
        //digitalWrite(FAN_PIN, HIGH); // Remove this line to stop miniscule ons at 0% cycle
        fanPhaseStartTime = millis();
        fanInOnPhase = true;
      } else {
        // Fan can stay off
        digitalWrite(FAN_PIN, LOW);
      }
    }
  } else {
    digitalWrite(FAN_PIN, LOW);
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
        digitalWrite(SSR_PIN, HIGH);
      }
    } else {
      // Heat is off, maybe needs to turn on?
      if (heatPhaseStartTime + heatDutyOff < millis()){
        // Time to turn on
        heatPhaseStartTime = millis();
        heatInOnPhase = true;
      } else {
        // Heat can stay off
        digitalWrite(SSR_PIN, LOW);
      }
    }
  } else {
    digitalWrite(SSR_PIN, LOW);
  }
}

// Watch the temperature during the menus. If it's over a certain number, run the fans.
void watchTempDuringMenu() {
  digitalWrite(SSR_PIN, LOW); // Heater is off
  collectData();
  if (avTemp > MENU_FAN_TEMPERATURE) { // We're back in menu mode, but we're still too hot, run the fans
    digitalWrite(FAN_PIN, HIGH);
  } else {
    digitalWrite(FAN_PIN, LOW);
  }
  displayDoorMessage = false;
}



// *********************************************************************
// LOOP
// *********************************************************************

void loop() { 
  // this function must called here, do not delete it
  LCDML_run(_LCDML_priority); 
  // put your main code here, to run repeatedly:
  //  buttonRefresh();
  if (mainState == menu) {
    //watchTempDuringMenu();
    //reactToButtons();
    //displayMenu();
  } else if (mainState == customTemplateMenu) {
      watchTempDuringMenu();
      //CTMreactToButtons();
      //CTMdisplayMenu();
  } else if (mainState == customMenu) {
      watchTempDuringMenu();
      //CM_reactToButtons();
      //CM_displayMenu();
  } else {
      collectData();    // Refreshes temperature readings
      //if (wasSelectPressed()) { // Go back to menu mode
      //mainMenuInit();
  }
    
  applyProfile();   // Sets global variables for handleSSRs;
  displayToLCD();   // Puts the current temperature on the LCD
  logToUSBSerial(); // Logs data to the USB serial conneciton
  handleSSRs();     // Pulses the fan and heat SSRs according to global variables
  //  }

  // A note for this next bit of code
  // This is necessary because the following can happen:
  // 1) a mainState change happens (drawAgain gets set to true)
  // 2) a draw to lcd function gets run (drawAgain gets set to false)
  // 3) the mainState has changed and we're in a different part of the code,
  //    but the lcd still hasn't gotten drawn and it still looks like we're in
  //    the old state
  if (lastState != mainState) {
    drawAgain = true;
    lastState = mainState;
  }

  //clearPresses();
}


// *********************************************************************
// check some errors - do not change here anything
// *********************************************************************
# if(_LCDML_DISP_rows > _LCDML_DISP_cfg_max_rows)
# error change value of _LCDML_DISP_cfg_max_rows in LCDMenuLib.h
# endif
# if(_LCDML_DISP_cols > _LCDML_DISP_cfg_max_string_length)
# error change value of _LCDML_DISP_cfg_max_string_length in LCDMenuLib.h
# endif
