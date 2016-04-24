void CM_init() {
  drawAgain = true;
  customMenuState = ePRHT_H_DUTY_ON;
}

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


// ---------------------CUSTOM TEMPLATE MENU-------------------

char* CTMMenuOptions[]={"[ BACK ]", "Lead", "Lead-Free"};
const int CTM_MENU_SIZE = 3;
int CTMMenuLocation = 0;

void CTMinit() {
  CTMMenuLocation = 0;
  drawAgain = true;
}

void CTMreactToButtons() {
  if (wasDownPressed()) {
    if (CTMMenuLocation + 1 < CTM_MENU_SIZE) {
      CTMMenuLocation++;
    }
    drawAgain = true;
  }
  if (wasUpPressed()) {
    if (CTMMenuLocation > 0) {
      CTMMenuLocation--;
    }
    drawAgain = true;
  }
  if (wasSelectPressed()) {
    if (CTMMenuLocation == 0) {
      // Go back to main menu
      mainMenuInit();
    } else if (CTMMenuLocation == 1) {
      // Load Lead and go to customize mode
      loadLeadProfile();
      loadCustomProfile();
    } else if (CTMMenuLocation == 2) {
      // Load Lead Free and go to customize mode
      loadLeadFreeProfile();
      loadCustomProfile();
    }
    drawAgain = true;
  }
}

boolean isValidCTMMenuNumber(int i) {
  return (i >= 0 && i < CTM_MENU_SIZE);
}
 
void CTMdisplayMenu() {
  if (drawAgain) {
    Serial1.write(12);
    Serial1.write(">");
    if (isValidCTMMenuNumber(CTMMenuLocation)) {
      Serial1.print(CTMMenuOptions[CTMMenuLocation]);
    }
    Serial1.write(13);
    Serial1.write(8);
    Serial1.write("<");
    if (isValidCTMMenuNumber(CTMMenuLocation + 1)) {
      Serial1.print(" ");
      Serial1.print(CTMMenuOptions[CTMMenuLocation + 1]);
    }
    drawAgain = false;
  }
}
