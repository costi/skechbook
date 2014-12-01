#include <Wire.h>
// the matrix
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <Adafruit_NeoPixel.h>


#include <SD.h>             // need to include the SD library for reading wave files
#define SD_ChipSelectPin 10
#include <TMRpcm.h>  
#include <SPI.h>

TMRpcm audioPlayer;

int redgreen = 6;
int lightPin = 0;  //define a pin for Photo resistor
int pressurePin = 1;  //define a pin for Pressure resistor
int color_index = 0;
boolean isMotion = false;

#define MOTIONPIN 7
#define SPEAKERPIN 9

#define NPIN 3
#define NPIXELS 1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NPIXELS, NPIN, NEO_RGB + NEO_KHZ800);

// Keep track of where we are in the color array
int ndelay = 75;
int nindex = 0;

#define NUMBER_OF_SONGS 14
char *songs[NUMBER_OF_SONGS] = {
"backoff.wav",
"brokeit.wav",
"brothers.wav",
"control.wav",
"da-man.wav",
"dreams.wav",
"emilio.wav",
"fantasy.wav",
"first1.wav",
"grabass.wav"
"happenin.wav"
"rgreco.wav"
"served.wav"
"whatlove.wav"
};

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


Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);  //Begin serial communcation
  // initialize the digital pin as an output.
  pinMode(redgreen, OUTPUT);
  pinMode(MOTIONPIN, INPUT);
  digitalWrite(redgreen, HIGH);
  
  
  pixels.begin(); // This initializes the NeoPixel library.
  //setupMatrix(matrix);
  setupAudioPlayer();
}

// the loop routine runs over and over again forever:
void loop() {
  
  
  if(audioPlayer.isPlaying()) {
    // fire on an LED
    color_index = ++color_index % num_colors;
    for (int pixel = 0; pixel < NPIXELS; pixel++){
      pixels.setPixelColor(pixel, colors[color_index]);
    }
    
  } else {
    digitalWrite(audioPlayer.speakerPin, LOW);
  }
  
  int lightamount = analogRead(lightPin);
  int pressureAmount = analogRead(pressurePin);
  isMotion = digitalRead(MOTIONPIN);
  
  if(isMotion){
      digitalWrite(redgreen, HIGH);
  } else {
      digitalWrite(redgreen, LOW);
  }
  
  pressureAlert(pressureAmount);
  //Serial.println(pressureAmount);
  
  //motionAlert(isMotion);
  Serial.println(isMotion);
  
  // lightPin for now from about 400 - dark, to 1000 - tons of light
  /*
  if(lightamount > 500) {
    digitalWrite(redgreen, LOW);
  } else {
    digitalWrite(redgreen, HIGH);
  }
  */
  
}

void motionAlert(boolean isMotion){
  static boolean motionAlerted = false;
  if(isMotion && !motionAlerted){
    motionAlerted = true;
    Serial.println("PLAY MELODY");
    digitalWrite(redgreen, HIGH);
    audioPlayer.play("0001.wav");

  } else if(!isMotion && motionAlerted){
    digitalWrite(redgreen, LOW);
    motionAlerted = false;

  }
}

void pressureAlert(int pressureAmount){  
  static int previousPressureAmount = 0;
  int diff, adjustedPressure;
  diff = previousPressureAmount - pressureAmount;
  previousPressureAmount = pressureAmount;
  adjustedPressure = pressureAmount * 9 / 1000; // adjusted per matrix size
  if(abs(diff) > 1){
    if(pressureAmount < 300){
      
    } else {
      if(!audioPlayer.isPlaying()){
        audioPlayer.play("whatlove.wav");
      }
    }
  }
}

void setupMatrix(Adafruit_8x8matrix matrix){
  matrix.begin(0x70);  // pass in the address
  matrix.clear();
  matrix.setRotation(3);
  matrix.writeDisplay();
}

void setupAudioPlayer(){
  audioPlayer.speakerPin = SPEAKERPIN;
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("Error: Cannot initialize SD CARD!");
    return;   // don't do anything more if not
  }
  audioPlayer.volume(1);
  audioPlayer.play("0000.wav"); //the sound file "1" will play each time the arduino powers up, or is reset
}


