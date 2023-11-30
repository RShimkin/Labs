// Считывать показания переменного резистора через АЦП, отображать уровень сигнала на светодиоде

#define POT 0

struct pinstr {
  uint8_t *ddr;
  uint8_t *port;
  uint8_t *port_in;
  uint8_t pin;
};

struct pinstr LOCK = { &DDRD, &PORTD, &PIND, 4 };
struct pinstr CLK = { &DDRD, &PORTD, &PIND, 7 };
struct pinstr DATA = { &DDRB, &PORTB, &PINB, 0 };
struct pinstr BUT1 = { &DDRC, &PORTC, &PINC, 1 };
struct pinstr LED[] = {
  { &DDRB, &PORTB, &PINB, 5 },
  { &DDRB, &PORTB, &PINB, 4 },
  { &DDRB, &PORTB, &PINB, 3 },
  { &DDRB, &PORTB, &PINB, 2 }
};
const byte SEGMENTS[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
const byte DIGITS[] = {0xF1,0xF2,0xF4,0xF8};
const int TIME = 5;

void mode(struct pinstr *pin, int mode, int val) {
  if (mode == INPUT) {
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

void shift(struct pinstr *data, struct pinstr *clk, uint8_t val)
{
  for (uint8_t i = 0; i < 8; i++)  {
    out(data, !!(val & (1 << (7 - i))));
    out(clk, 1);
    out(clk, 0);
  }
}

int ain(int pin) {
  ADMUX = (ADMUX & 0xF0) | (pin & 0x0F);
  ADCSRA |= 1 << ADSC;
  while(ADCSRA & (1 << ADSC));
  return ADC;
}

void WriteDigit(byte Segment, byte Value)
{
  out(&LOCK, 0);
  shift(&DATA, &CLK, SEGMENTS[Value]);
  shift(&DATA, &CLK, DIGITS[Segment] );
  out(&LOCK, 1);
}

void setup()
{
  ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));  // 125Кгц
  ADMUX |= (1<<REFS0);
  ADCSRB = (1<<ADTS2);
  ADCSRA |= (1<<ADEN);
 
  mode(&LOCK,OUTPUT, 0);
  mode(&CLK,OUTPUT, 0);
  mode(&DATA,OUTPUT, 0);

  for (int i=0; i<4; i++) {
    mode(&LED[i], OUTPUT, 1);
  }
}

void loop(){
  int potval;
  potval = ain(POT);
  WriteDigit(0, potval / 1000);
  delay(TIME);
  WriteDigit(1, (potval / 100) % 10);
  delay(TIME);
  WriteDigit(2, (potval / 10) % 10);
  delay(TIME);
  WriteDigit(3, potval % 10);
  delay(TIME);
  for (byte i=0; i<4; i++){
    if (potval > (1024/(i+2)))
      out(&LED[i], 0);
    else
      out(&LED[i], 1);
  }
}
