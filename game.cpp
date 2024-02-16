#include <ncurses.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "snake.hpp"
#include "food.hpp"
#include "game_window.hpp"
#include "key.hpp"
#include "game.hpp"
#include <time.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "vectors.cpp"
#include "obstacles.hpp"

using namespace std;


void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}

 //displays score 
void display_score(int x, int y, int score) {
  mvprintw(y, x, "Current score: %d", score);
}

//displays number of lives
void display_lives(int x, int y, int lives) {
    mvprintw(y, x, "Remaining Lives: %d", lives);
}

int get_direction() {
  switch (rand() % 4) {
    case 0:
      return LEFT;
      break;
    case 1:
      return RIGHT;
      break;
    case 2:
      return UP;
      break;
    case 3:
      return DOWN;
      break;
  }
}

Type get_type() {
  if (rand() % 2 == 1) {
    return Increase;
  }
  return Decrease;
}

int food_size(Food *food) {
    Food *temp = food;
    int size = 0;
    while (temp) {
        size++;
        temp = temp->next;
    }
    return size;
}

int random_value(int upperbound) {
    return rand() % upperbound + 1;
}

pair<int, int> valid_coors(vector <pair<int, int>> cords, int width, int height, int x_offset, int y_offset) {
    int x = rand() % width + x_offset;
    int y = rand() % height + +y_offset;

    while (vector_location(cords, x, y)) {
        x = rand() % width + x_offset;
        y = rand() % height + +y_offset;
    }
    return make_pair(x, y);
}
//adds open area to open space
void add_area(vector <pair<int, int>> *cords, pair<int, int> cord, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cords->push_back(make_pair(cord.first + i, cord.second + j));
        }
    }
}
//add snake to open space
void add_snake(Snake *snake, vector <pair<int, int>> *cords) {
    if (snake == nullptr) {
        return;
    }
    cords->push_back(make_pair(snake->x, snake->y));
    add_snake(snake->next, cords);
}
//adds obstacles to open space
void add_obstacles(vector <pair<int, int>> *cords, Obstacle *obstacles) {
    if (obstacles == nullptr) {
        return;
    }
    add_area(cords, make_pair(obstacles->row, obstacles->col), obstacles->size);
    add_obstacles(cords, obstacles->neighbor);
}
//adds border to the open space
void add_border(vector <pair<int, int>> *cords, int width, int height, int x_offset, int y_offset) {
    for (int i = 0; i < width; i++) {
        cords->push_back(make_pair(x_offset + i, y_offset));
        cords->push_back(make_pair(x_offset + i, y_offset + height));
    }
    for (int i = 0; i < height; i++) {
        cords->push_back(make_pair(x_offset, y_offset + i));
        cords->push_back(make_pair(x_offset + width, y_offset + i));
    }
}
//adds food to open space
void add_food(Food *food, vector <pair<int, int>> *cords) {
    if (food == nullptr) {
        return;
    }
    cords->push_back(make_pair(food->x, food->y));
    add_food(food->next, cords);
}

void end_screen(int score) {
      char message[] = "Game over";
    char final_score[] = "Final Score: ";
    int row, col;

    initscr();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 4, (col - strlen(message)) / 4, "%s", message);
    mvprintw(row / 4 + 5, (col - strlen(final_score)) / 4, final_score);
    mvprintw(row / 4 + 5, (col - strlen(final_score)) / 4 + strlen(final_score) + 4, "%d", score);
    refresh();

    mvprintw(row - 4, 0, "Press <ENTER> to exit.");

    while (getch() != 10) {
        refresh();
    }
    endwin();
}

