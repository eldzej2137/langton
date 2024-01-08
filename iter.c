#include "iter.h"

// Funkcje na obrót w prawo i w lewo.
char rotate_right(char dir){
	if (dir=='N')
		return 'E';
	else if (dir=='E')
		return 'S';
	else if (dir=='S')
		return 'W';
	else if (dir=='W')
		return 'N';
}
char rotate_left(char dir){
	if (dir=='N')
		return 'W';
	if (dir=='W')
		return 'S';
	if (dir=='S')
		return 'E';
	if (dir=='E')
		return 'N';
}

// Funkcja wykonująca ruch.
void move(board_t board, int a){
	if (board.dir=='N')
		board.x--;
	else if (board.dir=='S')
		board.x++;
	else if (board.dir=='W')
		board.y--;
	else if (board.dir=='E')
		board.y++;
	if ((board.x<0 || board.x>=board.m) || (board.y<0 || board.y>=board.n)){
		if (a==0)
			board.finish = 1;
		else if (board.x==-1)
			board.x=board.m-1;
		else if (board.x==board.m)
			board.x=0;
		else if (board.y==-1)
			board.y=board.n-1;
		else if (board.y==board.n)
			board.y=0;
	}
}

// Funkcja wykonująca pełną iterację.
void iter(board_t board, int a){
	if (board.data[board.x][board.y]==0){
		board.dir=rotate_right(board.dir);
		board.data[board.x][board.y]=1;
	} else {
		board.dir=rotate_left(board.dir);
		board.data[board.x][board.y]=0;
	}
	move(board,a);
}
