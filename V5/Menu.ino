// The menu options
char* mainMenuOptions[]={"Lead", "Lead-Free", "Test"/*, "Custom"*/};

// Make sure this matches the variable above
const int MAIN_MENU_SIZE = 3; // Should be 3 if we re-enable the custom menu.

// The current menu option
int mainMenuLocation = 0;

// The current menu scroll state
int mainMenuScroll = 0;

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
      //Lead-Free selected
      loadTestProfile();
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
    lcd.clear();
    lcd.home();
    for (int i = 0; i < 4; i++) {
      lcd.setCursor(0,i); // Move to correct line
      if (mainMenuLocation - mainMenuScroll == i) {
        lcd.print(">");
      } else {
        lcd.print(" ");
      }
      if (isValidMenuNumber(i - mainMenuScroll)) {
        lcd.print(mainMenuOptions[i - mainMenuScroll]);
      }
    }
    // Write the current temperature in the last 5 characters
    if (avTemp >= 100) {
      lcd.setCursor(13,3); //Move to last 8 characters
    } else {
      lcd.setCursor(14,3); //Move to last 7 characters
    }
    lcd.print(String(avTemp, LCD_TEMPERATURE_DECIMALS - 1));
    lcd.print(" C");
    drawAgain = false;
  }
}

