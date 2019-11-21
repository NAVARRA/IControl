# IControl
IControl Version 1.0 is a tool to create custom pictures / patterns on a Adafruit 8x8 LED Matrix

It was created for a Customer of Navarra Software Solutions, since we both believe in open source and sharing knowledge the Customer agreed on sharing this tool. (Thank you)

So far the tool is still in German but since its not very complex it should be fairly easy to understand.

Here is a little explanation on how it is supposed to be used.

The toolbar shows three buttons:
Datei = File -> Exit
*.Ino Datei = *.Ino File -> is used to select the Arduino file in our case “ArduinoProgrammfor2x8x8 Matrix.ino”
About -> Shows information about Navarra Software Solutions
The grey 8x8 Matrix on the left side represents the LEDs on the Adafruit 8x8 Matrix.
Since this is used to animate the eyes of a robot (Bob) you can hook up 2 Matrix to the Arduino and both will the same pattern. (That’s why its called (I)”Eye”Control)
When clicking on the grey squares the squares turn blue and the code for 8x8 Matrix will be created.
This Code can then be copied into the *.Ino File “ArduinoProgrammfor2x8x8 Matrix.ino”

The “_bmp” Value is the name of the pattern and also needs to be entered with a unique name.

_bmp_bmp[] =
 { 
B00000000, 
B00000000, 
B00000000, 
B00100000, 
B00001000, 
B00110000, 
B00100000, 
B00000000
},


The PWM Value is used to select the different patterns via a PWM signal from a pololu maestro servo control module (Or by any PWM Signal generated with whatever).


else if (pwm_value == 0) 
  { 
    Serial.println("_bmp"); 
    matrix[MATRIX_EYES].drawBitmap(0, 0, _bmp_bmp, 8, 8, LED_ON); 
    matrix[MATRIX_EYES].writeDisplay(); 
    delay(500); 
   }


So far the delay is a fix value in the code, you can of course change it manually but it works fine with 500. I might add another input-box in the next Version
delay(500);

When the Save button is clicked this matrix is converted to Arduino code.
The “Clear” button sets all squares back to 0 (grey).

Currently the following patterns are available:

100 = Glücklich = Happy
200 = Super Glücklich = Super Happy
300 = Denkt = Thinking
400 = Traurig = Unhappy
500 = Flirty
600 = alle LEDs an = All LEDs on
700 = alle LEDs aus = All LEDs off
800 = Augen mit Pupille = Eyes with pupils
900 = ? Frage = Questionsmark ?
1000 = bewegte augen = Moving Eyes (The movement of the eyes could have low framerate do to the unstable PWM signal, this might be updated in the next version)

1100 = linie = Single line
1200 = kreuz = X or Cross

Please feel free to fork this project to add features or simply translate it into any language.
Currently the GUI is “optimized” for Windows and might look different on Linux. If you have a good solutions to automatically adapt to the OS please DM me or oven Fork. (It is a known problem from TKinter)

I also included hook up instructions and links to the different needed parts.
As well as the Maestro settings file “maestro_settings.txt” The Matrix can be controlled via the Servo 6 output signal.

Adafruit 8x8 Matrix https://www.adafruit.com/product/959
Pololu Maestro      https://www.pololu.com/product/1350
Pololu Maestro IDE  https://www.pololu.com/docs/0J40/3.a
Arduino Uno         https://store.arduino.cc/arduino-uno-rev3
Arduino IDE         https://www.arduino.cc/en/Main/Software
Cable               https://store.arduino.cc/10-jumper-wires-150mm-male

You also need to install the following Libraries in your Arduino IDE
Adafruit_LED_Backpack
Adafruit_ILI9341
Adafruit-GFX-Library

If you like check out some pictures and videos on Instagram @contactnavarra
https://www.instagram.com/contactnavarra/?hl=de
Have fun...

