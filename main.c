#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include "iter.h"
#include "write.h"

// LINE_VERTICAL:│
// LINE_HORIZONTAL:─
// LINE_DOWN_RIGHT:┌
// LINE_DOWN_LEFT:┐
// LINE_UP_RIGHT:└
// LINE_UP_LEFT:┘
// SQUARE_WHITE: 
// SQUARE_BLACK:█
// ARROW_NORTH_WHITE:△
// ARROW_NORTH_BLACK:▲
// ARROW_EAST_WHITE:▷
// ARROW_EAST_BLACK:▶
// ARROW_SOUTH_WHITE:▽
// ARROW_SOUTH_BLACK:▼
// ARROW_WEST_WHITE:◁
// ARROW_WEST_BLACK:◀

int main(int argc, char**argv){
	srand(time(NULL));
	int finish=0;	   // jeśli 1 - kończymy program
	int c;
	int m=0;
	int n=0;
	int i=0;  	   // m,n - wymiary planszy, i - liczba iteracji
	char *name; 	   // przedrostek nazw plików, do których będziemy zapisywać
	int pos[2];	   // początkowa pozycja mrówki 
	pos[0]=-1;	   // -1 będzie oznaczać, że użytkownik nie podał pozycji mrówki
	pos[1]=-1;
	int dir=0; 	   // początkowy kierunek mrówki
	double blacks = 0; // % zapełnienia mapy czarnymi polami (domyślnie 0)
	char *file;	   // nazwa pliku, z którego wczytamy mapę użytkownika
	int aflag = 0;     // flaga -a
	int j,k,l;	   // zmienne sterujące ('i' używamy już do czegoś innego)
	int *tmp;

	// Dostępne flagi:
	// -c - wczytanie własnego pliku z gotową planszą (opcjonalnie)
	// -b - zapełnienie planszy losowo czarnymi polami (opcjonalnie)
	// -f - przedrostek pliku, w którym będą zapisane kolejne iteracje
	//      (jeżeli nie podana, wszystkie iteracje zostaną umieszczone na stdout)
	// -m - liczba wierszy (wymagana, jeśli nie podano -c)
	// -n - liczba kolumn (wymagana, jeśli nie podano -c)
	// -i - liczba iteracji (wymagana)
	// -a - jeżeli podana, program nie zakończy działania, gdy mrówka będzie chciała
	//      wyjść poza planszę - mrówka przejdzie wtedy na drugi koniec planszy
	// -x - początkowy wiersz mrówki (opcjonalnie)
	// -y - początkowa kolumna mrówki (opcjonalnie)
	//      jeżeli początkowa pozycja mrówki nie jest podana, mrówka zostanie ustawiona
	//      na środku planszy
	// -d - początkowy kierunek mrówki: 0-N, 1-E, 2-S, 3-W (opcjonalnie, domyślnie 0 (N))
	
	while ((c = getopt(argc, argv, "c:b:f:m:n:i:ax:y:d:")) != -1)
		switch(c){
			case 'c':
				file = optarg;
				break;
			case 'b':
				blacks = atof(optarg);
				break;
			case 'f':
				name = optarg;
				break;
			case 'm':
				m = atoi(optarg);
				break;
			case 'n':
				n = atoi(optarg);
				break;
			case 'i':
				i = atoi(optarg);
				break;
			case 'a':
				aflag = 1;
				break;
			case 'x':
				pos[0] = atoi(optarg);
				break;
			case 'y':
				pos[1] = atoi(optarg);
				break;
			case 'd':
				dir = atoi(optarg);
				break;
			case '?':
				if (optopt == 'c' || optopt == 'b' || optopt == 'f' || optopt == 'm' || optopt == 'n' || optopt == 'i' || optopt == 'x' || optopt == 'y')
					fprintf(stderr, "Opcja -%c wymaga podania argumentu.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Program nie rozpoznaje opcji -%c.\n", optopt);
				else
					fprintf(stderr, "Program nie rozpoznaje znaku %c (Unicode x%x).\n", optopt, optopt);
			default:
				abort();
		}
	// deklaracja planszy
	board_t board;
	if (file != NULL){
		FILE *in = fopen(file,"r");
		fscanf(in,"%d %d %d %d %d",&m,&n,&pos[0],&pos[1],&dir);
		board.m = m;
		board.n = n;
		if (m>0 && n>0)
			board.data = (int**)malloc(board.m*board.n*sizeof(int));
		else {
			fprintf(stderr, "Błąd: wymiary planszy podane w pliku niedodatnie.\n");
			fprintf(stderr, "Program kończy działanie (kod błędu: 004).\n");
			return 4;
		}
		int *datax=malloc(m*n*sizeof(int));
		for (j=0; j<m*n; j++){
			fscanf(in,"%d",&datax[j]);
		}
		board.pos = malloc(2*sizeof(int));
		board.pos[0] = pos[0];
		board.pos[1] = pos[1];

		for (j=0; j<board.m; j++){
			tmp = malloc(board.n*sizeof(int));
			for (k=0; k<board.n; k++){
				tmp[k]=datax[j*board.n+k];
			}
			board.data[j]=tmp;
		}
		fclose(in);
	} else {
		board.m = m;
		board.n = n;
		board.pos = malloc(2*sizeof(int));
		board.pos[0] = pos[0];
		board.pos[1] = pos[1];
		if (m>0 && n>0)
			board.data = (int**)malloc(board.m*board.n*sizeof(int));
	}
	
	if (m<=0 || n<=0){
		fprintf(stderr, "Błąd: wymiary planszy nie podane lub niedodatnie.\n");
		fprintf(stderr, "m = %d, n = %d\n", m, n);
		fprintf(stderr, "Program kończy działanie (kod błędu: 001).\n");
		return 1;
	}
	if (i<=0){
		fprintf(stderr, "Błąd: liczba iteracji nie podana lub niedodatnia.\n");
		fprintf(stderr, "i = %d\n", i);
		fprintf(stderr, "Program kończy działanie (kod błędu: 002).\n");
		return 2;
	}
	if (board.data == NULL){
		fprintf(stderr, "Błąd pamięci: utworzenie planszy nie powiodło się.\n");
		fprintf(stderr, "Program kończy działanie (kod błędu: 101).\n");
		return 101;
	}
	
	// jeżeli nie podano początkowej pozycji mrówki - ustawiamy ją na środku
	if (board.pos[0] == -1)
		board.pos[0] = board.m/2;
	if (board.pos[1] == -1)
		board.pos[1] = board.n/2;
	
	// domyślnie zapełniamy planszę białymi polami
	if (file==NULL){
		for (j=0; j<board.m; j++){
			tmp = (int*)malloc(n*sizeof(int));
			for (k=0; k<board.n; k++)
				tmp[k]=0;
			board.data[j]=tmp;
		}
	}

	// jeżeli flaga -b podana, wówczas:
	if (blacks>0){
		int b=blacks*board.m*board.n/100; // liczba pól, jakie zapełnimy
		int randindex[2]; // losowe pole, jakie w danym momencie zapełniamy
		j=0;
		while (j!=b){
			randindex[0] = rand() % board.m;
			randindex[1] = rand() % board.n;
			// jeżeli wylosowana komórka jest biała, zapełniamy ją
			// jeżeli już jest czarna - losujemy jeszcze raz, do skutku
			if (board.data[randindex[0]][randindex[1]] == 0){
				board.data[randindex[0]][randindex[1]]=1;
				j++;
			}
		}
	}
	
	// tutaj wykonujemy iteracje
	j=0;
	while (j<i && finish==0){
		j++;
		if (board.data[board.pos[0]][board.pos[1]]==0){
			dir++;
			if (dir==4)
				dir=0;
			board.data[board.pos[0]][board.pos[1]]=1;
		} else {
			dir--;
			if (dir==-1)
				dir=3;
			board.data[board.pos[0]][board.pos[1]]=0;
		}
		move(board,dir);
		
		if (out_of(board)==1){
			if (aflag==0){
				finish=1;
				break;
			} else
				put_back_on(board);
		}
		// tu będzie funkcja na zapisanie iteracji w pliku
		// narazie wypisuję tablicę (w postaci binarnej) na stdout
		
		printf("Iteracja nr %d\n", j);
		for (k=0;k<board.m;k++){
			printf("[%d", board.data[k][0]);
			for (l=1;l<board.n;l++)
				printf(" %d", board.data[k][l]);
			printf("]\n");
		}
		printf("\n");
		
	}
	if (finish==1){
		printf("Mrówka wyszła poza planszę.\n");
		printf("Program zakończył działanie po wykonaniu %d z %d iteracji.\n\n", --j, i);
	}
	
	free(tmp);
	return 0;
}
