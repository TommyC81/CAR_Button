
# CAR_Button Library Documentation

## Overview

The `CAR_Button` library simplifies working with buttons on Arduino. It provides debouncing, long press detection, multi-click detection, and customizable callbacks for various button states.

## Installation

To use the `CAR_Button` library, include it in your project by copying the `CAR_Button.h` and `CAR_Button.cpp` files into your project's directory and include the header file in your sketch.

```cpp
#include "CAR_Button.h"
```

## Constants

The following constants are defined for debounce, long press, and multi-click timing. These can be overridden as needed:

- `DEBOUNCE_MS` (default: 50 ms): Debounce timeout in milliseconds.
- `LONGPRESS_MS` (default: 250 ms): Duration in milliseconds to detect a long press.
- `LONGPRESS_REPEAT_MS` (default: 250 ms): Interval in milliseconds for repeating long press events.
- `MULTICLICK_MS` (default: 300 ms): Timeout in milliseconds for multi-click detection.

## Enumerations

The `CAR_ButtonState` enum defines the possible states for button events:

- `PRESSED`: Button has been pressed.
- `RELEASED`: Button has been released.
- `CLICK_FINISH`: Button click sequence is finished.
- `LONGPRESS_FIRST`: First long press event.
- `LONGPRESS_REPEAT`: Repeated long press event.
- `LONGPRESS_FINISH`: Long press event has finished.

## Class: `CAR_Button`

### Constructor

```cpp
CAR_Button(byte attachTo, byte buttonMode = INPUT_PULLUP, boolean activeLow = true, byte debounceTimeout = DEBOUNCE_MS);
```

- `attachTo`: The pin number to which the button is attached.
- `buttonMode`: The pin mode (default: `INPUT_PULLUP`).
- `activeLow`: Specifies if the button is active low (default: `true`).
- `debounceTimeout`: Debounce time in milliseconds (default: `DEBOUNCE_MS`).

### Methods

#### `void setDebounceTime(byte ms)`

Sets the debounce time.

- `ms`: Debounce time in milliseconds.

#### `void reset()`

Resets the button state and clears all timers.

#### `void setButtonHandler(CallbackFunction f)`

Sets the callback function to handle button events.

- `f`: The callback function of type `void (*CallbackFunction) (CAR_Button&, CAR_ButtonState callback_type)`.

#### `boolean isPressed()`

Returns `true` if the button is currently pressed.

#### `boolean isPressedRaw()`

Returns the raw pressed state of the button without debouncing.

#### `byte getNumberOfClicks()`

Returns the number of button clicks.

#### `unsigned int wasPressedFor()`

Returns the duration in milliseconds for which the button was pressed.

#### `void loop()`

Handles the button state and triggers events. This method should be called repeatedly in the `loop()` function of your sketch.

## Usage Examples

### Example 1: Basic Button Press

```cpp
#include <CAR_Button.h>

CAR_Button button(2);

void setup() {
    Serial.begin(9600);
    button.setButtonHandler(buttonEventHandler);
}

void loop() {
    button.loop();
}

void buttonEventHandler(CAR_Button &btn, CAR_ButtonState state) {
    switch (state) {
        case PRESSED:
            Serial.println("Button pressed");
            break;
        case RELEASED:
            Serial.println("Button released");
            break;
        case CLICK_FINISH:
            Serial.print("Button clicked ");
            Serial.print(btn.getNumberOfClicks());
            Serial.println(" times");
            break;
        case LONGPRESS_FIRST:
            Serial.println("Long press started");
            break;
        case LONGPRESS_REPEAT:
            Serial.println("Long press repeating");
            break;
        case LONGPRESS_FINISH:
            Serial.println("Long press finished");
            break;
    }
}
```

### Example 2: Debounce Time Adjustment

```cpp
#include <CAR_Button.h>

CAR_Button button(2);

void setup() {
    Serial.begin(9600);
    button.setDebounceTime(100);
    button.setButtonHandler(buttonEventHandler);
}

void loop() {
    button.loop();
}

void buttonEventHandler(CAR_Button &btn, CAR_ButtonState state) {
    if (state == PRESSED) {
        Serial.println("Button pressed with custom debounce time");
    }
}
```

### Example 3: Active High Button

```cpp
#include <CAR_Button.h>

CAR_Button button(2, INPUT, false);

void setup() {
    Serial.begin(9600);
    button.setButtonHandler(buttonEventHandler);
}

void loop() {
    button.loop();
}

void buttonEventHandler(CAR_Button &btn, CAR_ButtonState state) {
    if (state == PRESSED) {
        Serial.println("Active high button pressed");
    }
}
```

### Example 4: Long Press Detection

```cpp
#include <CAR_Button.h>

CAR_Button button(2);

void setup() {
    Serial.begin(9600);
    button.setButtonHandler(buttonEventHandler);
}

void loop() {
    button.loop();
}

void buttonEventHandler(CAR_Button &btn, CAR_ButtonState state) {
    if (state == LONGPRESS_FIRST) {
        Serial.println("Button long pressed");
    }
}
```

### Example 5: Multi-Click Detection

```cpp
#include <CAR_Button.h>

CAR_Button button(2);

void setup() {
    Serial.begin(9600);
    button.setButtonHandler(buttonEventHandler);
}

void loop() {
    button.loop();
}

void buttonEventHandler(CAR_Button &btn, CAR_ButtonState state) {
    if (state == CLICK_FINISH) {
        Serial.print("Button multi-clicked ");
        Serial.print(btn.getNumberOfClicks());
        Serial.println(" times");
    }
}
```
