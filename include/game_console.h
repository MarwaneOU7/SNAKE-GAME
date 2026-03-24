#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

// Game constants
#define GRID_WIDTH 40
#define GRID_HEIGHT 20
#define MAX_SNAKE_LENGTH 100

// Direction enum
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Position structure
typedef struct {
    int x;
    int y;
} Position;

// Snake structure
typedef struct {
    Position body[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
    Direction next_direction;
} Snake;

// Food structure
typedef struct {
    Position pos;
    bool active;
} Food;

// Game state structure
typedef struct {
    Snake snake;
    Food food;
    int score;
    int speed;
    bool game_over;
    bool paused;
} GameState;

// Function declarations
void InitGame(GameState *game);
void UpdateGame(GameState *game);
void DrawGame(const GameState *game);
void CleanupGame(GameState *game);
void SpawnFood(GameState *game);
bool CheckCollision(const Snake *snake);
void HandleInput(GameState *game);

#endif
