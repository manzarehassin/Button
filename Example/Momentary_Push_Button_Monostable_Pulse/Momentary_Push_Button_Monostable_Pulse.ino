/*
 ===================================================================== 
                      BUTTON LIBRARY Example file
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


Here we would like to attach a momentary push button as pullup mode in Digital pin 2,
Configure it as a monostable pulse generator. Also attach a LED on Digital pin 13.
When the button is pushed, then a the output will be High for 100ms. after that the output will be Low.
it will not turn on again until button pressed again. 
In this example we are using the internal resistor for pullup, so no extra registor is not necessary in our circuit.

Debounce filter is activated by default. Configuration in this example is:
Input type    : PullUp (internal resistor)
Debounce delay: 50ms
Pulse duration: 100ms
Button mode   : Monostable pulse generator

Circuit: Connect one end of the push button to Digital pin 2 and other end to ground (GND).
         Connect a LED across digital pin 13 and ground (GND).
         
*/

#include <Button.h>           // Button header file

const byte button_pin = 2;    // Button in pin 2 
const byte led_pin    = 13;   // Led in pin 13

Button btn(button_pin);  // Declaring digital pin 2 as a button. default type is INPUT_PULLUP. Object name: btn

void setup() 
{
  pinMode(led_pin,OUTPUT);                     // Declaring pin 13 as output.
  btn.selectMode(BTN_PULLUP, BTN_MODE_PULSE);  // Selecting mode as Pulse (pullup)
  
  /*
  Optional feature---------------------------------

  Input type: Pull Down
  Mode      : Monostable pulse
  btn.selectMode(BTN_PULLDOWN, BTN_MODE_PULSE); 

  Default pulse duration is 100ms. To change its duration, add this line in setup():
  
  btn.timingSetup(50, 200, 2000);

  50  : debounce delay as 50ms
  200 : Monostable pulse duration as 200ms
  2000: Button long press duration as 2s (not used in this example)

*/

}

void loop() 
{
  // Here we would like to turn on/off led for a short period of time on button press.

  bool buttonStatus = btn.read();         // Reading button monostable state.
  digitalWrite(led_pin, buttonStatus);    // Writing status to pin 13.
}