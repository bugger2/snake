#ifndef POSITION_H
#define POSITION_H

typedef struct {
	int x;
	int y;
} Position;

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NOWHERE
} Direction;

Position find_next_pos(Position* pos, Direction dir);
Direction negate_dir(Direction dir);

#endif
