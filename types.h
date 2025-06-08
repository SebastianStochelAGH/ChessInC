#ifndef TYPES_H
#define TYPES_H

typedef enum {
	EMPTY,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
} PieceType;

typedef enum {
	NONE,
	WHITE,
	BLACK
} Color;

typedef struct {
	PieceType type;
	Color color;
} Piece;

typedef struct {
	Color color;
	char name[20];
} Player;

typedef struct {
	int row;
	int col;
} Position;

typedef struct {
	Position from;
	Position to;
} Move;

typedef struct {
	int white_king_moved;
	int white_kingside_rook_moved;
	int white_queenside_rook_moved;

	int black_king_moved;
	int black_kingside_rook_moved;
	int black_queenside_rook_moved;
} CastlingRights;

#define BOARD_SIZE 8

typedef Piece Board[BOARD_SIZE][BOARD_SIZE];

#endif