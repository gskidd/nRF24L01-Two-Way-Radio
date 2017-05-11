/*
   I use the RF24 libraries listed at the bottom of the code for this project
*/


/* RF24 Audio Library TMRh20 2014

  This sketch is demonstrates the use of private channels (node-to-node) in a multi-radio group.

  Requirements:
  3 or more Arduinos (Uno,Nano,Mega, etc supported)
  3 or more NRF24LO1 Radio Modules
  1 or more input devices (microphone, ipod, etc)
  2 or more output devices (speaker, amplifier, etc)

  Setup:
  1. Change the CE,CS pins below to match your chosen pins (I use 7,8 on 328 boards, and 48,49 on Mega boards)
  2. Set the radio identifier number. This will identify each radio in the radio group.
  2. Upload this sketch to three or more devices
  3. Send commands via the serial monitor to control the devices

  Default Pin Selections:
  Speaker: pins 9,10 on UNO, Nano,  pins 11,12 on Mega 2560
  Input/Microphone: Analog pin A0 on all boards

  Note: Should support up to 10 radios currently. This has not been fully tested.
  Note: Pin selections can be overridden by modifying the userConfig.h file included with the library
  Note: If two or more radios share the same identifier, they will receive the same single-pipe transmissinos, but will be unable
  to establish node-to-node communication between each other.

*/


#include <RF24.h>
#include <SPI.h>
#include <RF24Audio.h>


RF24 radio(7, 8);		// Set radio up using pins 7 (CE) 8 (CS)

// instantiates the interrupt pins
const byte interruptPin = 2;
const byte interupptPin = 3;
short countCheck = 0;
short radioCom = 1;
short oldCom = 2;

//Pin connected to ST_CP of 74HC595
short latchPin = 0;
//Pin connected to SH_CP of 74HC595
short clockPin = 5;
////Pin connected to DS of 74HC595
short dataPin = 1;

/********* Set the Radio Identifier Here ************/
RF24Audio rfAudio(radio, 0);	// Set up the audio using the radio, and set to radio number 0.
// Setting the radio number is only important if one-to-one communication is desired
// in a multi-node radio group. See the private

void setup() {
  //  Serial.begin(115200);
  rfAudio.begin();		// The only thing to do is initialize the library.

  // sets up pins for 595 shift register output pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // sets up interrupt pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, GoToNode1, CHANGE);
  attachInterrupt(1, GoToNode2 , CHANGE);

  // sets channel of radio initially to 1
  radio.setChannel(1);

}

void loop() {
  // checks the raioCom value that is set by the interrupts
  if (radioCom == 1) {
    if (radioCom != oldCom) {
       // sets channel to 1
      radio.setChannel(1);

      // prints out 1 on the seven segment display
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 6);
      delay(100);
      digitalWrite(latchPin, HIGH);
      oldCom = radioCom;
    }
  } else if (radioCom == 2) {
    if (radioCom != oldCom) {
      // sets channel to 2

      radio.setChannel(2);
      // prints out 2 on the seven segment display
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 91);
      delay(100);
      digitalWrite(latchPin, HIGH);
      oldCom = radioCom;
    }
  }
}

// interrupt for pin 2
void GoToNode1() {
  radioCom = 1;

}

//interrupt for pin 3
void GoToNode2() {
  radioCom = 2;
}
/* Documentation and References:

  References:
  All Library documentation: http://tmrh20.github.io/
  New (2014) RF24 Audio Library: https://github.com/TMRh20/RF24Audio
  Optimized (2014) RF24 Lib Source Code: https://github.com/TMRh20/RF24
  Optimized (2014) RF24 Network Lib: https://github.com/TMRh20/RF24Network

*/
