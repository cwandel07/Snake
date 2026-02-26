void update();
void updateDirection();

void drawGrid();
int isOccupied(Vector2 pos);
void drawBox(Vector2 v);
void drawAllApples();

void drawApple(Vector2 v);
void drawSnakeBody();

void init();
int random(int max, int min);

enum  {
    UP, RIGHT, DOWN, LEFT
} direction;

int findAppleIndexByPos(Vector2 v);
Vector2 getValidApplePosition();