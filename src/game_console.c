#include "../include/game_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// Initialize game state
void InitGame(GameState *game) {
    if (!game) return;
    
    // Initialize snake in center of screen
    game->snake.length = 3;
    game->snake.direction = RIGHT;
    game->snake.next_direction = RIGHT;
    
    for (int i = 0; i < game->snake.length; i++) {
        game->snake.body[i].x = (GRID_WIDTH / 2) - i;
        game->snake.body[i].y = GRID_HEIGHT / 2;
    }
    
    // Initialize game state
    game->score = 0;
    game->speed = 100;  // milliseconds per move
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
        game->food.pos.x = rand() % GRID_WIDTH;
        game->food.pos.y = rand() % GRID_HEIGHT;
        
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
    if (head.x < 0 || head.x >= GRID_WIDTH ||
        head.y < 0 || head.y >= GRID_HEIGHT) {
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

// Non-blocking input check (cross-platform)
static int kbhit_custom(void) {
#ifdef _WIN32
    return _kbhit();
#else
    struct termios oldt, newt;
    int ch;
    int olderrno;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    olderrno = errno;
    ch = getchar();
    errno = olderrno;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
#endif
}

// Handle player input
void HandleInput(GameState *game) {
    if (!game || !kbhit_custom()) return;
    
#ifdef _WIN32
    int key = _getch();
#else
    int key = getchar();
#endif
    
    switch(key) {
        case 'w':
        case 'W':
            if (game->snake.direction != DOWN) {
                game->snake.next_direction = UP;
            }
            break;
        case 's':
        case 'S':
            if (game->snake.direction != UP) {
                game->snake.next_direction = DOWN;
            }
            break;
        case 'a':
        case 'A':
            if (game->snake.direction != RIGHT) {
                game->snake.next_direction = LEFT;
            }
            break;
        case 'd':
        case 'D':
            if (game->snake.direction != LEFT) {
                game->snake.next_direction = RIGHT;
            }
            break;
        case 'p':
        case 'P':
            game->paused = !game->paused;
            break;
        case 27:  // ESC
        case 'q':
        case 'Q':
            game->game_over = true;
            break;
    }
}

// Update game logic
void UpdateGame(GameState *game) {
    if (!game || game->game_over || game->paused) return;
    
    HandleInput(game);
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
        
        // Increase difficulty (speed up)
        if (game->speed > 50) {
            game->speed -= 5;
        }
        
        SpawnFood(game);
    }
}

// Draw game using ANSI escape codes
void DrawGame(const GameState *game) {
    if (!game) return;
    
    // Clear screen (ANSI escape clear)
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    // Top border
    printf("╔");
    for (int i = 0; i < GRID_WIDTH; i++) printf("═");
    printf("╗\n");
    
    // Game grid
    for (int y = 0; y < GRID_HEIGHT; y++) {
        printf("║");
        
        for (int x = 0; x < GRID_WIDTH; x++) {
            // Check if snake head
            if (game->snake.body[0].x == x && game->snake.body[0].y == y) {
                printf("@");  // Snake head
            }
            // Check if snake body
            else {
                bool is_body = false;
                for (int i = 1; i < game->snake.length; i++) {
                    if (game->snake.body[i].x == x && game->snake.body[i].y == y) {
                        printf("o");  // Snake body
                        is_body = true;
                        break;
                    }
                }
                
                if (!is_body) {
                    // Check if food
                    if (game->food.active && game->food.pos.x == x && game->food.pos.y == y) {
                        printf("*");  // Food
                    } else {
                        printf(" ");  // Empty space
                    }
                }
            }
        }
        printf("║\n");
    }
    
    // Bottom border
    printf("╚");
    for (int i = 0; i < GRID_WIDTH; i++) printf("═");
    printf("╝\n");
    
    // Info bar
    printf("\nScore: %04d | Snake Length: %d | Speed: %d\n", 
           game->score, game->snake.length, 101 - (game->speed / 5));
    
    // Status messages
    if (game->paused) {
        printf("\n*** PAUSED *** Press P to resume\n");
    }
    
    if (game->game_over) {
        printf("\n╔════════════════════════════════╗\n");
        printf("║  GAME OVER - Final Score: %04d  ║\n", game->score);
        printf("║  Press R for restart or Q quit  ║\n");
        printf("╚════════════════════════════════╝\n");
    } else {
        printf("\nControls: WASD or Arrow Keys | P: Pause | Q: Quit\n");
    }
}

// Cleanup
void CleanupGame(GameState *game) {
    if (game) {
        // Nothing to free for this simple implementation
    }
}
