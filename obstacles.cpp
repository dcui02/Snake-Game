#include <ncurses.h>
#include "obstacles.hpp"

using namespace std;


Obstacle* add_obstacle(int row, int col, int size, char shape) {
    auto *new_obstacle = new Obstacle;
    new_obstacle->row = row;
    new_obstacle->col = col;
    new_obstacle->size = size;
    new_obstacle->shape = shape;
    new_obstacle->neighbor = nullptr;
    return new_obstacle;
}


//adds a new obstacle 
void add_new_obstacle(Obstacle* obstacles, Obstacle* new_obstacles) {
    Obstacle* temp = obstacles;
    while(temp->neighbor) {
        temp = temp->neighbor;
    }
    temp->neighbor = new_obstacles;
}

// is there an obstacle at coordinates row ,col?
bool has_obstacle(Obstacle* obstacles, int row, int col) {
    if (obstacles == nullptr) {
        return false;
    }
//boolean that checks if obstacle exists at row,col based on obstacles row,col and size
    bool blocker  = obstacles->row  <= row && (obstacles->row + obstacles->size) >= row
            && obstacles->col <= col && (obstacles->col + obstacles->size) >= col;

    return blocker || has_obstacle(obstacles->neighbor,row, col);
}


// draws obstacles onto the screen
void draw_obstacle(Obstacle* obstacles) {
    Obstacle* temp = obstacles;
    while(temp) {
        int row_left  = temp->row;
        int col_left  = temp->col;
        int size = temp->size;
	char shape = obstacles->shape;


        init_pair(4, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(4));
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                mvprintw(col_left + i, row_left + j, "%c", shape);
            }
        }
        attroff(COLOR_PAIR(3));


        temp = temp->neighbor;
    }
}
