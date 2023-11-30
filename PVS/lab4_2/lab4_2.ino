// Часть 2: написать управление яркостью светодиодов Led1..Led8 с использованием модуля ШИМ микроконтроллера. Нажатие на кнопку увеличивает яркость на 20% циклично. 
// Попробовать с разной частотой ШИМ.

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

struct pinstr {
  uint8_t *ddr;
  uint8_t *port;
  uint8_t *port_in;
  uint8_t pin;
};

struct pinstr LED[] = {
  { &DDRB, &PORTB, &PINB, 5 },
  { &DDRB, &PORTB, &PINB, 4 },
  { &DDRB, &PORTB, &PINB, 3 },
  { &DDRB, &PORTB, &PINB, 2 }
};
struct pinstr BUT1 = { &DDRC, &PORTC, &PINC, 1 };
int index=0;
volatile byte power = 0;
void ChangePower(bool direction, int& index);
uint32_t timer = 0;
bool pressed = false, prev = false, cur = false;

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

void aout1(int val){
  sbi(TCCR1A, COM2B1);
  OCR1B = val;
}

void aout2(int val){
  sbi(TCCR2A, COM2B1);
  sbi(TCCR2B, CS22);
  OCR2A = val;
}

void setup() {
  for (int i=0; i<4; i++) {
    mode(&LED[i], OUTPUT, 1);
  }
  mode(&BUT1, INPUT, 0);
}

void loop() {
  aout1(power);
  prev = cur;
  cur = !in(&BUT1);
  if (prev==1 && cur==0)pressed = true;

  if(pressed && millis()-timer>200)
  { 
    if(power==252) power=0;
    else power+=42;
    pressed = false;
    timer = millis();
  }
}
