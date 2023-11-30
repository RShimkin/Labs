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
const int memaddr = 1;
bool dir = false;
void ChangePower(bool direction, int& index);
unsigned char ReadEEPROM(unsigned int addr);
void WriteEEPROM(unsigned int addr, unsigned char data);
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

void ChangePower(bool dir, int &index) {
  int LEDsnum = sizeof(LED)/sizeof(*LED);
  out(&LED[index],1);
  if(dir) {
    index--;
    if (index==-1) index=LEDsnum-1;
  } else {
    index++;
    if (index==LEDsnum) index = 0;
  }
  out(&LED[index],0);
}

void WriteEEPROM(unsigned int addr, unsigned char data) {
  while(EECR & (1<<EEPE));
  EEAR = addr;
  EEDR = data;
  EECR |= (1<<EEMPE);
  EECR |= (1<<EEPE);
}

unsigned char ReadEEPROM(unsigned int addr) {
  while(EECR & (1<<EEPE));
  EEAR = addr;
  EECR |= (1<<EERE);
  return EEDR;
}

ISR(TIMER1_OVF_vect) {
  TCNT1 = 57823;                                
  ChangePower(dir, index);
}

void setup() {
  for (int i=0; i<4; i++) {
    mode(&LED[i], OUTPUT, 1);
  }
  dir = ReadEEPROM(memaddr) - '0';

  noInterrupts();
  TCCR1A = TCCR1B = 0;
  TCNT1 = 57823;
  TCCR1B |= (1 << CS10)|(1 << CS12);
  TIMSK1 |= (1 << TOIE1);
  interrupts(); 
}

void loop() {
  prev = cur;
  cur = !in(&BUT1);
  if (prev==1 && cur==0) pressed = true;

  if(pressed && millis()-timer>200) { 
    dir=!dir;
    if (dir) WriteEEPROM(memaddr,'1');
    else WriteEEPROM(memaddr,'0');
    pressed = false;
    timer = millis();
  }
}
