
// https://github.com/mmarchetti
#include <DirectIO.h>

#define OUTPIN 3
Output<OUTPIN> output;

unsigned long t[] = {
0
,932
,1784
,2716
,3564
,5384
,6240
,7168
,8020
,8952
,9800
,10732
,11580
,12512
,13364
,14296
,15148
,16076
,16928
,17860
,18712
,19644
,20488
,21424
,23160
,24096
};

#define N 26

int n;
volatile bool state;
volatile bool carrier_state;
unsigned long last_state_changed;
unsigned long state_duration;

void setup() {
  output.write(LOW);  
  
  noInterrupts();

  // Configure timer
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 210;// = (16*10^6) / (2 * 38e8) - 1 
  //OCR1A = 15624; 1hz with 1024 prescaler
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 bit for no prescaler
  TCCR1B |= (1 << CS10);  

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts();

  reset();
}

ISR(TIMER1_COMPA_vect) {
  if (!state) {
    carrier_state = LOW;
    output.write(LOW);
  } else {
    carrier_state = !carrier_state;
    output.write(carrier_state);
  }
}

void reset() {
  noInterrupts();
  n = 0;
  state = LOW;
  carrier_state = LOW;
  last_state_changed = 0;
  state_duration = 0;
  interrupts();
}

void loop() {  
  if (n == N) {
    //delay(100);
    //reset();
    return;
  }

  unsigned long now = micros();
  unsigned long elapsed = now - last_state_changed;

  if (elapsed >= state_duration) {
    state = !state;
    last_state_changed = now;
    if (n < N - 1) {
      state_duration = t[n + 1] - t[n];
    }
    
    ++n;    
  }
}
