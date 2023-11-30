// Часть 1: написать управление яркостью светодиодов Led1..Led8 без использования аппаратного ШИМ. Нажатие на кнопку увеличивает коэфф. заполнения (яркость) на 20% циклично

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
struct pinstr PWM = { &DDRB, &PORTB, &PINB, 7 };
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

ISR(TIMER2_OVF_vect)                    
{
  TCNT2 = 255;                                
  pwmTick();
}

void pwmTick(){
  static volatile byte k = 0;
  if (k == 0) {
    if (power > 0 ){
      for (int i=0; i<4; i++) {
        out(&LED[i],0);
      }
    }
  }
  if (k == power) {
    for (int i=0; i<4; i++) {
      out(&LED[i],1);
    }
  }
  k++;
}

void setup()
{
  for (int i=0; i<4; i++) {
    mode(&LED[i], OUTPUT, 1);
  }
  mode(&BUT1, INPUT, 0);
  mode(&PWM, OUTPUT, 0);

  noInterrupts();
  TCCR2A = TCCR2B = 0;
  TCNT2 = 255; 
  TCCR2B |= (1 << CS22); // коэф деления предделителя
  TIMSK2 |= (1<<TOIE2); // разрешить вызов процедуры обработки прерывания переполнения счетчика
  interrupts();
}

void loop() {
  prev = cur;
  cur = !in(&BUT1);
  if (prev==1 && cur==0) pressed = true;

  if (pressed && millis()-timer>200) { 
    if (power == 252) power=0;
    else power += 42;
    pressed = false;
    timer = millis();
  }
}
