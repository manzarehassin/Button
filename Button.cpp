/*
 ===================================================================== 
                             BUTTON LIBRARY
 =====================================================================

MIT License

Copyright (c) 2024 Manzar-E-Hassin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


Published on Thursday, 5th September, 2024
Publisher: Manzar E Hassin

Library URL: https://github.com/manzarehassin/Button 

*/


#include "Button.h"

Button::Button(uint8_t Button_pin) // Constructor
{
	bs.buttonPin = Button_pin;
	bs.debounceDelay = 50;
	Button::selectMode(BTN_PULLUP, BTN_MODE_MOMENTARY);
	bs.ToggleState=0;
	bs.pulseDuration = 100;
	bs.pulse_active=0;
	bs.pulse_button_state=1;
	bs.Long_Press_Duration =2000;
}
	
void Button::selectMode(uint8_t Input_Mode, uint8_t Button_Mode)
{
  bs.mode= Button_Mode;
  switch(Input_Mode)
  {
    case BTN_PULLDOWN : pinMode(bs.buttonPin, INPUT); bs.action_state = 1;break;
    case BTN_PULLUP   : pinMode(bs.buttonPin, INPUT_PULLUP); bs.action_state = 0;break;
  }
  bs.lastButtonState=!bs.action_state;
  bs.buttonState=!bs.action_state;
}

void Button::timingSetup(uint8_t Debounce_Time, uint16_t Monostable_Pulse_Duration, uint16_t Long_Press_Duration)
{
  bs.pulseDuration = Monostable_Pulse_Duration;
  bs.Long_Press_Duration =Long_Press_Duration;
  bs.debounceDelay = Debounce_Time;
}

inline void Button::debounce() 
{
  union variables
  {
    bool reading :1;
  }v;

  static unsigned long lastDebounceTime = 0;
  v.reading = digitalRead(bs.buttonPin);
  bs.current_millis=millis();
  if (v.reading != bs.lastButtonState) 
  {
    lastDebounceTime = bs.current_millis;
  }
  if (bs.current_millis > lastDebounceTime + bs.debounceDelay) 
  {
    if (v.reading != bs.buttonState) 
    {
      bs.buttonState = v.reading;
      if (bs.buttonState == bs.action_state) 
      {
        bs.ToggleState = !bs.ToggleState; //EVENT TIME
        bs.buttonLongPressMillis = bs.current_millis; // For long press  time init
      }
    }
  }
  bs.lastButtonState = v.reading;
}

bool Button::read()
{
  union variable
  {
    bool result :1;
  }v;
  
  Button::debounce();
  switch(bs.mode)
  {
    case BTN_MODE_MOMENTARY : v.result = Button::pullup_check(); break;
    case BTN_MODE_TOGGLE: v.result= bs.ToggleState; break;
    case BTN_MODE_PULSE : Button::monostable(); v.result=bs.pulse_active;  break;
  }
  return v.result;
}

inline bool Button::pullup_check()
{
  if(bs.action_state)
  {
    return bs.buttonState;
  }
  else
  {
    return !(bs.buttonState);
  }
}

inline void Button::monostable() 
{
  union variable
  {
    bool state :1;
  }v;

  static unsigned long pulse_startTime = 0;
  v.state = Button::pullup_check();
  if (v.state != bs.pulse_button_state)
  {
    bs.pulse_button_state = v.state;
    pulse_startTime = bs.current_millis;
  }
  if (bs.pulse_active && bs.current_millis - pulse_startTime >= bs.pulseDuration) // LED is on and 1 second has elapsed
  { 
    bs.pulse_active = false; pulse_startTime = 0;
  }
  if (v.state == 1 && bs.current_millis - pulse_startTime > bs.pulseDuration)  // Button is still pressed after 1 second
  {
    bs.pulse_active = false;
  }
  else if (v.state == 1 && !bs.pulse_active) // Button pressed and LED off
  { 
    bs.pulse_active = true;
  }
}

void Button::Button_Long_Press()
{
  union variable
  {
    bool x :1;
  }v;

  Button::debounce();
  v.x= Button::pullup_check();
  if(v.x)
  {
    unsigned long buttonPressDuration = bs.current_millis - bs.buttonLongPressMillis;
    if (v.x && !bs.buttonStateLongPress && buttonPressDuration >= bs.Long_Press_Duration)
    {
      bs.buttonStateLongPress = 1;  // LONG PRESS EVENT
    }
  }
  else
  {
    bs.buttonStateLongPress = 0;
  }
}

bool Button::pressed()
{
  Button::debounce() ;
  return Button::pullup_check();
}

bool Button::released()
{
  Button::debounce() ;
  return !(Button::pullup_check());
}

bool Button::longPress()
{
  Button::Button_Long_Press();
  return bs.buttonStateLongPress;
}
