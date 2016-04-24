// The menu options
char* mainMenuOptions[]={"Lead", "Lead-Free", "Custom"};

// Make sure this matches the variable above
const int MAIN_MENU_SIZE = 3;

// The current menu option
int mainMenuLocation = 0;

// Call this when you want to bring the main menu up
void mainMenuInit() {
  mainState = menu;
  drawAgain = true;
}

// React to button presses during the main menu
void reactToButtons() {
  if (wasDownPressed()) {
    if (mainMenuLocation + 1 < MAIN_MENU_SIZE) {
      mainMenuLocation++;
    }
    drawAgain = true;
  }
  if (wasUpPressed()) {
    if (mainMenuLocation > 0) {
      mainMenuLocation--;
    }
    drawAgain = true;
  }
  if (wasSelectPressed()) {
    if (mainMenuLocation == 0) {
      //Lead selected
      loadLeadProfile();
    } else if (mainMenuLocation == 1) {
      //Lead-Free selected
      loadLeadFreeProfile();
    } else if (mainMenuLocation == 2) {
      // Go to the custom template menu
      customTemplateMenuInit();
    }
    drawAgain = true;
  }
}

// Check to see if the given index is a valid index for the main menu
boolean isValidMenuNumber(int i) {
  return (i >= 0 && i < MAIN_MENU_SIZE);
}

// Draw the main menu
void displayMenu() {
  if (drawAgain) {
    // Write the first row
    Serial1.write(12);
    Serial1.write(">");
    if (isValidMenuNumber(mainMenuLocation)) {
      Serial1.print(mainMenuOptions[mainMenuLocation]);
    }
    Serial1.write(13);
    Serial1.write(8);
    Serial1.write("<");
    // Write the second row
    if (isValidMenuNumber(mainMenuLocation + 1)) {
      Serial1.print(" ");
      Serial1.print(mainMenuOptions[mainMenuLocation + 1]);
    }

    // Write the current temperature in the last 5 characters
    Serial1.write(13);
    Serial1.write(8);
    Serial1.write(8);
    Serial1.write(8);
    Serial1.write(8);
    if (avTemp >= 100) {
      Serial1.write(8);
    }
    Serial1.print(String(avTemp, LCD_TEMPERATURE_DECIMALS - 1));
    drawAgain = false;
  }
}
