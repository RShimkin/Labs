#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

/*
#define PAUSE2 2000
#define INTSTART 100
#define INTEND -100
*/


const uint8_t HELLO[] = {
  0x00,
  0x00,
  0x00,
  SEG_B | SEG_C | SEG_E| SEG_F | SEG_G,
  SEG_A | SEG_D | SEG_E| SEG_F | SEG_G,
  SEG_D | SEG_E | SEG_F,
  SEG_D | SEG_E | SEG_F,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
  0x00, 
  0x54,//d
  0x5c,//d
  0x5e,//d
  0x79,//f
  0x00, 
  0x00,
  0x00,
  0x00
};

const uint8_t MINUS[] = {
  SEG_G
};

//const uint8_t SIGN[] = { SEG_G };

TM1637Display display(CLK, DIO);

void setup()
{
  display.setBrightness(0x0f);
}

void loop()
{
  
  int i,k;
  double f;

  /*
  // 1 задание
  for (i = 100; i >= -100; i-=20) {
    display.showNumberDec(i,false);
    delay(2000);
  }
  delay(3000);
  
  // 2 задание
  for (f = -3; f <= 3; f += 0.25) {
    display.clear();
    i = (int) (f*100);
    if (f >= 0) {
      display.showNumberDecEx(i, 0x80, true, 3, 1);
    }
    else {
      display.showNumberDecEx(i,0x40,false);
      if (f >= -1 && f < 0) {
        display.setSegments(MINUS,1,0);
        display.showNumberDecEx(-i, 0x80, true, 3,1);
      }
    }
    delay(1000);
  }
  delay(3000);
  */
  
  // 3 задание
  for (i = 0; ; i++) {
    i %= 14;
    display.setSegments(HELLO+i, 4, 0);
    delay(1000);
  }

}
