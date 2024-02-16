#include "game.hpp"
#include <cstdlib>
#include <cstdio>
#include <ncurses.h>
#include <string.h>
#include "key.hpp"

void difficulty(int level, int row, int col);

//displays start screen, int is the difficulty of the game
int display_start_screen() {
	char welcome[] = "Welcome!";
	int x,y;
	int hardness = 0;



	initscr();
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, x,y);
	mvprintw(x/4,(y-strlen(welcome))/4,"%s", welcome);
	char ch;
	
	while(ch != 10) {
		if(ch == DOWN && hardness <2) {
			hardness++;
		}
		else if (ch == UP && hardness > 0) {
			hardness--;
		}
		difficulty(hardness, x, y);
		mvprintw(x-2,0, "You may choose a different difficulty (the * over the difficulty is the one you are choosing) or press ENTER key to start");
		refresh();
		ch = get_char();
	}
	endwin();
	return hardness;
}

void difficulty(int level, int x, int y) {
    char level_easy[] = "EASY LEVEL";
    char level_moderate[] = "MODERATE LEVEL";
    char level_hard[] = "HARD LEVEL";
    mvprintw((x / 4) + 3,-3 + (y-strlen(level_easy)) / 4,"%s",level_easy);
    mvprintw((x / 4) + 4,-3 + (y-strlen(level_moderate)) / 4,"%s",level_moderate);
    mvprintw((x / 4) + 5,-2 + (y-strlen(level_easy)) / 4,"%s",level_hard);
    mvprintw((x/ 4) + 3 + level,(y-strlen(level_easy)-3) / 4,"%s", "*");
}

//starts the game
int main(void){
    srand(NULL);
    int level =  display_start_screen();
    game(level);
    endwin();
    return(0);
}
