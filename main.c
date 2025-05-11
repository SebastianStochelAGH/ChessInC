#include <stdio.h>
#include "types.h"
#include "board.h"


int main() {
	printf("Startujemy projekt xD\n");
	char board[8][8] = {
		{'W', 'S', 'G', 'K', 'H', 'G', 'S', 'W'},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},  //tego boarda trzeba zrobiæ lepiej
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},  //plan mam taki ¿eby ka¿dy pionek by³ jakimœ obiektem czy czyms tam i bêdzie mia³ kolor przypisany i te inne tegesy wiesz ocb 
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{'W', 'S', 'G', 'H', 'K', 'G', 'S', 'W'}
	};
	printBoard(board);
	return 0;
}