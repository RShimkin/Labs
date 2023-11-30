//№5: постоянно моргать нечетными, по кнопке начать моргать чётными

#define POT 0

struct pinstr {
  uint8_t *ddr;
  uint8_t *port;
  uint8_t *port_in;
  uint8_t pin;
};

struct pinstr LOCK = { &DDRD, &PORTD, &PIND, 4 };
struct pinstr CLOCK = { &DDRD, &PORTD, &PIND, 7 };
struct pinstr DATA = { &DDRB, &PORTB, &PINB, 0 };
struct pinstr BUT1 = { &DDRC, &PORTC, &PINC, 1 };
struct pinstr LED[] = {
  { &DDRB, &PORTB, &PINB, 5 },
  { &DDRB, &PORTB, &PINB, 4 },
  { &DDRB, &PORTB, &PINB, 3 },
  { &DDRB, &PORTB, &PINB, 2 }
};
const int TIME = 200;
const byte DIGITS[] = {0xF1,0xF2,0xF4,0xF8};
const byte SEGMENTS[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
bool state=true;

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

int in(struct pinstr *pin) {
  return (*pin->port_in >> pin->pin) & 1;
}

void out(struct pinstr *pin, int val) {
  if (val) {
    *pin->port |= 1 << pin->pin;
  } else {
    *pin->port &= ~(1 << pin->pin);
  }
}

void shift(struct pinstr *dataPin, struct pinstr *clockPin, uint8_t val)
{
  for (uint8_t i = 0; i < 8; i++)  {
    out(dataPin, !!(val & (1 << (7 - i))));
    out(clockPin, 1);
    out(clockPin, 0);
  }
}

bool ButtonClick(struct pinstr *but, bool& state){
  if(!in(but)){
    while(in(but));
    state=!state;
    return true;
  }
  return false;
}

void setup()
{
  ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
  ADMUX |= (1<<REFS0);
  ADCSRB = (1<<ADTS2);
  ADCSRA |= (1<<ADEN);

  for (int i = 0; i < 4; i++) {
    mode(&LED[i], OUTPUT, 0);
  }
  mode(&LOCK,OUTPUT, 0);
  mode(&CLOCK,OUTPUT, 0);
  mode(&DATA,OUTPUT, 0);
}

void loop(){
  bool pressed1 = in(&BUT1);
  int val=1;
  if (pressed1){
    val=0;
  }
  ButtonClick(&BUT1, state);
  if (state){
    val=0;
  } else {
   val=1; 
  }
  for (byte i = 0; i < 4; i++){
    out(&LED[i], (i+val)%2);
  }
  delay(TIME);
  for(byte i = 0; i < 4; i++){
    out(&LED[i], 1);
  }
  delay(TIME);
}
