int inPin = 2; //digital pin for push button 1
int val  = 0; //variable for storing the read value
int last_val = 0; //variable for storing the last read value
const int PRESS_DEFAULT = 1000; //the number of loops for a button bounce check
int press_counter = PRESS_DEFAULT; //variable for checking for button bounces


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Lets Get Toasty!");
  pinMode(inPin, INPUT); //sets the digital pin 2 as input
}


void loop() {
  val = digitalRead(inPin); //read the push button pin 2
  if (val == last_val) {
    press_counter = PRESS_DEFAULT;
  } else {
    
    if (press_counter == 0) {
      Serial.println(val); 
      last_val = val;
      press_counter = PRESS_DEFAULT; //reset the press_counter for next button bounce check
    } else {
        press_counter -= 1; //decrement the button press counter by 1
    }
    
  }
  
}
