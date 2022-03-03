#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>

#define DEFAULT_LONG_PRESS_TIME     2000  // 2 secoonds
#define DEFAULT_SHORT_PRESS_TIME    100   // 100 mS
# define DEFAULT_DEBOUNCE_TIME      10    // 10 ms
typedef struct {
  uint8_t       pin;                // Button pin number
  uint8_t       inputType;          // Button pin pullup config
  bool          buttonActive;       // Button pin active LOW/HIGH
  bool          lastState;                       
  bool          currentState;  
  uint8_t       debounceTime;                     
  ulong         lastPressedTime;
  ulong         releasedTime;
  ulong         lastDebounceTime;
  ulong         lastReadTime;
  uint16_t      shortPressTime;
  uint16_t      longPressTime;
} TButton;

  // callback type
//typedef void (*callback_t)();
using callback_t = void (*)();

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
};

#endif