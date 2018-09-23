// Very simple, stripped down version of Saruman's staff
// Sets the light to breathing, does not interact with the 9DOF sensor inside

//specify libraries needed
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPI.h>

//Global Variables

// From "sipping power with neopixels
// This bizarre construct isn't Arduino code in the conventional sense.
// It exploits features of GCC's preprocessor to generate a PROGMEM
// table (in flash memory) holding an 8-bit unsigned sine wave (0-255).
const int _SBASE_ = __COUNTER__ + 1; // Index of 1st __COUNTER__ ref below
#define _S1_ (sin((__COUNTER__ - _SBASE_) / 128.0 * M_PI) + 1.0) * 127.5 + 0.5,
#define _S2_ _S1_ _S1_ _S1_ _S1_ _S1_ _S1_ _S1_ _S1_ // Expands to 8 items
#define _S3_ _S2_ _S2_ _S2_ _S2_ _S2_ _S2_ _S2_ _S2_ // Expands to 64 items
const uint8_t PROGMEM sineTable[] = { _S3_ _S3_ _S3_ _S3_ }; // 256 items

// (PB) Similar to above, but for an 8-bit gamma-correction table.
#define _GAMMA_ 2.6
const int _GBASE_ = __COUNTER__ + 1; // Index of 1st __COUNTER__ ref below
#define _G1_ pow((__COUNTER__ - _GBASE_) / 255.0, _GAMMA_) * 255.0 + 0.5,
#define _G2_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_ _G1_ // Expands to 8 items
#define _G3_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_ _G2_ // Expands to 64 items
const uint8_t PROGMEM gammaTable[] = { _G3_ _G3_ _G3_ _G3_ }; // 256 items

#define HARDWARETEST 0 // set to 1 to test neopixles at start up
#define PRINTDATA 0   // set to 1 to print out sensor data 

//NeoPixels
#define LEDPIN 4 // sets pin LEDs connect to
#define NUMPIXELS 1 // specifies number of pixels in the strip
#define BRIGHTNESS 225 // max 225
// third argument  below necessary because LED is other than RGB
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRBW); 

// Setup
void setup() {
  Serial.begin(115200);
  setupNeoPixel();
}

// the loop function runs over and over again forever
void loop() { 
  staffBreathe();
}

// Rendering Functions

//causes LED to breathe (mod of mode_sine_gamma from 'sipping power with neo-pixels')
void staffBreathe() {
  uint32_t t = millis() / 16; // Divide by a higher number to slow this down
  uint8_t k = pgm_read_byte(&sineTable[(t + 512 / 12) & 0xFF]);
  k = pgm_read_byte(&gammaTable[k]);
  strip.setPixelColor(0, strip.Color(0, 0, 0, k));
  strip.show();
}

// set intial pixles status and set brightness
void setupNeoPixel() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  if (HARDWARETEST) {
    Serial.println("Testing NeoPixels");
    strip.setPixelColor(0, 0, 0, 0, 255); // White
    strip.setPixelColor(0, 0, 0, 0); // turns light off
  }
}