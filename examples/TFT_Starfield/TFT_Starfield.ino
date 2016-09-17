#include <TFT_ILI9163.h> // Hardware-specific library
#include <SPI.h>

TFT_ILI9163 tft = TFT_ILI9163();       // Invoke custom library

// With 256 stars the update frame rate is about 50Hz
#define NSTARS 256
uint8_t sx[NSTARS] = {};
uint8_t sy[NSTARS] = {};
uint8_t sz[NSTARS] = {};

uint8_t za, zb, zc, zx;

// Fast 0-255 random number generator from http://eternityforest.com/Projects/rng.php:
uint8_t __attribute__((always_inline)) rng()
{
  zx++;
  za = (za^zc^zx);
  zb = (zb+za);
  zc = (zc+(zb>>1)^za);
  return zc;
}

void setup() {
  za = random(256);
  zb = random(256);
  zc = random(256);
  zx = random(256);

  //Serial.begin(9600);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.fastSetup(); // Prepare plot window range for fast pixel plotting
}

void loop()
{
  //unsigned long t0 = micros();
  uint8_t spawnDepthVariation = 255;

  for(int i = 0; i < NSTARS; ++i)
  {
    if (sz[i] <= 1)
    {
      sx[i] = 80 - 64 + rng();
      sy[i] = rng();
      sz[i] = spawnDepthVariation--;
    }
    else
    {
      int old_screen_x = ((int)sx[i] - 80) * 256 / sz[i] + 80;
      int old_screen_y = ((int)sy[i] - 64) * 256 / sz[i] + 64;

      // This is a faster pixel drawing function for occassions where many single pixels must be drawn
      tft.fastPixel(old_screen_x, old_screen_y,TFT_BLACK);

      sz[i] -= 2;
      if (sz[i] > 1)
      {
        int screen_x = ((int)sx[i] - 80) * 256 / sz[i] + 80;
        int screen_y = ((int)sy[i] - 64) * 256 / sz[i] + 64;
  
        if (screen_x >= 0 && screen_y >= 0 && screen_x < 160 && screen_y < 128)
        {
          uint8_t r, g, b;
          r = g = b = 255 - sz[i];
          tft.fastPixel(screen_x, screen_y, tft.color565(r,g,b));
        }
        else
          sz[i] = 0; // Out of screen, die.
      }
    }
  }
  //unsigned long t1 = micros();
  //static char timeMicros[8] = {};

  //Serial.println(t1 - t0);
}


