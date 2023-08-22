#define LED (1<<PD6)
#define POT (1<<PC1)

unsigned int leitura;
unsigned long int aux;

int main(){
  
  DDRD = LED;
  PORTD &= ~LED;

  ADCSRA = (1<<ADEN)+(1<<ADPS2)+(1<<ADPS1)+(1<<ADPS0);
  ADMUX = (1<<REFS0)+(1<<MUX0);
  DIDR0 |= POT;

  TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0A1);
  TCCR0B = (1<<CS02)|(1<<CS00);

  for(;;){

    ADCSRA |= (1<<ADSC);

    while(!(ADCSRA & (1<<ADIF)));

    leitura = ADC;

    aux = (unsigned long int)(leitura)*255;
    aux = (unsigned long int)(aux/1023);

    OCR0A = aux;

  }
}