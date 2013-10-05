/*
 * Main.cpp
 *
 * Created: 9/28/2013 11:09:06 PM
 *  Author: Shady
 */



#ifndef SKETCH_H_
#include "Sketch.h"
#endif



#include "Arduino.h"
#include <digitalWriteFast.h>  // library for high performance reads and writes by jrraines
                               // see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1267553811/0
                               // and http://code.google.com/p/digitalwritefast/

#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>
// It turns out that the regular digitalRead() calls are too slow and bring the arduino down when
// I use them in the interrupt routines while the motor runs at full speed creating more than
// 40000 encoder ticks per second per motor.
 
// Quadrature encoders
// X encoder Pin + Int + Var def
#define c_XEncoderInterrupt 4
#define c_XEncoderPinA 19
#define c_XEncoderPinB 45
#define c_XEncoderPinI 47
#define LeftEncoderIsReversed
volatile bool _XEncoderBSet;
volatile bool _XEncoderISet;
volatile long _XEncoderTicks = 0;
volatile long _XEncoderIndex = 0;
// Y encoder Pin + Int + Var def
#define c_YEncoderInterrupt 5
#define c_YEncoderPinA 18
#define c_YEncoderPinB 41
#define c_YEncoderPinI 43
volatile bool _YEncoderBSet;
volatile bool _YEncoderISet;
volatile long _YEncoderTicks = 0;
volatile long _YEncoderIndex = 0;

#define X_DIR_PIN 12
#define Y_DIR_PIN 13
#define X_PWM_PIN 3
#define Y_PWM_PIN 11

//STOPS and joystic
#define X_MIN_PIN          16
#define X_MAX_PIN          17 
#define X_JOY_LEFT         35 
#define X_JOY_RIGHT        37 
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Y_JOY_UP           31
#define Y_JOY_DOWN         33
volatile bool _XMinPinSet;
volatile bool _XMaxPinSet;
volatile bool _XJoyLeft;
volatile bool _XJoyRight;
volatile bool _YMinPinSet;
volatile bool _YMaxPinSet;
volatile bool _YJoyUp;
volatile bool _YJoyDown;
 
