#define PINO0 0
#define PINO5 5
#define AMOSTRAS 50

unsigned int Leitura_AD;
float temp;
float second;

int main(){

	Serial.begin(115200);
	
	ADMUX = (0 << REFS1) + (1 << REFS0); //Utiliza 5V como referência (1023)
	
	ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0); //Habilita ADC e PS 128 (10 bits)
	
	ADCSRB = 0; //Conversão Única
	
	DIDR0 = (1 << PINO0)|(1 << PINO5); //Desabilita o PC1 como pino digital - Não obrigatório
	
	for(;;){
	
		//Leitura pino 0
		ADMUX = (ADMUX & 0xF8) | PINO0;

		//Leitura do ADC (Com média)
		unsigned long int SomaLeitura = 0, MediaLeitura;
		for(int i=0; i < AMOSTRAS; i++){
		
			ADCSRA |= (1 << ADSC); //Inicia a conversão
		
			while((ADCSRA & (1<<ADSC)) == (1<<ADSC)); //Esperar a conversão
		
			Leitura_AD = ADC;
			
			SomaLeitura += Leitura_AD;
		}
		
		MediaLeitura = SomaLeitura / AMOSTRAS;
		
		temp = (MediaLeitura * 5) / 1023.0; //Cálculo da Tensão

    //Leitura pino 5
    ADMUX = (ADMUX & 0xF8) | PINO5;
    SomaLeitura = 0;

		//Leitura do ADC (Com média)
		for(int i=0; i < AMOSTRAS; i++){
		
			ADCSRA |= (1 << ADSC); //Inicia a conversão
		
			while((ADCSRA & (1<<ADSC)) == (1<<ADSC)); //Esperar a conversão
		
			Leitura_AD = ADC;
			
			SomaLeitura += Leitura_AD;
		}
		
		MediaLeitura = SomaLeitura / AMOSTRAS;
		
		second = (MediaLeitura * 5) / 1023.0; //Cálculo da Tensão
		
		Serial.print("Tensao pino 0: ");
    Serial.print(temp);
    Serial.print("Tensao pino 5: ");
    Serial.print(second);
    
	}
	
}