#include "dynarray.h"
#include <stdlib.h>
#include <raylib.h>

#define min(A, B) (A < B ? A : B)
#define max(A, B) (A > B ? A : B)

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
		Dynarray xTurnPos;
		Dynarray yTurnPos;
		int xTail;
		int yTail;
		int xVel;
		int yVel;

	} Player;
	Player player = {
		player.xHead = (GRID_LENGTH / 2),
		player.yHead = (GRID_HEIGHT / 2),
		player.xTurnPos = dynarray_create(0), // list of x coordinates for turns. The closer to the begining of the array, the more recent the turn
		player.yTurnPos = dynarray_create(0), // list of y coordinates for turns. The closer to the begining of the array, the more recent the turn
		player.xTail = (GRID_LENGTH / 2),
		player.yTail = (GRID_HEIGHT / 2),
		player.xVel = 0,
		player.yVel = 0,
	};

	int frame_count = 0;

	int game_over = 0;


	/* Main Loop */
	while(!WindowShouldClose() && !(IsKeyDown(KEY_Q) || IsKeyDown(KEY_ESCAPE) || IsKeyDown(KEY_CAPS_LOCK))) {
		BeginDrawing();


		if(!game_over) {

			ClearBackground(BLACK);

			// Draw apple
			DrawRectangle(apple.x * TILE_WIDTH, apple.y * TILE_WIDTH, apple.width, apple.height, RED);

			// Draw player
			if(score == 0)
				DrawRectangle(player.xHead * TILE_WIDTH, player.yHead * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH, GREEN);
			else
				DrawRectangle(min(player.xHead, player.xTurnPos.ptr[0]) * TILE_WIDTH, min(player.yHead, player.yTurnPos.ptr[0]) * TILE_WIDTH, abs(player.xHead - player.xTurnPos.ptr[0]) > 0 ? TILE_WIDTH * abs(player.xHead - player.xTurnPos.ptr[0]) : TILE_WIDTH, abs(player.yHead - player.yTurnPos.ptr[0]) > 0 ? TILE_WIDTH * abs(player.yHead - player.yTurnPos.ptr[0]) : TILE_WIDTH, GREEN);

			for(unsigned int i = 0; i < player.xTurnPos.size; i++)
				DrawRectangle(min(player.xTurnPos.ptr[i], player.xTurnPos.ptr[i+1]) * TILE_WIDTH, min(player.yTurnPos.ptr[i], player.yTurnPos.ptr[i+1]) * TILE_WIDTH, abs(player.xTurnPos.ptr[i] - player.xTurnPos.ptr[i+1]) > 0 ? TILE_WIDTH * abs(player.xTurnPos.ptr[i] - player.xTurnPos.ptr[i+1]) : TILE_WIDTH, abs(player.yTurnPos.ptr[i] - player.yTurnPos.ptr[i+1]) > 0 ? TILE_WIDTH * abs(player.yTurnPos.ptr[i] - player.yTurnPos.ptr[i+1]) : TILE_WIDTH, GREEN);

			DrawRectangle(min(player.xTurnPos.ptr[player.xTurnPos.size], player.xTail) * TILE_WIDTH, min(player.yTurnPos.ptr[player.yTurnPos.size], player.yTail) * TILE_WIDTH, abs(player.xTail - player.xTurnPos.ptr[player.xTurnPos.size]) > 0 ? TILE_WIDTH * abs(player.xTail - player.xTurnPos.ptr[player.xTurnPos.size]) : TILE_WIDTH, abs(player.yTail - player.yTurnPos.ptr[player.yTurnPos.size]) > 0 ? TILE_WIDTH * abs(player.yTail - player.yTurnPos.ptr[player.yTurnPos.size]) : TILE_WIDTH, GREEN);

			// Draw score
			DrawText(TextFormat("Score: %d", score), 10, 10, 18, RAYWHITE);

			// Keybindings
			if((IsKeyDown(KEY_W) || IsKeyDown(KEY_K) || IsKeyDown(KEY_UP)) && (score >= 1 ? player.yVel != 1 : true)) {
				player.yVel = -1;
				player.xVel = 0;
				player.xTurnPos.prepend(&player.xTurnPos, player.xHead);
				player.yTurnPos.prepend(&player.yTurnPos, player.yHead);
			}
			if((IsKeyDown(KEY_A) || IsKeyDown(KEY_H) || IsKeyDown(KEY_LEFT)) && (score >= 1 ? player.xVel != 1 : true)) {
				player.xVel = -1;
				player.yVel = 0;
				player.xTurnPos.prepend(&player.xTurnPos, player.xHead);
				player.yTurnPos.prepend(&player.yTurnPos, player.yHead);
			}
			if((IsKeyDown(KEY_S) || IsKeyDown(KEY_J) || IsKeyDown(KEY_DOWN)) && (score >= 1 ? player.yVel != -1 : true)) {
				player.yVel = 1;
				player.xVel = 0;
				player.xTurnPos.prepend(&player.xTurnPos, player.xHead);
				player.yTurnPos.prepend(&player.yTurnPos, player.yHead);
			}
			if((IsKeyDown(KEY_D) || IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT)) && (score >= 1 ? player.xVel != -1 : true)) {
				player.xVel = 1;
				player.yVel = 0;
				player.xTurnPos.prepend(&player.xTurnPos, player.xHead);
				player.yTurnPos.prepend(&player.yTurnPos, player.yHead);
			}

			// Update position
			if(frame_count % 18 == 0) {
				player.xHead += player.xVel;
				player.yHead += player.yVel;

				player.xTail += player.xVel;
				player.yTail += player.yVel;

				if(player.xTail == player.xTurnPos.ptr[player.xTurnPos.size])
					player.xTurnPos.deleteEnd(&player.xTurnPos);
				if(player.yTail == player.yTurnPos.ptr[player.yTurnPos.size])
					player.yTurnPos.deleteEnd(&player.yTurnPos);
			}

			// Check collision
			if(player.xHead >= GRID_LENGTH || player.xHead < 0 || player.yHead >= GRID_HEIGHT || player.yHead < 0)
				game_over = 1;

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
			player.xTail = player.xHead;
			player.yTail = player.yHead;
			player.xVel = 0;
			player.yVel = 0;

			free(player.xTurnPos.ptr);
			player.xTurnPos.ptr = malloc(0);
			free(player.yTurnPos.ptr);
			player.yTurnPos.ptr = malloc(0);

			apple.x = GetRandomValue(0, GRID_LENGTH - 1);
			apple.y = GetRandomValue(0, GRID_HEIGHT - 1);
		}

		EndDrawing();
	}

	/* End */
	CloseWindow();

	return 0;
}