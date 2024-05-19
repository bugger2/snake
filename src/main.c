#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "position.h"
#include "list.h"

// Width of each tile in pixels
#define TILE_WIDTH 20
#define TILE_HEIGHT TILE_WIDTH

// Grid dimensions, keep in mind its 0 based
#define GRID_LENGTH 20
#define GRID_HEIGHT GRID_LENGTH

int random_x() {
	return GetRandomValue(0, GRID_LENGTH-1);
}

int random_y() {
	return GetRandomValue(0, GRID_LENGTH-1);
}

void randomize_position(Position* position) {
	position->x = random_x();
	position->y = random_y();
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
			for(int i = 0; i < (int)(sizeof(snake.data)/sizeof(snake.data[0])); i++) {
				int x = snake.data[i].x;
				int y = snake.data[i].y;
				DrawRectangle(x*TILE_WIDTH, y*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, GREEN);
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

			Position snake_pos = snake.data[0];

			// Update position
			if(frame_count % 18 == 0) {
				snake.data[0] = find_next_pos(&snake_pos, dir);

				frame_count = 0;
			}

			// Check collision
			if(snake_pos.x < 0 || snake_pos.x > GRID_LENGTH || snake_pos.y < 0 || snake_pos.y > GRID_HEIGHT) {
				game_over = 1;
			}

			if(snake_pos.x == apple.x && snake_pos.y == apple.y) {
				score++;
				randomize_position(&apple);
				while(apple.x == snake_pos.x && apple.y == snake_pos.y) {
					randomize_position(&apple);
				}
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

			randomize_position(&apple);
			while(apple.x == snake_pos.x && apple.y == snake_pos.y) {
				randomize_position(&apple);
			}

			dir = NOWHERE;
		}

		EndDrawing();
	}

	/* End */
	CloseWindow();

	return 0;
}
