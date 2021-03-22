#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window* window;
SDL_Renderer* renderer;

TTF_Font* font_big;
TTF_Font* font_small;
void loadFont();

enum TextAlignment {
	TEXT_ALIGN_LEFT = 0,
	TEXT_ALIGN_BELOW = 0,
	TEXT_ALIGN_CENTRE = 1,
	TEXT_ALIGN_RIGHT = 2,
	TEXT_ALIGN_ABOVE = 2,
};

struct DrawStringInfo {
	TTF_Font* font;
	SDL_Color colour;
	int x;
	int y;

	enum TextAlignment alignX;
	enum TextAlignment alignY;
};

void drawString(struct DrawStringInfo* dsi, const char* msg);
void drawStringf(struct DrawStringInfo* dsi, const char* fmt, ...);

#define BLOCKS_X 10
#define BLOCKS_Y 20

#define BLOCK_SIZE 40

#define BOARD_WIDTH (BLOCKS_X * BLOCK_SIZE)
#define BOARD_HEIGHT (BLOCKS_Y * BLOCK_SIZE)

#define WINDOW_WIDTH (BOARD_WIDTH * 2)
#define WINDOW_HEIGHT BOARD_HEIGHT

#define BOARD_LEFT ((WINDOW_WIDTH - BOARD_WIDTH) / 2)
#define BOARD_RIGHT (BOARD_LEFT + BOARD_WIDTH)

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
		.colour = {0xf0, 0xd9, 0x11, 0xff},
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
		.colour = {0x15, 0xb7, 0xe8, 0xff},
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
		.colour = {0xa8, 0x0b, 0xbd, 0xff},
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
		.colour = {0x1f, 0x50, 0xab, 0xff},
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
		.colour = {0xf0, 0xa9, 0x11, 0xff},
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
		.colour = {0x25, 0xcf, 0x60, 0xff},
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
		.colour = {0xff, 0x25, 0x25, 0xff},
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

enum PieceType {
	O_PIECE = 0,
	I_PIECE = 1,
	T_PIECE = 2,
	L_PIECE = 3,
	J_PIECE = 4,
	S_PIECE = 5,
	Z_PIECE = 6,
};

struct CurrentBlock {
	int x;
	int y;

	enum PieceType type;

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

int lines = 0;
int level = 1;

// Time left until block moves down
int blockTimer = 1000;

// Time between each time the block moves down
int blockTimerLength = 1000;

// Time left until block is permanently placed
int placementTimer = 1000;
const int placementTimerLength = 1000;

void GAME_PAUSED_update();
void GAME_PAUSED_draw();

void GAME_RUN_update();
void selectPiece();
void enqueuePiece();
void placeCurrent();
void dropCurrent();
bool tryMove();
void removeLine(int y);

void GAME_RUN_draw();
void drawBoard();
void drawCurrent();
void drawPieceQueue();
void drawHeldPiece();
void drawScore();

void GAME_OVER_update();

void GAME_LINE_CLEAR_update();

#define QUEUE_LENGTH 4
enum PieceType pieceQueue[QUEUE_LENGTH];

int time = 0;
int lastTime = 0;
int dt = 0;

enum {
	GAME_PAUSED,
	GAME_RUN,
	GAME_LINE_CLEAR,
	GAME_OVER
} gameState = GAME_RUN;

int keysLen;
Uint8* keys = NULL;
Uint8* lastKeys = NULL;

bool keyPressed(int scancode) {
	return keys[scancode] && !lastKeys[scancode];
}

bool mainLoop(double _emTime, void* _emUserData) {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return false;

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

	switch (gameState) {
	case GAME_PAUSED:
		GAME_PAUSED_update();
		break;
	case GAME_RUN:
		GAME_RUN_update();
		break;
	case GAME_LINE_CLEAR:
		GAME_LINE_CLEAR_update();
		break;
	case GAME_OVER:
		GAME_OVER_update();
		break;
	default:
		printf("Invalid game state\n");
		exit(-1);
	}

	return true;
}

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	window = SDL_CreateWindow(
		"Tetris",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0);

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	loadFont();

	//printf("%d\n", SDL_GetTicks());
	srand(SDL_GetTicks());

	selectPiece();

#ifdef __EMSCRIPTEN__
	emscripten_request_animation_frame_loop(mainLoop, 0);
#else
	while (true) {
		if (!mainLoop(0, NULL)) {
			break;
		}
	}
#endif

	return 0;
}

const SDL_Colour backgroundColour = { 0xf5, 0xf5, 0xf5, 0xff };