volatile int _XCsens;    // variable to read the value from the analog pin
 /*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void setup()
{
  Serial.begin(115200);
 
  //Setup Channel A
  pinMode(X_DIR_PIN, OUTPUT); //Initiates Motor Channel A pin
  pinMode(Y_DIR_PIN, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  pinMode(A0,INPUT);
  pinMode(X_MIN_PIN,INPUT_PULLUP);
  pinMode(X_MAX_PIN,INPUT_PULLUP);
  pinMode(X_JOY_LEFT,INPUT_PULLUP);
  pinMode(X_JOY_RIGHT,INPUT_PULLUP);
  pinMode(Y_JOY_DOWN,INPUT_PULLUP);
  pinMode(Y_JOY_UP,INPUT_PULLUP);
  // Quadrature encoders
  // X encoder
  pinMode(c_XEncoderPinA, INPUT);      // sets pin A as input
  digitalWrite(c_XEncoderPinA, LOW);  // turn on pullup resistors
  pinMode(c_XEncoderPinB, INPUT);      // sets pin B as input
  digitalWrite(c_XEncoderPinB, LOW);  // turn on pullup resistors
  attachInterrupt(c_XEncoderInterrupt, HandleLeftMotorInterruptA, RISING);
 
  // Y encoder
  pinMode(c_YEncoderPinA, INPUT);      // sets pin A as input
  digitalWrite(c_YEncoderPinA, LOW);  // turn on pullup resistors
  pinMode(c_YEncoderPinB, INPUT);      // sets pin B as input
  digitalWrite(c_YEncoderPinB, LOW);  // turn on pullup resistors
  attachInterrupt(c_YEncoderInterrupt, HandleRightMotorInterruptA, RISING);
  
  
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight

// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(150);
    lcd.noBacklight();
    delay(150);
  }
  lcd.backlight(); // finish with backlight on  

//-------- Write characters on the display ------------------
  // NOTE: Cursor Position: Lines and Characters start at 0  
  lcd.setCursor(3,0); //Start at character 4 on line 0
  lcd.print("Hello, world!");
 

}/*--(end setup )---*/

 
void updatelcd() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("XT");
  lcd.setCursor(3,0);
  lcd.print(_XEncoderTicks);
  lcd.setCursor(10,0);
  lcd.print("XI");
  lcd.setCursor(13,0);
  lcd.print(_XEncoderIndex);
  lcd.setCursor(18,0);
  lcd.print("X");
  lcd.setCursor(19,0);
  lcd.print(">" + _XMinPinSet);
  lcd.setCursor(17,0);
  lcd.print("<" + _XMaxPinSet);
  
  lcd.setCursor(0,1);
  lcd.print("YI");
  lcd.setCursor(3,1);
  lcd.print(_YEncoderTicks);
  lcd.setCursor(10,1);
  lcd.print("YI");
  lcd.setCursor(13,1);
  lcd.print(_YEncoderIndex);
  lcd.setCursor(18,1);
  lcd.print("Y");
  lcd.setCursor(19,1);
  lcd.print("V" + _YMinPinSet);
  lcd.setCursor(17,1);
  lcd.print("^" + _YMaxPinSet);
  /*
  lcd.setCursor(15,0);
  lcd.print(_XCsens);*/
  lcd.setCursor(3,3);
  lcd.print("<" + _XJoyLeft);
  lcd.setCursor(5,3);
  lcd.print(">" + _XJoyRight);
  lcd.setCursor(4,3);
  lcd.print("^" + _YJoyUp);
  lcd.setCursor(4,3);
  lcd.print("V" + _YJoyDown);
  delay(20);
}
 
 
void loop()
{
updatelcd();
_XJoyLeft = digitalReadFast(X_JOY_LEFT);
_XJoyRight = digitalReadFast(X_JOY_RIGHT);
_YJoyUp = digitalReadFast(Y_JOY_UP);
_YJoyDown = digitalReadFast(Y_JOY_DOWN);
analogWrite(X_PWM_PIN, 0);
analogWrite(Y_PWM_PIN, 0);
if(!_XJoyLeft)
{
digitalWrite(X_DIR_PIN, HIGH); //Establishes forward direction of Channel A
analogWrite(X_PWM_PIN, 255);   //Spins the motor on Channel A at full speed
}
if(!_XJoyRight)
{
digitalWrite(X_DIR_PIN,LOW); //Establishes backward direction of Channel A
analogWrite(X_PWM_PIN, 255);   //Spins the motor on Channel A at full speed
}
if(!_YJoyUp)
{
digitalWrite(Y_DIR_PIN,HIGH); //Establishes backward direction of Channel A
analogWrite(Y_PWM_PIN, 255);   //Spins the motor on Channel A at full speed
}
if(!_YJoyDown)
{
digitalWrite(Y_DIR_PIN,LOW); //Establishes backward direction of Channel A
analogWrite(Y_PWM_PIN, 255);   //Spins the motor on Channel A at full speed
}
delay(200);
}

// Interrupt service routines for the left motor's quadrature encoder
void HandleLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  //_XCsens=analogRead(A0);
  _XEncoderBSet = digitalReadFast(c_XEncoderPinB);   // read the input pin
  _XEncoderISet = digitalReadFast(c_XEncoderPinI);
  _XMinPinSet = digitalReadFast(X_MIN_PIN);
  _XMaxPinSet = digitalReadFast(X_MAX_PIN);
  // and adjust counter + if A leads B
  #ifdef LeftEncoderIsReversed
    _XEncoderTicks -= _XEncoderBSet ? -1 : +1;
    if (_XEncoderISet == 0)
    _XEncoderIndex -= _XEncoderBSet ? -1 : +1;
  #else
    _XEncoderTicks += _XEncoderBSet ? -1 : +1;
  #endif
}
 
// Interrupt service routines for the right motor's quadrature encoder
void HandleRightMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _YEncoderBSet = digitalReadFast(c_YEncoderPinB);   // read the input pin
   _YEncoderISet = digitalReadFast(c_YEncoderPinI);
   _YMinPinSet = digitalReadFast(Y_MIN_PIN);
   _YMaxPinSet = digitalReadFast(Y_MAX_PIN);
  // and adjust counter + if A leads B
  #ifdef RightEncoderIsReversed
    _YEncoderTicks -= _YEncoderBSet ? -1 : +1;
	 if (_YEncoderISet == 0)
	 _YEncoderIndex -= _YEncoderBSet ? -1 : +1;
  #else
    _YEncoderTicks += _YEncoderBSet ? -1 : +1;
  #endif
}