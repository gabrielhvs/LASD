
#ifndef RESPIRATOR_H_   /* Include guard */
#define RESPIRATOR_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#define Q_LEDS 8
#define Q_INT 15
#define MAXF 30
#define MINF 5
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include "nokia5110.h"
uint16_t tempo_ms;

////////////////////////////-----Comunicacao----////////////////////////////////////////
void USART_Init(unsigned int ubrr);
void Test_Pacote_USART();
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char data);
ISR(USART_RX_vect);
////////////////////////////-----TIMER----////////////////////////////////////////
ISR(TIMER0_COMPA_vect);
////////////////////////////-----Funcoes----////////////////////////////////////////
void monitor();
void AplicFreqLed ();
void AplicFreqServo ();
void AplicValServo ();
void CalculaBPM();
void Buzzer();
void ConvSensor();
////////////////////////////-----Interrupcoes----////////////////////////////////////////
ISR(INT0_vect);
ISR(INT1_vect);
ISR(PCINT2_vect);
ISR(PCINT0_vect);
ISR(ADC_vect);
////////////////////////////-----Inicializacao----////////////////////////////////////////
void Register();
void Inicialize();


#endif 