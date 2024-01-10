#ifndef ITER_H
#define ITER_H

typedef struct {
	int m,n;    // wymiary planszy
	int *pos;   // aktualna pozycja mrówki
	int **data; // dane
} board_t;

void move(board_t board, int dir);
int out_of(board_t board);
void put_back_on(board_t board);

#endif
