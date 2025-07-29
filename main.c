#include <stdio.h>
#include "miniaudio.h"
#include "raylib.h"

int main()
{
    // Initializations
    InitWindow(800, 600, "VIS");
    
    ma_result result;
    ma_engine engine;
    const char* song = "assets/megalovania.wav";
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return -1;
    }

    // Main loop
    ma_engine_play_sound(&engine, song, NULL);
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    // De-initializations
    CloseWindow();
    ma_engine_uninit(&engine);

    return 0;
}