#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1
 
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);
 
int delayval = 75;
 
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
 
// Keep track of where we are in the color array
int index = 0;
 
// Number of colors defined is the number of bytes of the array, divided by the size of the data type
int num_colors = sizeof(colors) / sizeof(uint32_t);
 
void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}
 
void loop() {   
  for (int pixel = 0; pixel < NUMPIXELS; pixel++)
  {
    pixels.setPixelColor(pixel, colors[index]);
    pixels.show();
    delay(delayval);
  }
  index++;
  if (index >= num_colors)
  {
    index = 0;
  }
}
