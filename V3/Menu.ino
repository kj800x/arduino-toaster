char* mainMenuOptions[]={"Lead", "Lead-Free", "Custom"};
const int MAIN_MENU_SIZE = 3;
int mainMenuLocation = 0;

void mainMenuInit() {
  mainState = menu;
  drawAgain = true;
}

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
      mainState = customTemplateMenu;
      CTMinit();
    }
    drawAgain = true;
  }
}

boolean isValidMenuNumber(int i) {
  return (i >= 0 && i < MAIN_MENU_SIZE);
}
 
void displayMenu() {
  if (drawAgain) {
    Serial1.write(12);
    Serial1.write(">");
    if (isValidMenuNumber(mainMenuLocation)) {
      Serial1.print(mainMenuOptions[mainMenuLocation]);
    }
    Serial1.write(13);
    Serial1.write(8);
    Serial1.write("<");
    if (isValidMenuNumber(mainMenuLocation + 1)) {
      Serial1.print(" ");
      Serial1.print(mainMenuOptions[mainMenuLocation + 1]);
    }
    Serial1.write(13);
    Serial1.write(8);
    Serial1.write(8);
    Serial1.write(8);
    Serial1.write(8);
    Serial1.write(8);
    Serial1.print(String(avTemp, LCD_TEMPERATURE_DECIMALS));
    drawAgain = false;
  }
}
