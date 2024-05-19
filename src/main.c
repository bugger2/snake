#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define MIN(A, B) (A < B ? A : B)
#define MAX(A, B) (A > B ? A : B)

// Width of each tile in pixels
#define TILE_WIDTH 20
#define TILE_HEIGHT TILE_WIDTH

// Grid dimensions, keep in mind its 0 based
#define GRID_LENGTH 20
#define GRID_HEIGHT GRID_LENGTH

static int color_equal(Color color_a, Color color_b) {
	return color_a.r == color_b.r
		   && color_a.g == color_b.g
		   && color_a.b == color_b.b
		   && color_a.a == color_b.a;
}

int random_x() {
	return GetRandomValue(0, GRID_LENGTH-1);
}

int random_y() {
	return GetRandomValue(0, GRID_LENGTH-1);
}

typedef struct {
	int x;
	int y;
} Position;

void randomize_position(Position* position) {
	position->x = random_x();
	position->y = random_y();
}

void randomize_and_avoid_position(Position* position, Position* obstacle) {
	randomize_position(position);
	while(position->x == obstacle->x && position->y == obstacle->y) {
		randomize_position(position);
	}
}

int main() {
	/* Raylib Setup */
	InitWindow(GRID_LENGTH * TILE_WIDTH + 1, GRID_HEIGHT * TILE_WIDTH + 1, "Snake");
	SetTargetFPS(144);

	/* Variables */
	int score = 0;
	int frame_count = 0;
	int game_over = 0;

	enum TileState {
		EMPTY = 0, // 00
		APPLE = 1, // 01
		SNAKE = 2, // 10
		SNAKE_OVER_APPLE = SNAKE | APPLE // 11
	};

	short int map[GRID_LENGTH][GRID_HEIGHT] = {EMPTY};

	Position snake = {
		.x = GRID_LENGTH/2,
		.y = GRID_HEIGHT/2,
	};
	map[snake.x][snake.y] = SNAKE;

	Position apple = {
		.x = random_x(),
		.y = random_y(),
	};
	while(snake.x == apple.x && snake.y == apple.y) {
		apple.x = random_x();
		apple.y = random_y();
	}
	map[apple.x][apple.y] = APPLE;

	typedef enum {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		NOWHERE
	} Direction;
	Direction dir = NOWHERE;

	/* Main Loop */
	while(!WindowShouldClose() && !(IsKeyDown(KEY_Q) || IsKeyDown(KEY_ESCAPE))) {
		BeginDrawing();

		if(!game_over) {

			ClearBackground(BLACK);

			// Refresh the map
			Color tile_color;
			for(int i = 0; i < GRID_LENGTH; i++) {
				for(int j = 0; j < GRID_HEIGHT; j++) {
					switch(map[i][j]) {
						case EMPTY:
							tile_color = BLACK;
							break;
						case APPLE:
							tile_color = RED;
							break;
						case SNAKE:
						case SNAKE_OVER_APPLE:
							tile_color = GREEN;
							break;
					}

					if(!color_equal(tile_color, BLACK)) {
						int x = i * TILE_WIDTH;
						int y = j * TILE_HEIGHT;
						DrawRectangle(x, y, TILE_WIDTH, TILE_HEIGHT, tile_color);
					}
				}
			}
			

			// Draw score
			DrawText(TextFormat("Score: %d", score), 10, 10, 18, RAYWHITE);

			// Keybindings
			int snake_is_lengthy = score > 0;
			if((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && (snake_is_lengthy ? dir != DOWN : true)) {
				dir = UP;
			}
			if((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && (snake_is_lengthy ? dir != RIGHT : true)) {
				dir = LEFT;
			}
			if((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && (snake_is_lengthy ? dir != UP : true)) {
				dir = DOWN;
			}
			if((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && (snake_is_lengthy ? dir != LEFT : true)) {
				dir = RIGHT;
			}

			// Update position
			if(frame_count % 18 == 0) {
				map[snake.x][snake.y] = EMPTY;

				switch(dir) {
					case NOWHERE:
						break;
					case UP:
						snake.y--;
						break;
					case DOWN:
						snake.y++;
						break;
					case LEFT:
						snake.x--;
						break;
					case RIGHT:
						snake.x++;
						break;
				}

				map[snake.x][snake.y] = SNAKE;

				frame_count = 0;
			}

			// Check collision
			if(snake.x >= GRID_LENGTH || snake.x < 0 || snake.y >= GRID_HEIGHT || snake.y < 0)
				game_over = 1;

			if(snake.x == apple.x && snake.y == apple.y) {
				score++;
				randomize_and_avoid_position(&apple, &snake);
				map[apple.x][apple.y] = APPLE;
			}

			
			// Increment frame count
			frame_count++;

		}

		// Game over
		if(game_over) {
			DrawText("You Lost", ((GRID_LENGTH * TILE_WIDTH) / 2) - 60, ((GRID_HEIGHT * TILE_WIDTH) / 2) - 60, 30, RAYWHITE);
			DrawText("Press 'r' to Restart", ((GRID_LENGTH * TILE_WIDTH) / 2) - 90, ((GRID_HEIGHT * TILE_WIDTH) / 2) - 30, 20, RAYWHITE);
		}

		if(IsKeyPressed(KEY_R)) {
			game_over = 0;
			score = 0;

			map[snake.x][snake.y] = EMPTY;
			map[apple.x][apple.y] = EMPTY;

			dir = NOWHERE;

			snake.x = GRID_LENGTH/2;
			snake.y = GRID_HEIGHT/2;
			map[snake.x][snake.y] = SNAKE;

			randomize_and_avoid_position(&apple, &snake);
			map[apple.x][apple.y] = APPLE;
		}

		EndDrawing();
	}

	/* End */
	CloseWindow();

	return 0;
}
