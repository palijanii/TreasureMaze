#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    start = s;
    maze = mazeim;
    base = baseim;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){
RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
pixel->r = 2*(pixel->r/4);
pixel->g = 2*(pixel->g/4);
pixel->b = 2*(pixel->b/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){
RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
unsigned char r_code = d%64 >> 4;
unsigned char g_code = d%16 >> 2;
unsigned char b_code = d%4;
r_code += (pixel->r >> 2) << 2;
g_code += (pixel->g >> 2) << 2;
b_code += (pixel->b >> 2) << 2;
pixel->r = r_code;
pixel->g = g_code;
pixel->b = b_code;
}

PNG treasureMap::renderMap(){
    PNG map = PNG(base);
    vector<bool> temp(base.height(), false);
    vector<vector<bool>> isVisited(base.width(),temp);
    vector<int> temp2(base.height(), -1);
    vector<vector<int>> distance(base.width(), temp2);
    Queue<pair<int, int>> q;
    int x_start = start.first;
    int y_start = start.second;
    isVisited[x_start][y_start] = true;
    distance[x_start][y_start] = 0;
    setLOB(map, start, 0);
    q.enqueue(start);
    pair<int, int> curr;

    // int distance_counter = 1;
    while(!q.isEmpty()) {
        curr = q.dequeue();
        vector<pair<int, int>> n = neighbors(curr);
        
        for(pair<int, int> p : n) {

            if (good(isVisited, curr, p)) {
                isVisited[get<0>(p)][get<1>(p)] = true;
                distance[p.first][p.second] = distance[curr.first][curr.second] + 1;
                setLOB(map, p, distance[p.first][p.second]);
                q.enqueue(p);
            }
        }
        // distance_counter++;        
    }
    return map;
}


PNG treasureMap::renderMaze(){
    PNG map = PNG(base);
    vector<bool> temp(base.height(), false);
    vector<vector<bool>> isVisited(base.width(),temp);

    setGrey(map, start);
    isVisited[start.first][start.second] = true;
    
    Queue<pair<int, int>> q;
    q.enqueue(start);
    pair<int, int> curr;
    while (!q.isEmpty()) {
        curr = q.dequeue();
        vector<pair<int, int>> n = neighbors(curr);
        for(pair<int, int> p : n) {
            if (good(isVisited, curr, p)) {
                setGrey(map,p);
                isVisited[p.first][p.second] = true;
                q.enqueue(p);
            }
        }
    }

    int x_start = get<0>(start);
    int y_start = get<1>(start);
    vector<pair<int, int>> redpairs;
    for (int i = x_start-3; i <= x_start+3; i++) {
        for (int j = y_start-3; j <= y_start+3; j++) {
            if (!(i<0 || i > maze.width()-1 || j<0 || j > maze.height()-1)) {
                redpairs.push_back(make_pair(i, j));
            }
        }
    }

    for (pair<int, int> p : redpairs) {
        RGBAPixel *pixel = map.getPixel(get<0>(p), get<1>(p));
        pixel->r = 255;
        pixel->g = 0;
        pixel->b = 0;
    }
    return map;
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
unsigned int x_next = next.first;
unsigned int y_next = next.second;
if (x_next < 0 || x_next >= maze.width()) return false;
if (y_next < 0 || y_next >= maze.height()) return false;
if (v[x_next][y_next]) return false;
unsigned int x_curr = curr.first;
unsigned int y_curr = curr.second;
if(*maze.getPixel(x_next,y_next) == *maze.getPixel(start.first, start.second)) {
    return true;
} else {
    return false;
}
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
int x = get<0>(curr);
int y = get<1>(curr);
vector<pair<int, int>> neighbors;
neighbors.push_back(make_pair(x-1, y)); //left
neighbors.push_back(make_pair(x, y-1)); //bottom
neighbors.push_back(make_pair(x+1, y)); //right
neighbors.push_back(make_pair(x, y+1)); //top
return neighbors;
}

