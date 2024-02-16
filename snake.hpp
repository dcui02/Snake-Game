#include "vectors.cpp"
#include <cstdbool>
#include "food.hpp"

struct Snake {
  int x;
  int y;
  int speed;
  char color[3];
  char symbol;
  struct Snake* next;
};


//new struct that shows snake moving while also factoring in the food that it may or not eat 
struct Snake_Movement {
    struct Snake* snake;
    bool eat_food;
    Type food_type;
    bool alive;
};

typedef struct Snake Snake;

typedef struct Snake_Movement Snake_Movement;

int snake_length(Snake* snake);
Snake* init_snake(int x, int y);
Snake* create_tail(int x, int y);
Snake_Movement* move_snake(Snake* snake, int direction, Food* food, vector<pair<int, int>> *vector_location);
void draw_snake(Snake* snake);
bool eat_itself(Snake* snake);
Snake* remove_tail(Snake* snake);
int len(Snake* snake);
