// The custom template menu is the menu that first pops up when the user
// selects custom. It determines which existing curve the user is going to modify
// in the custom menu. It selects the "template" that the custom curve is based on.

// CTM = Custom Template Menu

// The menu options for the template menu
char* CTMMenuOptions[]={"[ BACK ]", "Lead", "Lead-Free"};

// Make this match the array above
const int CTM_MENU_SIZE = 3;

// The scroll amount in the menu
int CTMMenuLocation = 0;

// Call this function to set the defaults back up again for the CTM.
void customTemplateMenuInit() {
  mainState = customTemplateMenu;
  CTMMenuLocation = 0;
  drawAgain = true;
}

// Call this function to allow the CTM to react to buttons.
void CTMreactToButtons() {
  if (wasDownPressed()) {
    if (isValidCTMMenuNumber(CTMMenuLocation + 1)) {
      CTMMenuLocation++;
    }
    drawAgain = true;
  }
  if (wasUpPressed()) {
    if (isValidCTMMenuNumber(CTMMenuLocation - 1)) {
      CTMMenuLocation--;
    }
    drawAgain = true;
  }
  if (wasSelectPressed()) {
    if (CTMMenuLocation == 0) {
      // Go back to main menu
      mainMenuInit();
    } else if (CTMMenuLocation == 1) {
      // Load "Lead" and go to customize mode
      loadLeadProfile();
      customMenuInit();
    } else if (CTMMenuLocation == 2) {
      // Load "Lead Free" and go to customize mode
      loadLeadFreeProfile();
      customMenuInit();
    }
    drawAgain = true;
  }
}

// Checks to see if the given number is a valid menu location
boolean isValidCTMMenuNumber(int i) {
  return (i >= 0 && i < CTM_MENU_SIZE);
}

// Draw the menu to the LCD
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
