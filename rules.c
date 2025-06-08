#include "rules.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int is_path_clear(Board board, int fr, int fc, int tr, int tc) {
	int dr = (tr - fr) ? (tr - fr) / abs(tr - fr) : 0;
	int dc = (tc - fc) ? (tc - fc) / abs(tc - fc) : 0;

	int r = fr + dr;
	int c = fc + dc;

	while (r != tr || c != tc) {
		if (board[r][c].type != EMPTY) {
			return 0;
		}
		r += dr;
		c += dc;
	}
	return 1;
}

int is_valid_move(Board board, Move move, Color player, CastlingRights* rights) {
	int fr = move.from.row;
	int fc = move.from.col;
	int tr = move.to.row;
	int tc = move.to.col;

	if (fr == tr && fc == tc) return 0;

	Piece piece = board[fr][fc];
	Piece dest = board[tr][tc];

	if (piece.color != player) return 0;
	if (dest.type != EMPTY && dest.color == player) return 0;

	int dr = tr - fr;
	int dc = tc - fc;

	switch (piece.type) {
	case PAWN:
		if (piece.color == WHITE) {
			if (dc == 0 && dr == -1 && dest.type == EMPTY) return 1;
			if (fc == tc && fr == 6 && tr == 4 && dest.type == EMPTY && board[5][fc].type == EMPTY) return 1;
			if (abs(dc) == 1 && dr == -1 && dest.type != EMPTY && dest.color == BLACK) return 1;
		}
		else {
			if (dc == 0 && dr == 1 && dest.type == EMPTY) return 1;
			if (fc == tc && fr == 1 && tr == 3 && dest.type == EMPTY && board[2][fc].type == EMPTY) return 1;
			if (abs(dc) == 1 && dr == 1 && dest.type != EMPTY && dest.color == WHITE) return 1;
		}
		break;

	case ROOK:
		if (fr == tr || fc == tc) {
			if (is_path_clear(board, fr, fc, tr, tc)) return 1;
		}
		break;

	case BISHOP:
		if (abs(dr) == abs(dc)) {
			if (is_path_clear(board, fr, fc, tr, tc)) return 1;
		}
		break;

	case QUEEN:
		if ((abs(dr) == abs(dc)) || (fr == tr || fc == tc)) {
			if (is_path_clear(board, fr, fc, tr, tc)) return 1;
		}
		break;

	case KNIGHT:
		if ((abs(dr) == 2 && abs(dc) == 1) || (abs(dr) == 1 && abs(dc) == 2)) return 1;
		break;

	case KING:
		if (abs(dr) <= 1 && abs(dc) <= 1)
			return 1;

		if (dr == 0 && abs(dc) == 2) {
			int row = fr;
			Color c = player;

			if (dc == 2) {
				if (
					c == WHITE &&
					row == 7 && fc == 4 &&
					board[7][5].type == EMPTY &&
					board[7][6].type == EMPTY &&
					board[7][7].type == ROOK && board[7][7].color == WHITE &&
					!rights->white_king_moved &&
					!rights->white_kingside_rook_moved &&
					!is_square_attacked(board, (Position) { 7, 4 }, BLACK, rights) &&
					!is_square_attacked(board, (Position) { 7, 5 }, BLACK, rights) &&
					!is_square_attacked(board, (Position) { 7, 6 }, BLACK, rights)
					) return 1;

				if (
					c == BLACK &&
					row == 0 && fc == 4 &&
					board[0][5].type == EMPTY &&
					board[0][6].type == EMPTY &&
					board[0][7].type == ROOK && board[0][7].color == BLACK &&
					!rights->black_king_moved &&
					!rights->black_kingside_rook_moved &&
					!is_square_attacked(board, (Position) { 0, 4 }, WHITE, rights) &&
					!is_square_attacked(board, (Position) { 0, 5 }, WHITE, rights) &&
					!is_square_attacked(board, (Position) { 0, 6 }, WHITE, rights)
					) return 1;
			}

			if (dc == -2) {
				if (
					c == WHITE &&
					row == 7 && fc == 4 &&
					board[7][1].type == EMPTY &&
					board[7][2].type == EMPTY &&
					board[7][3].type == EMPTY &&
					board[7][0].type == ROOK && board[7][0].color == WHITE &&
					!rights->white_king_moved &&
					!rights->white_queenside_rook_moved &&
					!is_square_attacked(board, (Position) { 7, 4 }, BLACK, rights) &&
					!is_square_attacked(board, (Position) { 7, 3 }, BLACK, rights) &&
					!is_square_attacked(board, (Position) { 7, 2 }, BLACK, rights)
					) return 1;

				if (
					c == BLACK &&
					row == 0 && fc == 4 &&
					board[0][1].type == EMPTY &&
					board[0][2].type == EMPTY &&
					board[0][3].type == EMPTY &&
					board[0][0].type == ROOK && board[0][0].color == BLACK &&
					!rights->black_king_moved &&
					!rights->black_queenside_rook_moved &&
					!is_square_attacked(board, (Position) { 0, 4 }, WHITE, rights) &&
					!is_square_attacked(board, (Position) { 0, 3 }, WHITE, rights) &&
					!is_square_attacked(board, (Position) { 0, 2 }, WHITE, rights)
					) return 1;
			}
		}
		break;


	default:
		return 0;
	}

	return 0;
}

