#include <utility>
#include <vector>
#include <algorithm>

#ifndef HELPER
#define HELPER


using namespace std;

//since this function is used in a lot of the other files decided to make it in its own file so i don't have to remake it over and over in the other files 

//returns true if x,y is found in the cords vector
static bool vector_location(vector<pair<int, int> > cords, int x, int y) {
    return find(
            cords.begin(),
            cords.end(),
            make_pair(x, y)) != cords.end();
}

#endif
