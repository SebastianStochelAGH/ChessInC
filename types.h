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

#define BOARD_SIZE 8

typedef Piece Board[BOARD_SIZE][BOARD_SIZE];

#endif
#pragma once
