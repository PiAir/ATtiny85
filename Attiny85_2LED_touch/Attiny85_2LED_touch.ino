// 2 LED blink code using capacitive touch
// 2019-01-26
//
// based on the code by high-low tech
// see: http://highlowtech.org/?p=1653
//
// Modifications by Pierre
// http://ictoblog.nl
//
// diagram:
//                           attiny85
//                       reset -+---+- power
// (fading always)         pb3 -+*  +- pb2 (touch input)
// (fading while touching) pb4 -+   +- pb1 (not used)
//                      ground -+---+- pb0 (not used)


int fadepin1 = 3; // the led that fades on and off
int fadepin2 = 4; // the led that fades on and off while you're touching the input pin
int touchpin = 2; // the touch input

int calibration = 0;

int randomval = 0;
int fadeval = 0, fadestep = 1;
int togglestate = LOW;

void setup()
{
  pinMode(fadepin1, OUTPUT);
  pinMode(fadepin2, OUTPUT);
  
  delay(100);
  for (int i = 0; i < 8; i++) {
    calibration += chargeTime(touchpin);
    delay(20);
  }
  calibration = (calibration + 4) / 8;
}

void loop()
{
  int n = chargeTime(touchpin);
  
  
  analogWrite(fadepin1, fadeval);
  if (n > calibration) analogWrite(fadepin2, fadeval);
  else analogWrite(fadepin2, 0);
  fadeval = fadeval + fadestep;
  if (fadeval == 255) fadestep = -1;
  if (fadeval == 0) fadestep = 1;
  
  delayMicroseconds(500);
}

byte chargeTime(byte pin)
{
  byte mask = (1 << pin);
  byte i;

  DDRB &= ~mask; // input
  PORTB |= mask; // pull-up on

  for (i = 0; i < 16; i++) {
    if (PINB & mask) break;
  }

  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge

  return i;
}
