/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "my_funcs/my_funcs.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include <stdio.h>
#include <time.h>


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


volatile int game_field[ROWS][COLS];
volatile BigPill big_pills[BIG_PILLS];
volatile int pills_index=0;
volatile int direction=0; //0=still, 1=up, 2=right, 3=down, 4=left
volatile int i_pacman; //current i in field for pacman
volatile int j_pacman; //current j in field for pacman
volatile int x_pacman; //current x position for pacman
volatile int y_pacman; //current y position for pacman
volatile int score=0;
volatile int pills_eaten=0;
volatile int lives=1;
volatile int x_lives=INITIAL_X_LIVES;
volatile int y_lives=INITIAL_Y_LIVES;
volatile int next_live=0; //count the points. When >= 1000 a new life is added and the variable is zeroed
volatile int game_started=0;
volatile int game_paused=0;
char score_string[4];
volatile int countdown=60;
char countdown_string[2];

	
int main(void)
{
	SystemInit();  /* System Initialization (i.e., PLL)  */
	
	LED_init();
	BUTTON_init();
	
	LCD_Initialization();
	
	joystick_init();
	
	
	
	LPC_SC -> PCONP |= (1 << 22);  // TURN ON TIMER 2
	LPC_SC -> PCONP |= (1 << 23);  // TURN ON TIMER 3	
	
	enable_timer(0);
	
	InitGameField(game_field);
	
	
	LCD_Clear(Black);
	
	GUI_Text(16,15, (uint8_t *)  "Time:", White, Black);
	sprintf(countdown_string, "%d", countdown);
	GUI_Text(X_COUNTDOWN, Y_COUNTDOWN, (uint8_t *) countdown_string, White, Black);
	
	GUI_Text(130,15, (uint8_t *)  "Score:", White, Black);
	sprintf(score_string, "%d", score);
	GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
	
	DrawGameField(game_field);
	
	LCD_DrawLife(x_lives, y_lives);
	
	
	
	
	PauseGame();
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       */	//for running it on landtiger
	enable_RIT();													/* enable RIT to count 50ms				 */
	
	while (!game_started)	
  {
		__ASM("wfi");
  }
	
	
	GenerateBigPills(game_field);
	disable_timer(0);
	reset_timer(0);
	
	
	
	
	//init_timer(0, 0, 0, 3, 0x017D7840); //MR0, 0x017D7840=1s, for running it on landtiger
	init_timer(0, 0, 0, 3, 0x02FAF080); //for running it on simulator
	init_timer(1, 0, 0, 7, 0x005F5E10); 
	ResumeGame();
	enable_timer(0);
						
  
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= 0xFFFFFFFFD;						
	
	while (1)	
  {
		__ASM("wfi");
  }
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/