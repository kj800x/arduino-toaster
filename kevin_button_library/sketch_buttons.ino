void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Lets Get Toasty!");
  
  buttonSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
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
  clearPresses();
}
