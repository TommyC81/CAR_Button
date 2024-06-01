/////////////////////////////////////////////////////////////////
/*
    CAR_Button.h - Arduino Library to simplify working with buttons.
    Created by Tommy Carlsson.
*/
/////////////////////////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <CAR_Timer.h>

/////////////////////////////////////////////////////////////////

#ifndef DEBOUNCE_MS
    #define DEBOUNCE_MS 50
#endif
#ifndef LONGPRESS_MS 
    #define LONGPRESS_MS 250
#endif
#ifndef LONGPRESS_REPEAT_MS 
    #define LONGPRESS_REPEAT_MS 250
#endif
#ifndef MULTICLICK_MS
    #define MULTICLICK_MS 300
#endif

/////////////////////////////////////////////////////////////////

enum class CAR_ButtonState {
    PRESSED,
    RELEASED,
    CLICK_FINISH,
    LONGPRESS_FIRST,
    LONGPRESS_REPEAT,
    LONGPRESS_FINISH
};

/////////////////////////////////////////////////////////////////

class CAR_Button {
  protected:
    byte _inputPin;
    byte _previousState;
    byte _currentState;
    byte _pressedState;
    byte _clickCount = 0;
    CAR_Timer _timeSincePressed;
    CAR_Timer _timeSinceLongPressRepeat;
    unsigned int _timePressedMs = 0;
    byte _debounceTimeMs;
    bool _pressedTriggered = false;
    bool _longpressDetected = false;
    
    typedef void (*CallbackFunction) (CAR_Button&, CAR_ButtonState callback_type);
    
    CallbackFunction _buttonCallback = nullptr;
    
  public:
    CAR_Button(byte attachTo, byte buttonMode = INPUT_PULLUP, boolean activeLow = true, byte debounceTimeout = DEBOUNCE_MS);
    void setDebounceTime(byte ms);
    void reset();
    
    void setButtonHandler(CallbackFunction f);
    
    boolean isPressed();
    boolean isPressedRaw();
    
    byte getNumberOfClicks();
    unsigned int wasPressedFor();
    
    void loop();
};
