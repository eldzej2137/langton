#ifndef ITER_H
#define ITER_H

typedef struct {
	int finish; // jeżeli równe 1 - kończymy działanie
	int m,n;    // wymiary planszy
	int x,y;    // aktualna pozycja mrówki
	char dir;   // aktualny kierunek mrówki
	int **data; // dane
} board_t;

char rotate_right(char dir);
char rotate_left(char dir);
void move(board_t board, int a);
void iter(board_t board, int a);

#endif
