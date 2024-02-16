#ifndef OBSTACLES
#define OBSTACLES

using namespace std;


struct Obstacle {
	int row;
	int col;
	int size;
	char shape;
	struct Obstacle* neighbor;
};

typedef struct Obstacle Obstacle;

void add_new_obstacle(Obstacle* obstacles, Obstacle* new_obstacles);

Obstacle* add_obstacle(int row, int col, int size, char shape);

bool has_ostacle(Obstacle* obstacles, int row, int col);

void draw_obstacle(Obstacle* obstacles);


#endif //OBSTACLES
