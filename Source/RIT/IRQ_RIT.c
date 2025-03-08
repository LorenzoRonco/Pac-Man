/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "GLCD/GLCD.h"
#include "my_funcs/my_funcs.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down_key1=0;
volatile int down_int0=0;
extern volatile int direction;
extern volatile int game_field[ROWS][COLS];
extern volatile int game_paused;
extern int game_started;

void RIT_IRQHandler (void)
{					
	static int up_joystick=0;
	static int down_joystick=0;
	static int right_joystick=0;
	static int left_joystick=0;
	static int first_input=0;
	
	static int position=0;	
	
	
	if(!game_paused){ //disable joystick input when game paused
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
			/* Joytick UP pressed */
			up_joystick++;
			switch(up_joystick){ /* pay attention here: please see slides 19_ to understand value 2 */
				case 2: 
					direction = ChangeDirection(UP);
					break;
				default:
					break;
			}
		}
		else{
				up_joystick=0;
		}
		
		
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
			/* Joytick down pressed */
			down_joystick++;
			switch(down_joystick){ /* pay attention here: please see slides 19_ to understand value 2 */
				case 2: 
					direction = ChangeDirection(DOWN);
					break;
				default:
					break;
			}
		}
		else{
				down_joystick=0;
		}
		
		
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
			/* Joytick right pressed */
			right_joystick++;
			switch(right_joystick){ /* pay attention here: please see slides 19_ to understand value 2 */
				case 2: 
					direction = ChangeDirection(RIGHT);
					break;
				default:
					break;
			}
		}
		else{
				right_joystick=0;
		}
		
		
		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
			/* Joytick left pressed */
			left_joystick++;
			switch(left_joystick){ /* pay attention here: please see slides 19_ to understand value 2 */
				case 2: 
					direction = ChangeDirection(LEFT);
					break;
				default:
					break;
			}
		}
		else{
				left_joystick=0;
		}
		
		
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
			/* Joytick right pressed */
			right_joystick++;
			switch(right_joystick){ /* pay attention here: please see slides 19_ to understand value 2 */
				case 2: 
					direction = ChangeDirection(RIGHT);
					break;
				default:
					break;
			}
		}
		else{
				right_joystick=0;
		}
	}

	/* button management */
	if(down_key1>=1){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			switch(down_key1){				
				case 2:				/* pay attention here: please see slides 19_ to understand value 2 */
					first_input++;
					if(first_input==1){
							srand(LPC_TIM0->TC);
							game_started=1;
					}
					break;
				default:
					break;
			}
			down_key1++;
		}
		else {	/* button released */
			down_key1=0;
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	
	if(down_int0>=1){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			switch(down_int0){				
				case 2:				/* pay attention here: please see slides 19_ to understand value 2 */
					first_input++;
					if(first_input==1){
							srand(LPC_TIM0->TC);
							game_started=1;
					}else{
						if(game_paused){
							ResumeGame();
						}else{
							PauseGame();
						}
					}
					break;
				default:
					break;
			}
			down_int0++;
		}
		else {	/* button released */
			down_int0=0;
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	

	
	if((!game_paused) && game_started){
		MakeMovement(game_field, direction);
	}else{
		reset_RIT();
	}
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
