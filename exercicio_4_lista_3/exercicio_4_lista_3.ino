#define MOTOR (1<<PD6)
#define LIGA (1<<PB1)
#define DESLIGA (1<<PB2)

int contagem = 0;
float aux = 0;

int main(){
  
  //--------------------------------------------------------------------------------------------
  DDRD = MOTOR;
  PORTD &= ~MOTOR;
  PORTB |= DESLIGA;

  //--------------------------------------------------------------------------------------------
  TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0A1);
  TCCR0B = (1<<CS02)|(1<<CS00); //1024
	
  //--------------------------------------------------------------------------------------------
  TCCR2A = (1<<WGM21);  //ctc
  TCCR2B = (1<<CS21);   //prescaler 8
  OCR2A = 0b11000111; //199  100us
  
  //--------------------------------------------------------------------------------------------
  PCICR = (1<<PCIE0);
  PCMSK0 = LIGA+DESLIGA;

  //--------------------------------------------------------------------------------------------
  sei();
  
  //--------------------------------------------------------------------------------------------
  for(;;){
  }
  //--------------------------------------------------------------------------------------------
}

//Interrupção do timer 2------------------------------------------------------------------------
ISR(TIMER2_COMPA_vect){
	contagem++;
  if(contagem == 10000){
    aux += (255/8.0);
    if(aux > 255){
      //aux = 0;
      TIMSK2 &= ~(1<<OCIE2A);
    }
    else 
      OCR0A = int(aux);
    contagem=0;
  }
}

//Interrupção PCINT Portal B--------------------------------------------------------------------
ISR(PCINT0_vect){
  if((PINB & LIGA) == LIGA){
    TIMSK2 |= (1<<OCIE2A);
    //aux = (255/8);
    aux = 0;
    OCR0A = int(aux);
    contagem = 0;
  }
  if((PINB & DESLIGA) != DESLIGA){
    TIMSK2 &= ~(1<<OCIE2A);
    OCR0A = 0;
  }
}