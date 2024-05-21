#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "position.h"
#include "list.h"

// Width of each tile in pixels
#define TILE_WIDTH 20
#define TILE_HEIGHT TILE_WIDTH

#define MOVEMENT_BUFFER_LENGTH 3

int apple_intersects_snake(Position* apple, List* snake) {
	for(int i = 0; i < snake->size; i++) {
		Position snake_pos = snake->data[i];
		if(snake_pos.x == apple->x && snake_pos.y == apple->y) {
			return 1;
		}
	}
	return 0;
}

int snake_intersects_itself(List* snake) {
	Position head = snake->data[0];
	for(int i = 1; i < snake->size; i++) {
		Position body = snake->data[i];
		if(head.x == body.x && head.y == body.y) return 1;
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

void add_direction_to_buffer(Direction* buf, Direction dir) {
	for(int i = 0; i < MOVEMENT_BUFFER_LENGTH; i++) {
		if(buf[i] == NOWHERE) {
			buf[i] = dir;
			return;
		}
	}
}

void consume_direction_from_buffer(Direction* buf) {
	for(int i = 1; i < MOVEMENT_BUFFER_LENGTH; i++) {
		buf[i-1] = buf[i];
	}
	buf[MOVEMENT_BUFFER_LENGTH-1] = NOWHERE;
}

int main() {
	SetTraceLogLevel(LOG_INFO);
	
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

	Direction movement_buffer[MOVEMENT_BUFFER_LENGTH] = {NOWHERE};
	/* for(int i = 0; i < MOVEMENT_BUFFER_LENGTH; i++) { */
	/* 	TraceLog(LOG_DEBUG, TextFormat("%d\n", movement_buffer[i])); */
	/* } */

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
			Rectangle apple_rec = {
				.x = apple.x*TILE_WIDTH,
				.y = apple.y*TILE_HEIGHT,
				.width = TILE_WIDTH,
				.height = TILE_HEIGHT,
			};
			DrawRectangleRec(apple_rec, RED);
			DrawRectangleLinesEx(apple_rec, 3.0f, MAROON);

			for(int i = 0; i < snake.size; i++) {
				int x = snake.data[i].x;
				int y = snake.data[i].y;
				Rectangle snake_rec = {
					.x = x*TILE_WIDTH,
					.y = y*TILE_HEIGHT,
					.width = TILE_WIDTH,
					.height = TILE_HEIGHT,
				};
				DrawRectangleRec(snake_rec, GREEN);
				DrawRectangleLinesEx(snake_rec, 2.0f, DARKGREEN);
			}

			// Draw score
			DrawText(TextFormat("Score: %d", score), 10, 10, 18, RAYWHITE);

			// Keybindings
			Direction dir = movement_buffer[0];
			if((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && (snake.size > 1 ? dir != DOWN : true)) {
				add_direction_to_buffer(movement_buffer, UP);
			}

			if((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && (snake.size > 1 ? dir != RIGHT : true)) {
				add_direction_to_buffer(movement_buffer, LEFT);
			}

			if((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && (snake.size > 1 ? dir != UP : true)) {
				add_direction_to_buffer(movement_buffer, DOWN);
			}

			if((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && (snake.size > 1 ? dir != LEFT : true)) {
				add_direction_to_buffer(movement_buffer, RIGHT);
			}

			// Update position
			Position snake_pos = snake.data[0];

			if(frame_count == 18) {
				Position bookmark_one = snake.data[0];
				snake.data[0] = find_next_pos(&snake_pos, movement_buffer[0]);
				if(movement_buffer[1] != NOWHERE) {
					consume_direction_from_buffer(movement_buffer);
				}

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

			// Check collision with wall and itself
			if(snake_pos.x < 0 || snake_pos.x > GRID_LENGTH-1 || snake_pos.y < 0 || snake_pos.y > GRID_HEIGHT-1
			   || snake_intersects_itself(&snake))
			{
				game_over = 1;
			}

			// Check apple collision
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

			for(int i = 0; i < MOVEMENT_BUFFER_LENGTH; i++) {
				consume_direction_from_buffer(movement_buffer);
			}
		}

		EndDrawing();
	}

	/* End */
	CloseWindow();
	free_list(&snake);

	return 0;
}
