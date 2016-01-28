//BUTTON PIN CONSTANTS
#define UP_PIN 2
#define DOWN_PIN 3
#define LEFT_PIN 4
#define RIGHT_PIN 5
#define PRESS_PIN 6

//SETTING FOR TUNING THE BOUNCE SETTING
#define PRESS_DEFAULT 1000

//Button code below
int up_laststate = 0;
int down_laststate = 0;
int left_laststate = 0;
int right_laststate = 0;
int press_laststate = 0;

int up_presscounter = 0;
int down_presscounter = 0;
int left_presscounter = 0;
int right_presscounter = 0;
int press_presscounter = 0;

bool unreadUpPress = false;
bool unreadDownPress = false;
bool unreadLeftPress = false;
bool unreadRightPress = false;
bool unreadPressPress = false;

void buttonSetup() {
  //Set the pins for input
  pinMode(UP_PIN,    INPUT);
  pinMode(DOWN_PIN,  INPUT);
  pinMode(LEFT_PIN,  INPUT);
  pinMode(RIGHT_PIN, INPUT);
  pinMode(PRESS_PIN, INPUT);
}

void buttonRefresh() {
  int val;

  //UP
  val = digitalRead(UP_PIN);
  if (val == up_laststate) {
    up_presscounter = PRESS_DEFAULT;
  } else {
    if (up_presscounter == 0) {
      //We have pressed/depressed the button
      if (val == 1) {
        //We have pressed the button
        unreadUpPress = true;
      }
      
      up_laststate = val;
      up_presscounter = PRESS_DEFAULT; //reset the press_counter for next button bounce check
    } else {
      up_presscounter -= 1; //decrement the button press counter by 1
    }
  }

  //DOWN
  val = digitalRead(DOWN_PIN);
  if (val == down_laststate) {
    down_presscounter = PRESS_DEFAULT;
  } else {
    if (down_presscounter == 0) {
      //We have pressed/depressed the button
      if (val == 1) {
        //We have pressed the button
        unreadDownPress = true;
      }
      
      down_laststate = val;
      down_presscounter = PRESS_DEFAULT; //reset the press_counter for next button bounce check
    } else {
      down_presscounter -= 1; //decrement the button press counter by 1
    }
  }

  //LEFT
  val = digitalRead(LEFT_PIN);
  if (val == left_laststate) {
    left_presscounter = PRESS_DEFAULT;
  } else {
    if (left_presscounter == 0) {
      //We have pressed/depressed the button
      if (val == 1) {
        //We have pressed the button
        unreadLeftPress = true;
      }
      
      left_laststate = val;
      left_presscounter = PRESS_DEFAULT; //reset the press_counter for next button bounce check
    } else {
      left_presscounter -= 1; //decrement the button press counter by 1
    }
  }

  //RIGHT
  val = digitalRead(RIGHT_PIN);
  if (val == right_laststate) {
    right_presscounter = PRESS_DEFAULT;
  } else {
    if (right_presscounter == 0) {
      //We have pressed/depressed the button
      if (val == 1) {
        //We have pressed the button
        unreadRightPress = true;
      }
      
      right_laststate = val;
      right_presscounter = PRESS_DEFAULT; //reset the press_counter for next button bounce check
    } else {
      right_presscounter -= 1; //decrement the button press counter by 1
    }
  }

  //PRESS
  val = digitalRead(PRESS_PIN);
  if (val == press_laststate) {
    press_presscounter = PRESS_DEFAULT;
  } else {
    if (press_presscounter == 0) {
      
      //We have pressed/depressed the button
      if (val == 1) {
        //We have pressed the button
        unreadPressPress = true;
      }
      
      press_laststate = val;
      press_presscounter = PRESS_DEFAULT; //reset the press_counter for next button bounce check
    } else {
      press_presscounter -= 1; //decrement the button press counter by 1
    }
  }
  
}

bool wasUpPressed() {
  return unreadUpPress;
}

bool wasDownPressed() {
  return unreadDownPress;
}

bool wasLeftPressed() {
  return unreadLeftPress;
}

bool wasRightPressed() {
  return unreadRightPress;
}

bool wasPressPressed() {
  return unreadPressPress;
}

void clearPresses() {
  unreadUpPress = false;
  unreadDownPress = false;
  unreadLeftPress = false;
  unreadRightPress = false;
  unreadPressPress = false;
}
