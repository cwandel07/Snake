#include "raylib.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 20
#define SIZE 400


#define FPS 1.5
float moveTimer = 0.0f;


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

struct {
    Vector2 pos[GRID_SIZE*GRID_SIZE];
    int length;
} snakeBody;


int main() {
    init();  



    
    InitWindow(SIZE, SIZE, "Snake v0.1");
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            update();
            ClearBackground(RAYWHITE);
            drawGrid();
            drawSnakeBody();
        EndDrawing();
    }
    CloseWindow();
    return 0;
    
}
enum  {
    UP, RIGHT, DOWN, LEFT
} direction;

void updateDirection() {
    if (IsKeyPressed(KEY_UP)) {
        direction = UP;
    } else if (IsKeyPressed(KEY_LEFT)) {
        direction = LEFT;
    } else if (IsKeyPressed(KEY_DOWN)) {
        direction = DOWN;
    } else if (IsKeyPressed(KEY_RIGHT)) {
        direction = RIGHT;
    }

    // Else, direction can be used again
}

Vector2 getNextHeadPos() {
    switch(direction) {
        case RIGHT:
            return (Vector2){snakeBody.pos[0].x +1, snakeBody.pos[0].y};
            break;
        case LEFT:
            return (Vector2) {snakeBody.pos[0].x -1, snakeBody.pos[0].y};
            break;
        case UP:
            return (Vector2) {snakeBody.pos[0].x, snakeBody.pos[0].y-1};
            break;
        case DOWN:
            return (Vector2) {snakeBody.pos[0].x, snakeBody.pos[0].y+1};
            break;
        default:
            fprintf(stderr, "Error while getting the Next Head Position");
            return snakeBody.pos[0];
    }
}

void update() {
    updateDirection();
    
    moveTimer += GetFrameTime();
    
    if (moveTimer >= 1/FPS) {
        moveTimer = 0;

        // Updating the Screen should only happen here!
        Vector2 nextHeadPos = getNextHeadPos();
        snakeBody.pos[0] = nextHeadPos;
    }

    

    
}



void init() {
    // Set snakeBody to zeroes
    for (int i = 0; i < GRID_SIZE*GRID_SIZE; i++)
    {
        snakeBody.pos[i].x = 0;
        snakeBody.pos[i].y = 0;
    }
    snakeBody.length = 1;
    snakeBody.pos[0].x = (SIZE/GRID_SIZE)/2 -1;
    snakeBody.pos[0].y = (SIZE/GRID_SIZE)/2 -1;  
    
    direction = RIGHT;
}



void drawSnakeBody() {
    for (int i = 0; i < snakeBody.length; i++)
    {
        drawBox(snakeBody.pos[i].x, snakeBody.pos[i].y);
    }
    
}

void drawGrid() {
    for (unsigned int i = GRID_SIZE; i < SIZE; i+=GRID_SIZE)
            {
                DrawLine(0, i, SIZE, i, BLACK);
                DrawLine(i, 0, i, SIZE, BLACK);
            }
}

void drawBox(int x, int y) {
    if (x <0) {
        fprintf(stderr, "Drawing out of Bounds - x ");
    } else if (y < 0) {
        fprintf(stderr, "Drawing out of Bounds - y ");
    } else {
        DrawRectangle(GRID_SIZE*x, GRID_SIZE*y, GRID_SIZE, GRID_SIZE, BLACK);
    }
}