int is_legal_move(Board board, Move move, Color player, CastlingRights* rights) {
	if (!is_valid_move(board, move, player, rights)) {
		return 0;
	}

	Piece moving_piece = board[move.from.row][move.from.col];
	Piece captured = board[move.to.row][move.to.col];

	board[move.to.row][move.to.col] = moving_piece;
	board[move.from.row][move.from.col] = (Piece){ EMPTY, NONE };

	int still_in_check = is_in_check(board, player, rights);

	board[move.from.row][move.from.col] = moving_piece;
	board[move.to.row][move.to.col] = captured;

	return !still_in_check;
}


Position find_king_position(Board board, Color color) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			Piece p = board[row][col];
			if (p.type == KING && p.color == color) {
				return (Position) { row, col };
			}
		}
	}
	return (Position) { -1, -1 };
}

int is_square_attacked(Board board, Position target, Color by_color, CastlingRights* rights) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			Piece attacker = board[row][col];
			if (attacker.color != by_color)
				continue;

			Move move = {
				.from = { row, col },
				.to = target
			};

			if (is_valid_move(board, move, by_color, rights)) {
				return 1;
			}
		}
	}
	return 0;
}

int is_in_check(Board board, Color color, CastlingRights* rights) {
	Position king_pos = find_king_position(board, color);
	Color opponent = (color == WHITE) ? BLACK : WHITE;
	return is_square_attacked(board, king_pos, opponent, rights);
}

int has_legal_moves(Board board, Color color, CastlingRights* rights) {
	for (int fr = 0; fr < BOARD_SIZE; fr++) {
		for (int fc = 0; fc < BOARD_SIZE; fc++) {
			Piece piece = board[fr][fc];
			if (piece.color != color) continue;

			for (int tr = 0; tr < BOARD_SIZE; tr++) {
				for (int tc = 0; tc < BOARD_SIZE; tc++) {
					Move move = {
						.from = { fr, fc },
						.to = { tr, tc }
					};

					if (is_legal_move(board, move, color, rights)) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}


int is_checkmate(Board board, Color color, CastlingRights* rights) {
	return is_in_check(board, color, rights) && !has_legal_moves(board, color, rights);
}

int is_stalemate(Board board, Color color, CastlingRights* rights) {
	return !is_in_check(board, color, rights) && !has_legal_moves(board, color, rights);
}

void pawn_promotion(Board board, Move move) {
	Piece pawn = board[move.to.row][move.to.col];

	int promotion_row = (pawn.color == WHITE) ? 0 : 7;
	if (pawn.type != PAWN || move.to.row != promotion_row) {
		return;
	}

	char choice = 'Q';
	printf("Promote pawn to [Q]ueen, [R]ook, [B]ishop, or [N]ight: ");
	scanf_s(" %c", &choice);
	choice = toupper(choice);

	switch (choice) {
	case 'R': pawn.type = ROOK; break;
	case 'B': pawn.type = BISHOP; break;
	case 'N': pawn.type = KNIGHT; break;
	case 'Q':
	default:  pawn.type = QUEEN; break;
	}

	board[move.to.row][move.to.col] = pawn;
}