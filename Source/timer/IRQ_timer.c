/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "my_funcs/my_funcs.h"
#include "../RIT/RIT.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
  if(LPC_TIM0->IR & 1) // MR0
	{ 
		UpdateCountdown();
		
		LPC_TIM0->IR = 1;			//clear interrupt flag
	}
	return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		reset_RIT();
		enable_RIT();
		LPC_TIM1->IR = 1;			//clear interrupt flag
	}
	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0
	{ 
		//your action
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}else if(LPC_TIM2->IR & 2){ // MR1
		//your action
		LPC_TIM2->IR = 2;			// clear interrupt flag 
	}
  return;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void)
{
    if(LPC_TIM3->IR & 1) // MR0
	{ 
		//your action
		LPC_TIM3->IR = 1;			//clear interrupt flag
	}else if(LPC_TIM3->IR & 2){ // MR1
		//your action
		LPC_TIM3->IR = 2;			// clear interrupt flag 
	}
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/