#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// SOFTWARE Stand : 12.09.2019 AUSGELIEFERT !!!
//es können 9 Emotionen über ein PWM Signal vom Maestro angewählt werden.
//das PWM Signal entspricht immer der angewählten emotion *110
//Beispiel Emotion 1 PWM Signal = 100 / 2 Super Happy = 200
// Pins 2 + 3 werden für das PWM Signal benötigt sowie Ground
//Pin 2 ist das äussere Signal des Servos signals normalerweise weiss oder gelb
//Pin 3 ist das mittlere Signal rot bei den servos
//Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
volatile int pwm_value = 1000; //Variablen die von Ausserhalb des VOID Loops verändert werden.
volatile int prev_time = 0;
/* ########################### Adafruit ######################## */
// Because the two eye matrices share the same address, only four
// matrix objects are needed for the five displays:
#define MATRIX_EYES         0
#define MATRIX_MOUTH_LEFT   1
#define MATRIX_MOUTH_MIDDLE 2
#define MATRIX_MOUTH_RIGHT  3
Adafruit_8x8matrix matrix[4] = { // Array of Adafruit_8x8matrix objects
  Adafruit_8x8matrix(), Adafruit_8x8matrix(),
  Adafruit_8x8matrix(), Adafruit_8x8matrix() };

// Rather than assigning matrix addresses sequentially in a loop, each
// has a spot in this array.  This makes it easier if you inadvertently
// install one or more matrices in the wrong physical position --
// re-order the addresses in this table and you can still refer to
// matrices by index above, no other code or wiring needs to change.
static const uint8_t matrixAddr[] = { 0x70, 0x71, 0x72, 0x73 };

static const uint8_t PROGMEM // Bitmaps are stored in program memory
  blinkImg[][8] = {    // Eye animation frames
  { B00111100,         // Fully open eye
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00000000,
    B00000000,
    B00111100,
    B11111111,
    B11111111,
    B11111111,
    B00111100,
    B00000000 },
  { B00000000,
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B01111110,
    B00011000,
    B00000000 },
  { B00000000,         // Fully closed eye
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000 } },
  mouthImg[][24] = {                 // Mouth animation frames
  { B00000000, B00000000, B00000000, // Mouth position A
    B00000000, B00000000, B00000000,
    B01111111, B11111111, B11111110,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position B
    B00000000, B00000000, B00000000,
    B00111111, B11111111, B11111100,
    B00000111, B00000000, B11100000,
    B00000000, B11111111, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position C
    B00000000, B00000000, B00000000,
    B00111111, B11111111, B11111100,
    B00001000, B00000000, B00010000,
    B00000110, B00000000, B01100000,
    B00000001, B11000011, B10000000,
    B00000000, B00111100, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position D
    B00000000, B00000000, B00000000,
    B00111111, B11111111, B11111100,
    B00100000, B00000000, B00000100,
    B00010000, B00000000, B00001000,
    B00001100, B00000000, B00110000,
    B00000011, B10000001, B11000000,
    B00000000, B01111110, B00000000 },
  { B00000000, B00000000, B00000000, // Mouth position E
    B00000000, B00111100, B00000000,
    B00011111, B11000011, B11111000,
    B00000011, B10000001, B11000000,
    B00000000, B01111110, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 },
  { B00000000, B00111100, B00000000, // Mouth position F
    B00000000, B11000011, B00000000,
    B00001111, B00000000, B11110000,
    B00000001, B00000000, B10000000,
    B00000000, B11000011, B00000000,
    B00000000, B00111100, B00000000,
    B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000 } };

uint8_t
  blinkIndex[] = { 1, 2, 3, 4, 3, 2, 1 }, // Blink bitmap sequence
  blinkCountdown = 100, // Countdown to next blink (in frames)
  gazeCountdown  =  75, // Countdown to next eye movement
  gazeFrames     =  50, // Duration of eye movement (smaller = faster)
  mouthPos       =   0, // Current image number for mouth
  mouthCountdown =  10; // Countdown to next mouth change
int8_t
  eyeX = 3, eyeY = 3,   // Current eye position
  newX = 3, newY = 3,   // Next eye position
  dX   = 0, dY   = 0;   // Distance from prior to new position 

