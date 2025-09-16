#define ARDUINO_UNO_STANDARD_BAUD_RATE 9600
#define ARDUINO_UNO_DIGITAL_INPUT      12
#define ARDUINO_UNO_DIGITAL_OUTPUT     13


#include <stdint.h>

volatile static uint16_t      ticks       = 0;
volatile static float         rpm         = 0.0f;
volatile static float         delta       = 0.0f;
volatile static unsigned long last_millis = 0;


void tick()
{
  ticks++;
}


unsigned long delta_time()
{
  unsigned long now_time = millis();
  unsigned long delta    = now_time - last_millis;

  return delta;
}


void setup() 
{
    Serial.begin(ARDUINO_UNO_STANDARD_BAUD_RATE);

    pinMode(ARDUINO_UNO_DIGITAL_INPUT, INPUT);
    pinMode(ARDUINO_UNO_DIGITAL_OUTPUT, OUTPUT);
                                                                  

    attackInterrupt(
      digitalPinToInterrupt(ARDUINO_UNO_DIGITAL_INPUT),
      tick,
      RISING
    );

}

void loop() 
{
  if (delta_time() > 50)
  {
    last_millis = 50;

    rpm = (float)ticks * 1200.0f; // 60s/50ms aka 60s/0.05s
    Serial.printf("Current RPM: %.1f%%\n", rpm);

    ticks = 0;
  }
}
