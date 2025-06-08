#include <ctype.h>
#include <string.h>
#include "input.h"

int parse_move(const char* input, Move* move) {
	if (!input || strlen(input) < 4) return 0;

	char fc = tolower(input[0]);
	char fr = input[1];
	char tc = tolower(input[2]);
	char tr = input[3];

	if (fc < 'a' || fc > 'h' || tc < 'a' || tc > 'h') return 0;
	if (fr < '1' || fr > '8' || tr < '1' || tr > '8') return 0;

	move->from.col = fc - 'a';
	move->from.row = 8 - (fr - '0');
	move->to.col = tc - 'a';
	move->to.row = 8 - (tr - '0');

	return 1;
}
