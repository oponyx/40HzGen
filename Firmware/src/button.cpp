/*
button.cpp - Button class based on Limor Fried's version of debounce


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
#include "button.hpp"


    Button::Button( uint8_t pin, uint8_t inputType = INPUT_PULLUP, bool active = false ){
        _button_T.pin = pin;
        _button_T.inputType = inputType;
        _button_T.buttonActive = active;
        _button_T.debounceTime = DEBOUNCE_TIME;
        _button_T.shortPressTime = SHORT_PRESS_TIME;
        _button_T.longPressTime = LONG_PRESS_TIME;
        _buttonLongPressedCallback = nullptr;
        _buttonPressedCallback = nullptr;
        _buttonReleasedCallback = nullptr;
        _buttonShortPressedCallback = nullptr;
    }

    uint8_t Button::begin(){
        pinMode(_button_T.pin, _button_T.inputType);
        return 0;
    }

    void Button::handle(){ //26uS
        _button_T.lastRead = ( (bool)digitalRead( _button_T.pin ) == _button_T.buttonActive ) ? true : false;
        if ( _button_T.lastRead != _button_T.lastState ){
            _button_T.lastDebounceTime = millis();
        }
        if( (millis() - _button_T.lastDebounceTime) > _button_T.debounceTime ){
            if( _button_T.lastRead != _button_T.state ){ // state changed
                _button_T.state = _button_T.lastRead;  

                if( _button_T.state ){                   // if active
                    _button_T.lastPressedTime = millis();
                    if(_buttonPressedCallback != nullptr) _buttonPressedCallback(_button_T.pin);  
                }
                else{                                   // not active             
                    _button_T.releasedTime = millis();
                    if(_buttonReleasedCallback != nullptr) _buttonReleasedCallback(_button_T.pin);  
                    u_long pressTime = _button_T.releasedTime - _button_T.lastPressedTime;
                    if( pressTime > _button_T.longPressTime ){
                        if(_buttonLongPressedCallback != nullptr) _buttonLongPressedCallback(_button_T.pin);
                    }else if( pressTime > _button_T.shortPressTime ){
                        if(_buttonShortPressedCallback != nullptr)  _buttonShortPressedCallback(_button_T.pin);  
                    }
                }
            } 
        }  // _button_T.currentState != _button_T.lastState
        _button_T.lastState = _button_T.lastRead;   // update last state
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

    bool Button::status(){
        return _button_T.state;
    }