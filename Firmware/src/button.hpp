/*
button.hpp - Button class based on Limor Fried's version of debounce


copyright Copyright (c) 2022 Onofrio Pagliarulo (oponyx@hotmail.it)

MIT License

Copyright (c) 2022 Pagliarulo Onofrio

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
*/

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>

#ifndef LONG_PRESS_TIME
#define LONG_PRESS_TIME     2000  // 2 secoonds
#endif
#ifndef SHORT_PRESS_TIME
#define SHORT_PRESS_TIME    100   // 100 mS
#endif
#ifndef DEBOUNCE_TIME
# define DEBOUNCE_TIME      10    // 10 ms
#endif

/**
 * @brief struct containing button datas
 * 
 */
typedef struct {
  uint8_t       pin;                // Button pin number
  uint8_t       inputType;          // Button pin pullup config
  bool          buttonActive;       // Button pin active LOW/HIGH
  bool          lastState;                       
  bool          lastRead;  
  bool          state;  
  uint8_t       debounceTime;                     
  ulong         lastPressedTime;
  ulong         releasedTime;
  ulong         lastDebounceTime;
//  ulong         lastReadTime;
  uint16_t      shortPressTime;
  uint16_t      longPressTime;
} TButton;

  // callback type
//typedef void (*callback_t)();
using callback_t = void (*)(uint8_t btnPin);

class Button {

  private:
    TButton _button_T;
    callback_t _buttonShortPressedCallback;
    callback_t _buttonLongPressedCallback;
    callback_t _buttonPressedCallback;
    callback_t _buttonReleasedCallback;
 
    void setupButtonPin();


  public:
    /**
     * @brief Construct a new Button object
     * 
     * @param pin 
     * @param inputType 
     * @param active 
     */
    Button( uint8_t pin, uint8_t inputType, bool active );
    
    void onButtonShortPressed( callback_t buttonShortPressedCallback );
    void onButtonLongPressed( callback_t buttonLongPressedCallback );
    void onButtonPressed( callback_t buttonPressedCallback );
    void onButtonReleased( callback_t buttonReleasedCallback );
    void handle();
    uint8_t begin();
    bool status();
};

#endif