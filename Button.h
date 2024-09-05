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

#ifndef button_h
#define button_h

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class Button
{
	public:
		// constructor
		Button(uint8_t pin);
		
		// public methods
		void selectMode(uint8_t Input_Mode, uint8_t Button_Mode);
		void timingSetup(uint8_t Debounce_Time, uint16_t Monostable_Pulse_Duration, uint16_t Long_Press_Duration);
		bool read();
		bool pressed();
		bool released();
		bool longPress();
		
	private:
		// Private methods
		inline void debounce();
		inline bool pullup_check();
		inline void monostable();
		void Button_Long_Press();
		
		// global variables
		
		struct Button_Storage
		{
			bool ToggleState :1;
			bool buttonState :1;
			bool lastButtonState :1;
			bool action_state :1;
			bool buttonStateLongPress :1;
			bool pulse_active :1;
			bool pulse_button_state :1;
			uint8_t debounceDelay =50;
			uint8_t mode: 3;
			uint8_t buttonPin;
			uint16_t pulseDuration :14;
			uint16_t Long_Press_Duration :14;
			unsigned long buttonLongPressMillis=0;
			unsigned long current_millis=0;
		} bs;
		
		// defines
		#define BTN_PULLUP          INPUT_PULLUP
		#define BTN_PULLDOWN        INPUT
		#define BTN_MODE_MOMENTARY  0
		#define BTN_MODE_TOGGLE     1
		#define BTN_MODE_PULSE      2
};
#endif