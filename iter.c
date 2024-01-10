#include "iter.h"

// Funkcja wykonująca ruch.
void move(board_t board, int dir){
	if (dir==0)
		board.pos[0]--;
	else if (dir==2)
		board.pos[0]++;
	else if (dir==3)
		board.pos[1]--;
	else if (dir==1)
		board.pos[1]++;
}

// Funkcja sprawdzająca, czy mrówka przypadkiem nie wyszła poza planszę.
int out_of(board_t board){
	if((board.pos[0]<0 || board.pos[0]>=board.m) || (board.pos[1]<0 || board.pos[1]>=board.n))
		return 1;
	else
		return 0;
}

// Funkcja przywracająca mrówkę na planszę.
void put_back_on(board_t board){
	if (board.pos[0]<0)
		board.pos[0]=board.m-1;
	else if (board.pos[0]>=board.m)
		board.pos[0]=0;
	else if (board.pos[1]<0)
		board.pos[1]=board.n-1;
	else if (board.pos[1]>=board.m)
		board.pos[1]=0;
}
