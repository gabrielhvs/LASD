/*
 * GccApplication1.c
 *
 * Created: 05/03/2021 08:39:52
 * Author : gabbr
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#define Q_LEDS 8
#define Q_INT 15 
#define MAXF 30
#define MINF 5

void TesteBotao(uint8_t *freq){
	
	if (!((PIND) & (1<<0))){
		//while(!((PIND) & (1<<0)));
		*freq = (*freq>=MAXF) ? (*freq) : (*freq)+1;
	}
	
	if (!((PIND) & (1<<1))){
		//while(!((PIND) & (1<<1)));
		*freq = (*freq<=MINF) ? (*freq) : (*freq)-1;
	}
}

void AplicFreq (uint8_t *FreqRespiracao){
 double td;
 uint8_t static i = 0;
  /*for(int i=0; i<=15; i++){
	TesteBotao(FreqRespiracao);
    td=((60.0/(*FreqRespiracao))/16)*1000;
	
	PORTB = i>7? (PORTB &(~(1<<15-i))):(PORTB|(1<<i));
    
	_delay_ms(td);
	
    }*/
	td=((60.0/(*FreqRespiracao))/(2*Q_LEDS))*1000;//Realiza o calculo de quanto tempo cada Led terá que ficar aceso
	for(int j=0; j<=td; j++)// repete td vezes um delay de 1 milisegundo 
	   _delay_ms(1);
	
	i = (i>Q_INT|i<1)? (i=0) : i; //Testa o i para que recomeçe o periodo 
	PORTB = i>=Q_LEDS ? (PORTB &(~(1<<Q_INT-i))):(PORTB|(1<<i));//Acender e apaga os leds dependendo de que ciclo se encontra
	i++;
	
	
  }


int main(void){


    DDRD &= 0b11111100;
	DDRC |= 0b11111111;
    PORTD |= ~(0b11111100);
	PORTC &= 0b11111111;
	DDRB = 0xFF;
    PORTB &= 0b00000000;
	
	uint8_t FreqRespiracao = 5;

while(1){

      TesteBotao(&FreqRespiracao);
      AplicFreq(&FreqRespiracao);
	  PORTC = FreqRespiracao;
    
    }
}
