#pragma once

#include "Arduino.h"

#define HOLDING 300
#define DEBOUNCE_TIMER 10
#define MULTICLICK_TIMER 150

enum GetClickType
{
  NOCLICK,
  DEBOUNCE,
  PRESSED,
  WAIT_NEXT_CLICK,
  ONECLICK,
  DOUBLECLICK,
  HOLD,
  MULTICLICK
};


class GButton
{
public:
  unsigned long HoldTimer;

  GButton(uint8_t pin)
  {
    ButtonPin = pin;
  }


  GetClickType state = GetClickType::NOCLICK;
  GetClickType result = GetClickType::NOCLICK;

  GetClickType GetClickType()
  {
    bool pressed = digitalRead(ButtonPin);
    unsigned long now = millis();
    

    switch (state)
    {

    case GetClickType::NOCLICK:
    {
      if (pressed)
      {
        state = DEBOUNCE;
        PressTimer = now;
        DebounceTimer = now;
      }
      else
      result = NOCLICK;
      //state = NOCLICK;
      break;
    }
    case GetClickType::DEBOUNCE:
    {
      if (!pressed)
      {
        state = NOCLICK;
      }
      else if (now - DebounceTimer >= DEBOUNCE_TIMER)
      {
        ClickCount++;
        state = PRESSED;
      }
      break;
    }

    case GetClickType::PRESSED:
    {
      if (!pressed) 
      {
          state = WAIT_NEXT_CLICK;
          NextClickTimer = now;
      }
      else if (now - PressTimer >= HOLDING)
      {
        state = HOLD;
      }
      break;
    }

    case GetClickType::WAIT_NEXT_CLICK:
    {
      if (pressed) {
          state = DEBOUNCE;
          DebounceTimer = now;
          PressTimer = now;
        }
      else if (now - NextClickTimer >= MULTICLICK_TIMER)
      {
          if (ClickCount == 1) result = ONECLICK;
          else if (ClickCount == 2) result = DOUBLECLICK;
          else if (ClickCount >= 3) result = MULTICLICK;

          state = NOCLICK;
          ClickCount = 0;
      }  
      break;
    }

    case GetClickType::HOLD:
    {
      ClickCount = 0;

      if (pressed)
      {
        //Serial.println("HOLDING");
        result = HOLD;
        state = HOLD;
      }

      else
      state = NOCLICK;
      HoldTimer = now - DebounceTimer;

      break;
    }
    }
    return result; 
  }

private:
  uint8_t ClickCount = 0;
  uint8_t ButtonPin;
  unsigned long DebounceTimer;
  unsigned long PressTimer;
  unsigned long NextClickTimer;

};
