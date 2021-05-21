
#include "Respirator.h"

// Variaveis auxiliares
unsigned long int tempoant[4]= {0,0,0,0};
unsigned long int contt[2]= {0,0};
int cont = 0,c;
char dat[9]="FFFFFFFFF";
char datT[9]="HHHXLLL";
char datE[9] = " ERROR ";

// Variaveis de selecao
int sel = 0;

// Variaveis de Resgistro Dados
long int bpm = 0;
int Vol = 1, IE=1, EI=1;
int AberturaVal = 0;
int FreqRespiracao = 5;
int ArmFreq =5;
int temp = 30;
int SpO2 = 0;
uint16_t leitura_ADC[2] = {0,0};

////////////////////////////-----Comunicacao----////////////////////////////////////////

void USART_Init(unsigned int ubrr){
	
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<USBS0)|(1<<UCSZ00);
	
	
}

void Test_Pacote_USART(){
	
	while(!(UCSR0A & (1<<UDRE0)));
	int a = 0;
	for (int i = 1; i<=3; i++){
		
		if((dat[i] - '0')<10 && (dat[i+4] - '0')<10)
		a++;

	}
	
	if(dat[0] == ';' && dat[8] == ':'&& dat[4] == 'X'&& a==3){
		for(int i = 1; i<=7; i++)
		datT[i-1] = dat[i];
	}
	else{
		cont = 0;
		for(int i = 1; i<=7; i++)
		datT[i-1] = datE[i];
		
		
	}
}

unsigned char USART_Receive(){
	while(!(UCSR0A & (1<<RXC0)));
	dat[cont] = UDR0;
	
	if(cont == 8){
		Test_Pacote_USART();
		cont = 0;
	}
	else
	cont++;
}

void USART_Transmit(unsigned char data)
{
	while(!( UCSR0A & (1<<UDRE0)));//Espera a limpeza do registr. de transmiss?o
	UDR0 = data; //Coloca o dado no registrador e o envia
}

ISR(USART_RX_vect){

	
	USART_Transmit(USART_Receive());

}

////////////////////////////-----TIMER----////////////////////////////////////////
ISR(TIMER0_COMPA_vect){
	
	tempo_ms+=1;
}

////////////////////////////-----Funcoes----////////////////////////////////////////

