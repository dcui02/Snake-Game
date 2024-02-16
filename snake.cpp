#include <cstring>
#include <cstdbool>
#include <cstdlib>
#include "snake.hpp"
#include "key.hpp"
#include <ncurses.h>
#include <stdexcept>
#include <vector>
#include <algorithm>

#define DIRECTION_ERROR "Invalid direction"

using namespace std;



// Initialize snake
Snake *init_snake(int x, int y)
{
  Snake *head = create_tail(x, y);
  Snake *tail1 = create_tail(x - 1, y);
  Snake *tail2 = create_tail(x - 2, y);
  tail1->next = tail2;
  head->next = tail1;
  return head;
}

// Creates one tail
Snake *create_tail(int x, int y)
{
  Snake *snake = new Snake;
  snake->color[0] = 0;
  snake->color[1] = 0;
  snake->color[2] = 255;
  /* snake->color = {0, 0, 255}; */
  snake->symbol = '#';
  snake->next = NULL;
  snake->x = x;
  snake->y = y;
  return snake;
}

//checks if the position provided is allowed or not
bool check_position(Snake *snake, int y, int x) {
    if (snake == NULL) return false;
    return (snake->y == y && snake->x == x) || check_position(snake->next, y, x);
}
//new head of the snake
////sets all features of the old snake to the new snake
void snake_new_head(Snake* old_snake, Snake* new_head, int x_coor, int y_coor) {
    new_head->x = x_coor;
    new_head->y = y_coor;
    new_head->speed = old_snake->speed;
    strncpy(new_head->color, old_snake->color, 3);
    new_head->symbol = old_snake->symbol;
    new_head->next = old_snake;
}

//length of snake
int snake_length(Snake* snake) {
	if(snake == nullptr) {
		return 0;
	}
	return 1 + snake_length(snake->next);
}
// Moves the snake in the input direction
// //adding food to this function since if the snake grows or shrinks based on the food it eats
Snake_Movement *move_snake(Snake *snake, int direction, Food* foods, vector<pair<int, int>> *cords)
{
  // TODO
  Snake *new_head = new Snake;
  Snake_Movement* new_snake = new Snake_Movement;
  int x_coor = 0;
  int y_coor = 0;

  // Set the new head to have the x and y coordinates as the existing head of the snake
  switch (direction)
  {
  case UP:
    // Write code to make the new head go up by 1 cell
    x_coor = snake->x;
    y_coor = snake->y - 1;
    break;
  case DOWN:
    // Write code to make the new head go down by 1 cell
    x_coor = snake->x;
    y_coor = snake->y + 1;
    break;
  case RIGHT:
    // Write code to make the new head go right by 1 cell
    x_coor = snake->x + 1;
    y_coor = snake->y;
    break;
  case LEFT:
    // Write code to make the new head go left by 1 cell
    x_coor = snake->x - 1;
    y_coor = snake->y;
    break;
  //assumes snake isn't eating any food 
  default:
    new_snake->snake = snake;
    new_snake->eat_food = false;
    return new_snake;
  }
  
  snake_new_head(snake, new_head, x_coor, y_coor);
  new_snake->alive = true;

  if(eat_itself(new_head)) {
	new_snake->alive = false;
  }
  else if (food_exists(foods, x_coor, y_coor)) {
	if(food_type(foods, x_coor, y_coor) == Decrease) {
		if(snake_length(new_head) > 2) {
			new_head = remove_tail(new_head);	
			new_head = remove_tail(new_head);
		}
		else {
			new_snake->alive = false;
		}
		new_snake->food_type = Decrease;
		if (new_head == nullptr) {
			new_snake->alive = false;
		}
	}
	else {
		new_snake->food_type = Increase;
	}
	new_snake->eat_food = true;
	} 
	else if (vector_location(*cords, x_coor, y_coor)) {
		new_snake->alive = false;
		new_snake->eat_food = false;
	}
	else {
		new_head = remove_tail(new_head);
		new_snake->eat_food = false;
	}
	new_snake->snake = new_head;  
  return new_snake;
}

Snake *remove_tail(Snake *snake)
{
  Snake *end = snake;
  while (end->next->next)
    end = end->next;
  free(end->next);
  end->next = NULL;
  return snake;
}
// draws the snake on the board
void draw_snake(Snake *snake)
{
  init_pair(5, COLOR_BLACK, COLOR_WHITE);
  attron(COLOR_PAIR(5));
  while (snake)
  {
    mvprintw(snake->y, snake->x, "%c", snake->symbol);
    snake = snake->next;
  }
  attroff(COLOR_PAIR(5));
}
// checks if it eats itself, if it does, then return true
bool eat_itself(Snake *snake)
{
      return check_position(snake->next, snake->y, snake->x);
}
