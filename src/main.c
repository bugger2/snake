#include "dynarray.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

#define MIN(A, B) (A < B ? A : B)
#define MAX(A, B) (A > B ? A : B)

// Width of each tile in pixels
#define TILE_WIDTH 20

// Grid dimensions, keep in mind its 0 based
#define GRID_LENGTH 20
#define GRID_HEIGHT GRID_LENGTH

int main() {
	/* Raylib Setup */
	InitWindow(GRID_LENGTH * TILE_WIDTH + 1, GRID_HEIGHT * TILE_WIDTH + 1, "Snake");
	SetTargetFPS(144);

	/* Variables */
	int score = 0;

	Rectangle apple = {
		GetRandomValue(0, GRID_LENGTH - 1),
		GetRandomValue(0, GRID_LENGTH - 1),
		TILE_WIDTH,
		TILE_WIDTH,
	};

	typedef struct Player {
		int xHead;
		int yHead;
		Dynarray xTurnPos; // list of x coordinates for turns. The closer to the begining of the array, the more recent the turn
		Dynarray yTurnPos; // list of y coordinates for turns. The closer to the begining of the array, the more recent the turn
		int xTail;
		int yTail;
		int xVel;
		int yVel;

	} Player;
	Player player = {
		player.xHead = (GRID_LENGTH / 2),
		player.yHead = (GRID_HEIGHT / 2),
		player.xTurnPos = dynarray_create(0),
		player.yTurnPos = dynarray_create(0),
		player.xTail = (GRID_LENGTH / 2) + 1,
		player.yTail = (GRID_HEIGHT / 2) + 1,
		player.xVel = 0,
		player.yVel = 0,
	};

	int frame_count = 0;

	int game_over = 0;


	/* Main Loop */

	while(!WindowShouldClose() && !(IsKeyDown(KEY_Q) || IsKeyDown(KEY_ESCAPE))) {
		BeginDrawing();


		if(!game_over) {

			ClearBackground(BLACK);

			// Draw apple
			DrawRectangle(apple.x * TILE_WIDTH, apple.y * TILE_WIDTH, apple.width, apple.height, RED);

			/* Draw player */
			// head
			if(player.xTurnPos.size > 0) {
				DrawRectangle(
							  MIN(player.xHead, player.xTurnPos.ptr[0]) * TILE_WIDTH,
							  MIN(player.yHead, player.yTurnPos.ptr[0]) * TILE_WIDTH,
							  abs(player.xHead - player.xTurnPos.ptr[0]) * TILE_WIDTH,
							  abs(player.yHead - player.yTurnPos.ptr[0]) * TILE_WIDTH,
							  GREEN
							  );
			} else {
				DrawRectangle(
							  MIN(player.xHead, player.xTail) * TILE_WIDTH,
							  MIN(player.yHead, player.yTail) * TILE_WIDTH,
							  abs(player.xHead - player.xTail) * TILE_WIDTH,
							  abs(player.yHead - player.yTail) * TILE_WIDTH,
							  GREEN
							  );
			}

			// body
			for(int i = 0; i < (signed long)player.xTurnPos.size-1; i++) {
				DrawRectangle(
							  MIN(player.xTurnPos.ptr[i], player.xTurnPos.ptr[i+1]) * TILE_WIDTH,
							  MIN(player.yTurnPos.ptr[i], player.yTurnPos.ptr[i+1]) * TILE_WIDTH,
							  abs(player.xTurnPos.ptr[i] - player.xTurnPos.ptr[i+1]) * TILE_WIDTH,
							  abs(player.yTurnPos.ptr[i] - player.yTurnPos.ptr[i+1]) * TILE_WIDTH,
							  GREEN
							  );
			}

			// tail
			if(player.xTurnPos.size > 0) {
				DrawRectangle(
							  MIN(player.xTurnPos.ptr[player.xTurnPos.size-1], player.xTail) * TILE_WIDTH,
							  MIN(player.yTurnPos.ptr[player.yTurnPos.size-1], player.yTail) * TILE_WIDTH,
							  abs(player.xTurnPos.ptr[player.xTurnPos.size-1] - player.xTail) * TILE_WIDTH,
							  abs(player.yTurnPos.ptr[player.yTurnPos.size-1] - player.yTail) * TILE_WIDTH,
							  GREEN
							  );
			}

			// Draw score
			DrawText(TextFormat("Score: %d", score), 10, 10, 18, RAYWHITE);

			// Keybindings
			if((IsKeyDown(KEY_W) || IsKeyDown(KEY_K) || IsKeyDown(KEY_UP)) && (score >= 1 ? player.yVel != 1 : true)) {
				player.yVel = -1;
				player.xVel = 0;
				dynarray_prepend(&player.xTurnPos, player.xHead);
				dynarray_prepend(&player.yTurnPos, player.yHead);
			}
			if((IsKeyDown(KEY_A) || IsKeyDown(KEY_H) || IsKeyDown(KEY_LEFT)) && (score >= 1 ? player.xVel != 1 : true)) {
				player.xVel = -1;
				player.yVel = 0;
				dynarray_prepend(&player.xTurnPos, player.xHead);
				dynarray_prepend(&player.yTurnPos, player.yHead);
			}
			if((IsKeyDown(KEY_S) || IsKeyDown(KEY_J) || IsKeyDown(KEY_DOWN)) && (score >= 1 ? player.yVel != -1 : true)) {
				player.yVel = 1;
				player.xVel = 0;
				dynarray_prepend(&player.xTurnPos, player.xHead);
				dynarray_prepend(&player.yTurnPos, player.yHead);
			}
			if((IsKeyDown(KEY_D) || IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT)) && (score >= 1 ? player.xVel != -1 : true)) {
				player.xVel = 1;
				player.yVel = 0;
				dynarray_prepend(&player.xTurnPos, player.xHead);
				dynarray_prepend(&player.yTurnPos, player.yHead);
			}

			// Update position
			if(frame_count % 18 == 0) {
				
				player.xHead += player.xVel;
				player.yHead += player.yVel;

				player.xTail += player.xVel;
				player.yTail += player.yVel;

				frame_count = 0;
			}

			// Check collision
			if(player.xHead >= GRID_LENGTH || player.xHead < 0 || player.yHead >= GRID_HEIGHT || player.yHead < 0)
				game_over = 1;

			if(score > 0) {
				if(player.xTail == player.xTurnPos.ptr[player.xTurnPos.size])
					dynarray_deleteEnd(&player.xTurnPos);
				if(player.yTail == player.yTurnPos.ptr[player.yTurnPos.size])
					dynarray_deleteEnd(&player.yTurnPos);
			}

			if(player.xHead == apple.x && player.yHead == apple.y) {
				score++;
				apple.x = GetRandomValue(0, GRID_LENGTH - 1);
				apple.y = GetRandomValue(0, GRID_HEIGHT - 1);
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

			player.xHead = GRID_LENGTH / 2;
			player.yHead = GRID_LENGTH / 2;
			player.xTail = player.xHead + 1;
			player.yTail = player.yHead + 1;
			player.xVel = 0;
			player.yVel = 0;

			free(player.xTurnPos.ptr);
			player.xTurnPos.ptr = NULL;
			player.xTurnPos.size = 0;
			
			free(player.yTurnPos.ptr);
			player.yTurnPos.ptr = NULL;
			player.yTurnPos.size = 0;

			apple.x = GetRandomValue(0, GRID_LENGTH - 1);
			apple.y = GetRandomValue(0, GRID_HEIGHT - 1);
		}

		EndDrawing();
	}

	/* End */
	CloseWindow();

	return 0;
}
