
#include "Respirator.h"

unsigned long int tempAnt= 0;

int main(void)
{
	//Inicializacao
	Register();
	Inicialize();
	
	while (1)
	{
		//Atualizacao do Display
		if((tempo_ms-tempAnt) >= 300){
			monitor();
			tempAnt = tempo_ms;
		}
		//Aplicacao das funcoes
		AplicFreqServo();
		AplicValServo();
		Buzzer();
		SensorPressao();
		ControleAltMaqua();
	}
}

