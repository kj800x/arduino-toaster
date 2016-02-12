// sets the flux capacitor (heating element) at a value for PWM
#define SSR_PIN xx
void setFluxCapacitor (int pwm_heat_value)
{
  analogWrite(SSR_PIN, pwm_heat_value);
}
