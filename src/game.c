#include "../include/game.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

// Initialize game state
void InitGame(GameState *game) {
    if (!game) return;
    
    // Initialize snake
    game->snake.length = 3;
    game->snake.direction = RIGHT;
    game->snake.next_direction = RIGHT;
    
    for (int i = 0; i < game->snake.length; i++) {
        game->snake.body[i].x = (GRID_SIZE / 2) - i;
        game->snake.body[i].y = GRID_SIZE / 2;
    }
    
    // Initialize game state
    game->score = 0;
    game->speed = INITIAL_SPEED;
    game->game_over = false;
    game->paused = false;
    
    // Spawn first food
    SpawnFood(game);
    
    srand((unsigned int)time(NULL));
}

// Spawn food at random position
void SpawnFood(GameState *game) {
    if (!game) return;
    
    bool valid_position = false;
    
    while (!valid_position) {
        game->food.pos.x = rand() % (SCREEN_WIDTH / GRID_SIZE);
        game->food.pos.y = rand() % (SCREEN_HEIGHT / GRID_SIZE);
        
        valid_position = true;
        // Check if food overlaps with snake
        for (int i = 0; i < game->snake.length; i++) {
            if (game->snake.body[i].x == game->food.pos.x &&
                game->snake.body[i].y == game->food.pos.y) {
                valid_position = false;
                break;
            }
        }
    }
    game->food.active = true;
}

// Check collision with walls or self
bool CheckCollision(const Snake *snake) {
    if (!snake) return true;
    
    Position head = snake->body[0];
    
    // Check wall collision
    if (head.x < 0 || head.x >= SCREEN_WIDTH / GRID_SIZE ||
        head.y < 0 || head.y >= SCREEN_HEIGHT / GRID_SIZE) {
        return true;
    }
    
    // Check self collision
    for (int i = 4; i < snake->length; i++) {
        if (head.x == snake->body[i].x && head.y == snake->body[i].y) {
            return true;
        }
    }
    
    return false;
}

// Handle player input
void HandleInput(Snake *snake) {
    if (!snake) return;
    
    if (IsKeyPressed(KEY_UP) && snake->direction != DOWN) {
        snake->next_direction = UP;
    }
    if (IsKeyPressed(KEY_DOWN) && snake->direction != UP) {
        snake->next_direction = DOWN;
    }
    if (IsKeyPressed(KEY_LEFT) && snake->direction != RIGHT) {
        snake->next_direction = LEFT;
    }
    if (IsKeyPressed(KEY_RIGHT) && snake->direction != LEFT) {
        snake->next_direction = RIGHT;
    }
}

// Update game logic
void UpdateGame(GameState *game) {
    if (!game || game->game_over || game->paused) return;
    
    static int frame_counter = 0;
    frame_counter++;
    
    // Control speed via frame counter
    if (frame_counter < game->speed) {
        return;
    }
    frame_counter = 0;
    
    HandleInput(&game->snake);
    game->snake.direction = game->snake.next_direction;
    
    // Move snake
    Position new_head = game->snake.body[0];
    
    switch (game->snake.direction) {
        case UP:
            new_head.y--;
            break;
        case DOWN:
            new_head.y++;
            break;
        case LEFT:
            new_head.x--;
            break;
        case RIGHT:
            new_head.x++;
            break;
    }
    
    // Check collision
    if (CheckCollision(&game->snake)) {
        game->game_over = true;
        return;
    }
    
    // Shift snake body
    for (int i = game->snake.length - 1; i > 0; i--) {
        game->snake.body[i] = game->snake.body[i - 1];
    }
    game->snake.body[0] = new_head;
    
    // Check food collision
    if (game->snake.body[0].x == game->food.pos.x &&
        game->snake.body[0].y == game->food.pos.y) {
        
        // Grow snake
        if (game->snake.length < MAX_SNAKE_LENGTH) {
            game->snake.length++;
        }
        game->score += 10;
        
        // Increase difficulty
        if (game->speed > 2) {
            game->speed--;
        }
        
        SpawnFood(game);
    }
}

// Draw game
void DrawGame(const GameState *game) {
    if (!game) return;
    
    ClearBackground(BLACK);
    
    // Draw food
    if (game->food.active) {
        Rectangle food_rect = {
            game->food.pos.x * GRID_SIZE,
            game->food.pos.y * GRID_SIZE,
            GRID_SIZE,
            GRID_SIZE
        };
        DrawRectangleRec(food_rect, RED);
    }
    
    // Draw snake head
    Rectangle head_rect = {
        game->snake.body[0].x * GRID_SIZE,
        game->snake.body[0].y * GRID_SIZE,
        GRID_SIZE,
        GRID_SIZE
    };
    DrawRectangleRec(head_rect, LIME);
    DrawRectangleLines((int)head_rect.x, (int)head_rect.y, GRID_SIZE, GRID_SIZE, DARKGREEN);
    
    // Draw snake body
    for (int i = 1; i < game->snake.length; i++) {
        Rectangle body_rect = {
            game->snake.body[i].x * GRID_SIZE,
            game->snake.body[i].y * GRID_SIZE,
            GRID_SIZE,
            GRID_SIZE
        };
        DrawRectangleRec(body_rect, GREEN);
        DrawRectangleLines((int)body_rect.x, (int)body_rect.y, GRID_SIZE, GRID_SIZE, DARKGREEN);
    }
    
    // Draw grid
    for (int i = 0; i <= SCREEN_WIDTH / GRID_SIZE; i++) {
        DrawLine(i * GRID_SIZE, 0, i * GRID_SIZE, SCREEN_HEIGHT, DARKGRAY);
    }
    for (int i = 0; i <= SCREEN_HEIGHT / GRID_SIZE; i++) {
        DrawLine(0, i * GRID_SIZE, SCREEN_WIDTH, i * GRID_SIZE, DARKGRAY);
    }
    
    // Draw score
    DrawText("SNAKE", 10, 10, 20, WHITE);
    DrawText(TextFormat("Score: %d", game->score), 10, 35, 20, WHITE);
    DrawText(TextFormat("Speed: %d", INITIAL_SPEED - game->speed + 1), 10, 60, 20, WHITE);
    
    // Draw pause/game over messages
    if (game->paused) {
        DrawText("PAUSED - Press P to Resume", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 20, 20, YELLOW);
    }
    
    if (game->game_over) {
        DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 40, 40, RED);
        DrawText(TextFormat("Final Score: %d", game->score), SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 20, 20, WHITE);
        DrawText("Press R to Restart or ESC to Quit", SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 60, 16, LIGHTGRAY);
    }
}

// Cleanup
void CleanupGame(GameState *game) {
    if (game) {
        // Nothing to free for this simple implementation
        // In a more complex game, you might free dynamically allocated resources
    }
}
