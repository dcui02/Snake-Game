#include <cstdlib>
#include <ncurses.h>
#include <cstdbool>
#include "food.hpp"
#include <stdexcept>

using namespace std;
//Create new food
Food* create_food(int x, int y, enum Type type){
    Food* new_food = (Food*)malloc(sizeof(Food));
     
    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
        new_food->type = 'O';
    }
    else if(type == Decrease){
        new_food->type = 'X';
    }
    new_food->next = NULL;

    return new_food;
}
//Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}
//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food;
}


enum Type food_type(Food* foods, int x, int y){
    //Implement the code to return the type of the food 
    //    //present at position (x, y)	
  Food* temp = foods;
  while(temp) {
    if(temp->x == x && temp->y == y) {
      if(temp->type == 'O' || temp->type == '+') {
        return Increase;
      }
      else if(temp->type == 'X' || temp->type == '-') {
        return Decrease;
      }
      else {
        throw invalid_argument("Wrong food type");
      }
    }
    temp = temp->next;
  }
  throw invalid_argument("No food");	
}

Food* remove_eaten_food(Food* foods, int x, int y){
	//Implement the code to remove food at position (x,y).
	//	//Create a new linked list of type Food containing only the
	//		//needed food and return this new list
  Food* temp = foods;

  if(temp == NULL) {
    return temp;
  }
 
 if(temp->x == x && temp->y == y) {
    return temp->next;
  }
  else {
    temp->next = remove_eaten_food(temp->next, x, y);
    return temp;
  }
  throw invalid_argument("No food");
}

// Display all the food
void draw_food (Food *foods)
{   Food* temp = foods;
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    while(temp) {
        if (temp->type == 'O' || temp->type == '+') {
            attron(COLOR_PAIR(3));
        } else {
            attron(COLOR_PAIR(2));
        }
        mvprintw(temp->y, temp->x, "%c", temp->type);
        if (temp->type == 'O' || temp->type == '-') {
            attroff(COLOR_PAIR(3));
        } else {
            attroff(COLOR_PAIR(2));
        }
        temp = temp->next;
    }
}
