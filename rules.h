#ifndef RULES_H
#define RULES_H

#include "types.h"

int is_path_clear(Board board, int fr, int fc, int tr, int tc);
int is_valid_move(Board board, Move move, Color player, CastlingRights* rights);
int is_legal_move(Board board, Move move, Color player, CastlingRights* rights);
Position find_king_position(Board board, Color color);
int is_square_attacked(Board board, Position target, Color by_color, CastlingRights* rights);
int is_in_check(Board board, Color color, CastlingRights* rights);
int has_legal_moves(Board board, Color color, CastlingRights* rights);
int is_checkmate(Board board, Color color, CastlingRights* rights);
int is_stalemate(Board board, Color color, CastlingRights* rights);
void pawn_promotion(Board board, Move move);

#endif
