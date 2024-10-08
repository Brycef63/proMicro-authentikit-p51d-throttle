#include <Arduino.h>
#include <Joystick.h>

// Initialize Joystick Library with more buttons for throttle positions
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
    JOYSTICK_TYPE_JOYSTICK,  
    5,  // Button Count 5 (1 physical + 4 virtual for throttle and mixture)
    0,  // Hat Switch Count
    true, true, true,  // X, Y, Z Axis
    false, false, false,  // No Rx, Ry, Rz
    false, true,  // No rudder, yes throttle
    false, false, false);  // No accelerator, brake, or steering

const int analogInputPinX = A0; //throttle axis
const int analogInputPinY = A1; //prop axis
const int analogInputPinZ = A2; //grip axis    
const int analogInputPinThrottle = A3;  //mixture axis

int buttonPin = 10; //ptt button on grip

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Joystick.begin();
}

void loop() {
  int xAxis = analogRead(analogInputPinX);
  int yAxis = analogRead(analogInputPinY);
  int zAxis = analogRead(analogInputPinZ);
  int ThrottleAxis = analogRead(analogInputPinThrottle);

  // Map sensor readings to joystick range (0 to 1023)
  xAxis = map(xAxis, 245, 786, 1023, 0);  //reversed 
  yAxis = map(yAxis, 184, 820, 1023, 0);  //reversed
  zAxis = map(zAxis, 277, 731, 0, 1023);
  ThrottleAxis = map(ThrottleAxis, 249, 719, 0, 1023);

  // Set Joystick Axes
  Joystick.setXAxis(xAxis);
  Joystick.setYAxis(yAxis);
  Joystick.setZAxis(zAxis);
  Joystick.setThrottle(ThrottleAxis);

  // Physical button
  Joystick.setButton(0, !digitalRead(buttonPin));

  // Virtual buttons for Throttle
  if (ThrottleAxis < 341) { // Less than 1/3 of the range (0 - 1023)
    Joystick.setButton(1, HIGH);
    Joystick.setButton(2, LOW);
    Joystick.setButton(3, LOW);
  } else if (ThrottleAxis < 682) { // Between 1/3 and 2/3 of the range
    Joystick.setButton(1, LOW);
    Joystick.setButton(2, HIGH);
    Joystick.setButton(3, LOW);
  } else { // Above 2/3 of the range
    Joystick.setButton(1, LOW);
    Joystick.setButton(2, LOW);
    Joystick.setButton(3, HIGH);
  }

  //Virtual button for xAxis War Emergancy power
    if (xAxis > 950) { // at the upper limit of the axis
    Joystick.setButton(4, HIGH);
  } else { // otherwise off
    Joystick.setButton(4, LOW);
  }


  delay(10);
}