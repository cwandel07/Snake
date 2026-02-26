#include "raylib.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 6 // For a 10x10 field, this should be set to 10!
#define SIZE 400 // SHOULD BE atleast GRID_SIZE * GRID_SIZE
#define APPLE_COUNT 14
#define START_LENGTH 3
int CELL_SIZE = SIZE/GRID_SIZE;
#define FPS 2.5 // ALWAYS AS A FLOAT!
float moveTimer = 0.0f;
int finalScore = 0;

struct {
    Vector2 pos[GRID_SIZE*GRID_SIZE];
    int length;
} snakeBody;

Vector2 applePositions[APPLE_COUNT] = {0,0};


int main() {
    SetRandomSeed(time(NULL));
    
    init(); // Initialize all game values

    InitWindow(SIZE, SIZE, "Snake v0.1");
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            draw();
            
        EndDrawing();
    }
    CloseWindow();
    return 0;
    
}

void draw() {
    switch (gameState)
    {
    case RUNNING:
        if (IsKeyPressed(KEY_SPACE)) gameState = PAUSED;
        update();
        ClearBackground(WHITE);
        drawSnakeBody();
        drawAllApples();
        drawGrid();
        break;
    case MENU:
        drawMenu();
        if (IsKeyPressed(KEY_ENTER)) gameState = RUNNING;
        break;
    case PAUSED:
        drawPause();
        if (IsKeyPressed(KEY_SPACE)) gameState = RUNNING;
        break;
    case LOSS:
        ClearBackground(WHITE);
        drawCenteredText(TextFormat("You lost! Final Score: %i", finalScore), 0, 0, 30, BLACK);
        if(GetKeyPressed() != 0) {
            init();
            gameState = RUNNING;
        }
        break;
    case WIN:
        ClearBackground(WHITE);
        drawCenteredText(TextFormat("YOU WON! \nFinal Score of %i", snakeBody.length), 0,0, 30, GOLD);
        if(GetKeyPressed() != 0) {
            init();
            gameState = RUNNING;
        }
        break;
    default:
        gameState = MENU;
        break;
    }
    
}

void drawMenu() {
    ClearBackground(WHITE);
    DrawText("HELLO WORLD!!!1!!11!!", SIZE/2, SIZE/2, 10, DARKGRAY);
    
}

void drawPause() {
    ClearBackground(LIGHTGRAY);
    drawCenteredText("Pause!", 0, 0, 30, BLACK);
    drawCenteredText(TextFormat("Current Score: %i", snakeBody.length), 0, 40, 30, BLACK);
}


void updateDirection() {
    if (IsKeyPressed(KEY_UP)) {
        if (!(snakeBody.pos[1].x == snakeBody.pos[0].x && snakeBody.pos[1].y == snakeBody.pos[0].y -1))direction = UP;
    } else if (IsKeyPressed(KEY_LEFT)) {
        if (!(snakeBody.pos[1].x == snakeBody.pos[0].x -1 && snakeBody.pos[1].y == snakeBody.pos[0].y))direction = LEFT;
    } else if (IsKeyPressed(KEY_DOWN)) {
        if (!(snakeBody.pos[1].x == snakeBody.pos[0].x && snakeBody.pos[1].y == snakeBody.pos[0].y +1))direction = DOWN;
    } else if (IsKeyPressed(KEY_RIGHT)) {
        if (!(snakeBody.pos[1].x == snakeBody.pos[0].x +1 && snakeBody.pos[1].y == snakeBody.pos[0].y))direction = RIGHT;
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

        // NOTE: Updating the Screen should only happen here!

        //New Head Pos
        Vector2 nextHeadPos = getNextHeadPos();

        // Check if there is a fail
        switch(isOccupied(nextHeadPos)) {
            case 0:
                break;
            case 1: //LOSS
                finalScore = snakeBody.length;
                gameState = LOSS;
                break;
                //BUG:
            case -1:
                if (snakeBody.length + APPLE_COUNT >= GRID_SIZE*GRID_SIZE) {
                    gameState = WIN;
                    break;
                }
                else {
                    snakeBody.length++;
                    snakeBody.pos[snakeBody.length -1] = snakeBody.pos[snakeBody.length-2];
                    
                    int appleIndex = findAppleIndexByPos(nextHeadPos);
                    
                    applePositions[appleIndex] = getValidApplePosition();
                }
                break; //TODO: CHECK FOR FUNCTION
        }

        // Move Snake
        Vector2 tempPos = snakeBody.pos[0];
        snakeBody.pos[0] = nextHeadPos;
        
        for (int i = snakeBody.length; i > 1; i--)
        {
            snakeBody.pos[i] = snakeBody.pos[i-1];
        }
        snakeBody.pos[1] = tempPos;
        
        
        


    }

    

    
}

int findAppleIndexByPos(Vector2 v) {
    for (int i = 0; i < APPLE_COUNT; i++)
    {
        if ((int)v.x == (int)applePositions[i].x && (int)v.y == (int)applePositions[i].y) return i;
    } 
    fprintf(stdout, "Invalid Apple Position supplied!");
    exit(1);
}

void init() {
    // Set snakeBody to zeroes
    for (int i = 0; i < GRID_SIZE*GRID_SIZE; i++)
    {
        snakeBody.pos[i].x = 0;
        snakeBody.pos[i].y = 0;
    }
    snakeBody.length = START_LENGTH;

    for (int i = 0; i < snakeBody.length; i++)
    {
        snakeBody.pos[i].x = (GRID_SIZE)/2 -1 -i;
        snakeBody.pos[i].y = (GRID_SIZE)/2; 
    } 
    
    // printf("Snake Body: ");
    // for (int i = 0; i < snakeBody.length; i++)
    // {
    //     printf(" %f,%f", snakeBody.pos[i].x, snakeBody.pos[i].y);
    // }


    direction = RIGHT;

    // Apples
    for (int i = 0; i < APPLE_COUNT; i++)
    {
         applePositions[i] = getValidApplePosition();
    }
    

}