void save_scores(int score) {
    string PATH = "saves/save_best_10.game";
    ifstream infile(PATH.c_str());
    if (!infile) {
        invalid_argument("Invalid text file");
    }
    ostringstream stream_output;
  
    stream_output << infile.rdbuf();
  
    string file_scores = stream_output.str();

    stringstream string_stream;
    vector<int> in;
    string_stream << file_scores;
    string temp;

    int f;

    while (!string_stream.eof()) {
        string_stream >> temp;
        if (stringstream(temp) >> f) {
            in.push_back(f);
        }
        temp = "";
    }

    if (in.size() < 10) {
        in.push_back(score);
    } else {
        int lowest_score = *min_element(in.begin(), in.end());
        if (score > lowest_score) {
            for (int i = 0; i < in.size(); i++) {
                if (in[i] == lowest_score) {
                    in[i] = score;
                    break;
                }
            }
        }
    }

    ofstream file_out(PATH, ios::ate);
    string output;
    for (auto score: in) {
        output += (to_string(score) + "\n");
    }
    file_out << output;
    file_out.close();
}

  
void game(int level) {
  //level of the game
  int difficulty = 1 * pow(2, level);
  
  enum State state = INIT;// Set the initial state  
  static int x_max, y_max;//Max screen size variables
  static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
  gamewindow_t *window; // Name of the board
  Snake *snake;// The snake
  Food *foods,*new_food;  // List of foods (Not an array) 
  Obstacle *obstacles, *new_obstacle;
  vector <pair<int, int>> cords;
  const int height = 30; 
  const int width = 70;
  char ch;

  struct timespec timeret;
  timeret.tv_sec = 0;
  timeret.tv_nsec = 999999999/(4*difficulty);

  //score of current game
  int score = 0;
  //highest score
  int high_score = 0;
  //snakes direction
  int snake_direction = get_direction();

    while (snake_direction == LEFT) {
        snake_direction = get_direction();
    }
    int MAX_FOOD = 10 * difficulty;

    int LIVES = 3;
    int lives = LIVES;
 

  while(state != EXIT){
      switch(state){
      case INIT:
        initscr();
        start_color();
        nodelay(stdscr, TRUE);//Dont wait for char 
        noecho(); // Don't echo input chars
        getmaxyx(stdscr, y_max, x_max);
        keypad(stdscr, TRUE);// making keys work
        curs_set(0); // hide cursor
        timeout(100);

     // Setting height and width of the board
        x_offset = (x_max / 2) - (width / 2);
        y_offset = (y_max / 2) - (height / 2);
            
        //Init board
        window = init_GameWindow(x_offset, y_offset, width, height);
        draw_Gamewindow(window);

        add_border(&cords, width, height, x_offset, y_offset);

        // Init snake
        {
          int snake_x = x_offset + (width / 2);
          int snake_y = y_offset + (height / 2);
          snake = init_snake(snake_x, snake_y);

          add_snake(snake, &cords);
        }

            
         // Init foods
        int food_x, food_y, i;
        enum Type type;


//init obstacles
        { 
          pair<int, int> valid_coor = valid_coors(cords, width, height, x_offset, y_offset);
          int size = random_value(3);
          obstacles = add_obstacle(valid_coor.first, valid_coor.second, size, '?');
          add_area(&cords, valid_coor, size); 
          int num_obstacles = random_value(7) + 3;
          num_obstacles *= difficulty;
          for (int i = 0; i < num_obstacles; i++) {
            size = random_value(3);
            valid_coor = valid_coors(cords, width, height, x_offset, y_offset);
            new_obstacle = add_obstacle(valid_coor.first, valid_coor.second, size, '?');
            add_new_obstacle(obstacles, new_obstacle);
            add_area(&cords, valid_coor, size);
          }
        }
        //Generate 20 foods
        generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
        type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food 
        foods = create_food(food_x, food_y, type);
        for(i = 1; i < 20; i++) {
          generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
          while (food_exists(foods,food_x, food_y) || vector_location(cords, food_x, food_y)) {           
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
          }
          cords.push_back(make_pair(food_x, food_y));
          type = (rand() > RAND_MAX/2) ? Increase : Decrease;
          new_food = create_food(food_x, food_y, type);
          add_new_food(foods, new_food);
        }
        state = ALIVE;
        break;

      case ALIVE:
        ch = get_char();

        cords.clear();
        if (ch == UP || ch == DOWN || ch == RIGHT || ch == LEFT) {
          snake_direction = ch;
        }
   //pausing, resuming, and exiting the game
        if (isalpha(ch) && tolower(ch) == 'q') {
                    state = EXIT;
            } else if (tolower(ch) == 'p') {
                while (true) {
                    char next_input = get_char();
                    if (isalpha(next_input)) {
                        if (tolower(next_input) == 'p') {
                            break;
                        } else if (tolower(next_input) == 'q') {
                            state = EXIT;
                            break;
                        }
                    }
                }
            }
            add_border(&cords, width, height, x_offset, y_offset);
            add_snake(snake, &cords);
            add_obstacles(&cords, obstacles);
            add_food(foods, &cords);
          
//moves snake and updates the score based on the food it eats
	{
       		Snake_Movement *slither = move_snake(snake, snake_direction, foods, &cords);
		snake = slither->snake;
          if (!slither->alive) {
            lives--;
            if(lives == 0) {
              state = DEAD;
            }
            else {
              state = INIT;
            }
          }
          else if (slither->eat_food) {
          	foods = remove_eaten_food(foods, snake->x, snake->y);
          	if (slither->food_type == Increase) {
            	score += 20;
         	}else {
           	 score -= 10;
         	}
       		}
	}
     //updates the speed of the game based on the score
        if (high_score < score) {
		high_score = score;
		if (high_score % 100 == 0) {
			timeret.tv_nsec /= 1.5;
		}
	}
	if (food_size(foods) < MAX_FOOD) {
        	Food *new_food = create_food(rand() % width + x_offset, rand() % height + + y_offset, get_type());
               add_new_food(foods, new_food);
         }


        if (food_size(foods) < MAX_FOOD) {
          while (food_exists(foods, food_x, food_y) || vector_location(cords, food_x, food_y)) {
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
          }
          Food *new_food = create_food(food_x, food_y, get_type());
            add_new_food(foods, new_food);
          }

        clear();
        mvprintw(20,20, "Key entered: %c", ch);                  
        mvprintw(8, 20, "Difficulty: %d", level + 1);
        display_score(20, 10, score);
	      draw_Gamewindow(window);
        draw_snake(snake);
        draw_food(foods);
        draw_obstacle(obstacles);
        display_lives(20,5,lives);
        
        
	break; 

      case DEAD:
        endwin();
        clear();
        refresh();


        save_scores(score);
        end_screen(score);



        state = EXIT;
        break;
      }
      refresh();
      nanosleep(&timeret, NULL);
    }
}