void setup() {
  Serial.begin(115200);
  Serial.println("NAVARRA SOFTWARE SOLUTIONS");
  Serial.println("Arduino ist bereit alles zu tun");
  //matrix.begin(0x70);
  attachInterrupt(0, rising, RISING); //wenn pin 2 high ist rufe rising funktion auf
  /* ########################### Adafruit ######################## */
  // Seed random number generator from an unused analog input:
  randomSeed(analogRead(A0));

  // Initialize each matrix object:
  for(uint8_t i=0; i<4; i++) {
    matrix[i].begin(matrixAddr[i]);
    // If using 'small' (1.2") displays vs. 'mini' (0.8"), enable this:
    // matrix[i].setRotation(3);
}
}

static const uint8_t PROGMEM
happy_bmp[] =
{ 
B00000000,
B00000000,
B00000000,
B10000001,
B11000011,
B01111110,
B00111100,
B00000000
},
superhappy_bmp[] =
{ 
B00000000,
B10000001,
B10000001,
B10000001,
B11111111,
B01111110,
B00111100,
B00011000
},
thinking_bmp[] =
{ 
B11111111,
B11111111,
B01111110,
B00111100,
B00000000,
B00000000,
B00000000,
B00000000
},
sad_bmp[] =
{ 
B00000000,
B00000000,
B00000000,
B00000000,
B00010000,
B00111000,
B01100110,
B11000011 
},
flirty_bmp[] =
{ 
B00000000,
B01100110,
B11111111,
B11111111,
B01111110,
B00111100,
B00011000,
B00000000 
},
allon_bmp[] =
{ 
B11111111,
B11111111,
B11111111,
B11111111,
B11111111,
B11111111,
B11111111,
B11111111
},
alloff_bmp[] =
{ 
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000
},
default_bmp[] =
{ 
B00111100,
B01111110,
B11111111,
B11100111,
B11100111,
B11111111,
B01111110,
B00111100 
},
line_bmp[] = 
{ 
B00000000, 
B00000000, 
B00000000, 
B11111111, 
B11111111, 
B00000000, 
B00000000, 
B00000000
},
kreuz_bmp[] =
 { 
B10000001, 
B01000010, 
B00100100, 
B00011000, 
B00011000, 
B00100100, 
B01000010, 
B10000001
},
questionmark_bmp[] =
{ 
B00011000,
B00111100,
B01100110,
B00001100,
B00011000,
B00011000,
B00000000,
B00011000 
};//jedes mal wenn eine neue emotion dazu kommt muss das ; druch ein , ersetzt werden. Oder du fügst sie immer oberhalb ein
//hier kannst du die neuen emotion aus dem GUI einfügen.

void rising() {
  attachInterrupt(0, falling, FALLING);
  prev_time = micros();
}
 
void falling() {
  attachInterrupt(0, rising, RISING);
  pwm_value = micros()-prev_time;
}


