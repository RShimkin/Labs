#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>
//#include <DHT_U.h>

#define CLK 51
#define DIO 50
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
TM1637Display display(CLK, DIO);

const uint8_t FAR[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,
  SEG_A | SEG_E | SEG_F | SEG_G
};

const uint8_t HUM[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,
  SEG_C | SEG_D | SEG_E | SEG_G
};

void setup() {
  display.setBrightness(0x0f);
  dht.begin();
}

int h,f;

void loop() {

  h = (int) dht.readHumidity();
  f = (int) dht.readTemperature(true);

  display.showNumberDec(f,false,2,0);
  display.setSegments(FAR,2,2);
  delay(1000);
  display.showNumberDec(h,false,2,0);
  display.setSegments(HUM,2,2);
  delay(2000);

}
