#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
	#error This code is designed to run on nRF52-based Nano-33-BLE boards using mbed-RTOS platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
// For Nano33-BLE, don't use Serial.print() in ISR as system will definitely hang.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "TimerInterrupt_Generic.h"

#include "ISR_Timer_Generic.h"

#include <Arduino.h>
#include "mbed.h"

// Init NRF52 timer NRF_TIMER3
NRF52_MBED_Timer ITimer(NRF_TIMER_3);

// Init ISR_Timer
// Each ISR_Timer can service 16 different ISR-based timers
ISR_Timer NRF52_ISR_Timer;

#define TIMER_INTERVAL_1S             1000L
#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L

void TimerHandler()
{
	NRF52_ISR_Timer.run();
}

// In NRF52, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething1()
{
	switchLED();
}

int global = 0;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void switchLED()
{
  if(global == 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    global++;
  }
  else 
  {
    digitalWrite(LED_BUILTIN, LOW);
    global--;  
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //sleep();
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  switchLED();
  delay(1000);
}
