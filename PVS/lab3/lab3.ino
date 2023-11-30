// Переписать №1 с прерываниями, реализовать антидребезг кнопки

#define POT 0

int debounce=50;
int lag=0;

struct pinstr {
  uint8_t *ddr;
  uint8_t *port;
  uint8_t *port_in;
  uint8_t pin;
};

struct pinstr LATCH = { &DDRD, &PORTD, &PIND, 4 };
struct pinstr CLK = { &DDRD, &PORTD, &PIND, 7 };
struct pinstr DATA = { &DDRB, &PORTB, &PINB, 0 };
struct pinstr BUT1 = { &DDRC, &PORTC, &PINC, 1 };
struct pinstr LED[] = {
  { &DDRB, &PORTB, &PINB, 5 },
  { &DDRB, &PORTB, &PINB, 4 },
  { &DDRB, &PORTB, &PINB, 3 },
  { &DDRB, &PORTB, &PINB, 2 }
};
const byte SEGMENTS[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90},
           DIGITS[] = {0xF1,0xF2,0xF4,0xF8};
bool press=false, flag = false;
uint32_t btnTimer = 0;
bool state=true, but1pressed=false, LEDState=false;
unsigned long LEDTimer=0;

void mode(struct pinstr *pin, int mode, int val) {
  if(mode == INPUT) {
    *pin->ddr &= ~(1 << pin->pin);
  } else {
    *pin->ddr |= 1 << pin->pin;
    if(val) {
      *pin->port |= 1 << pin->pin;
    } else {
      *pin->port &= ~(1 << pin->pin);
    }
  }
}

void out(struct pinstr *pin, int val) {
  if(val) {
    *pin->port |= 1 << pin->pin;
  } else {
    *pin->port &= ~(1 << pin->pin);
  }
}

int in(struct pinstr *pin) {
  return (*pin->port_in >> pin->pin) & 1;
}

void shift(struct pinstr *data, struct pinstr *clk, uint8_t val) {
  for (uint8_t i = 0; i < 8; i++) {
    out(data, !!(val & (1 << (7 - i))));
    out(clk, 1);
    out(clk, 0);
  }
}

void scanBUT1(){
  bool btnState = !in(&BUT1);
  if (btnState && !flag && millis() - btnTimer > 100) {
    flag = true;
    btnTimer = millis();
  }
  if (!btnState && flag && millis() - btnTimer > 100) {
    flag = false;
    btnTimer = millis();
  }
}

void setup() {
  ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
  ADMUX |= (1<<REFS0);
  ADCSRB = (1<<ADTS2);
  ADCSRA |= (1<<ADEN);
 
  mode(&LATCH,OUTPUT, 0);
  mode(&CLK,OUTPUT, 0);
  mode(&DATA,OUTPUT, 0);

  for (int i=0; i<4; i++) {
    mode(&LED[i], OUTPUT, 0);
  }
}

void loop() {
  scanBUT1();
  if (flag) {
    if (!press) {
      but1pressed=!but1pressed;
    }
    press=true;
  } else {
    press=false;
  }
  
  int val=1;

  if (but1pressed) {
    val=0;
  } else {
   val=1; 
  }
  if (millis()-LEDTimer>250) {
    LEDState=!LEDState;
    LEDTimer=millis();
  }
  if (LEDState) {
    for (byte i =0;i<4;i++) {
      out(&LED[i], (i+val)%2);
    }
  } else {
    for (byte i =0;i<4;i++)
    {
      out(&LED[i], 1);
    }
  }
}
