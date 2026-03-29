#include "raylib.h"
#include "../include/game.h"
#include <stdio.h>

int main(void) {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SNAKE - A Classic Game in C");
    SetTargetFPS(60);
    
    // Initialize game
    GameState game = {0};
    InitGame(&game);
    
    printf("\n╔════════════════════════════════════╗\n");
    printf("║     SNAKE Game - Raylib Edition    ║\n");
    printf("║      Press R to restart game       ║\n");
    printf("║      Press Q to quit               ║\n");
    printf("╚════════════════════════════════════╝\n\n");
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        if (game.game_over) {
            if (IsKeyPressed(KEY_R)) {
                CleanupGame(&game);
                InitGame(&game);
            } else if (IsKeyPressed(KEY_Q)) {
                break;
            }
        } else {
            if (IsKeyPressed(KEY_Q)) {
                break;
            }
            UpdateGame(&game);
        }
        
        // Draw
        DrawGame(&game);
    }
    
    // Cleanup
    CleanupGame(&game);
    CloseWindow();
    
    printf("Thanks for playing SNAKE!\n");
    
    return 0;
}
