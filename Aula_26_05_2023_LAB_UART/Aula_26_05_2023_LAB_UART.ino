#define BOTAO (1<<PB1)
#define LEDR (1<<PB2)
#define LEDG (1<<PB3)
#define LEDPWM (1<<PD6)

int contagem=0;
short int horas=0;
short int minutos=0;
short int segundos=0;

// Variáveis para entrada e saída
char RX_buffer[32];
char RX_index = 0;

// Buffer para estado anterior do RX
char old_rx_hs[32];

// A inicialização do UART consiste em definir a taxa de transmissão,
// definir o formato de quadro, e ativar o Transmissor ou o receptor.

//Variável para armazenar número de vezes q o botao for precisonado
int press = 0;
char msg_tx[20];

//Variável para armazenar o valor de entrada
int aux = 0;

//Variável para armazenar o valor do pwm
float intensidade;
bool muda = false;

// Função de configuração do UART
void UART_init(int baud)
{
    // Calcula a taxa de transmissão
    int MYUBRR = 16000000 / 16 / baud - 1;

    // Definindo a taxa de transmissão
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)(MYUBRR);

    // Definindo o formato de quadro, 8 bits e 1 stop bit
    UCSR0C = (0 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);

    // Ativa o Transmissor, receptor e define a interrupção
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}

// Função para envio de dados via UART
void UART_send(char *TX_buffer)
{
    // Enquanto tiver caracteres para enviar
    while (*TX_buffer != 0)
    {
        // Prepara o buffer para o envio
        UDR0 = *TX_buffer;

        // Espera o envio ser completado
        while (!(UCSR0A & (1 << UDRE0))){};

        // Avança o ponteiro do buffer
        TX_buffer++;
    }
}

// Limpa o buffer de entrada e saída
void limpa_RX_buffer(void)
{
    unsigned char dummy;

    // Enquanto houver dados no buffer
    while (UCSR0A & (1 << RXC0))
    {
        // Lê o dado
        dummy = UDR0;
    }

    // Reseta o índice
    RX_index = 0;

    // Limpa todos os dados do buffer
    for (int i = 0; i < 32; i++)
    {
        old_rx_hs[i] = RX_buffer[i];
        RX_buffer[i] = 0;
    }
}

// Função ISR que salva um array de dados recebidos via UART
ISR(USART_RX_vect)
{
    // Salva o dado recebido
    RX_buffer[RX_index] = UDR0;
    RX_buffer[RX_index +1] = 0;

    // Adiciona mais 1 na contagem
    RX_index++;

    muda = true;
}

int main()
{   
    // Configuração do UART
    UART_init(9600);

    // Habilitando a interrupção
    sei();
    
    //Configuração das saídas
    /*DDRD = LEDPWM;
    DDRB = LEDR+LEDG;
    PORTB &= ~(LEDR+LEDG);
    PORTD &= ~LEDPWM;*/

    //BOTAO com pull-up
    /*PORTB |= BOTAO;*/

    //Configura a interrupção externa do botao
    /*PCICR = (1<<PCIE0);
    PCMSK0 = BOTAO;*/

    //Configuração PWM PD6
    /*TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0A1);
    TCCR0B = (1<<CS02)|(1<<CS00);*/

    //Configuração do Timer
    TCCR2A = (1<<WGM21);  //ctc
    TCCR2B = (1<<CS21);   //prescaler 8
    OCR2A = 0b11000111; //199  100us

    // Super Loop
    while (1)
    { 
      //UART_send("\n");
    }
}

ISR(TIMER2_COMPA_vect){
  
  contagem++;
  if(contagem == 10000){
    contagem = 0;
    segundos++;
    if(segundos == 60){
      segundos = 0;
      minutos++;
      if(minutos == 60){
        minutos = 0;
        horas++;
        if(horas == 24)
          horas = 0;
      }
    }
    itoa(horas,msg_tx,10);
    UART_send(msg_tx);
    UART_send(":");
    itoa(minutos,msg_tx,10);
    UART_send(msg_tx);
    UART_send(":");
    itoa(segundos,msg_tx,10);
    UART_send(msg_tx);
    UART_send("\n");
  }
}
