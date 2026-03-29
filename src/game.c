#include "../include/game.h"
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

// Initialize game state
void InitGame(GameState *game) {
    if (!game) return;
    
    // Initialize snake in center of screen
    game->snake.length = 3;
    game->snake.direction = RIGHT;
    game->snake.next_direction = RIGHT;
    
    // Center the snake on the grid
    int grid_width = (int)(GetScreenWidth() / GRID_SIZE);
    int grid_height = (int)(GetScreenHeight() / GRID_SIZE);
    
    for (int i = 0; i < game->snake.length; i++) {
        game->snake.body[i].x = (grid_width / 2) - i;
        game->snake.body[i].y = grid_height / 2;
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
    
    int grid_width = (int)(GetScreenWidth() / GRID_SIZE);
    int grid_height = (int)(GetScreenHeight() / GRID_SIZE);
    
    bool valid_position = false;
    
    while (!valid_position) {
        game->food.pos.x = rand() % grid_width;
        game->food.pos.y = rand() % grid_height;
        
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
    
    int grid_width = (int)(GetScreenWidth() / GRID_SIZE);
    int grid_height = (int)(GetScreenHeight() / GRID_SIZE);
    
    Position head = snake->body[0];
    
    // Check wall collision
    if (head.x < 0 || head.x >= grid_width ||
        head.y < 0 || head.y >= grid_height) {
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
    
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        if (snake->direction != DOWN) {
            snake->next_direction = UP;
        }
    } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        if (snake->direction != UP) {
            snake->next_direction = DOWN;
        }
    } else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
        if (snake->direction != RIGHT) {
            snake->next_direction = LEFT;
        }
    } else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        if (snake->direction != LEFT) {
            snake->next_direction = RIGHT;
        }
    }
}

// Update game logic
void UpdateGame(GameState *game) {
    if (!game || game->game_over) return;
    
    // Handle pause
    if (IsKeyPressed(KEY_P)) {
        game->paused = !game->paused;
    }
    
    if (game->paused) return;
    
    HandleInput(&game->snake);
    
    // Update snake direction
    game->snake.direction = game->snake.next_direction;
    
    // Move snake with frame-based timing
    static int frame_counter = 0;
    frame_counter++;
    
    if (frame_counter < (10 - game->speed)) {
        return;  // Skip this frame to control speed
    }
    frame_counter = 0;
    
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
        
        // Increase difficulty (speed up)
        if (game->speed < 9) {
            game->speed++;
        }
        
        SpawnFood(game);
    }
}

// Draw game with Raylib graphics
void DrawGame(const GameState *game) {
    if (!game) return;
    
    BeginDrawing();
    ClearBackground(BLACK);
    
    int grid_width = (int)(GetScreenWidth() / GRID_SIZE);
    int grid_height = (int)(GetScreenHeight() / GRID_SIZE);
    
    // Draw grid (optional - light gray)
    for (int x = 0; x <= grid_width; x++) {
        DrawLine(x * GRID_SIZE, 0, x * GRID_SIZE, GetScreenHeight(), 
                 (Color){64, 64, 64, 50});
    }
    for (int y = 0; y <= grid_height; y++) {
        DrawLine(0, y * GRID_SIZE, GetScreenWidth(), y * GRID_SIZE, 
                 (Color){64, 64, 64, 50});
    }
    
    // Draw food
    if (game->food.active) {
        int fx = game->food.pos.x * GRID_SIZE + GRID_SIZE / 2;
        int fy = game->food.pos.y * GRID_SIZE + GRID_SIZE / 2;
        DrawCircle(fx, fy, GRID_SIZE / 3, RED);
        
        // Draw a glowing effect
        DrawCircleLines(fx, fy, GRID_SIZE / 2, ORANGE);
    }
    
    // Draw snake body (from tail to head)
    for (int i = game->snake.length - 1; i > 0; i--) {
        int bx = game->snake.body[i].x * GRID_SIZE;
        int by = game->snake.body[i].y * GRID_SIZE;
        DrawRectangle(bx + 2, by + 2, GRID_SIZE - 4, GRID_SIZE - 4, GREEN);
        DrawRectangleLines(bx, by, GRID_SIZE, GRID_SIZE, (Color){0, 200, 0, 100});
    }
    
    // Draw snake head
    {
        int hx = game->snake.body[0].x * GRID_SIZE;
        int hy = game->snake.body[0].y * GRID_SIZE;
        DrawRectangle(hx + 1, hy + 1, GRID_SIZE - 2, GRID_SIZE - 2, LIME);
        DrawRectangleLines(hx, hy, GRID_SIZE, GRID_SIZE, YELLOW);
        
        // Draw eyes
        Color eye_color = WHITE;
        switch (game->snake.direction) {
            case UP:
                DrawCircle(hx + 6, hy + 6, 2, eye_color);
                DrawCircle(hx + 14, hy + 6, 2, eye_color);
                break;
            case DOWN:
                DrawCircle(hx + 6, hy + 14, 2, eye_color);
                DrawCircle(hx + 14, hy + 14, 2, eye_color);
                break;
            case LEFT:
                DrawCircle(hx + 6, hy + 6, 2, eye_color);
                DrawCircle(hx + 6, hy + 14, 2, eye_color);
                break;
            case RIGHT:
                DrawCircle(hx + 14, hy + 6, 2, eye_color);
                DrawCircle(hx + 14, hy + 14, 2, eye_color);
                break;
        }
    }
    
    // Draw UI (score, controls)
    {
        int panel_height = 60;
        DrawRectangle(0, GetScreenHeight() - panel_height, GetScreenWidth(), 
                      panel_height, (Color){20, 20, 20, 200});
        
        // Score
        DrawText(TextFormat("SCORE: %04d", game->score), 20, 
                 GetScreenHeight() - 45, 30, YELLOW);
        
        // Snake length
        DrawText(TextFormat("LENGTH: %d", game->snake.length), 250, 
                 GetScreenHeight() - 45, 20, GREEN);
        
        // Speed/Level
        DrawText(TextFormat("LEVEL: %d", game->speed), 450, 
                 GetScreenHeight() - 45, 20, ORANGE);
        
        // Status
        if (game->paused) {
            DrawText("PAUSED - Press P to resume", 650, 
                     GetScreenHeight() - 45, 18, RED);
        } else {
            DrawText("P: Pause | Q: Quit", 650, 
                     GetScreenHeight() - 45, 18, GRAY);
        }
    }
    
    // Draw game over screen
    if (game->game_over) {
        // Semi-transparent overlay
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), 
                      (Color){0, 0, 0, 150});
        
        // Game over text
        const char *text = "GAME OVER";
        int text_width = MeasureText(text, 60);
        DrawText(text, GetScreenWidth() / 2 - text_width / 2, 
                 GetScreenHeight() / 2 - 60, 60, RED);
        
        // Final score
        char score_text[50];
        snprintf(score_text, sizeof(score_text), "Final Score: %d", game->score);
        int score_width = MeasureText(score_text, 40);
        DrawText(score_text, GetScreenWidth() / 2 - score_width / 2, 
                 GetScreenHeight() / 2 + 20, 40, YELLOW);
        
        // Restart instruction
        DrawText("Press R to Restart or Q to Quit", 
                 GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 + 100, 
                 20, WHITE);
    }
    
    EndDrawing();
}

// Cleanup
void CleanupGame(GameState *game) {
    if (game) {
        // Nothing to free for this simple implementation
    }
}
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