bool unpausing = false;
int unpauseTimer;
int unpauseCounter;
void GAME_PAUSED_update() {
	if (keyPressed(SDL_SCANCODE_ESCAPE)) {
		unpausing = true;
		unpauseTimer = 1000;
		unpauseCounter = 3;
	}

	if (unpausing) {
		unpauseTimer -= dt;
		if (unpauseTimer <= 0) {
			unpauseTimer = 1000;
			unpauseCounter--;
		}

		if (unpauseCounter <= 0) {
			gameState = GAME_RUN;
			unpausing = false;
			return;
		}
	}

	GAME_PAUSED_draw();
}

void GAME_PAUSED_draw() {
	SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
	SDL_RenderClear(renderer);

	drawBoard();
	drawCurrent();
	drawPieceQueue();
	drawHeldPiece();
	drawScore();

	if (unpausing) {
		struct DrawStringInfo dsi = {
			.font = font_big,
			.colour = {0xff, 0xff, 0xff, 0xff},
			.x = BOARD_LEFT + BOARD_WIDTH / 2,
			.y = BOARD_HEIGHT / 2,
			.alignX = TEXT_ALIGN_CENTRE,
			.alignY = TEXT_ALIGN_CENTRE,
		};
		drawStringf(&dsi, "%d", unpauseCounter);
	}
	else {
		struct DrawStringInfo dsi = {
			.font = font_big,
			.colour = {0xff, 0xff, 0xff, 0xff},
			.x = BOARD_LEFT + BOARD_WIDTH / 2,
			.y = BOARD_HEIGHT / 2,
			.alignX = TEXT_ALIGN_CENTRE,
			.alignY = TEXT_ALIGN_CENTRE,
		};
		drawString(&dsi, "Paused");
	}

	SDL_RenderPresent(renderer);
}

void GAME_OVER_draw();

void GAME_OVER_update() {
	GAME_OVER_draw();
}

void GAME_OVER_draw() {
	SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
	SDL_RenderClear(renderer);

	drawBoard();
	drawCurrent();
	drawPieceQueue();
	drawHeldPiece();
	drawScore();

	struct DrawStringInfo dsi = {
		.font = font_big,
		.colour = {0xff, 0x00, 0x00, 0xff},
		.x = BOARD_LEFT + BOARD_WIDTH / 2,
		.y = BOARD_HEIGHT / 2,
		.alignX = TEXT_ALIGN_CENTRE,
		.alignY = TEXT_ALIGN_CENTRE,
	};
	drawString(&dsi, "Game Over");

	SDL_RenderPresent(renderer);
}

int linesToClear[BLOCKS_Y];
int lineCount = 0;

void GAME_LINE_CLEAR_draw();

void GAME_LINE_CLEAR_update() {
	GAME_LINE_CLEAR_draw();
}

void GAME_LINE_CLEAR_draw() {
	SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
	SDL_RenderClear(renderer);

	drawBoard();
	drawCurrent();
	drawPieceQueue();
	drawHeldPiece();
	drawScore();

#define CLEAR_TIMER_LENGTH 10

	static int clearTimer = CLEAR_TIMER_LENGTH;
	if (--clearTimer <= 0) {
		for (int i = 0; i < lineCount; i++) {
			removeLine(linesToClear[i]);
		}

		gameState = GAME_RUN;
		clearTimer = CLEAR_TIMER_LENGTH;
		lineCount = 0;
	}

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, (CLEAR_TIMER_LENGTH - clearTimer) * (256 / CLEAR_TIMER_LENGTH));
	for (int i = 0; i < lineCount; i++) {
		SDL_Rect rect = { BOARD_LEFT, linesToClear[i] * BLOCK_SIZE, BOARD_WIDTH, BLOCK_SIZE };
		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_RenderPresent(renderer);
}

bool tryRotate();
bool checkResting();

bool pieceHeld = false;
bool canHold = true;
enum PieceType heldPieceType;

