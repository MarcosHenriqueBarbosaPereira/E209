#define PINO 1
#define AMOSTRAS 50

unsigned int Leitura_AD;
float temp;

int main(){

	Serial.begin(115200);
	
	ADMUX = (0 << REFS1) + (1 << REFS0); //Utiliza 5V como referência (1023)
	
	ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0); //Habilita ADC e PS 128 (10 bits)
	
	ADCSRB = 0; //Conversão Única
	
	DIDR0 = (1 << PINO); //Desabilita o PC1 como pino digital - Não obrigatório
	
	for(;;){
	
		//Determinar o pino de leitura
		ADMUX = (ADMUX & 0xF8) | PINO;

		//Leitura do ADC (Com média)
		unsigned long int SomaLeitura = 0, MediaLeitura;
		for(int i=0; i < AMOSTRAS; i++){
		
			ADCSRA |= (1 << ADSC); //Inicia a conversão
		
			while((ADCSRA & (1<<ADSC)) == (1<<ADSC)); //Esperar a conversão
		
			Leitura_AD = ADC;
			
			SomaLeitura += Leitura_AD;
		}
		
		MediaLeitura = SomaLeitura / AMOSTRAS;

    if(MediaLeitura > (1023/5.0)){
		
		temp = (((float)(MediaLeitura)-(1023/5.0)) * 200) / 818.4; //Cálculo da Tensão
		
		Serial.print(temp);
    Serial.println(" graus Celsius;");
    }
    else Serial.println("Termômetro com defeito!!");
	}
	
}