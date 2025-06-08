#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void clear_screen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

char get_piece_symbol(Piece piece) {
	if (piece.type == EMPTY) return '.';

	char base;
	switch (piece.type) {
	case PAWN:   base = 'P'; break;
	case KNIGHT: base = 'N'; break;
	case BISHOP: base = 'B'; break;
	case ROOK:   base = 'R'; break;
	case QUEEN:  base = 'Q'; break;
	case KING:   base = 'K'; break;
	default:     base = '?'; break;
	}

	return (piece.color == WHITE) ? base : (char)(base + 32);
}


void print_board(Board board, Color perspective) {
	clear_screen();

	int row_start = (perspective == WHITE) ? 0 : 7;
	int row_end = (perspective == WHITE) ? 8 : -1;
	int step = (perspective == WHITE) ? 1 : -1;

	printf("\n    a   b   c   d   e   f   g   h\n");
	printf("  +---+---+---+---+---+---+---+---+\n");

	for (int i = row_start; i != row_end; i += step) {
		printf("%d |", 8 - i);

		for (int j = 0; j < 8; j++) {
			Piece p = board[i][j];
			printf(" %c |", get_piece_symbol(p));
		}

		printf(" %d\n", 8 - i);
		printf("  +---+---+---+---+---+---+---+---+\n");
	}

	printf("    a   b   c   d   e   f   g   h\n\n");
}

void init_board(Board board) {
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			board[i][j] = (Piece){ EMPTY, NONE };

	Piece back_rank[8] = {
		{ROOK, WHITE}, {KNIGHT, WHITE}, {BISHOP, WHITE}, {QUEEN, WHITE},
		{KING, WHITE}, {BISHOP, WHITE}, {KNIGHT, WHITE}, {ROOK, WHITE}
	};

	for (int i = 0; i < 8; ++i) {
		board[7][i] = back_rank[i];
		board[6][i] = (Piece){ PAWN, WHITE };
		board[1][i] = (Piece){ PAWN, BLACK };
		board[0][i] = (Piece){ back_rank[i].type, BLACK };
	}
}
