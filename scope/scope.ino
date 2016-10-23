
#include <avr/interrupt.h>
#include <avr/io.h>


#define NCAPTURES 128
#define INPIN 13

unsigned long t[NCAPTURES];
uint8_t initialState = HIGH;
uint8_t state = HIGH;
int32_t n = -1;

void setup() {
  pinMode(INPIN, INPUT);
  Serial.begin(9600);
  while (!Serial);

  reset();
}

void reset() {
  Serial.println("log: resetting capture state.");
  n = -1;
}

void printResults() {
  unsigned long start = t[0];
  
  Serial.print("data: ");
  for (int i = 0; i < NCAPTURES; ++i) {
    Serial.print((unsigned long)(t[i] - start));
    if (i != NCAPTURES - 1)
      Serial.print(" ");
  }
  Serial.println();
}

void loop() {

  if (n == -1) {
    state = initialState;
    n = 0;
  } else if (n == NCAPTURES) {
    printResults();
    reset();
  } else {
    uint8_t v = digitalRead(INPIN);
    if (v != state) {
      t[n++] = micros();
      state = v;
    }
  }

}
