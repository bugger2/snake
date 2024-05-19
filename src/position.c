#include "position.h"

Position find_next_pos(Position* pos, Direction dir) {
	Position ret = {
		pos->x,
		pos->y,
	};

	switch(dir) {
		case UP:
			ret.y--;
			break;
		case DOWN:
			ret.y++;
			break;
		case LEFT:
			ret.x--;
			break;
		case RIGHT:
			ret.x++;
			break;
		case NOWHERE:
			break;
	};

	return ret;
}

Direction negate_dir(Direction dir) {
	switch(dir) {
		case UP: return DOWN;
		case DOWN: return UP;
		case LEFT: return RIGHT;
		case RIGHT: return LEFT;
		case NOWHERE: return NOWHERE;
	};

	return NOWHERE;
}