// Returns: 1 if there is snake or wall, 0 if empty, -1 if apple
int isOccupied(Vector2 pos) {
    if (pos.x > GRID_SIZE -1 || pos.y > GRID_SIZE -1) return 1; // OUT OF BOUNDS
    if (pos.x < 0 || pos.y < 0) return 1;
    for (int i = 0; i < snakeBody.length; i++)
    {
        if (snakeBody.pos[i].x == pos.x && snakeBody.pos[i].y == pos.y) return 1; // IS SNAKE
    }

    for (int i = 0; i < APPLE_COUNT; i++)
    {
        if(applePositions[i].x == pos.x && applePositions[i].y == pos.y) return -1;
    }
    return 0;
    
}


void drawAllApples() {
    for (int i = 0; i < APPLE_COUNT; i++)
    {
        drawApple(applePositions[i]);
    }
    
}
void drawApple(Vector2 v) {
    if (v.x <0) {
        fprintf(stderr, "Drawing apple out of Bounds - x ");
    } else if (v.y < 0) {
        fprintf(stderr, "Drawing apple out of Bounds - y ");
    } else {
        DrawRectangle(v.x * CELL_SIZE, v.y * CELL_SIZE, CELL_SIZE-2, CELL_SIZE-2,RED);
    }
}


void drawSnakeBody() {
    for (int i = 0; i < snakeBody.length; i++) {
        drawBox(snakeBody.pos[i]);
    }

    // 1. Get pixel position of the head
    float hX = snakeBody.pos[0].x * CELL_SIZE;
    float hY = snakeBody.pos[0].y * CELL_SIZE;

    // 2. Size of the eye rectangles
    float eyeSize = CELL_SIZE / 6; 
    float offset = CELL_SIZE / 5; // Distance from the edges

    // 3. Draw eyes based on direction
    switch (direction) {
        case UP:
            DrawRectangle(hX + offset, hY + offset, eyeSize, eyeSize, GRAY);
            DrawRectangle(hX + CELL_SIZE - offset - eyeSize, hY + offset, eyeSize, eyeSize, GRAY);
            break;
        case DOWN:
            DrawRectangle(hX + offset, hY + CELL_SIZE - offset - eyeSize, eyeSize, eyeSize, GRAY);
            DrawRectangle(hX + CELL_SIZE - offset - eyeSize, hY + CELL_SIZE - offset - eyeSize, eyeSize, eyeSize, GRAY);
            break;
        case LEFT:
            DrawRectangle(hX + offset, hY + offset, eyeSize, eyeSize, GRAY);
            DrawRectangle(hX + offset, hY + CELL_SIZE - offset - eyeSize, eyeSize, eyeSize, GRAY);
            break;
        case RIGHT:
            DrawRectangle(hX + CELL_SIZE - offset - eyeSize, hY + offset, eyeSize, eyeSize, GRAY);
            DrawRectangle(hX + CELL_SIZE - offset - eyeSize, hY + CELL_SIZE - offset - eyeSize, eyeSize, eyeSize, GRAY);
            break;
    }
}

void drawGrid() {
    for (unsigned int i = CELL_SIZE; i < SIZE; i+=CELL_SIZE)
            {
                DrawLine(0, i, SIZE, i, BLACK);
                DrawLine(i, 0, i, SIZE, BLACK);
            }
    for (unsigned int i = 0; i < GRID_SIZE; i++)
    {
        for (unsigned int j = 0; j < GRID_SIZE; j++)
        {
            DrawRectangleLines(i*CELL_SIZE+1, j*CELL_SIZE+1, CELL_SIZE-2, CELL_SIZE-2, WHITE);
        }
        
    }
    
    
}

void drawBox(Vector2 v) {
    if (v.x <0) {
        fprintf(stderr, "Drawing out of Bounds - x ");
    } else if (v.y < 0) {
        fprintf(stderr, "Drawing out of Bounds - y ");
    } else {
        DrawRectangle((float)(CELL_SIZE*v.x), (float)(CELL_SIZE*v.y), CELL_SIZE, CELL_SIZE, BLACK);
    }
}

int random(int min, int max) {
    return (rand() % (max - min +1)) + min;
}

Vector2 getValidApplePosition() {
    Vector2 randPos;
    // Calculate max index once to avoid confusion
    // 400 / 20 = 20, so we want indices 0 through 19
    int maxIndex = (SIZE / GRID_SIZE) - 1; 

    do {
    // Use Raylib's GetRandomValue for guaranteed seeding and bounds
        randPos.x = (float)GetRandomValue(0, maxIndex);
        randPos.y = (float)GetRandomValue(0, maxIndex);
    } while (isOccupied(randPos) != 0);

    return randPos;
}


void drawCenteredText(const char* text, int xOffset, int yOffset, int fontSize, Color color) {
    // 1. Calculate how wide the text is in pixels
    int textWidth = MeasureText(text, fontSize);
    
    // 2. Calculate coordinates
    // We take the screen middle and subtract half the text size
    int posX = (GetScreenWidth() / 2) - (textWidth / 2) + xOffset;
    int posY = (GetScreenHeight() / 2) - (fontSize / 2) + yOffset;
    
    // 3. Draw
    DrawText(text, posX, posY, fontSize, color);
}