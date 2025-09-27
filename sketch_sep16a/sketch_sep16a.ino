#define ARDUINO_UNO_STANDARD_BAUD_RATE 9600
#define ARDUINO_UNO_DIGITAL_PIN_2      2
#define ARDUINO_UNO_DIGITAL_PIN_3      3
#define PPR                            600.0f
#define time_window_ms                 500

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

    // Channel A (Green)
    pinMode(ARDUINO_UNO_DIGITAL_PIN_2, INPUT_PULLUP);

    // Channel B (White)
    // pinMode(ARDUINO_UNO_DIGITAL_PIN_3, INPUT_PULLUP); // Not needed unless you're interested in direction.
                                                                  

    attachInterrupt(
      digitalPinToInterrupt(ARDUINO_UNO_DIGITAL_PIN_2),
      tick,
      RISING
    );

    last_millis = millis();
}

void loop() 
{
  unsigned long delta_time_ms = delta_time();
  if (delta_time_ms > time_window_ms)
  {
    float    delta_time_sec;
    uint16_t tick_snapshot;

    noInterrupts();
    tick_snapshot = ticks;
    ticks         = 0;
    interrupts();

    // Convert milliseconds to seconds
    delta_time_sec  = delta_time_ms / 1000.0f; 

    // Upscale to a minute (60 seconds)
    rpm = ((float)tick_snapshot * 60.0f) / (PPR * delta_time_sec); 

    Serial.print  ("Current RPM: ");
    Serial.println(rpm, 1);

    last_millis += delta_time_ms;
  }
}
