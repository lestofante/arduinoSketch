int secondsBecauseOverflow=0;
static int SECONDS_PER_TIMER0_OVERFLOW = MICROSECONDS_PER_TIMER0_OVERFLOW / 1000; //defined in wiring.c
void setup(){
}

void loop(){
}

ISR(TIMER1_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  secondsBecauseOverflow += SECONDS_PER_TIMER0_OVERFLOW;
}

unsigned long getSeconds(){
  return millis()/1000 + secondsBecauseOverflow;
}