void monitor(){
	static char FreqConv[3];
	static char BPMconv[4];
	static char AberturaValConv[4];
	static char Tempconv[5];
	static char SpO2conv[5];
	static char ConvVol[2];
	static char ConvIE[4];
	
	int temp2=temp;
	// -----Conversoes----
	FreqConv[1] = (FreqRespiracao%10)+'0';//Convers?o da vari?vel FreqRespiracao em char
	FreqConv[0] = (FreqRespiracao/10)+'0';
	SpO2conv[2] = ((SpO2%100)%10)+'0';
	SpO2conv[1] = ((SpO2%100)/10)+'0';//Convers?o da vari?vel FreqRespiracao em char
	SpO2conv[0] = (SpO2/100)+'0';
	Tempconv[3] =((temp2%100)%10)+'0';
	Tempconv[2] ='.';
	Tempconv[1] =((temp2%100)/10)+'0';//Convers?o da vari?vel FreqRespiracao em char
	Tempconv[0] = ((temp2/100))+'0';
	BPMconv[2] = ((bpm%100)%10)+'0';//Convers?o da vari?vel FreqRespiracao em char
	BPMconv[1] = ((bpm%100)/10)+'0';//Convers?o da vari?vel FreqRespiracao em char
	BPMconv[0] = (bpm/100)+'0';
	AberturaValConv[2] = ((AberturaVal%100)%10)+'0';//Convers?o da vari?vel FreqRespiracao em char
	AberturaValConv[1] = ((AberturaVal%100)/10)+'0';//Convers?o da vari?vel FreqRespiracao em char
	AberturaValConv[0] = (AberturaVal/100)+'0';
	ConvVol[0] = (Vol)+'0';
	ConvIE[0] = IE+'0';
	ConvIE[1] = ':';
	ConvIE[2] = EI+'0';
	// -----Aplicacoes----
	// Modo Dados//
	if(sel == 0){
		nokia_lcd_clear();
		nokia_lcd_set_cursor(0,0);
		nokia_lcd_write_string("SINAIS VITAIS", 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(0,8);
		nokia_lcd_write_string("--------------", 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(0,12);
		nokia_lcd_write_string(BPMconv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,12);
		nokia_lcd_write_string("bpm", 1);
		nokia_lcd_set_cursor(0,21);
		nokia_lcd_write_string(SpO2conv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,21);
		nokia_lcd_write_string("%SpO2", 1);
		nokia_lcd_set_cursor(0,30);
		nokia_lcd_write_string(Tempconv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,30);
		nokia_lcd_write_string("C", 1);
		nokia_lcd_set_cursor(0,40);
		nokia_lcd_write_string(datT, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(50,40);
		nokia_lcd_write_string("mmHg", 1);
	}
	// Modo Alteracao de Frequencia//
	else if(sel == 2){		
		nokia_lcd_clear();
		nokia_lcd_set_cursor(0,0);
		nokia_lcd_write_string("  PARAMETROS  ", 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(0,7);
		nokia_lcd_write_string("--------------", 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(0,11);
		nokia_lcd_write_string(FreqConv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,11);
		nokia_lcd_write_string("*resp/Min", 1);
		nokia_lcd_set_cursor(0,20);
		nokia_lcd_write_string(AberturaValConv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,20);
		nokia_lcd_write_string("%O2", 1);
		nokia_lcd_set_cursor(0,30);
		nokia_lcd_write_string(ConvVol, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,30);
		nokia_lcd_write_string("vol", 1);
		nokia_lcd_set_cursor(0,40);
		nokia_lcd_write_string(ConvIE, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,40);
		nokia_lcd_write_string("IE", 1);
	}
	// Modo Alteracao da Oxigenacao//
	else if(sel == 4){
		nokia_lcd_clear();
		nokia_lcd_set_cursor(0,0);
		nokia_lcd_write_string("  PARAMETROS  ", 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(0,7);
		nokia_lcd_write_string("--------------", 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(0,11);
		nokia_lcd_write_string(FreqConv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,11);
		nokia_lcd_write_string("resp/Min", 1);
		nokia_lcd_set_cursor(0,20);
		nokia_lcd_write_string(AberturaValConv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,20);
		nokia_lcd_write_string("*%O2", 1);
		nokia_lcd_set_cursor(0,30);
		nokia_lcd_write_string(ConvVol, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,30);
		nokia_lcd_write_string("vol", 1);
		nokia_lcd_set_cursor(0,40);
		nokia_lcd_write_string(ConvIE, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,40);
		nokia_lcd_write_string("IE", 1);
	}
	// Modo Alteracao Volume de Oxigenacao//
	else if(sel == 6){
		nokia_lcd_clear();
		nokia_lcd_set_cursor(0,0);
		nokia_lcd_write_string("  PARAMETROS  ", 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(0,7);
		nokia_lcd_write_string("--------------", 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(0,11);
		nokia_lcd_write_string(FreqConv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,11);
		nokia_lcd_write_string("resp/Min", 1);
		nokia_lcd_set_cursor(0,20);
		nokia_lcd_write_string(AberturaValConv, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,20);
		nokia_lcd_write_string("%O2", 1);
		nokia_lcd_set_cursor(0,30);
		nokia_lcd_write_string(ConvVol, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,30);
		nokia_lcd_write_string("*vol", 1);
		nokia_lcd_set_cursor(0,40);
		nokia_lcd_write_string(ConvIE, 1);//Exibi??o da Frequ?ncia no LCD
		nokia_lcd_set_cursor(35,40);
		nokia_lcd_write_string("IE", 1);
	}
	else if(sel ==8){
			nokia_lcd_clear();
			nokia_lcd_set_cursor(0,0);
			nokia_lcd_write_string("  PARAMETROS  ", 1);//Exibi??o da Frequ?ncia no LCD
			nokia_lcd_set_cursor(0,7);
			nokia_lcd_write_string("--------------", 1);//Exibi??o da Frequ?ncia no LCD
			nokia_lcd_set_cursor(0,11);
			nokia_lcd_write_string(FreqConv, 1);//Exibi??o da Frequ?ncia no LCD
			nokia_lcd_set_cursor(35,11);
			nokia_lcd_write_string("resp/Min", 1);
			nokia_lcd_set_cursor(0,20);
			nokia_lcd_write_string(AberturaValConv, 1);//Exibi??o da Frequ?ncia no LCD
			nokia_lcd_set_cursor(35,20);
			nokia_lcd_write_string("%O2", 1);
			nokia_lcd_set_cursor(0,30);
			nokia_lcd_write_string(ConvVol, 1);//Exibi??o da Frequ?ncia no LCD
			nokia_lcd_set_cursor(35,30);
			nokia_lcd_write_string("vol", 1);
			nokia_lcd_set_cursor(0,40);
			nokia_lcd_write_string(ConvIE, 1);//Exibi??o da Frequ?ncia no LCD
			nokia_lcd_set_cursor(35,40);
			nokia_lcd_write_string("*IE", 1);
		}
	else if(sel == 10){ // Ajuste de Cama
			nokia_lcd_clear();
			nokia_lcd_set_cursor(0,0);
			nokia_lcd_write_string("  PARAMETROS  ", 1);//Exibi??o da Frequ?ncia no LCD
			nokia_lcd_set_cursor(0,7);
			nokia_lcd_write_string("--------------", 1);//Exibi??o da Frequ?ncia no LCD
			nokia_lcd_set_cursor(0,11);
			nokia_lcd_write_string("   Alt. Cama", 1);
			if(c==1){ 
				
				nokia_lcd_set_cursor(0,22);
				nokia_lcd_write_string("   UP!   ", 2);//Exibi??o da Frequ?ncia no LCD
				
			}
			else if(c==2){ 
				nokia_lcd_set_cursor(0,22);
				nokia_lcd_write_string(" DONW!   ", 2);//Exibi??o da Frequ?ncia no LCD
				
			}
			else{}
			
		}
	
	nokia_lcd_render();
}

void AplicFreqLed (){
	uint8_t static i = 0;
	double static td = 0;
	td=((60.0/(FreqRespiracao))/(2*Q_LEDS))*1000;//Realiza o calculo de quanto tempo cada Led ter? que ficar aceso
	if((tempo_ms-tempoant[1]) >= td){
		i = ((i>Q_INT)|(i<1))? (i=0) : i; //Testa o i para que recome?e o periodo
		PORTB = i>=Q_LEDS ? (PORTB &(~(1<<(Q_INT-i)))):(PORTB|(1<<i));//Acender e apaga os leds dependendo de que ciclo se encontra
		i++;
		tempoant[1]=tempo_ms;
	}
	
}

void AplicFreqServo (){
	float static   aux = 1, angle=0, i = 0;
	double static td = 0;
	
	td=((60.0/(FreqRespiracao))/(2*(Vol+1)))*1000;//Realiza o calculo de quanto tempo cada posicao do servo deve permanecer
	if((tempo_ms-tempoant[1]) >= (2*aux*td/(EI+IE))){//Tempo de mudança de estado agora depende da proporção I:E
		if(i>=45*(Vol))//Teste de retorno de ciclo
			 i=0;
		if(i>=22.5*(Vol))//Mudança de ciclo (Expiração)
			 angle = 45*(Vol) - i, aux=EI;	
		else
			 angle = i, aux=IE;//Mudança de ciclo (Inspiração)
			
		OCR1A = 4000.0 - (11.11*angle);
		i+=22.5;
		tempoant[1]=tempo_ms;
	}
}

void AplicValServo (){

	OCR1B = 4000.0 - (20*AberturaVal);
	
}

void CalculaBPM(){
	double static FreqBPM = 0;
	FreqBPM = 1000.0/(contt[1]-contt[0]);//Calculo da frequ?ncia sendo o periodo a diferenca entre o tempo das borda de de subida e descida
	//A multiplicacao por 1000 ocorre por que a unidade de tempo ? millis segundos assim para gerar a frequencia em Hertz se faz necessario a multiplicacao
	bpm = FreqBPM*30; // Os batimentos s?o calculados por meio da ra??o  Freq/bpm = (1/3)/20, depois ocorre a divis?o por dois
	//por que o periodo ? composto pela a diferen??o entre uma borada de subida e outra, assim como est? acontecendo em todas as bordas
	//temos que multiplicar o periodo por dois, ou dividir a frequancia por dois
	contt[0] = 0;
	contt[1] = 0;
}

void Buzzer(){

	if(temp>=410 || temp<=350){
		PORTD |=(1<<5);
	}
	else {
		
		PORTD &=~(1<<5);
		
	}
	if(SpO2<=60){
		PORTD |=(1<<6);
	}
	else {
		
		PORTD &=~(1<<6);
		
	}
	if(OCR1A==4000){
			PORTD |=(1<<7);
		}
		else {
			PORTD &=~(1<<7);
			
		}
		
}///ERRADO!

void ConvSensor(){

	if(leitura_ADC[0]<=410){
		temp = 300.0;
	}
	else if(leitura_ADC[0]>717){
		temp = 450.0;
	}
	else{
		temp = ((leitura_ADC[0]*5/1024.0)+1)/0.01;
	}
	if(leitura_ADC[1]>=819){
		SpO2 = 100;
	}
	
	else if(leitura_ADC[1]<819){
		
		SpO2 = (int)((100.0*leitura_ADC[1])/819.0);
	}
	
		
	}

void SensorPressao(){
	
	if(PINB & (1 << PINB0)){
		if((tempo_ms-tempoant[2]) >= 1000){
			FreqRespiracao = (FreqRespiracao<=MINF) ? (FreqRespiracao) : (FreqRespiracao)-1;//Diminui a frequancia a cada 1 segundo
			tempoant[2]=tempo_ms;
		}
		
	}
	else{
		if((tempo_ms-tempoant[2]) >= 1000){
			FreqRespiracao = (FreqRespiracao>=ArmFreq) ? (FreqRespiracao) : (FreqRespiracao+1);//Retorna a frequancia original
			tempoant[2]=tempo_ms;
		}
		

	}
	
	
}	

void ControleAltMaqua(){
	static int i=0;
		if(c==1)//Teste para reconhecer o sentido do movimento
			PORTB |=(1<<3);
		else if(c==2)
			PORTB &=~(1<<3);
		else{}
		if((tempo_ms-tempoant[3]) >= 1000){//Aplica a mudança por um segundo
			if(i){
				PORTB &=~(1<<4);
				i=0;
				
			}
			else if(~i && c>0){
				c=0;
				PORTB |=(1<<4); 
				
				i=1;
			}
			tempoant[3]=tempo_ms;
		}
		
}


////////////////////////////-----Interrupcoes----////////////////////////////////////////

ISR(INT0_vect)
{
	if(sel==2){
	FreqRespiracao = (FreqRespiracao>=MAXF) ? (FreqRespiracao) : (FreqRespiracao)+1;
	ArmFreq = FreqRespiracao;}
	else if (sel==4)
	AberturaVal = (AberturaVal>=100) ? (AberturaVal) : (AberturaVal)+10;
	else if (sel == 6)
	Vol = (Vol>=8) ? (Vol) : (Vol)+1;
	else if (sel == 8){//Incremento da proporção I:E que no nosso caso seria IE:EI
		if(EI>1){
			EI--;
		}
		else{
			if(IE<3){
				IE++;
				
			}
			
		}
	}
	else if(sel == 10){
		c=1;	
	}
	else{}
	
	
}

ISR(INT1_vect)
{

	if(sel==4)
	AberturaVal = (AberturaVal<=0) ? (AberturaVal) : (AberturaVal)-10;
	else if (sel==2){
	FreqRespiracao = (FreqRespiracao<=MINF) ? (FreqRespiracao) : (FreqRespiracao-1);
	}
	else if (sel == 6)
	Vol = (Vol<=1) ? (Vol) : (Vol)-1;
	else if(sel == 8){//Incremento da proporção I:E que no nosso caso seria IE:EI
		if(IE<=1){
			if(EI<3)
				EI++;
		}
		else{
			IE--;
			
		}
	}
	else if(sel==10){
		c=2;
		
	}	
	else{}

}

ISR(PCINT2_vect)
{
	
	if(contt[0]==0){  // Armazena o valor tempo no qual ocorreu a primeira borda
		contt[0]= tempo_ms;
	}
	else{
		contt[1] = tempo_ms;// Armazena o valor tempo no qual ocorreu a segunda borda
		CalculaBPM();
	}
}

ISR(PCINT0_vect)
{
	
	sel++;
	
	if(sel>=12)
		sel = 0;
	

}

ISR(ADC_vect){
 static int aux = 0;
	if((tempo_ms-tempoant[0]) >= 150){
		if(aux==0){
			ADMUX = 0b01000001;
			DIDR0 = 0b00111101;
			leitura_ADC[1]=ADC;
			aux++;
			
		}
		
		else{
			ADMUX = 0b01000000;
			DIDR0 = 0b00111110;
			leitura_ADC[0]=ADC;
			aux=0;
		}
		tempoant[0]=tempo_ms;
		ConvSensor();
	}
}

void Register(){
	//Definicao Entras e saidas
	DDRB =  0b10111110;
	PORTB = 0b01000001;
	DDRD =  0b11100000;
	PORTD = 0b00011111;
	EICRA = 0b00001010;
	EIMSK = 0b00000011;
	
	//Temporizadores
	TCCR0A = 0b00000010;
	TCCR0B = 0b00000011;
	OCR0A = 249;
	TIMSK0 = 0b00000010;
	//Interrupcao
	PCICR = 0b00000101;
	PCMSK2 = 0b00010000;
	PCMSK0 = 0b01000000;
	
	//ADC
	ADCSRA = 0b11101111;
	ADCSRB = 0b00000000;
	
	//PWM
	ICR1 = 39999;
	TCCR1A = 0b10100010;
	TCCR1B = 0b00011010;
	
	sei();
	
	
}

void Inicialize(){
	nokia_lcd_init();
	monitor();
	USART_Init(MYUBRR);
}