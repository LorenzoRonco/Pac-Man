#include "my_funcs.h"
#include "../GLCD/GLCD.h"
#include <time.h>
#include <stdio.h>
#include "../timer/timer.h"
#include "../RIT/RIT.h"


extern volatile int i_pacman; //current i in field for pacman
extern volatile int j_pacman; //current j in field for pacman
extern volatile int x_pacman; //current x position for pacman
extern volatile int y_pacman; //current y position for pacman
extern volatile int x_lives;
extern volatile int y_lives;
extern volatile int score;
extern volatile int pills_eaten;
extern char score_string[4];
extern volatile int lives;
extern volatile int next_live; //count the points. When >= 1000 a new life is added and the variable is zeroed
extern volatile int game_paused;
volatile int previous_direction;
extern volatile int direction;
extern volatile int game_field[ROWS][COLS];
extern volatile int countdown;
extern char countdown_string[2];
extern volatile int pills_index;
extern volatile BigPill big_pills[BIG_PILLS];
volatile int wait_flag=0;
extern int game_started;



//-2=gate, -1=wall, 0=empty, 1=pill, 2=special pill, 3=teleport, 4=pac-man
const int game_field_input[ROWS][COLS]={{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,1,1,1,1,1,1,1,1,1,1,1,1,-1},
{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},
{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},
{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},
{-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1},
{-1,1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,1,-1},
{-1,1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,1,-1},
{-1,1,1,1,1,1,1,-1,-1,1,0,0,0,-1,-1,0,0,0,1,-1,-1,1,1,1,1,1,1,-1},
{-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,0,-1,-1,0,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1},
{0,0,0,0,0,-1,1,-1,-1,-1,-1,-1,0,-1,-1,0,-1,-1,-1,-1,-1,1,-1,0,0,0,0,0},
{0,0,0,0,0,-1,1,-1,-1,0,0,0,0,0,0,0,0,0,0,-1,-1,1,-1,0,0,0,0,0},
{0,0,0,0,0,-1,1,-1,-1,0,-1,-1,-2,-2,-2,-2,-1,-1,0,-1,-1,1,-1,0,0,0,0,0},
{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,0,0,0,0,0,0,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},
{3,0,0,0,0,0,1,0,0,0,-1,0,0,0,0,0,0,-1,0,0,0,1,0,0,0,0,0,3},
{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,0,0,0,0,0,0,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},
{0,0,0,0,0,-1,1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,1,-1,0,0,0,0,0},
{0,0,0,0,0,-1,1,-1,-1,0,0,0,0,0,0,4,0,0,0,-1,-1,1,-1,0,0,0,0,0},
{0,0,0,0,0,-1,1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,1,-1,0,0,0,0,0},
{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},
{-1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,1,1,1,1,1,1,1,1,1,1,1,1,-1},
{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},
{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},
{-1,1,1,1,-1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,1,1,1,-1},
{-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1},
{-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1},
{-1,1,1,1,1,1,1,-1,-1,1,1,1,1,-1,-1,1,1,1,1,-1,-1,1,1,1,1,1,1,-1},
{-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1},
{-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1},
{-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
};


//----------------------------------------------------------------------------------------------//

void GenerateBigPills(volatile int field[ROWS][COLS]){
	int i;
	volatile int random_i, random_j;
	volatile int next_pill=0; //1=true, 0=false -> true significa che ho trovato una posizione valida
	for(i=0; i<BIG_PILLS; i++){
		while(!next_pill){
			random_i=randInRange(1,ROWS-2); //exclude top and bottom line of matrix 'cause it's only walls
			random_j=randInRange(1,COLS-2); ////exclude first and last col of matrix 'cause it's only walls
			if(field[random_i][random_j]==1){ //found a valid position
				big_pills[i].i_pos=random_i;
				big_pills[i].j_pos=random_j;
				big_pills[i].time=randInRange(30,60); //big pills will be generated from start untill 30 s
				next_pill=1;
			}
		}
		next_pill=0;
	}
	
	//order vect for descreasing time w/ bubble sort optimized
	int swapped, j;
	BigPill temp;
	for (i = 0; i < BIG_PILLS - 1; i++) {
        swapped = 0;
        for (j = 0; j < BIG_PILLS - 1 - i; j++) {
            if (big_pills[j].time < big_pills[j + 1].time) {
                temp = big_pills[j];
                big_pills[j] = big_pills[j + 1];
                big_pills[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}


volatile int randInRange(int min, int max) { //max & min included
    return (rand() % (max - min + 1)) + min;
}
//----------------------------------------------------------------------------------------------//
/* The game field is divided in squares of LxL pixels, where L is define by constant SQUARE_SIDE */

void InitGameField(volatile int field[ROWS][COLS]){
	int i,j;
  for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			field[i][j] = game_field_input[i][j];
    }
  }
}


void DrawGameField(volatile int field[ROWS][COLS]){
	int i, j;
	int x_curr=INITIAL_X;
	int y_curr=INITIAL_Y; //initial position
	
	for(i=0; i<ROWS; i++){
		for(j=0; j<COLS; j++){
			switch(field[i][j]){
				case -2:
					LCD_DrawGate(x_curr, y_curr);
					break;
				case -1:
					LCD_DrawWall(x_curr, y_curr);
					break;
				case 1:
					LCD_DrawPoint(x_curr, y_curr);
					break;
				case 2:
					LCD_DrawBigPoint(x_curr, y_curr);
					break;
				case 4:
					x_pacman=x_curr;
					y_pacman=y_curr;
					i_pacman=i;
					j_pacman=j;
					LCD_DrawPacMan(x_curr, y_curr);
					break;
			}
			x_curr+=SQUARE_SIDE;
		}
		x_curr=INITIAL_X;
		y_curr+=SQUARE_SIDE;
	}
}


void LCD_DrawWall(int x_start, int y_start){
	int i;
	for(i=0; i<8; i++)
		LCD_DrawLine(x_start, y_start+i, x_start+7, y_start+i, Blue);
}


void LCD_DrawPoint(int x_start, int y_start){
	int i;
	for(i=3; i<5; i++)
		LCD_DrawLine(x_start+3, y_start+i, x_start+4, y_start+i, White);
}

void LCD_DrawBigPoint(int x_start, int y_start){
	int i;
	for(i=2; i<6; i++)
		LCD_DrawLine(x_start+2, y_start+i, x_start+5, y_start+i, White);
}

void LCD_DrawGate(int x_start, int y_start){
	int i;
	for(i=2; i<6; i++)
		LCD_DrawLine(x_start, y_start+i, x_start+7, y_start+i, Magenta);
}

void LCD_DrawPacMan(int x_start, int y_start){
	int i;
	for(i=1; i<7; i+=5){
		LCD_DrawLine(x_start+3, y_start+i, x_start+4, y_start+i, Yellow);
	}
	
	for(i=2; i<6; i+=3){
		LCD_DrawLine(x_start+2, y_start+i, x_start+5, y_start+i, Yellow);
	}
	
	for(i=3; i<5; i+=1){
		LCD_DrawLine(x_start+1, y_start+i, x_start+6, y_start+i, Yellow);
	}
}

void LCD_ClearSquare(int x_start, int y_start){
	int i;
	for(i=0; i<8; i++)
		LCD_DrawLine(x_start, y_start+i, x_start+7, y_start+i, Black);
}

void LCD_DrawLife(int x_start, int y_start){
	wait_flag=1;
		
	LCD_DrawLine(x_start+5, y_start, x_start+10, y_start, Yellow);
	LCD_DrawLine(x_start+3, y_start+1, x_start+12, y_start+1, Yellow);
	LCD_DrawLine(x_start+2, y_start+2, x_start+13, y_start+2, Yellow);
	LCD_DrawLine(x_start+1, y_start+3, x_start+14, y_start+3, Yellow);
	LCD_DrawLine(x_start+1, y_start+4, x_start+14, y_start+4, Yellow);
	LCD_DrawLine(x_start, y_start+5, x_start+12, y_start+5, Yellow);
	LCD_DrawLine(x_start, y_start+6, x_start+9, y_start+6, Yellow);
	
	LCD_DrawLine(x_start, y_start+7, x_start+6, y_start+7, Yellow); //center line
	
	LCD_DrawLine(x_start, y_start+8, x_start+9, y_start+8, Yellow);
	LCD_DrawLine(x_start, y_start+9, x_start+12, y_start+9, Yellow);
	LCD_DrawLine(x_start+1, y_start+10, x_start+14, y_start+10, Yellow);
	LCD_DrawLine(x_start+1, y_start+11, x_start+14, y_start+11, Yellow);
	LCD_DrawLine(x_start+2, y_start+12, x_start+13, y_start+12, Yellow);
	LCD_DrawLine(x_start+3, y_start+13, x_start+12, y_start+13, Yellow);
	LCD_DrawLine(x_start+5, y_start+14, x_start+10, y_start+14, Yellow);
	
	LCD_DrawLine(x_start+9, y_start+2, x_start+10, y_start+2, Black); //draw eye
	LCD_DrawLine(x_start+9, y_start+3, x_start+10, y_start+3, Black);
	
	x_lives+=(SQUARE_SIDE_LIVES+8);
	
	wait_flag=0;
}


// -------------------------------------------------------
//Custom functions for movement

volatile int ChangeDirection(enum Direction new_direction){
	return new_direction;
}


void MakeMovement(volatile int field[ROWS][COLS], enum Direction direction){
	disable_RIT();
	wait_flag=1;
	switch(direction){
		case UP:
			if(field[i_pacman-1][j_pacman]>-1){ //next cell is not a wall or a gate
				switch(field[i_pacman-1][j_pacman]){
					case 1: //pacman is eating a small pill
						score+=PILL_SCORE;
						next_live+=PILL_SCORE;
						pills_eaten+=1;
						sprintf(score_string, "%d", score);
						GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
					break;
					case 2: //pacman is eating a big pill
						score+=BIG_PILL_SCORE;
						next_live+=BIG_PILL_SCORE;
						pills_eaten+=1;
						sprintf(score_string, "%d", score);
						GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
					break;
				}
				
				LCD_ClearSquare(x_pacman, y_pacman); //remove pacman from curr position
				
				y_pacman-=SQUARE_SIDE; //update pacman position
				field[i_pacman][j_pacman]=0;
				i_pacman--;
				field[i_pacman][j_pacman]=4;
				
				LCD_DrawPacMan(x_pacman, y_pacman); //draw pacman in new position
			}else{
				direction=STILL;
			}
			break;
		case RIGHT:
			if(field[i_pacman][j_pacman+1]>-1){ //next cell is not a wall or a gate
				switch(field[i_pacman][j_pacman+1]){
					case 1: //pacman is eating a small pill
						score+=PILL_SCORE;
						next_live+=PILL_SCORE;
						pills_eaten+=1;
						sprintf(score_string, "%d", score);
						GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
					break;
					case 2: //pacman is eating a big pill
						score+=BIG_PILL_SCORE;
						next_live+=BIG_PILL_SCORE;
						pills_eaten+=1;
						sprintf(score_string, "%d", score);
						GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
					break;
				}
				if(field[i_pacman][j_pacman+1]==3){ //teleport
					LCD_ClearSquare(x_pacman, y_pacman); //remove pacman from curr position
					
					x_pacman=INITIAL_X; //update pacman position
					field[i_pacman][j_pacman]=0;
					j_pacman=0;
					field[i_pacman][j_pacman]=4;
					
					LCD_DrawPacMan(x_pacman, y_pacman); //draw pacman in new position
				}else{
					LCD_ClearSquare(x_pacman, y_pacman); //remove pacman from curr position
					
					x_pacman+=SQUARE_SIDE; //update pacman position
					field[i_pacman][j_pacman]=0;
					j_pacman++;
					field[i_pacman][j_pacman]=4;
					
					LCD_DrawPacMan(x_pacman, y_pacman); //draw pacman in new position
				}
			}else{ //next cell is a wall
				direction=STILL;
			}
			break;
		case DOWN:
			if(field[i_pacman+1][j_pacman]>-1){ //next cell is not a wall or a gate
				switch(field[i_pacman+1][j_pacman]){
					case 1: //pacman is eating a small pill
						score+=PILL_SCORE;
						next_live+=PILL_SCORE;
						pills_eaten+=1;
						sprintf(score_string, "%d", score);
						GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
					break;
					case 2: //pacman is eating a big pill
						score+=BIG_PILL_SCORE;
						next_live+=BIG_PILL_SCORE;
						pills_eaten+=1;
						sprintf(score_string, "%d", score);
						GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
					break;
				}
				
				LCD_ClearSquare(x_pacman, y_pacman); //remove pacman from curr position
				
				y_pacman+=SQUARE_SIDE; //update pacman position
				field[i_pacman][j_pacman]=0;
				i_pacman++;
				field[i_pacman][j_pacman]=4;
				
				LCD_DrawPacMan(x_pacman, y_pacman); //draw pacman in new position
			}else{//next cell is a wall
				direction=STILL;
			}
			break;
			
		case LEFT:
			if(field[i_pacman][j_pacman-1]>-1){ //next cell is not a wall or a gate
				switch(field[i_pacman][j_pacman-1]){
					case 1: //pacman is eating a small pill
						score+=PILL_SCORE;
						next_live+=PILL_SCORE;
						pills_eaten+=1;
						sprintf(score_string, "%d", score);
						GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
					break;
					case 2: //pacman is eating a big pill
						score+=BIG_PILL_SCORE;
						next_live+=BIG_PILL_SCORE;
						pills_eaten+=1;
						sprintf(score_string, "%d", score);
						GUI_Text(X_POINTS, Y_POINTS, (uint8_t *) score_string, White, Black);
					break;
				}
				
				if(field[i_pacman][j_pacman-1]==3){ //teleport
					LCD_ClearSquare(x_pacman, y_pacman); //remove pacman from curr position
					
					x_pacman=224; //update pacman position
					field[i_pacman][j_pacman]=0;
					j_pacman=COLS-1;
					field[i_pacman][j_pacman]=4;
					
					LCD_DrawPacMan(x_pacman, y_pacman); //draw pacman in new position
				}else{
					LCD_ClearSquare(x_pacman, y_pacman); //remove pacman from curr position
					
					x_pacman-=SQUARE_SIDE; //update pacman position
					field[i_pacman][j_pacman]=0;
					j_pacman--;
					field[i_pacman][j_pacman]=4;
					
					LCD_DrawPacMan(x_pacman, y_pacman); //draw pacman in new position
				}
			}else{ //next cell is a wall
				direction=STILL;
			}
			break;
			
		case STILL:
			break;
	}
	
	if(next_live>=1000){
		lives++;
		next_live=0;
		LCD_DrawLife(x_lives, y_lives);
	}
	
	if (pills_eaten>=N_PILLS_TO_WIN){
		Victory();
	}
	wait_flag=0;
	
	enable_timer(1);
}


// -------------------------------------------------------
//Functions for game control


void PauseGame(){
	game_paused=1;
	
	previous_direction=direction;
	direction=ChangeDirection(STILL);
	
	if(game_started){ //if !game_started time0 is used as seed for srand
		disable_timer(0);
	}
	
	GUI_Text(75 ,160, (uint8_t *)  "GAME PAUSED", White, Red);
	
	GUI_Text(40,180, (uint8_t *)  "PRESS INT0 TO RESUME", White, Red);
	
	reset_RIT();
}


void ResumeGame(){
	int i, j;
	int x_curr=INITIAL_X;
	int y_curr=INITIAL_Y+13*SQUARE_SIDE;;
	for (i=13; i<19; i++){
		for (j=0; j<COLS; j++){
			LCD_ClearSquare(x_curr, y_curr);
			switch(game_field[i][j]){
				case -2:
					LCD_DrawGate(x_curr, y_curr);
					break;
				case -1:
					LCD_DrawWall(x_curr, y_curr);
					break;
				case 1:
					LCD_DrawPoint(x_curr, y_curr);
					break;
				case 2:
					LCD_DrawBigPoint(x_curr, y_curr);
					break;
				case 4:
					LCD_DrawPacMan(x_curr, y_curr);
					break;
				}
			x_curr+=SQUARE_SIDE;
		}
		x_curr=INITIAL_X;
		y_curr+=SQUARE_SIDE;
	}
	direction=ChangeDirection(previous_direction);
	game_paused=0;
	enable_timer(0);
	reset_RIT();
}


void UpdateCountdown(){
	countdown--;
	if(countdown>=10){
		sprintf(countdown_string, "%d", countdown);
	}else{
		sprintf(countdown_string, "0%d", countdown);
	}
	
	GUI_Text(X_COUNTDOWN, Y_COUNTDOWN, (uint8_t *) countdown_string, White, Black);
	
	if(pills_index<BIG_PILLS){
		if(big_pills[pills_index].time==countdown){
			if(game_field[big_pills[pills_index].i_pos][big_pills[pills_index].j_pos]==1){ //generate big pill if smaller one still not eaten
			game_field[big_pills[pills_index].i_pos][big_pills[pills_index].j_pos]=2;
			LCD_DrawBigPoint(INITIAL_X+big_pills[pills_index].j_pos*SQUARE_SIDE, INITIAL_Y+big_pills[pills_index].i_pos*SQUARE_SIDE);
			}
			pills_index++;
		}
	}
	if(countdown<=0 & pills_eaten<240){
		GameOver();
	}
}

void GameOver(){
	disable_timer(0);
	direction=ChangeDirection(STILL);
	game_paused=1;
	GUI_Text(80,160, (uint8_t *)  "GAME OVER!", White, Red);
	GUI_Text(20,180, (uint8_t *)  "PRESS RESET TO PLAY AGAIN", White, Red);
	
}


void Victory(){
	disable_timer(0);
	direction=ChangeDirection(STILL);
	game_paused=1;
	GUI_Text(85,160, (uint8_t *)  "VICTORY!", White, Red);
	GUI_Text(20,180, (uint8_t *)  "PRESS RESET TO PLAY AGAIN", White, Red);
}