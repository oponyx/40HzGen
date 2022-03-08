/**
 * @file button.hpp
 * @author Onofrio Pagliarulo (oponyx@hotmail.it)
 * @brief 
 * @version 0.1
 * @date 2022-03-06
 * 
 * @copyright Copyright (c) 2022
 * 
 * 
 * Button class based on Limor Fried's version of debounce
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