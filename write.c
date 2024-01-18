#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
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

//czarny - xx1 biały- xx0
//góra -00x dół-01x wschód - 10x zachód 11x
//┌───┐
//│   │
//└───┘
//
//┌───┐
//│ ▲ │
//└───┘
//┌───┐
//│█△█│
//└───┘
//┌───┐
//│███│
//└───┘


void bialez(int pole,FILE *file){
	
fprintf(file,"│ ");
        switch (pole){                  
                                       case 0:
						fprintf(file,"▲");
						break;

                                       case 1:
						fprintf(file,"▼");
						break;

                                       case 2:
						fprintf(file,"▶");
						break;

                                       case 3:
						fprintf(file,"◀");
						break;
                                       
                               }
        fprintf(file," │");
        
        
}

void czarnez(int pole, FILE *file){
	fprintf(file,"│█");
	switch (pole){

                                       case 0:
						fprintf(file,"△");
						break;

                                       case 1:
						fprintf(file,"▽");
						break;

                                       case 2:
						fprintf(file,"▷");
						break;

                                       case 3:
						fprintf(file,"◁");
						break;
                               }
        fprintf(file,"█│");
}













void write(int n, int m, int x, int  y, int** plansza, int itnum, struct board_t, int kierunek)

 {	
	char nazwaPliku[10]; 
        sprintf(nazwaPliku, "ite_%d", itnum);
	FILE *file = fopen(nazwaPliku, "w");
 	for(int i=0;i<n;i++){
		for (int s=0;s<3;s++){

		for (int j=0;j<m;j++)
		{
			if(s==1){
				if (i==x && j==y){
					if(plansza[x][y]==0)
						{
							bialez(kierunek,file);	
						}
					else{
						czarnez(kierunek,file);
					}	
				}
				else
				{
					  if(plansza[i][j]==0)
                                                {
                                                       fprintf(file,"│   │");
                                                }
                                        else{
                                                fprintf(file,"│████│");
                                        }
				}
		
				}



		




			if(s==0){
				
				fprintf(file,"┌───┐");
			}
			 if(s==2){
                              
                                fprintf(file,"└───┘");
                }       
		 }
		fprintf(file,"\n");
		}
		fprintf(file,"\n");
		}
	fclose(file);
}

