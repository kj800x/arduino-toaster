ALL NAMES ARE DIFFERENT EVERYWHERE

//Top level functions
[ ] - loop() -- code to do the main loop.

//First level looping functions
[ ] - runOven() -- code to run the oven using the shared data stored during the programOven() phase
[ ] - programOven() -- collect data for the runOven() phase

//Helpful functions for runOven
//Data-in functions
[X] - timeSinceRunStart() -- get the time since the runOven() phase started
[ ] - getCurrentExpectedTemperature() -- get the current expected temperature
[X] - readTemperatureFromProbes() -- get the current actual temperature
//Program-out functions
[X] - setFanSpeed() -- update the speed of the fans
[X] - setHeaterOutput() -- update the heater output level
