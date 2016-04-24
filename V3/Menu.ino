char* mainMenuOptions[]={"Lead", "Lead-Free", "Custom"};
const int MAIN_MENU_SIZE = 3;
int mainMenuLocation = 0;

void reactToButtons() {
  if (wasDownPressed()) {
    mainMenuLocation--;
  }
  if (wasUpPressed()) {
    mainMenuLocation++;
  }
  if (wasSelectPressed()) {
    if (mainMenuLocation == 0) {
      //Lead selected
      loadLeadProfile();
      mainState = run;
    } else if (mainMenuLocation == 1) {
      //Lead-Free selected
      loadLeadFreeProfile();
      mainState = run;
    } else if (mainMenuLocation == 2) {
      //Custom selected
      //DO OTHER STUFF
    }
  }
}

boolean isValidMenuNumber(int i) {
  return (i >= 0 && i < MAIN_MENU_SIZE);
}
 
void displayMenu() {
  int i;
  Serial1.write(12);
  for (i = 0; i < 2; i++) {
    if (isValidMenuNumber(i + mainMenuLocation)) {
      Serial1.println(mainMenuOptions[i + mainMenuLocation]);
    } else {
      Serial1.println("");  
    }
  }
}
