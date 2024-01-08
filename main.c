#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include "iter.h"
#include "read.h"
#include "write.h"

int main(int argc, char**argv){
	srand(time(NULL));
	int c;
	int m=0;
	int n=0;
	int i=0;  	   // m,n - wymiary planszy, i - liczba iteracji
	char *name; 	   // przedrostek nazw plików, do których będziemy zapisywać
	int pos[2];	   // początkowa pozycja mrówki 
	char dir='N'; 	   // początkowy kierunek mrówki
	double blacks = 0; // % zapełnienia mapy czarnymi polami (domyślnie 0)
	char *file;	   // nazwa pliku, z którego wczytamy mapę użytkownika
	int aflag = 0;     // flaga -a
	int j,k;	   // zmienne sterujące ('i' używamy już do czegoś innego)

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
	// -d - początkowy kierunek mrówki: 'N', 'S', 'W' lub 'E' (opcjonalnie, domyślnie 'N')
	
	while ((c = getopt(argc, argv, "c:b:f:m:n:i:ax:y:")) != -1)
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
				dir = optarg;
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
	board.finish = 0;
	if (file != NULL){
		// tutaj zapiszemy mapę użytkownika
		// i ustalimy wymiary
	} else {
		board.m = m;
		board.n = n;
		board.x = pos[0];
		board.y = pos[1];
		board.dir = dir;
		if (m>0 && n>0)
			board.data = malloc(m*n*sizeof(int));
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

	// domyślnie zapełniamy planszę białymi polami
	for (j=0; j<board.m; j++){
		for (k=0; k<board.n; k++)
			board.data[j][k]=0;
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
	while (j<i && board.finish==0){
		iter(board,aflag);
		// tu będzie funkcja na zapisanie iteracji w pliku
		j++;
	}

	return 0;
}
