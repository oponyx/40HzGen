#include "button.hpp"

    Button::Button( uint8_t pin, uint8_t inputType = INPUT_PULLUP, bool active = false ){
        _button_T.pin = pin;
        _button_T.inputType = inputType;
        _button_T.buttonActive = active;
        _button_T.debounceTime = DEFAULT_DEBOUNCE_TIME;
        _button_T.shortPressTime = DEFAULT_SHORT_PRESS_TIME;
        _button_T.longPressTime = DEFAULT_LONG_PRESS_TIME;
        _buttonLongPressedCallback = nullptr;
        _buttonPressedCallback = nullptr;
        _buttonReleasedCallback = nullptr;
        _buttonShortPressedCallback = nullptr;
    }

    uint8_t Button::begin(){
        pinMode(_button_T.pin, _button_T.inputType);
        // ISR should be setted here..
        return 0;
    }

    void Button::handle(){
        _button_T.currentState = ( (bool)digitalRead( _button_T.pin ) == _button_T.buttonActive ) ? true : false;
        if (_button_T.currentState != _button_T.lastState ){
            _button_T.lastDebounceTime = millis();
        }

        if(( millis() - _button_T.lastDebounceTime) > _button_T.debounceTime ){
            if( _button_T.currentState != _button_T.lastState){ // state changed
                _button_T.lastState = _button_T.currentState;   // update last state
                if(_button_T.currentState){                     // if active
                    _button_T.lastPressedTime = millis();
                    _buttonPressedCallback();
                }else{ // not active             
                    _button_T.releasedTime = millis();
                    _buttonReleasedCallback();
                    if( _button_T.releasedTime > _button_T.longPressTime){
                        _buttonLongPressedCallback();
                    }else if( _button_T.releasedTime > _button_T.shortPressTime ){
                        _buttonShortPressedCallback();
                    }
                }
            } 
        }  // _button_T.currentState != _button_T.lastState
    }

    void Button::onButtonShortPressed(callback_t buttonShortPressedCallback ){
        _buttonShortPressedCallback = buttonShortPressedCallback;
    }

    void Button::onButtonLongPressed( callback_t buttonLongPressedCallback ){
        _buttonLongPressedCallback = buttonLongPressedCallback;
    }

    void Button::onButtonPressed( callback_t buttonPressedCallback ){
        _buttonPressedCallback = buttonPressedCallback;
    }

    void Button::onButtonReleased( callback_t buttonReleasedCallback ){
        _buttonReleasedCallback = buttonReleasedCallback;
    }