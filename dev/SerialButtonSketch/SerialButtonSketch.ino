const int FOneButtonPin = 2;     // the number of the pushbutton pin
const int FTenButtonPin = 3;     // the number of the pushbutton pin
const int BOneButtonPin = 4;     // the number of the pushbutton pin
const int BTenButtonPin = 5;     // the number of the pushbutton pin

int numberState = 128;
bool hold = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(FOneButtonPin, INPUT);
  pinMode(FTenButtonPin, INPUT);
  pinMode(BOneButtonPin, INPUT);
  pinMode(BTenButtonPin, INPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // read the states of the pins.
  int FOneButtonState = digitalRead(FOneButtonPin);
  int FTenButtonState = digitalRead(FTenButtonPin);
  int BOneButtonState = digitalRead(BOneButtonPin);
  int BTenButtonState = digitalRead(BTenButtonPin);

  // if we were currently held down, see if we have been released
  if (hold) {
    if (FOneButtonState == LOW && FTenButtonState == LOW &&
        BOneButtonState == LOW && BTenButtonState == LOW) {
      hold = false;
    }
  } else { // If not, check to see if any of the buttons have been pressed, and do the action associated with that button
    if (FOneButtonState == HIGH){
      numberState += 1;
      hold = true;
      Serial.print(numberState);
    } else if (FTenButtonState == HIGH){
      numberState += 10;
      hold = true;
      Serial.print(numberState);
    } else if (BOneButtonState == HIGH){
      numberState -= 1;
      hold = true;
      Serial.print(numberState);
    } else if (BTenButtonState == HIGH){
      numberState -= 10;
      hold = true;
      Serial.print(numberState);
    }
  }
  
}
