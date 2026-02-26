#include "raylib.h"
#define GRID_SIZE 20
#define SIZE 400

// int main() {
//     InitWindow(400, 400, "Snake Setup Test");
//     while (!WindowShouldClose()) {
//         BeginDrawing();
//             ClearBackground(RAYWHITE);
//             DrawText("Raylib is working!", 100, 200, 20, LIGHTGRAY);
//         EndDrawing();
//     }
//     CloseWindow();
//     return 0;
// }


int main() {
    InitWindow(SIZE, SIZE, "Snake v0.1");
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            drawGrid();
            
            
        EndDrawing();
    }
    
}

void drawGrid() {
    for (unsigned int i = GRID_SIZE; i < SIZE; i+=GRID_SIZE)
            {
                DrawLine(0, i, SIZE, i, BLACK);
                DrawLine(i, 0, i, SIZE, BLACK);
            }
}