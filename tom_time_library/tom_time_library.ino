// millis() returns the number of milliseoncds since the arduino began running the current program. returns an unsigned long.

int time_zero;
int& delta_time

// stores the time 
int setTimeZero (int& time_zero)
{
  time_zero = millis();
}

// find the elapsed time, returns an unsigned long 
int elapsedTime (int& delta_time)
{
  delta_time = millis() - time_zero;
}

