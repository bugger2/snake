#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "position.h"
#include "list.h"

// Width of each tile in pixels
#define TILE_WIDTH 20
#define TILE_HEIGHT TILE_WIDTH

int apple_intersects_snake(Position* apple, List* snake) {
	for(int i = 0; i < snake->size; i++) {
		Position snake_pos = snake->data[i];
		if(snake_pos.x == apple->x && snake_pos.y == apple->y) {
			return 1;
		}
	}
	return 0;
}

Direction tail_direction(List* snake, Direction head_direction) {
	if(snake->size <= 1) return head_direction;
	Position tail_pos = snake->data[snake->size-1];
	Position body_pos = snake->data[snake->size-2];
	if(tail_pos.x - body_pos.x == 1) {
		return LEFT;
	} else if(tail_pos.x - body_pos.x == -1) {
		return RIGHT;
	} else if(tail_pos.y - body_pos.y == 1) {
		return UP;
	} else if(tail_pos.y - body_pos.y == -1) {
		return DOWN;
	} else {
		/* TraceLog(LOG_DEBUG, "heheheheh"); */
		return NOWHERE;
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

	List snake = new_list(1, (Position) {
		.x = GRID_LENGTH/2,
		.y = GRID_HEIGHT/2,
	});

	Direction dir = NOWHERE;

	Position apple = {
		.x = random_x(),
		.y = random_y(),
	};
	Position snake_pos = snake.data[0];
	while(apple.x == snake_pos.x && apple.y == snake_pos.y) {
		randomize_position(&apple);
	}

	/* Main Loop */
	while(!WindowShouldClose() && !(IsKeyDown(KEY_Q) || IsKeyDown(KEY_ESCAPE))) {
		BeginDrawing();

		if(!game_over) {

			ClearBackground(BLACK);

			// Refresh the map
			DrawRectangle(apple.x*TILE_WIDTH, apple.y*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, RED);
			for(int i = 0; i < snake.size; i++) {
				int x = snake.data[i].x;
				int y = snake.data[i].y;
				DrawRectangle(x*TILE_WIDTH, y*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, GREEN);
			}

			// Draw score
			DrawText(TextFormat("Score: %d", score), 10, 10, 18, RAYWHITE);

			// Keybindings
			if((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && (snake.size > 1 ? dir != DOWN : true)) {
				dir = UP;
			}
			if((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && (snake.size > 1 ? dir != RIGHT : true)) {
				dir = LEFT;
			}
			if((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && (snake.size > 1 ? dir != UP : true)) {
				dir = DOWN;
			}
			if((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && (snake.size > 1 ? dir != LEFT : true)) {
				dir = RIGHT;
			}

			Position snake_pos = snake.data[0];

			// Update position
			if(frame_count == 18) {
				Position bookmark_one = snake.data[0];
				snake.data[0] = find_next_pos(&snake_pos, dir);

				if(snake.size > 1) {
					Position bookmark_two = snake.data[1];
					for(int i = 1; i < snake.size-1; i++) {
						snake.data[i] = bookmark_one;
						bookmark_one = bookmark_two;
						bookmark_two = snake.data[i+1];
					}
					snake.data[snake.size-1] = bookmark_one;
				}

				frame_count = 0;
			}

			// Check collision
			if(snake_pos.x < 0 || snake_pos.x > GRID_LENGTH-1 || snake_pos.y < 0 || snake_pos.y > GRID_HEIGHT-1) {
				game_over = 1;
			}

			if(snake_pos.x == apple.x && snake_pos.y == apple.y) {
				score++;
				do {
					randomize_position(&apple);
				} while(apple_intersects_snake(&apple, &snake));

				Direction tail_dir = negate_dir(tail_direction(&snake, dir));
				Position tail_pos = snake.data[snake.size-1];
				Position extra_tail = find_next_pos(&tail_pos, tail_dir);
				add_list(&snake, &extra_tail);

				/* SetTraceLogLevel(LOG_DEBUG); */
				/* for(int i = 0; i < snake.size; i++) { */
				/* 	Position pos = snake.data[i]; */
				/* 	TraceLog(LOG_DEBUG, TextFormat("x: %d, y: %d", pos.x, pos.y)); */
				/* } */
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

			free_list(&snake);
			snake = new_list(1, (Position) {
				.x = GRID_LENGTH/2,
				.y = GRID_HEIGHT/2,
			});

			do {
				randomize_position(&apple);
			} while(apple.x == snake_pos.x && apple.y == snake_pos.y);

			dir = NOWHERE;
		}

		EndDrawing();
	}

	/* End */
	CloseWindow();
	free_list(&snake);

	return 0;
}