void GAME_RUN_update() {
	if (keyPressed(SDL_SCANCODE_ESCAPE)) {
		gameState = GAME_PAUSED;
		return;
	}

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

#define SOFT_DROP_TIMER_LENGTH 25
	if (keys[SDL_SCANCODE_DOWN]) {
		static int lastDown = 0;

		if ((time - lastDown) > SOFT_DROP_TIMER_LENGTH || !lastKeys[SDL_SCANCODE_DOWN]) {
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

	if (keyPressed(SDL_SCANCODE_C) && canHold) {
		currentBlock.x = BLOCKS_X / 2 - 1;
		currentBlock.y = 0;
		currentBlock.rotation = 0;

		if (pieceHeld) {
			enum PieceType tmp = currentBlock.type;
			currentBlock.type = heldPieceType;
			heldPieceType = tmp;
		}
		else {
			heldPieceType = currentBlock.type;
			currentBlock.type = pieceQueue[0];
			enqueuePiece();
		}

		pieceHeld = true;
		canHold = false;
	}

	blockTimer -= dt;
	if (blockTimer <= 0) {
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

		if (placementTimer <= 0) {
			placementTimer = placementTimerLength;
			placeCurrent();
		}
	}

	if (gameState == GAME_LINE_CLEAR) {
		return;
	}

	GAME_RUN_draw();
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

	canHold = true;

	checkForLines();

	selectPiece();
}

void selectPiece() {
	static bool pieceQueueInitialised = false;
	if (!pieceQueueInitialised) {
		pieceQueueInitialised = true;

		for (int i = 0; i < QUEUE_LENGTH; i++) {
			enqueuePiece();
		}
	}

	currentBlock.x = BLOCKS_X / 2 - 1;
	currentBlock.y = 0;
	currentBlock.rotation = 0;

	currentBlock.type = pieceQueue[0];
	enqueuePiece();

	// If any cells that would be occupied by the new piece are solid, game over
	struct BlockDef* block = &BLOCKS[currentBlock.type];
	struct BlockRotation* br = &block->rotations[currentBlock.rotation];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = j + currentBlock.x;
			int y = i + currentBlock.y;

			if (br->vals[i][j] && board.cells[x][y].solid) {
				gameState = GAME_OVER;
				return;
			}
		}
	}
}

void enqueuePiece() {
	static bool usedPieces[7] = { 0 };

	bool allUsed = true;
	for (int i = 0; i < 7; i++) {
		if (!usedPieces[i]) {
			allUsed = false;
			break;
		}
	}

	if (allUsed) {
		allUsed = false;
		for (int i = 0; i < 7; i++) {
			usedPieces[i] = false;
		}
	}

	int type;
	do
	{
		type = rand() % 7;
	} while (usedPieces[type]);

	usedPieces[type] = true;

	memmove(&pieceQueue[0], &pieceQueue[1], (QUEUE_LENGTH - 1) * sizeof(enum PieceType));
	pieceQueue[QUEUE_LENGTH - 1] = type;
}

void drawPieceQueue() {
	int queueLeft = BOARD_RIGHT + 30;
	int queueWidth = 100;

	int queueTop = 60;

	struct DrawStringInfo dsi = {
		.font = font_small,
		.colour = { 0x2b, 0x2b, 0x2b, 0xff },
		.x = queueLeft + queueWidth / 2,
		.y = queueTop,
		.alignX = TEXT_ALIGN_CENTRE,
		.alignY = TEXT_ALIGN_ABOVE
	};
	drawString(&dsi, "Next");

	int y = queueTop;

	for (int i = 0; i < QUEUE_LENGTH; i++) {
		struct BlockDef* block = &BLOCKS[pieceQueue[i]];
		struct BlockRotation* br = &block->rotations[0];

		for (int i = 0; i < 4; i++) {
			bool anySolid = false;

			for (int j = 0; j < 4; j++) {
				if (br->vals[i][j]) {
					anySolid = true;

					SDL_Color colour = block->colour;

					SDL_Rect rect = { queueLeft + j * BLOCK_SIZE, y, BLOCK_SIZE, BLOCK_SIZE };
					SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
					SDL_RenderFillRect(renderer, &rect);

				}
			}
			if (anySolid) {
				y += BLOCK_SIZE;
			}
		}
		y += BLOCK_SIZE;
	}
}

void drawHeldPiece() {
	int left = BOARD_LEFT - 30;
	int y = 60;

	struct DrawStringInfo dsi = {
		.font = font_small,
		.colour = { 0x2b, 0x2b, 0x2b, 0xff },
		.x = left,
		.y = y,
		.alignX = TEXT_ALIGN_RIGHT,
		.alignY = TEXT_ALIGN_ABOVE
	};
	drawString(&dsi, "Held");

	if (pieceHeld) {
		struct BlockDef* block = &BLOCKS[heldPieceType];
		struct BlockRotation* br = &block->rotations[0];

		for (int i = 0; i < 4; i++) {
			bool anySolid = false;

			for (int j = 0; j < 4; j++) {
				if (br->vals[i][j]) {
					anySolid = true;

					SDL_Color colour = block->colour;

					SDL_Rect rect = { 50 + j * BLOCK_SIZE, y, BLOCK_SIZE, BLOCK_SIZE };
					SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
					SDL_RenderFillRect(renderer, &rect);
				}
			}
			if (anySolid) {
				y += BLOCK_SIZE;
			}
		}
	}
}