void loop() {
  matrix[MATRIX_EYES].clear();
  Serial.println(pwm_value);
  
  if (pwm_value == 100)
  {
    Serial.println("Happy");
    Serial.println(pwm_value);
    matrix[MATRIX_EYES].drawBitmap(0, 0, happy_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  
  else if (pwm_value == 200)
  {
    Serial.println("Super Happy");
    Serial.println(pwm_value);
    matrix[MATRIX_EYES].drawBitmap(0, 0, superhappy_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  
  else if (pwm_value == 300)
  {
    Serial.println("Thinking");
    matrix[MATRIX_EYES].drawBitmap(0, 0, thinking_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  
  else if (pwm_value == 400)
  {
    Serial.println("Sad");
    matrix[MATRIX_EYES].drawBitmap(0, 0, sad_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  
  else if (pwm_value == 500)
  {
    Serial.println("Flirty");
    matrix[MATRIX_EYES].drawBitmap(0, 0, flirty_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  
  else if (pwm_value == 600)
  {
    Serial.println("All LEDs on");
    matrix[MATRIX_EYES].drawBitmap(0, 0, allon_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  
  else if (pwm_value == 700)
  {
    Serial.println("All LEDs off");
    matrix[MATRIX_EYES].drawBitmap(0, 0, alloff_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  
  else if (pwm_value == 800)
  {
    Serial.println("Eyes with pupil");
    matrix[MATRIX_EYES].drawBitmap(0, 0, default_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  else if (pwm_value == 900)
  {
    Serial.println("Questionmark");
    matrix[MATRIX_EYES].drawBitmap(0, 0, questionmark_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
  else if (pwm_value == 1000)
  {
      // Draw eyeball in current state of blinkyness (no pupil).  Note that
      // only one eye needs to be drawn.  Because the two eye matrices share
      // the same address, the same data will be received by both.
    matrix[MATRIX_EYES].clear();
      // When counting down to the next blink, show the eye in the fully-
      // open state.  On the last few counts (during the blink), look up
      // the corresponding bitmap index.
    matrix[MATRIX_EYES].drawBitmap(0, 0,
        blinkImg[
            (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
            blinkIndex[blinkCountdown] :            // Yes, look up bitmap #
            0                                       // No, show bitmap 0
        ], 8, 8, LED_ON);
      // Decrement blink counter.  At end, set random time for next blink.
    if(--blinkCountdown == 0) blinkCountdown = random(5, 180);

      // Add a pupil (2x2 black square) atop the blinky eyeball bitmap.
      // Periodically, the pupil moves to a new position...
    if(--gazeCountdown <= gazeFrames) {
        // Eyes are in motion - draw pupil at interim position
        matrix[MATRIX_EYES].fillRect(
            newX - (dX * gazeCountdown / gazeFrames),
            newY - (dY * gazeCountdown / gazeFrames),
            2, 2, LED_OFF);
        if(gazeCountdown == 0) {    // Last frame?
            eyeX = newX; eyeY = newY; // Yes.  What's new is old, then...
            do { // Pick random positions until one is within the eye circle
            newX = random(7); newY = random(7);
            dX   = newX - 3;  dY   = newY - 3;
          } while((dX * dX + dY * dY) >= 10);      // Thank you Pythagoras
          dX            = newX - eyeX;             // Horizontal distance to move
          dY            = newY - eyeY;             // Vertical distance to move
          gazeFrames    = random(3, 15);           // Duration of eye movement
          gazeCountdown = random(gazeFrames, 120); // Count to end of next movement
        }
    } else {
        // Not in motion yet -- draw pupil at current static position
    matrix[MATRIX_EYES].fillRect(eyeX, eyeY, 2, 2, LED_OFF);
    }

      // Draw mouth, switch to new random image periodically
    drawMouth(mouthImg[mouthPos]);
    if(--mouthCountdown == 0) {
        mouthPos = random(6); // Random image
        // If the 'neutral' position was chosen, there's a 1-in-5 chance we'll
        // select a longer hold time.  This gives the appearance of periodic
        // pauses in speech (e.g. between sentences, etc.).
    mouthCountdown = ((mouthPos == 0) && (random(5) == 0)) ?
        random(10, 40) : // Longer random duration
        random(2, 8);    // Shorter random duration
    }

      // Refresh all of the matrices in one quick pass
    for(uint8_t i=0; i<4; i++) matrix[i].writeDisplay();

    delay(20); // ~50 FPS
 } 
 //ab hier kannst du die neuen emotion aus dem GUI einfügen. ==>
 /* else if (pwm_value == 1100)
    {
    Serial.println("BEISPIEL");
    matrix[MATRIX_EYES].drawBitmap(0, 0, BEISPIEL_bmp, 8, 8, LED_ON);
    matrix[MATRIX_EYES].writeDisplay();
    delay(500);
  }
 */
   
  else if (pwm_value == 1100) 
  { 
    Serial.println("line"); 
    matrix[MATRIX_EYES].drawBitmap(0, 0, line_bmp, 8, 8, LED_ON); 
    matrix[MATRIX_EYES].writeDisplay(); 
    delay(500); 
   }
 else if (pwm_value == 1200) 
  { 
    Serial.println("kreuz"); 
    matrix[MATRIX_EYES].drawBitmap(0, 0, kreuz_bmp, 8, 8, LED_ON); 
    matrix[MATRIX_EYES].writeDisplay(); 
    delay(500); 
   }



} //Void loop ende hier nichts mehr ändern!
// Draw mouth image across three adjacent displays
void drawMouth(const uint8_t *img) {
  for(uint8_t i=0; i<3; i++) {
    matrix[MATRIX_MOUTH_LEFT + i].clear();
    matrix[MATRIX_MOUTH_LEFT + i].drawBitmap(i * -8, 0, img, 24, 8, LED_ON);
  }
}
