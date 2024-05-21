#ifndef POSITION_H
#define POSITION_H

// Grid dimensions, keep in mind its 0 based
#define GRID_LENGTH 20
#define GRID_HEIGHT GRID_LENGTH

typedef struct {
	int x;
	int y;
} Position;

typedef enum {
	NOWHERE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
} Direction;

int random_x();
int random_y();
void randomize_position(Position* pos);
Position find_next_pos(Position* pos, Direction dir);
Direction negate_dir(Direction dir);

#endif
