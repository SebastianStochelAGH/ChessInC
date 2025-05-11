#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <Windows.h>

void printBoard(char board[8][8]) {
	SetConsoleOutputCP(CP_UTF8);
	printf("    A   B   C   D   E   F   G   H\n");
	printf("  +---+---+---+---+---+---+---+---+\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%d | %c | %c | %c | %c | %c | %c | %c | %c |\n", i+1, board[i][0], board[i][1], board[i][2], board[i][3], board[i][4], board[i][5], board[i][6], board[i][7]);
		printf("  +---+---+---+---+---+---+---+---+\n");
	}
}