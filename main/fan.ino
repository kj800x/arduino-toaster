// sets the fan SSR at a value for PWM
#define FAN_PIN xx
void setFluxCapacitor (int pwm_fan_value)
{
  analogWrite(FAN_PIN, pwm_fan_value);
}
