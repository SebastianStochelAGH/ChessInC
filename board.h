#ifndef BOARD_H
#define BOARD_H

#include "types.h"

void init_board(Board board);
void print_board(Board board, Color perspective);
void clear_screen();
char get_piece_symbol(Piece piece);

#endif
