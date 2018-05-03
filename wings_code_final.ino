// This code was written for the Social Butterfly project in Critical Making, Provocation 3.
// Justine, Vivian, Varna, Yuki
// Last modified April 29, 2018. 

#include <CapacitiveSensor.h>
#include <Adafruit_NeoPixel.h>

// initialize the cap touch library
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 1M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire 
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 1M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire

long touchTime = 0;                      // time of last touch
int touchState = 0;                      // if the wire si being touched
int threshold = 300;                    // touch sensitivity threshold
int touchDelay = 500;                    // how long to delay before resetting touch sensor

long touchTime2 = 0;                      // time of last touch
int touchState2 = 0;                      // if the wire is being touched
int threshold2 = 300;                    // touch sensitivity threshold
int touchDelay2 = 500;                    // how long to delay before resetting touch sensor

const int ledPin = 12; // the I/O pin for neopixels for sensor1
const int ledPin2 = 11; // the I/O pin for neopixels for sensor2
const int numPixels = 9; // number of pixels in the strips

// for the motor
int potPin = A0;   // select the input pin for the potentiometer
int motorPin = 9; // select the pin for the Motor
int val = 0;      // variable to store the value coming from the sensor

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, ledPin, NEO_GRB + NEO_KHZ800); // initialize the strip
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(numPixels, ledPin2, NEO_GRB + NEO_KHZ800); // initialize the strip


void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);// turn off autocalibrate 
   cs_4_6.set_CS_AutocaL_Millis(0xFFFFFFFF);// turn off autocalibrate 
   Serial.begin(9600);
   strip.begin(); // initialize pixel strip
   strip.show(); // Initialize all pixels to 'off'
   strip2.begin(); // initialize pixel strip
   strip2.show(); // Initialize all pixels to 'off'
}

// functions for light patterns

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void showStrip2() {
   strip2.show();
}
void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setPixel2(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip2.setPixelColor(Pixel, strip2.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < numPixels; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void setAll2(byte red, byte green, byte blue) {
  for(int i = 0; i < numPixels; i++ ) {
    setPixel2(i, red, green, blue); 
  }
  strip2.show();
}

void theaterChaseRainbow(int SpeedDelay) {
  byte *c;
  
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < numPixels; i=i+3) {
          c = Wheel( (i+j) % 255);
          setPixel(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        showStrip();
       
        delay(SpeedDelay);
       
        for (int i=0; i < numPixels; i=i+3) {
          setPixel(i+q, 0,0,0);        //turn every third pixel off
        }
    }
  }
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< numPixels; i++) {
      c=Wheel(((i * 256 / numPixels) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

void rainbowCycle2(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< numPixels; i++) {
      c=Wheel(((i * 256 / numPixels) + j) & 255);
      setPixel2(i, *c, *(c+1), *(c+2));
    }
    strip2.show();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void FadeInOut2(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll2(r,g,b);
    strip2.show();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll2(r,g,b);
    strip2.show();
  }
}

uint16_t color_offset = 0;

void loop()                    
{
 byte *c;
 uint16_t i;
 
 long sensorTouch =  cs_4_2.capacitiveSensor(50); // read touch sensor1
 long sensorTouch2 =  cs_4_6.capacitiveSensor(50); // read touch sensor2
 
 // if sensor is touched and it wasn't touched on the last check
  if (sensorTouch > threshold && touchState == 0 && touchTime + touchDelay < millis()){
    Serial.println("setting touchstate to 1");
    touchState = 1;
    touchState2 = 0;
    touchTime = millis();
 }



//  if the touch delay has passed, reset touchState:
  if (sensorTouch > threshold && touchTime + touchDelay < millis() && touchState ==1) {
    touchState = 0;
    touchTime = millis();
    
 }
// sensor 2
//Serial.println(sensorTouch2);
if (sensorTouch2 > threshold && touchState2 == 0 && touchTime2 + touchDelay2 < millis()){
  Serial.println("setting touchstate2 to 1");
    Serial.println("AHH WHERES MY TRACTOR");
    touchState2 = 1;
    touchState = 0;    
    touchTime2 = millis();
 }

//  if the touch delay has passed, reset touchState:
  if (sensorTouch2 > threshold && touchTime2 + touchDelay2 < millis() && touchState2 ==1) {
    touchState2 = 0;
    touchTime2 = millis();
    
 }

 // set strip colors
 if (touchState == 1) {
    for(i=0; i< numPixels; i++) {
      c=Wheel(((i * 256 / numPixels) + color_offset) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
 } else {
  setAll(0,0,0);
 }

  if (touchState2 == 1) {
    for(i=0; i< numPixels; i++) {
      c=Wheel(((i * 256 / numPixels) + color_offset) & 255);
      setPixel2(i, *c, *(c+1), *(c+2));
    }
 } else {
  setAll2(0,0,0);
 }

 color_offset++;
 // update the strip:
 strip.show();
 strip2.show();

// motor
 val = analogRead(A0);
 val = min(val, 100 );// read the value from the sensor, between 0 - 1024
 //Serial.println(val);
 //  Serial.println(digitalRead(2));
 analogWrite(motorPin, val); // analogWrite can be between 0-255

}


