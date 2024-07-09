#include <Arduino.h>
#include "mbed.h"

#include "nrf.h"
#include "nrf_timer.h"
//#include "nrf_drv_common.h"


int global = 0;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  initializeTimer0();
}

void initializeTimer0()
{
  // Disable the timer before configuration
  NRF_TIMER0->TASKS_STOP = 1;
  NRF_TIMER0->TASKS_CLEAR = 1;

  // Set the timer mode to Timer
  NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;

  // Set the prescaler to get a 1 MHz timer frequency
  NRF_TIMER0->PRESCALER = 4;

  // Set the timer to compare event at 1000000 ticks (1 second)
  NRF_TIMER0->CC[0] = 1000000;

  // Enable interrupt for Compare0
  NRF_TIMER0->INTENSET = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;

  // Clear the event
  NRF_TIMER0->EVENTS_COMPARE[0] = 0;

  // Enable the timer interrupt in the NVIC
  NVIC_EnableIRQ(TIMER0_IRQn);

  // Start the timer
  NRF_TIMER0->TASKS_START = 1;
}

// Interrupt Service Routine for TIMER0
void TIMER0_IRQHandler(void) 
{
  if (NRF_TIMER0->EVENTS_COMPARE[0]) 
  {
    // Clear the event
    NRF_TIMER0->EVENTS_COMPARE[0] = 0;

    // Your code to handle the timer interrupt
    // For example, toggle an LED
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    // Reset the timer
    NRF_TIMER0->TASKS_CLEAR = 1;
  }
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
  //delay(1000);
}
