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

// A variable to store the current selected index of the custom
int customMenuState = ePRHT_H_DUTY_ON;

// Call this before the custom menu is brought up
void customMenuInit() {
  Serial.println("Loading Custom Profile");
  mainState = customMenu;
  drawAgain = true;
  customMenuState = ePRHT_H_DUTY_ON;
  runType = "Custom";
  profileStage = PRHT;
}

// React to buttons for the Custom Menu
void CM_reactToButtons() {
  if (wasDownPressed()) {
    if (customMenuState + 1 < CUSTOM_MENU_SIZE) {
      customMenuState++;
    }
    drawAgain = true;
  }
  if (wasUpPressed()) {
    if (customMenuState > 0) {
      customMenuState--;
    }
    drawAgain = true;
  }
  if (wasRightPressed()) {
    if (customMenuState == ePRHT_H_DUTY_ON) {
      PRHT_H_DUTY_ON += 5;
    } else if (customMenuState == ePRHT_H_DUTY_OFF) {
      PRHT_H_DUTY_OFF += 5;
    } else if (customMenuState == eSOAK_H_DUTY_ON) {
      SOAK_H_DUTY_ON += 5;
    } else if (customMenuState == eSOAK_H_DUTY_OFF) {
      SOAK_H_DUTY_OFF += 5;
    } else if (customMenuState == eRAMP_H_DUTY_ON) {
      RAMP_H_DUTY_ON += 5;
    } else if (customMenuState == eRAMP_H_DUTY_OFF) {
      RAMP_H_DUTY_OFF += 5;
    } else if (customMenuState == eCOOL_F_DUTY_ON) {
      COOL_F_DUTY_ON += 5;
    } else if (customMenuState == eCOOL_F_DUTY_OFF) {
      COOL_F_DUTY_OFF += 5;
    } else if (customMenuState == eSOAK_START_TEMP) {
      SOAK_START_TEMP += 5;
    } else if (customMenuState == eRAMP_START_TEMP) {
      RAMP_START_TEMP += 5;
    } else if (customMenuState == eCOOL_START_TEMP) {
      COOL_START_TEMP += 5;
    }
    drawAgain = true;
  }
  if (wasLeftPressed()) {
    if (customMenuState == ePRHT_H_DUTY_ON) {
      PRHT_H_DUTY_ON -= 5;
    } else if (customMenuState == ePRHT_H_DUTY_OFF) {
      PRHT_H_DUTY_OFF -= 5;
    } else if (customMenuState == eSOAK_H_DUTY_ON) {
      SOAK_H_DUTY_ON -= 5;
    } else if (customMenuState == eSOAK_H_DUTY_OFF) {
      SOAK_H_DUTY_OFF -= 5;
    } else if (customMenuState == eRAMP_H_DUTY_ON) {
      RAMP_H_DUTY_ON -= 5;
    } else if (customMenuState == eRAMP_H_DUTY_OFF) {
      RAMP_H_DUTY_OFF -= 5;
    } else if (customMenuState == eCOOL_F_DUTY_ON) {
      COOL_F_DUTY_ON -= 5;
    } else if (customMenuState == eCOOL_F_DUTY_OFF) {
      COOL_F_DUTY_OFF -= 5;
    } else if (customMenuState == eSOAK_START_TEMP) {
      SOAK_START_TEMP -= 5;
    } else if (customMenuState == eRAMP_START_TEMP) {
      RAMP_START_TEMP -= 5;
    } else if (customMenuState == eCOOL_START_TEMP) {
      COOL_START_TEMP -= 5;
    }
    drawAgain = true;
  }
  if (wasSelectPressed()) {
    if (customMenuState == eCANCEL_A || customMenuState == eCANCEL_B) {
      mainMenuInit();
    }
    if (customMenuState == eRUN) {
      mainState = run;
    }
    drawAgain = true;
  }
}

// Draw the menu
void CM_displayMenu() {
  if (drawAgain) {
    drawAgain = false;
    Serial1.write(12);
    if (customMenuState == eCANCEL_A || customMenuState == eCANCEL_B) {
      Serial1.print("[ CANCEL ]");
      return;
    } else if (customMenuState == ePRHT_H_DUTY_ON) {
      Serial1.print("PRHT HEAT ON");
    } else if (customMenuState == ePRHT_H_DUTY_OFF) {
      Serial1.print("PRHT HEAT OFF");
    } else if (customMenuState == eSOAK_H_DUTY_ON) {
      Serial1.print("SOAK HEAT ON");
    } else if (customMenuState == eSOAK_H_DUTY_OFF) {
      Serial1.print("SOAK HEAT OFF");
    } else if (customMenuState == eRAMP_H_DUTY_ON) {
      Serial1.print("RAMP HEAT ON");
    } else if (customMenuState == eRAMP_H_DUTY_OFF) {
      Serial1.print("RAMP HEAT OFF");
    } else if (customMenuState == eCOOL_F_DUTY_ON) {
      Serial1.print("COOL FANS ON");
    } else if (customMenuState == eCOOL_F_DUTY_OFF) {
      Serial1.print("COOL FANS OFF");
    } else if (customMenuState == eSOAK_START_TEMP) {
      Serial1.print("SOAK START TEMP");
    } else if (customMenuState == eRAMP_START_TEMP) {
      Serial1.print("RAMP START TEMP");
    } else if (customMenuState == eCOOL_START_TEMP) {
      Serial1.print("COOL START TEMP");
    } else {
      Serial1.print("[ RUN ]");
      return;
    }

    Serial1.write(13);

    if (customMenuState == ePRHT_H_DUTY_ON) {
      Serial1.print(String(PRHT_H_DUTY_ON));
    } else if (customMenuState == ePRHT_H_DUTY_OFF) {
      Serial1.print(String(PRHT_H_DUTY_OFF));
    } else if (customMenuState == eSOAK_H_DUTY_ON) {
      Serial1.print(String(SOAK_H_DUTY_ON));
    } else if (customMenuState == eSOAK_H_DUTY_OFF) {
      Serial1.print(String(SOAK_H_DUTY_OFF));
    } else if (customMenuState == eRAMP_H_DUTY_ON) {
      Serial1.print(String(RAMP_H_DUTY_ON));
    } else if (customMenuState == eRAMP_H_DUTY_OFF) {
      Serial1.print(String(RAMP_H_DUTY_OFF));
    } else if (customMenuState == eCOOL_F_DUTY_ON) {
      Serial1.print(String(COOL_F_DUTY_ON));
    } else if (customMenuState == eCOOL_F_DUTY_OFF) {
      Serial1.print(String(COOL_F_DUTY_OFF));
    } else if (customMenuState == eSOAK_START_TEMP) {
      Serial1.print(String(SOAK_START_TEMP));
    } else if (customMenuState == eRAMP_START_TEMP) {
      Serial1.print(String(RAMP_START_TEMP));
    } else if (customMenuState == eCOOL_START_TEMP) {
      Serial1.print(String(COOL_START_TEMP));
    }
  }
}
