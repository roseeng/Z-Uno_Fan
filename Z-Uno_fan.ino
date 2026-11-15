/*
 * Z-Uno Fan
 * Register a dimmer on the z-wave net, and control an ordinary PC fan
 * using PWM.
 * 
 */
 
// Variables that hold the data we send of z-wave
byte dimmerValue = 0; // Set speed of fan
byte rotationValue = 0; // Measured speed of fan
int temperatureValue = 0;

// PWM is available on pin 13-16
#define PWM_PIN 14
// Pin 13 - user LED on the Z-Uno board
#define LED_PIN 13
// Not yet implemented
#define TACHO_PIN 666 // Figure out a good pin to measure the tacho signal on
 
ZUNO_SETUP_CHANNELS(ZUNO_SWITCH_MULTILEVEL(getSwitchMultilevelValue,setSwitchMultilevelValue),
                    ZUNO_SWITCH_MULTILEVEL(getSwitchMultilevelValue,setSwitchMultilevelValue),
                    ZUNO_SWITCH_MULTILEVEL(getSwitchMultilevelValue,setSwitchMultilevelValue),
                    ZUNO_SWITCH_MULTILEVEL(getSwitchMultilevelValue,setSwitchMultilevelValue),                   
                    ZUNO_SENSOR_MULTILEVEL_ROTATION(rotationValue),
                    ZUNO_SENSOR_MULTILEVEL_TEMPERATURE(&temperatureValue));

void setup() {
  dimmerValue = 0;
  rotationValue = 0;
  temperatureValue = 0;

  Serial.begin(115200); 

  pinMode(PWM_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT); // Also use LED to get something you can see
  
  analogWriteFrequency(25000); // PWM frequency 25 kHz (standard for PC fans)
  //analogWriteResolution(8); // We use the standard 8 bits, uncomment for a different number
  Serial.println("Z-Uno Fan Control set up.");
  delay(10);
  Serial.println("Z-Uno Fan Control set up. Version 1.3");
}

void loop() {
  // TODO: Measure rotation using interrupts on pin 4 on the fan connector
  // TODO: Measure temperature using a DS18B20 for example
}

// Set a new value for the fan speed
void setSwitchMultilevelValue(byte channel, byte newValue) 
{
  Serial.print("Set value, channel: ");
  Serial.println(channel);

  if (newValue > 99) {
    // by Z-Wave specification, this value can't be more than 99
    newValue = 99;
  }
  
  dimmerValue = newValue;
  long pwmValue = ((long)dimmerValue) * 255 / 99;
  analogWrite(PWM_PIN, pwmValue);
  analogWrite(LED_PIN, pwmValue);

  Serial.print("Speed: ");
  Serial.print(dimmerValue);
  Serial.print("   PWM: ");
  Serial.println(pwmValue);
}

// Return the fan speed (runs only when the controller asks for the current value by Z-Wave)
byte getSwitchMultilevelValue(byte channel) 
{
  Serial.print("Fetch pwm value, channel: ");
  Serial.println(channel);
  
  return dimmerValue;
}
