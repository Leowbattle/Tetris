#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;

#define BLOCKS_X 10
#define BLOCKS_Y 20

#define BLOCK_SIZE 40

#define WIDTH (BLOCKS_X * BLOCK_SIZE)
#define HEIGHT (BLOCKS_Y * BLOCK_SIZE)

struct BlockRotation {
	bool vals[4][4];
};

struct BlockDef {
	SDL_Color colour;
	struct BlockRotation rotations[4];
};

#define NUM_BLOCKS 7

const struct BlockDef BLOCKS[NUM_BLOCKS] = {
	// O piece
	{
		.colour = {0xff, 0xff, 0x00, 0xff},
		.rotations = {
			{{
				{1, 1, 0, 0},
				{1, 1, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{1, 1, 0, 0},
				{1, 1, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{1, 1, 0, 0},
				{1, 1, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{1, 1, 0, 0},
				{1, 1, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
		}
	},
	// I piece
	{
		.colour = {0x00, 0xff, 0xff, 0xff},
		.rotations = {
			{{
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 0, 0},
			}},
			{{
				{0, 0, 0, 0},
				{1, 1, 1, 1},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 0, 1, 0},
				{0, 0, 1, 0},
				{0, 0, 1, 0},
				{0, 0, 1, 0},
			}},
			{{
				{0, 0, 0, 0},
				{1, 1, 1, 1},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
		}
	},
	// T piece
	{
		.colour = {0xff, 0x00, 0xff, 0xff},
		.rotations = {
			{{
				{0, 1, 0, 0},
				{1, 1, 1, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 1, 0, 0},
				{0, 1, 1, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 0, 0, 0},
				{1, 1, 1, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 1, 0, 0},
				{1, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
		}
	},
	// L piece
	{
		.colour = {0x50, 0x00, 0xff, 0xff},
		.rotations = {
			{{
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 1, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 0, 0, 0},
				{1, 1, 1, 0},
				{1, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{1, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 0, 1, 0},
				{1, 1, 1, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
		}
	},
	// J piece
	{
		.colour = {0xff, 0xbb, 0x00, 0xff},
		.rotations = {
			{{
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{1, 1, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{1, 0, 0, 0},
				{1, 1, 1, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 1, 1, 0},
				{0, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 0, 0, 0},
				{1, 1, 1, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 0},
			}},
		}
	},
	// S piece
	{
		.colour = {0x00, 0xff, 0x00, 0xff},
		.rotations = {
			{{
				{0, 1, 1, 0},
				{1, 1, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{1, 0, 0, 0},
				{1, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 1, 1, 0},
				{1, 1, 0, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{1, 0, 0, 0},
				{1, 1, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
		}
	},
	// Z piece
	{
		.colour = {0xff, 0x00, 0x00, 0xff},
		.rotations = {
			{{
				{1, 1, 0, 0},
				{0, 1, 1, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 0, 1, 0},
				{0, 1, 1, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{1, 1, 0, 0},
				{0, 1, 1, 0},
				{0, 0, 0, 0},
				{0, 0, 0, 0},
			}},
			{{
				{0, 0, 1, 0},
				{0, 1, 1, 0},
				{0, 1, 0, 0},
				{0, 0, 0, 0},
			}},
		}
	},
};

struct CurrentBlock {
	int x;
	int y;

	enum {
		O_PIECE = 0,
		I_PIECE = 1,
		T_PIECE = 2,
		L_PIECE = 3,
		J_PIECE = 4,
		S_PIECE = 5,
		Z_PIECE = 6,
	} type;

	int rotation;

	// How much to move on this frame.
	int dx;
	int dy;

	// How much to rotate on this frame.
	int dr;
} currentBlock;

struct Cell {
	bool solid;
	SDL_Color colour;
};

struct {
	struct Cell cells[BLOCKS_X][BLOCKS_Y];
} board = { 0 };

// Time left until block moves down
int blockTimer = 1000;

// Time between each time the block moves down
int blockTimerLength = 1000;

// Time left until block is permanently placed
int placementTimer = 1000;
const int placementTimerLength = 1000;

void update();
void selectPiece();
void placeCurrent();
void dropCurrent();
bool tryMove();

void draw();

int time = 0;
int lastTime = 0;
int dt = 0;

enum {
	GAME_RUN
} gameState;

int keysLen;
Uint8* keys = NULL;
Uint8* lastKeys = NULL;

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(
		"Tetris",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH,
		HEIGHT,
		0);

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//printf("%d\n", SDL_GetTicks());
	srand(SDL_GetTicks());

	selectPiece();

	while (true) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				goto end;

			default:
				break;
			}
		}

		if (lastKeys == NULL) {
			Uint8* keys1 = SDL_GetKeyboardState(&keysLen);
			keys = malloc(keysLen);
			lastKeys = malloc(keysLen);
			memset(keys, 0, keysLen);
			memset(lastKeys, 0, keysLen);
		}
		else {
			memcpy(lastKeys, keys, keysLen);

			Uint8* keys1 = SDL_GetKeyboardState(NULL);
			memcpy(keys, keys1, keysLen);
		}

		lastTime = time;
		time = SDL_GetTicks();
		dt = time - lastTime;

		update();
		draw();
	}

end:;

	return 0;
}

bool tryRotate();
bool checkResting();

void update() {
#define MOVEMENT_TIMER_LENGTH 100
	if (keys[SDL_SCANCODE_LEFT]) {
		static int lastLeft = 0;

		if ((time - lastLeft) > MOVEMENT_TIMER_LENGTH || !lastKeys[SDL_SCANCODE_LEFT]) {
			currentBlock.dx--;
			lastLeft = time;
		}
	}

	if (keys[SDL_SCANCODE_RIGHT]) {
		static int lastRight = 0;

		if ((time - lastRight) > MOVEMENT_TIMER_LENGTH || !lastKeys[SDL_SCANCODE_RIGHT]) {
			currentBlock.dx++;
			lastRight = time;
		}
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		static int lastDown = 0;

		if ((time - lastDown) > MOVEMENT_TIMER_LENGTH || !lastKeys[SDL_SCANCODE_DOWN]) {
			currentBlock.dy++;
			blockTimer = blockTimerLength;
			lastDown = time;
		}
	}

#define ROT_TIMER_LENGTH 250
	if (keys[SDL_SCANCODE_R]) {
		static int lastRotR = 0;

		if ((time - lastRotR) > ROT_TIMER_LENGTH || !lastKeys[SDL_SCANCODE_R]) {
			currentBlock.dr++;
			blockTimer = blockTimerLength;
			lastRotR = time;
		}
	}
	if (keys[SDL_SCANCODE_E]) {
		static int lastRotL = 0;

		if ((time - lastRotL) > ROT_TIMER_LENGTH || !lastKeys[SDL_SCANCODE_E]) {
			currentBlock.dr--;
			blockTimer = blockTimerLength;
			lastRotL = time;
		}
	}

	// The user must press space seperately for each hard drop.
	if (keys[SDL_SCANCODE_SPACE] && !lastKeys[SDL_SCANCODE_SPACE]) {
		dropCurrent();
	}

	blockTimer -= dt;
	if (blockTimer < 0) {
		blockTimer = blockTimerLength;

		currentBlock.dy++;
	}

	if (currentBlock.dx != 0 || currentBlock.dy != 0 || currentBlock.dr != 0) {
		tryMove();
	}
	if (currentBlock.dr != 0) {
		tryRotate();
	}

	if (checkResting()) {
		placementTimer -= dt;
		//printf("%d\n", placementTimer);

		if (placementTimer < 0) {
			placementTimer = placementTimerLength;
			placeCurrent();
		}
	}
}

void dropCurrent() {
	bool success;
	do {
		currentBlock.dy++;
		success = tryMove();
	} while (success);
	placeCurrent();
	placementTimer = placementTimerLength;
}

bool checkResting() {
	struct BlockDef* block = &BLOCKS[currentBlock.type];
	struct BlockRotation* br = &block->rotations[currentBlock.rotation];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = j + currentBlock.x + currentBlock.dx;
			int y = i + currentBlock.y + currentBlock.dy;

			if (!br->vals[i][j]) {
				continue;
			}

			// Non - inclusion of check for y >= 0 intentional
			bool inRange = x >= 0 && x < BLOCKS_X&& y < BLOCKS_Y;

			bool resting = y == BLOCKS_Y - 1;
			if (inRange && y < (BLOCKS_Y - 1)) {
				resting |= board.cells[x][y + 1].solid;
			}

			if (resting) {
				return true;
			}
		}
	}

	return false;
}

void selectPiece() {
	currentBlock.x = BLOCKS_X / 2 - 1;
	currentBlock.y = 0;
	currentBlock.rotation = 0;

	int type = rand() % 7;

	currentBlock.type = type;
}

void checkForLines();

void placeCurrent() {
	blockTimer = blockTimerLength;

	struct BlockDef* block = &BLOCKS[currentBlock.type];
	struct BlockRotation* br = &block->rotations[currentBlock.rotation];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = j + currentBlock.x + currentBlock.dx;
			int y = i + currentBlock.y + currentBlock.dy;

			if (br->vals[i][j]) {
				struct Cell* cell = &board.cells[x][y];
				cell->solid = true;
				cell->colour = block->colour;
			}
		}
	}

	checkForLines();

	selectPiece();
}

void removeLine(int y);

void checkForLines() {
	struct BlockDef* block = &BLOCKS[currentBlock.type];
	struct BlockRotation* br = &block->rotations[currentBlock.rotation];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = j + currentBlock.x + currentBlock.dx;
			int y = i + currentBlock.y + currentBlock.dy;

			if (br->vals[i][j]) {
				for (int x2 = 0; x2 < BLOCKS_X; x2++) {
					if (!board.cells[x2][y].solid) {
						goto nextLine;
					}
				}

				removeLine(y);

				goto nextLine;
			}
		}
	nextLine:;
	}
}

void removeLine(int y) {
	//printf("%d\n", y);

	for (int x = 0; x < BLOCKS_X; x++) {
		board.cells[x][y].solid = false;
	}

	for (int y2 = y; y2 > 0; y2--) {
		for (int x = 0; x < BLOCKS_X; x++) {
			board.cells[x][y2] = board.cells[x][y2 - 1];
			board.cells[x][y2 - 1].solid = false;
		}
	}
	for (int x = 0; x < BLOCKS_X; x++) {
		board.cells[x][0].solid = false;
	}
}

bool tryMove() {
	struct BlockDef* block = &BLOCKS[currentBlock.type];
	struct BlockRotation* br = &block->rotations[currentBlock.rotation];

	bool success = true;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = j + currentBlock.x + currentBlock.dx;
			int y = i + currentBlock.y + currentBlock.dy;

			// Non - inclusion of check for y >= 0 intentional
			bool inRange = x >= 0 && x < BLOCKS_X&& y < BLOCKS_Y;

			bool blocked = false;
			if (inRange) {
				blocked = board.cells[x][y].solid;
			}

			if (br->vals[i][j] && (!inRange || blocked)) {
				success = false;
				goto moveFailed;
			}
		}
	}

	currentBlock.x += currentBlock.dx;
	currentBlock.y += currentBlock.dy;

moveFailed:;
	currentBlock.dx = 0;
	currentBlock.dy = 0;

	return success;
}

bool tryRotate() {
	bool success = true;

	int newRot = currentBlock.rotation + currentBlock.dr;
	if (newRot == -1) {
		newRot = 3;
	}
	else if (newRot == 4) {
		newRot = 0;
	}

	struct BlockDef* block = &BLOCKS[currentBlock.type];
	struct BlockRotation* br = &block->rotations[newRot];

	int oldRot = currentBlock.rotation;
	currentBlock.rotation = newRot;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = j + currentBlock.x + currentBlock.dx;
			int y = i + currentBlock.y + currentBlock.dy;

			// Non - inclusion of check for y >= 0 intentional
			bool inRange = x >= 0 && x < BLOCKS_X&& y < BLOCKS_Y;

			bool blocked = false;
			if (inRange) {
				blocked = board.cells[x][y].solid;
			}

			if (br->vals[i][j] && (!inRange || blocked)) {
				//printf("%d %d\n", x, y);

				success = false;

				currentBlock.dx++;
				success = tryMove();
				if (success) {
					continue;
				}
				currentBlock.dx += 2;
				success = tryMove();
				if (success) {
					continue;
				}

				currentBlock.dx--;
				success = tryMove();
				if (success) {
					continue;
				}
				currentBlock.dx -= 2;
				success = tryMove();
				if (success) {
					continue;
				}

				currentBlock.dy--;
				success = tryMove();
				if (success) {
					continue;
				}
				currentBlock.dy -= 2;
				success = tryMove();
				if (success) {
					continue;
				}

				if (!success) {
					currentBlock.rotation = oldRot;
					goto rotationFailed;
				}
			}
		}
	}

rotationFailed:;
	currentBlock.dr = 0;

	return success;
}

void drawBoard();
void drawCurrent();

void draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderClear(renderer);

	drawBoard();
	drawCurrent();

	SDL_RenderPresent(renderer);
}

void drawBoard() {
	for (int i = 0; i < BLOCKS_Y; i++) {
		for (int j = 0; j < BLOCKS_X; j++) {
			int x = j * BLOCK_SIZE;
			int y = i * BLOCK_SIZE;

			SDL_Rect rect = { x, y, BLOCK_SIZE, BLOCK_SIZE };
			SDL_Color colour = { 0xff, 0xff, 0xff, 0xff };

			SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
			SDL_RenderDrawRect(renderer, &rect);

			struct Cell* cell = &board.cells[j][i];

			if (cell->solid) {
				SDL_Color colour = cell->colour;

				SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

void drawCurrent() {
	struct BlockDef* block = &BLOCKS[currentBlock.type];
	struct BlockRotation* br = &block->rotations[currentBlock.rotation];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = currentBlock.x + j;
			int y = currentBlock.y + i;

			if (x < 0 || x >= BLOCKS_X || y < 0 || y >= BLOCKS_Y) {
				continue;
			}

			if (br->vals[i][j]) {
				SDL_Color colour = block->colour;

				SDL_Rect rect = { x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
				SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	// Draw ghost block

	int y = currentBlock.y;
	bool success;
	do {
		currentBlock.dy++;
		success = tryMove();
	} while (success);
	int bottom = currentBlock.y;
	currentBlock.y = y;

	//printf("%d\n", bottom);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = currentBlock.x + j;
			int y = bottom + i;

			if (x < 0 || x >= BLOCKS_X || y < 0 || y >= BLOCKS_Y) {
				continue;
			}

			if (br->vals[i][j]) {
				SDL_Color colour = { 0xff, 0xff, 0xff, 0xff };

				SDL_Rect rect = { x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
				SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}