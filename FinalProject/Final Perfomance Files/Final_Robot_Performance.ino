
//Cap1188
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>

//MotorShield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
#include <SoftwareSerial.h>
#endif
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);


//NeoPixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>

#endif
#define NUMPIXELS1      14
#define NUMPIXELS2      4  // Mouth
#define NUMPIXELS3      27
//Chest
//Mouth
//Frame
Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS1, 4, NEO_GRB + NEO_KHZ800);//mouth
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS2, 5, NEO_GRB + NEO_KHZ800);//frame
Adafruit_NeoPixel pixels3 = Adafruit_NeoPixel(NUMPIXELS3, 6, NEO_GRB + NEO_KHZ800);//Chest

int speaking = 0;
int counter = 70;
int pbutton = 0;


int partyToggle;
int prevParty;
int partyCounter = 0;
int partyChestCounter = 0;


int frameToggle;
int frameCounter;
int prevFrame;



int breathToggle;
int prevBreath;
int breathloc1 = 13;
int breathloc2 = 14;
int breathCounter = 0;
int pace = 15;



#define CAP1188_CS  10

volatile byte interrupt = 0;

Adafruit_CAP1188 cap = Adafruit_CAP1188();



//Solenoid
int solenoidPin = 7;



//Servo
#include <Servo.h>
Servo myservo;


//end of initialising ****************************

void setup() {
  Serial.begin(9600);

  //Cap1188
  Serial.println("CAP1188 test!");

  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");
  uint8_t reg = cap.readRegister( 0x1f ) & 0x0f;
  cap.writeRegister( 0x1f, reg | 0x5A ); // or whatever value you want


  // DC motor
  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
//  delay(10);
  // turn on motor
  myMotor->run(RELEASE);
//  delay(10);

  
  //Solenoid
  pinMode(solenoidPin, OUTPUT);
  digitalWrite(solenoidPin, LOW);

  //Servo
  

  //`neopixel
  //Neopixels
  pixels1.begin(); // This initializes the NeoPixel library. Frame
  pixels2.begin();//mouth
  pixels3.begin();//chest
  //pixels1.show();
  //pixels2.show();
  //pixels3.show();


  //End of setup()
}

void loop() {
  //Cap1188
  uint8_t touched = cap.touched();
  Serial.println(touched);


  if (touched == 1 && touched != pbutton) {
    speaking ++;
    pbutton = touched;
    //    Serial.println(speaking);
    //Toggle mouth


  }

  if (touched == 4 && touched != pbutton) {
    frameToggle++;
    //Frame on off
    pbutton = touched;


  }
  if (touched == 8 && touched != pbutton) {
    breathToggle++;
    //Frame on off
    pbutton = touched;
    // Chest toggle on or off
    pace = 20;


  }


  if (touched == 16 && touched != pbutton) {

    pace = 10;
    pbutton = touched;

  }

  //
  if (touched == 32) { //C6
    //Arm
    myMotor->setSpeed(150);
    myMotor->run(FORWARD);
        delay(10);
    Serial.println("motor go");
  } else {
    myMotor->run(RELEASE);
    delay(10);
    Serial.println("motor stopped");
  }


  if (touched == 64) { //C7
    //Servo
  myservo.attach(9);
  myservo.write(180);
  } else {
myservo.detach();
  }

  if (touched == 128) {
    //Solenoid
    digitalWrite(solenoidPin, HIGH);    //Switch Solenoid ON
  } else {
    digitalWrite(solenoidPin, LOW);
  }

  if (speaking % 2 == 1) {
    talkie();
  } else {
    pixels2.setPixelColor(1, pixels2.Color(100, 100, 100));
    pixels2.setPixelColor(2, pixels2.Color(100, 100, 100));
    pixels2.setPixelColor(3, pixels2.Color(100, 100, 100));
    pixels2.setPixelColor(4, pixels2.Color(100, 100, 100));
    pixels2.show();

  }


  if (frameToggle % 2 == 1) {
    frameTime();

  } else {
    pixels1.clear();
  }


  if (breathToggle % 2 == 1) {
    Breath();

  } else {
    pixels3.clear();
  }

  if (touched == 2) {
    partyTime();
  }

  pixels1.show();
  pixels2.show();
  pixels3.show();
  //  pbutton = touched;
  //  prevParty = touched;
  //  prevFrame = touched;
  //  prevBreath = touched;

  //end of loop
}


//////Functions


void talkie() {
  if (counter > 35) {
    pixels2.setPixelColor(1, pixels2.Color(100, 100, 100));
    pixels2.setPixelColor(2, pixels2.Color(100, 100, 100));
    pixels2.setPixelColor(3, pixels2.Color(100, 100, 100));
    pixels2.setPixelColor(4, pixels2.Color(100, 100, 100));
  }
  if (counter < 35) {
    pixels2.setPixelColor(1, pixels2.Color(0, 0, 0));
    pixels2.setPixelColor(2, pixels2.Color(0, 0, 0));
    pixels2.setPixelColor(3, pixels2.Color(0, 0, 0));
    pixels2.setPixelColor(4, pixels2.Color(0, 0, 0));
  }
  counter--;
  if (counter == 0) {
    counter = 70;
  }
}


void frameTime() {
  pixels1.setPixelColor(frameCounter, pixels1.Color(200, 180, 150));
  frameCounter ++;
  if (frameCounter == NUMPIXELS1) {
    frameCounter = 0;
  }

}
//////////////////////////
void partyTime() {
  int  r = random(255);
  int  g = random(255);
  int b = random(255);
  pixels1.setPixelColor(partyCounter, pixels1.Color(r, g, b));
  pixels3.setPixelColor(partyChestCounter, pixels3.Color(r, g, b));


  partyCounter ++;
  partyChestCounter++;

  if (partyChestCounter == NUMPIXELS3) {
    partyChestCounter = 0;
  }

  if (partyCounter == NUMPIXELS1) {
    partyCounter = 0;
  }
}


void Breath() {
  pixels3.setPixelColor(breathloc1, pixels3.Color(180, 150, 120));
  pixels3.setPixelColor(breathloc2, pixels3.Color(180, 150, 120));
  pixels3.setPixelColor(breathloc1 - 5, pixels3.Color(0, 0, 0));
  pixels3.setPixelColor(breathloc2 + 5, pixels3.Color(0, 0, 0));
  if ((breathCounter % pace) == 1) {
    breathloc1--;
    breathloc2++;
  }
  if (breathloc1 == 0) {
    breathloc1 = 13;
    breathloc2 = 14;
  }
  breathCounter++;

}
