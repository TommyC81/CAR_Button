/////////////////////////////////////////////////////////////////
/*
    CAR_Button.cpp - Arduino Library to simplify working with buttons.
    Created by Tommy Carlsson.
*/
/////////////////////////////////////////////////////////////////

#include <CAR_Button.h>

/////////////////////////////////////////////////////////////////

CAR_Button::CAR_Button(byte p_inputPin, byte p_buttonMode /* = INPUT_PULLUP */, boolean p_activeLow /* = true */, byte p_debounceTimeout /* = DEBOUNCE_MS */) {
    _inputPin = p_inputPin;
    _pressedState = p_activeLow ? LOW : HIGH;
    _previousState = _currentState = p_activeLow ? HIGH : LOW;
    _pressedTriggered = false;
    setDebounceTime(p_debounceTimeout);
    pinMode(p_inputPin, p_buttonMode);
}

void CAR_Button::setDebounceTime(byte ms) {
    _debounceTimeMs = ms;
}

void CAR_Button::setButtonHandler(CallbackFunction f) {
    _buttonCallback = f; 
}

boolean CAR_Button::isPressed() {
    return (_currentState == _pressedState);
}

boolean CAR_Button::isPressedRaw() {
    return (digitalRead(_inputPin) == _pressedState);
}

byte CAR_Button::getNumberOfClicks() {
    return _clickCount;
}

unsigned int CAR_Button::wasPressedFor() {
    return _timePressedMs;
}

void CAR_Button::loop() {
    _previousState = _currentState;
    _currentState = digitalRead(_inputPin);
    // Check if button is pressed
    if (_currentState == _pressedState) {
        // Check if the button was pressed now
        if (_previousState != _pressedState) {
            _timeSincePressed = 0;
            _timeSinceLongPressRepeat = 0;
            _pressedTriggered = false;
            _longpressDetected = false;
        // Only trigger pressed event after debounce
        } else if (!_pressedTriggered && _timeSincePressed >= _debounceTimeMs) {
            _pressedTriggered = true;
            _clickCount++;
			if(_buttonCallback)
				_buttonCallback(*this, CAR_ButtonState::PRESSED);
        // Set _longpressDetected after button has been _pressedState for LONGPRESS_MS
        // Then repeat the longpress events
        } else if (_pressedTriggered && _clickCount == 1) {
            if (!_longpressDetected && _timeSincePressed >= LONGPRESS_MS) {
                _timePressedMs = _timeSincePressed;
                _longpressDetected = true;
                _timeSinceLongPressRepeat = 0;
				if(_buttonCallback)
					_buttonCallback(*this, CAR_ButtonState::LONGPRESS_FIRST);
            } else if (_longpressDetected && _timeSinceLongPressRepeat >= LONGPRESS_REPEAT_MS) {
                _timePressedMs = _timeSincePressed;
                _timeSinceLongPressRepeat = 0;
				if(_buttonCallback)
					_buttonCallback(*this, CAR_ButtonState::LONGPRESS_REPEAT);
            }
        }
    // Was _pressedTriggered? (the button is no longer in _pressedState)
    } else if (_pressedTriggered) {
        // was the button released now?
        if (_previousState == _pressedState) {
            _timePressedMs = _timeSincePressed;
            // was it the end of a longpress? (precedes any other clicks/released events)
            // however, once multiple clicks have started, don't change to a long press
            if (_clickCount == 1 && _longpressDetected) {
                // trigger long-press finish before resetting
				if(_buttonCallback)
					_buttonCallback(*this, CAR_ButtonState::LONGPRESS_FINISH);
                _longpressDetected = false;
                _pressedTriggered = false;
                _clickCount = 0;
            } else {
                // trigger release if it was not the end of a long-press
                _buttonCallback(*this, CAR_ButtonState::RELEASED);
            }
        } else if (_clickCount > 0 && _timeSincePressed >= MULTICLICK_MS) {
            // trigger click finish first before resetting
			if(_buttonCallback)
				_buttonCallback(*this, CAR_ButtonState::CLICK_FINISH);
            _pressedTriggered = false;
            _clickCount = 0;
        }
    }
}

void CAR_Button::reset() {
    _timeSincePressed = 0;
    _timeSinceLongPressRepeat = 0;
    _pressedTriggered = false;
    _longpressDetected = false;
    _clickCount = 0;
    _buttonCallback = nullptr;
}
