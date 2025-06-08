#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "board.h"
#include "rules.h"
#include "input.h"

int main() {
	Board board;
	init_board(board);

	Player white = { WHITE, "Player White" };
	Player black = { BLACK, "Player Black" };
	Player* current_player = &white;
	CastlingRights rights = { 0 };

	char input[10];
	Move move;
	char status_msg[128] = "";

	while (1) {
		print_board(board, current_player->color);

		if (strlen(status_msg) > 0) {
			printf(">> %s\n\n", status_msg);
			status_msg[0] = '\0';
		}
		printf("%s to move (e.g., e2e4): ", current_player->name);

		if (!fgets(input, sizeof(input), stdin)) {
			snprintf(status_msg, sizeof(status_msg), "Error reading input.");
			continue;
		}

		input[strcspn(input, "\n")] = 0;

		if (!parse_move(input, &move)) {
			snprintf(status_msg, sizeof(status_msg), "Invalid input format. Use e.g. e2e4.");
			continue;
		}

		Piece piece = board[move.from.row][move.from.col];

		if (piece.color != current_player->color) {
			snprintf(status_msg, sizeof(status_msg), "Illegal move: You must move your own piece.");
			continue;
		}

		if (!is_legal_move(board, move, current_player->color, &rights)) {
			snprintf(status_msg, sizeof(status_msg), "Illegal move according to piece rules.");
			continue;
		}

		Player* moving_player = current_player;
		Player* opponent = (current_player == &white) ? &black : &white;
		Color opponent_color = opponent->color;

		Piece moved_piece = piece;
		Piece captured_piece = board[move.to.row][move.to.col];

		board[move.to.row][move.to.col] = moved_piece;
		board[move.from.row][move.from.col] = (Piece){ EMPTY, NONE };

		if (moved_piece.type == KING && abs(move.to.col - move.from.col) == 2) {
			int row = move.from.row;
			if (move.to.col == 6) {
				board[row][5] = board[row][7];
				board[row][7] = (Piece){ EMPTY, NONE };
			}
			else if (move.to.col == 2) {
				board[row][3] = board[row][0];
				board[row][0] = (Piece){ EMPTY, NONE };
			}
		}

		if (moved_piece.type == KING) {
			if (moved_piece.color == WHITE) rights.white_king_moved = 1;
			else rights.black_king_moved = 1;
		}

		if (moved_piece.type == ROOK) {
			if (moved_piece.color == WHITE) {
				if (move.from.row == 7 && move.from.col == 0)
					rights.white_queenside_rook_moved = 1;
				if (move.from.row == 7 && move.from.col == 7)
					rights.white_kingside_rook_moved = 1;
			}
			if (moved_piece.color == BLACK) {
				if (move.from.row == 0 && move.from.col == 0)
					rights.black_queenside_rook_moved = 1;
				if (move.from.row == 0 && move.from.col == 7)
					rights.black_kingside_rook_moved = 1;
			}
		}

		pawn_promotion(board, move);

		if (is_checkmate(board, opponent_color, &rights)) {
			print_board(board, moving_player->color);
			printf(">> Checkmate! %s wins.\n", moving_player->name);
			break;
		}

		if (is_stalemate(board, opponent_color, &rights)) {
			print_board(board, moving_player->color);
			printf(">> Stalemate! The game is a draw.\n");
			break;
		}

		if (is_in_check(board, opponent_color, &rights)) {
			snprintf(status_msg, sizeof(status_msg), "%s is in check!", opponent->name);
		}

		current_player = opponent;
	}

	return 0;
}
