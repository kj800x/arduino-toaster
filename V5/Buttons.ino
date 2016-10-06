//SETTING FOR TUNING THE BOUNCE SETTING
// In ms
#define FIRST_PRESS_DEFAULT 50
#define REP_PRESS_DEFAULT 400

// Buttom variables
int up_laststate = 0;
int down_laststate = 0;
int left_laststate = 0;
int right_laststate = 0;
int select_laststate = 0;

unsigned long up_lastpresstime = 0;
unsigned long down_lastpresstime = 0;
unsigned long left_lastpresstime = 0;
unsigned long right_lastpresstime = 0;
unsigned long select_lastpresstime = 0;

bool unreadUpPress = false;
bool unreadDownPress = false;
bool unreadLeftPress = false;
bool unreadRightPress = false;
bool unreadSelectPress = false;

bool up_hasRegisteredPress = false;
bool down_hasRegisteredPress = false;
bool left_hasRegisteredPress = false;
bool right_hasRegisteredPress = false;
bool select_hasRegisteredPress = false;

// Call this to set up the buttons
void buttonSetup() {
  //Set the pins for input
  pinMode(UP_PIN,    INPUT_PULLUP);
  pinMode(DOWN_PIN,  INPUT_PULLUP);
  pinMode(LEFT_PIN,  INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(SELECT_PIN, INPUT_PULLUP);
}

// Call this often (as often as possible), to ensure you always have the latest
// button information
void buttonRefresh() {
  int val;

  //UP
  val = digitalRead(UP_PIN);
  if (val == up_laststate) { // The button has been consistent
    if (up_hasRegisteredPress) {
      if (up_lastpresstime + REP_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadUpPress = true;
          up_lastpresstime = millis();
          drawAgain = true;
        }
      }
    } else {
      if (up_lastpresstime + FIRST_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadUpPress = true;
          up_hasRegisteredPress = true;
          up_lastpresstime = millis();
          drawAgain = true;
        }
      }
    }
  } else {
    up_laststate = val;
    up_hasRegisteredPress = false;
    up_lastpresstime = millis();
  }

  //DOWN
  val = digitalRead(DOWN_PIN);
  if (val == down_laststate) { // The button has been consistent
    if (down_hasRegisteredPress) {
      if (down_lastpresstime + REP_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadDownPress = true;
          down_lastpresstime = millis();
          drawAgain = true;
        }
      }
    } else {
      if (down_lastpresstime + FIRST_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadDownPress = true;
          down_hasRegisteredPress = true;
          down_lastpresstime = millis();
          drawAgain = true;
        }
      }
    }
  } else {
    down_laststate = val;
    down_hasRegisteredPress = false;
    down_lastpresstime = millis();
  }

  //LEFT
  val = digitalRead(LEFT_PIN);
  if (val == left_laststate) { // The button has been consistent
    if (left_hasRegisteredPress) {
      if (left_lastpresstime + REP_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadLeftPress = true;
          left_lastpresstime = millis();
          drawAgain = true;
        }
      }
    } else {
      if (left_lastpresstime + FIRST_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadLeftPress = true;
          left_hasRegisteredPress = true;
          left_lastpresstime = millis();
          drawAgain = true;
        }
      }
    }
  } else {
    left_laststate = val;
    left_hasRegisteredPress = false;
    left_lastpresstime = millis();
  }

  //RIGHT
  val = digitalRead(RIGHT_PIN);
  if (val == right_laststate) { // The button has been consistent
    if (right_hasRegisteredPress) {
      if (right_lastpresstime + REP_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadRightPress = true;
          right_lastpresstime = millis();
          drawAgain = true;
        }
      }
    } else {
      if (right_lastpresstime + FIRST_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadRightPress = true;
          right_hasRegisteredPress = true;
          right_lastpresstime = millis();
          drawAgain = true;
        }
      }
    }
  } else {
    right_laststate = val;
    right_hasRegisteredPress = false;
    right_lastpresstime = millis();
  }

  //PRESS
  val = digitalRead(SELECT_PIN);
  if (val == select_laststate) { // The button has been consistent
    if (select_hasRegisteredPress) {
      if (select_lastpresstime + REP_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadSelectPress = true;
          select_lastpresstime = millis();
          drawAgain = true;
        }
      }
    } else {
      if (select_lastpresstime + FIRST_PRESS_DEFAULT < millis()) {
        if (val == 0) {
          //We have pressed the button
          unreadSelectPress = true;
          select_hasRegisteredPress = true;
          select_lastpresstime = millis();
          drawAgain = true;
        }
      }
    }
  } else {
    select_laststate = val;
    select_hasRegisteredPress = false;
    select_lastpresstime = millis();
  }
}

// Has the up key been pressed since it was last cleared?
bool wasUpPressed() {
  return unreadUpPress;
}

// Has the down key been pressed since it was last cleared?
bool wasDownPressed() {
  return unreadDownPress;
}

// Has the left key been pressed since it was last cleared?
bool wasLeftPressed() {
  return unreadLeftPress;
}

// Has the right key been pressed since it was last cleared?
bool wasRightPressed() {
  return unreadRightPress;
}

// Has the select key been pressed since it was last cleared?
bool wasSelectPressed() {
  return unreadSelectPress;
}

// Clear all presses
void clearPresses() {
  unreadUpPress = false;
  unreadDownPress = false;
  unreadLeftPress = false;
  unreadRightPress = false;
  unreadSelectPress = false;
}