void drawScore() {
	int left = BOARD_LEFT - 30;
	int y = 300;

	struct DrawStringInfo dsi = {
		.font = font_small,
		.colour = { 0x2b, 0x2b, 0x2b, 0xff },
		.x = left,
		.y = y,
		.alignX = TEXT_ALIGN_RIGHT,
		.alignY = TEXT_ALIGN_ABOVE
	};
	drawString(&dsi, "Lines");

	dsi.alignY = TEXT_ALIGN_BELOW;
	drawStringf(&dsi, "%d", lines);

	dsi.y += 100;
	dsi.alignY = TEXT_ALIGN_ABOVE;
	drawString(&dsi, "Level");

	dsi.alignY = TEXT_ALIGN_BELOW;
	drawStringf(&dsi, "%d", level);
}

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
						goto checkNextLine;
					}
				}

				linesToClear[lineCount++] = y;
				gameState = GAME_LINE_CLEAR;

				goto checkNextLine;
			}
		}
	checkNextLine:;
	}
}

void removeLine(int y) {
	//printf("%d\n", y);

	lines++;
	if (lines % 10 == 0) {
		level++;
		blockTimerLength *= 2;
		blockTimerLength /= 3;
	}

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

void GAME_RUN_draw() {
	SDL_SetRenderDrawColor(renderer, backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
	SDL_RenderClear(renderer);

	drawBoard();
	drawCurrent();
	drawPieceQueue();
	drawHeldPiece();
	drawScore();

	SDL_RenderPresent(renderer);
}

void drawBoard() {
	for (int i = 0; i < BLOCKS_Y; i++) {
		for (int j = 0; j < BLOCKS_X; j++) {
			int x = BOARD_LEFT + j * BLOCK_SIZE;
			int y = i * BLOCK_SIZE;

			struct Cell* cell = &board.cells[j][i];

			SDL_Rect rect = { x, y, BLOCK_SIZE, BLOCK_SIZE };
			if (cell->solid) {
				SDL_Color colour = cell->colour;
				SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
				SDL_RenderFillRect(renderer, &rect);
			}
			else {
				SDL_Color colour = (SDL_Color){ 0x2b, 0x2b, 0x2b, 0xff };
				SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
				SDL_RenderFillRect(renderer, &rect);

				colour = (SDL_Color){ 0x45, 0x45, 0x45, 0xff };
				SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
				SDL_RenderDrawRect(renderer, &rect);
			}
		}
	}
}

void drawCurrent() {
	struct BlockDef* block = &BLOCKS[currentBlock.type];
	struct BlockRotation* br = &block->rotations[currentBlock.rotation];

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
				SDL_Color colour = { 0x45, 0x45, 0x45, 0xbf };

				SDL_Rect rect = { BOARD_LEFT + x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
				SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int x = currentBlock.x + j;
			int y = currentBlock.y + i;

			if (x < 0 || x >= BLOCKS_X || y < 0 || y >= BLOCKS_Y) {
				continue;
			}

			if (br->vals[i][j]) {
				SDL_Color colour = block->colour;

				SDL_Rect rect = { BOARD_LEFT + x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
				SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

void loadFont() {
	font_big = TTF_OpenFont("resources/Blinker/Blinker-Regular.ttf", 80);
	font_small = TTF_OpenFont("resources/Blinker/Blinker-Regular.ttf", 40);
}

void drawString(struct DrawStringInfo* dsi, const char* msg) {
	SDL_Surface* surf = TTF_RenderUTF8_Blended(dsi->font, msg, dsi->colour);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	int x = dsi->x;
	int y = dsi->y;

	switch (dsi->alignX) {
	case TEXT_ALIGN_LEFT:
		break;

	case TEXT_ALIGN_RIGHT:
		x -= surf->w;
		break;

	case TEXT_ALIGN_CENTRE:
		x -= surf->w / 2;
		break;

	default:
		printf("Invalid text align\n");
		exit(-1);
	}

	switch (dsi->alignY) {
	case TEXT_ALIGN_BELOW:
		break;

	case TEXT_ALIGN_ABOVE:
		y -= surf->h;
		break;

	case TEXT_ALIGN_CENTRE:
		y -= surf->h / 2;
		break;

	default:
		printf("Invalid text align\n");
		exit(-1);
	}

	SDL_Rect dest = {
		x,
		y,
		surf->w,
		surf->h
	};

	SDL_RenderCopy(renderer, tex, NULL, &dest);

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(tex);
}

void drawStringf(struct DrawStringInfo* dsi, const char* fmt, ...) {
	static char* buf;
	static int bufSize;

	while (true) {
		va_list args;
		va_start(args, fmt);

		if (buf == NULL) {
			bufSize = 4;
			buf = malloc(bufSize);
		}
		int ret = vsnprintf(buf, bufSize, fmt, args);
		if (ret > bufSize) {
			bufSize = ret + 1;
			buf = realloc(buf, bufSize);
		}
		else {
			drawString(dsi, buf);
			return;
		}

		va_end(args);
	}
}