#include <stdio.h>
#include <ncurses.h>

#define MAX_POINTS 10
#define PROP_HEAT_SELECTED 0
#define PROP_TIME_SELECTED 1
#define TERMINAL_WIDTH 16
#define TERMINAL_WIDTH_PLUS_ONE 17
#define MAX_TEMP 200
#define MAX_TIME 100

struct Point {
  int time;
  int temp;
} data[MAX_POINTS];
char timeString[TERMINAL_WIDTH_PLUS_ONE];
int currentPoint = 0;
int totalPoints = 0;
int selected_prop = 0;
int i;

void drawstate() {
  attron(COLOR_PAIR(1));
  mvprintw(0,0,"                \n                ");
  mvprintw(0,6,"%d/%d", currentPoint+1, totalPoints+1);
  
  if (selected_prop == PROP_HEAT_SELECTED){
    attron(COLOR_PAIR(2));
  } else {
    attron(COLOR_PAIR(1));
  }
  mvprintw(0,0,"Heat");
  mvprintw(1,0,"%d", data[currentPoint].temp);

  if (selected_prop == PROP_TIME_SELECTED){
    attron(COLOR_PAIR(2));
  } else {
    attron(COLOR_PAIR(1));
  }
  i = snprintf(timeString, TERMINAL_WIDTH + 1, "%d", data[currentPoint].time);
  mvprintw(0,12, "Time");
  mvprintw(1,16 - i, "%s", timeString);
}

void setup(){
  int i;
  for (i = 0; i < MAX_POINTS; i++){
    data[i].time = 0;
    data[i].temp = 0;
  }
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
  curs_set(0);
}

bool is_point_empty (int i){
  return ((data[i].temp == 0) && (data[i].time == 0));
}

int main (int argc, char **argv){
  initscr();
  setup();
  noecho();
  drawstate();
  char ch = 0;
  timeout(1000);
  mvprintw(0,0,"Lets Get Toasty!\n                ");
  while ((ch = getch()) != 'x') {
    if(ch == '6') { // Right key was pressed
      if (selected_prop == PROP_TIME_SELECTED){
        if (currentPoint != (MAX_POINTS - 1)){
          currentPoint++;
          selected_prop = PROP_HEAT_SELECTED;
          if (is_point_empty(currentPoint)){
            totalPoints++;
          }
        }
      } else {
        selected_prop = PROP_TIME_SELECTED;
      }
    }
    if(ch == '4') { // Left key was pressed
      if (selected_prop == PROP_HEAT_SELECTED){
        if (currentPoint != 0){
          if (is_point_empty(currentPoint)){
            totalPoints--;
          }
          currentPoint--;
          selected_prop = PROP_TIME_SELECTED;
        }
      } else {
        selected_prop = PROP_HEAT_SELECTED;
      }
    }
    if(ch == '8') { // Up key was pressed
      if (selected_prop == PROP_HEAT_SELECTED){
        if (data[currentPoint].temp + 1 <= MAX_TEMP){
          data[currentPoint].temp++;
        }
      } else {
        if (data[currentPoint].time + 1 <= MAX_TIME){
          data[currentPoint].time++;
        }
      }
    }
    if(ch == '2') { // Down key was pressed
      if (selected_prop == PROP_HEAT_SELECTED){
        if (data[currentPoint].temp - 1 >= 0){
          data[currentPoint].temp--;
        }
      } else {
        if (data[currentPoint].time - 1 >= 0){
          data[currentPoint].time--;
        }
      }
    }
    drawstate();
  }
  endwin();
  return 0;
}
