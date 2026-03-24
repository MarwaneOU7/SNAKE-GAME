#include "raylib.h"
#include "../include/game.h"

int main(void) {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SNAKE - A Classic Game in C");
    SetTargetFPS(60);
    
    // Initialize game
    GameState game = {0};
    InitGame(&game);
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Handle input
        if (IsKeyPressed(KEY_P)) {
            game.paused = !game.paused;
        }
        
        if (game.game_over) {
            if (IsKeyPressed(KEY_R)) {
                CleanupGame(&game);
                InitGame(&game);
            }
        } else {
            UpdateGame(&game);
        }
        
        // Draw
        BeginDrawing();
        DrawGame(&game);
        EndDrawing();
    }
    
    // Cleanup
    CleanupGame(&game);
    CloseWindow();
    
    return 0;
}
