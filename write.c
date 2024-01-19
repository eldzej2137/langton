#include "iter.h"
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

//┌───┐
//│   │
//└───┘
//┌───┐
//│ △ │
//└───┘
//┌───┐
//│█▲█│
//└───┘
//┌───┐
//│███│
//└───┘


void czarnez(int pole,FILE *file){
	
fprintf(file,"│█");
        switch (pole){                  
                                       case 0:
						fprintf(file,"▲");
						break;

                                       case 1:
						fprintf(file,"▶");
						break;

                                       case 2:
						fprintf(file,"▼");
						break;

                                       case 3:
						fprintf(file,"◀");
						break;
                                       
                               }
        fprintf(file,"█│");
        
        
}

void bialez(int pole, FILE *file){
	fprintf(file,"│ ");
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
        fprintf(file," │");
}












// write1, a nie write, bo write jest zdefiniowane w unistd.h
void write1(int itnum, board_t board, int kierunek)

 {	
	char nazwaPliku[20]; 
        sprintf(nazwaPliku, "iter/iter_%d", itnum);
	FILE *file = fopen(nazwaPliku, "w");
 	for(int i=0;i<board.m;i++){
		for (int s=0;s<3;s++){

		for (int j=0;j<board.n;j++)
		{
			if(s==1){
				if (i==board.pos[0] && j==board.pos[1]){
					if(board.data[board.pos[0]][board.pos[1]]==0)
						{
							bialez(kierunek,file);	
						}
					else{
						czarnez(kierunek,file);
					}	
				}
				else
				{
					  if(board.data[i][j]==0)
                                                {
                                                       fprintf(file,"│   │");
                                                }
                                        else{
                                                fprintf(file,"│███│");
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
		}
	fclose(file);
}

