#ifndef __MY_FUNCS_H
#define __MY_FUNCS_H 
#define ROWS 31
#define COLS 28
#define N_PILLS_TO_WIN 240
#define BIG_PILLS 6
#define SQUARE_SIDE 8
#define INITIAL_X 8
#define INITIAL_Y 48

#define INITIAL_X_LIVES 16
#define INITIAL_Y_LIVES 301
#define SQUARE_SIDE_LIVES 15

#define X_POINTS 190
#define Y_POINTS 15

#define X_COUNTDOWN 60
#define Y_COUNTDOWN 15

#define PILL_SCORE 10
#define BIG_PILL_SCORE 50


typedef struct {
    int i_pos; //define the row in the gamefield matrix
    int j_pos; //define the col in the gamefield matrix
    int time; //time in which must be shown
} BigPill;

void GenerateBigPills(volatile int field[ROWS][COLS]);
volatile int randInRange(int min, int max);

// custom functions for LCD
void InitGameField(volatile int field[ROWS][COLS]);
void DrawGameField(volatile int field[ROWS][COLS]);
void LCD_DrawWall(int x_start, int y_start);
void LCD_DrawPoint(int x_start, int y_start);
void LCD_DrawBigPoint(int x_start, int y_start);
void LCD_DrawGate(int x_start, int y_start);
void LCD_DrawPacMan(int x_start, int y_start);
void LCD_ClearSquare(int x_start, int y_start);
void LCD_DrawLife(int x_start, int y_start);

//functions for manage movement
enum Direction {
    STILL = 0,
    UP = 1,
		RIGHT = 2,
		DOWN = 3,
		LEFT = 4
};

volatile int ChangeDirection(enum Direction new_direction);
void MakeMovement(volatile int field[ROWS][COLS], enum Direction direction);

//functions for game control
void ResumeGame();
void PauseGame();
void UpdateCountdown();
void GameOver();
void Victory();

#endif 