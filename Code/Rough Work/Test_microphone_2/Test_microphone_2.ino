// FAILURE! 

//clipping indicator variables
boolean clipping = 0;

//data storage variable
byte newData = 0;
byte prevData = 0;

//freq variables
unsigned int timer = 0; // counts period of wave
unsigned int period;
int frequency;

void setup() {
  Serial.begin (9600);
  cli(); // disable interrupts

  // set up continuous sampling of analog pin 0

  // clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;

  ADMUX |= (1 << REFS0); // set reference voltage
  ADMUX |= (1 << ADLAR); // left align the ADC value- so we can read highest 8 bits from ACH register only

  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); // set ADC clock with 32 prescaler - 16 mHz/ 32 = 500 kHz
  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADIE); // enable interrupts when measurement complete
  ADCSRA  |= (1 << ADSC); // start ADC measurements
  
  sei(); // enable interrupts
}

ISR (ADC_vect) { // when new AD value ready
  prevData = newData; // store previous value
  newData = ADCH; // get value from A0
  if (prevData < 127 && newData >= 127) { // if increasing and 
    period = timer; // get periods
    timer = 0; // reset timer
  }

  if (newData == 0 || newData == 1023) { // if clipping
    clipping = 1;//currently clipping
  }

  timer++;
}

void loop() {
  if (clipping) {//if currently clipping
    clipping = 0;
  }

  frequency = 38462/period;//timer rate/period
  //print results
  Serial.print(frequency);
  Serial.println(" hz");
  
  delay(100);
}
