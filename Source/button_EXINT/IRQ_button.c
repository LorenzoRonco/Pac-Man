#include "button.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h"

extern int down_key1;
extern int game_started;
extern volatile int down_int0;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	down_int0=1;

	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
	
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	/* put here action for when KEY1 is pressed */
	
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	down_key1=1;
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */
}	
