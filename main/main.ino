void setup() {
  // put your setup code here, to run once:
  
  /*Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Lets Get Toasty!");
  
  buttonSetup();*/

  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

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
  // show the addresses we found on the bus
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
  
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  buttonRefresh();
  if (wasUpPressed()) {
    Serial.println("Up!");
  }
  if (wasDownPressed()) {
    Serial.println("Down!");
  }
  if (wasLeftPressed()) {
    Serial.println("Left!");
  }
  if (wasRightPressed()) {
    Serial.println("Right!");
  }
  if (wasPressPressed()) {
    Serial.println("Press!");
  }
  clearPresses();*/

  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  printData(insideThermometer);
  printData(outsideThermometer);
  
}
