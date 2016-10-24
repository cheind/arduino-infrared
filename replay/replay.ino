
// https://github.com/mmarchetti
#include <DirectIO.h>

#define OUTPIN 3
Output<OUTPIN> output;

unsigned long t[] = {
0
,936
,1784
,2716
,3568
,5392
,6240
,7172
,8024
,8952
,9804
,10736
,11584
,12516
,13368
,14296
,15148
,16080
,17820
,18752
,19600
,21424
,22272
,23204
};

#define N 24

int n = 0;
volatile bool state = LOW;
volatile bool carrier_state = LOW;

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
  //TCCR1B |= (1 << CS12) | (1 << CS10); 
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  n = 0;
  state = LOW;
  carrier_state = LOW;

  interrupts();
}

ISR(TIMER1_COMPA_vect) {
  if (!state) {
    output.write(LOW);
    carrier_state = LOW;
  } else {
    carrier_state = !carrier_state;
    output.write(carrier_state);
  }
}

void loop() {  
  if (n == N) return;
  
  state = !state;
  if (n < (N-1)) {
    delayMicroseconds(t[n + 1] - t[n]);      
  }
  ++n;  
}
