
#include <Adafruit_NeoPixel.h>

/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */
// constants won't change. Used here to set a pin number :
const int ledPin =  13;      // the number of the LED pin

// Variables will change :
int ledState = LOW;             // ledState used to set the LED

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6
 
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      5
 
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);


// The colors of the rainbow that we will cycle through
uint32_t colors[] = {
  pixels.Color(128, 0, 0),
  pixels.Color(128, 64, 0),
  pixels.Color(128, 128, 0),
  pixels.Color(64, 128, 0),
  pixels.Color(0, 128, 0),
  pixels.Color(0, 128, 64),
  pixels.Color(0, 128, 128),
  pixels.Color(0, 64, 128),
  pixels.Color(0, 0, 128),
  pixels.Color(64, 0, 128),
  pixels.Color(128, 0, 128),
  pixels.Color(128, 0, 64)
};


// Number of colors defined is the number of bytes of the array, divided by the size of the data type
int num_colors = sizeof(colors) / sizeof(uint32_t);


long blinkingStart = 0;
int blinkingDuration = 5000;

// Keep track of where we are in the color array
int colorIndex = 0;
long previousLedUpdateMillis = 0;
int pixelDelay = 50;
int currentPixel = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  
  long currentMillis = millis();
  bool isBlinking = ((currentMillis - blinkingStart) < blinkingDuration);
  
  if (voltage > 1 && !isBlinking) {
    // print out the value you read:
    Serial.println(voltage);
    blinkingStart = millis();
  }
 
  if(isBlinking){ 
    doBlinky();
  } else {
    for (int pixel = 0; pixel < NUMPIXELS; pixel++) {
       pixels.setPixelColor(pixel, colors[0]);
    }
    pixels.show();
  }
  
}

void doBlinky(){
  long currentMillis = millis();
  if(currentMillis - previousLedUpdateMillis > pixelDelay){
    previousLedUpdateMillis = currentMillis;
    pixels.setPixelColor(currentPixel, colors[colorIndex]);
       pixels.show();
     }

     currentPixel++;
     if (currentPixel >= NUMPIXELS){
       currentPixel = 0;
       colorIndex++;     
       if (colorIndex >= num_colors) {
         colorIndex = 0;
       }
     }
}
