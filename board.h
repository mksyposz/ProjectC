#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct node *Board;

struct node
{ 
    struct node *up, *down, *next, *prev;
    int val; 
    int win_cond;
    //-1 - nic nie ma 
    //0 - krzyżyl 
    //1 - kółko    
};

Board init(int,int); 
Board single_column(int,int);
Board add_row(Board);
Board move1(Board,int,int); 
Board move2(Board,int,int); 
Board add_right_column(Board); 
Board add_left_column(Board);
bool is_legal(Board,int,int);
void write_board(Board); 
int check_win(struct node *board, int x); 
int dir1(struct node *board, int player);
int dir2(struct node *board, int player);
int dir3(struct node *board, int player);
int dir4(struct node *board, int player);
int dir5(struct node *board, int player);
int dir6(struct node *board, int player);
int dir7(struct node *board, int player);
int dir8(struct node *board, int player);
int width(struct node*); 
int height(struct node*);
#endif
