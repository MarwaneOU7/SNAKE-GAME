#include "../include/game_console.h"
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #include <ctype.h>
    #define SLEEP(ms) usleep((ms) * 1000)
    #define _getch() getchar()
#endif

int main(void) {
    GameState game = {0};
    InitGame(&game);
    
    printf("\n");
    printf("╔════════════════════════════════╗\n");
    printf("║   SNAKE GAME - Console Edition  ║\n");
    printf("║     Press any key to start      ║\n");
    printf("╚════════════════════════════════╝\n");
    printf("\n");
    
#ifdef _WIN32
    system("pause > nul");
#else
    printf("Press ENTER to start...\n");
    getchar();
#endif
    
    // Main game loop
    while (!game.game_over) {
        DrawGame(&game);
        UpdateGame(&game);
        SLEEP(game.speed);
        
        // Allow restart after game over
        if (game.game_over) {
            DrawGame(&game);
            printf("\nPress R to restart, Q to quit: ");
            fflush(stdout);
            
#ifdef _WIN32
            int choice = _getch();
#else
            int choice = getchar();
#endif
            
            if (choice == 'r' || choice == 'R') {
                CleanupGame(&game);
                InitGame(&game);
            } else {
                break;
            }
        }
    }
    
    CleanupGame(&game);
    printf("\nThanks for playing!\n");
    
    return 0;
}
