#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {
   vector<bool> tempV(mapImg.height(), false);
   vector<vector<bool>> isVisited(mapImg.width(), tempV);
   vector<int> tempD(mapImg.height(), -1);
   vector<vector<int>> distances(mapImg.width(), tempD);
   pair<int,int> temp3 = make_pair(-1,-1);
   vector<pair<int,int>> temp4(mapImg.height(), temp3);
   vector<vector<pair<int,int>>> parentPth(mapImg.width(), temp4);

   distances[start.first][start.second] = 0;
   isVisited[start.first][start.second] = true;
   

   Queue<pair<int,int>> q;
   q.enqueue(start);
   pair<int,int> parent;
   while(!q.isEmpty()) {
      parent = q.dequeue();
      vector<pair<int,int>> n = neighbors(parent);
      for (pair<int,int> p: n) {
         if (good(isVisited, distances, parent, p)) {
            isVisited[p.first][p.second] = true;
            distances[p.first][p.second] = distances[parent.first][parent.second] + 1;
            parentPth[p.first][p.second] = parent;
            q.enqueue(p);
         }
      }
   }
   Stack<pair<int,int>> stack;
   stack.push(parent);
   while (stack.peek() != start) {
      stack.push(parentPth[parent.first][parent.second]);
      parent = parentPth[parent.first][parent.second];
   }
   stack.push(start);
   while (!stack.isEmpty()) {
      pathPts.push_back(stack.pop());
   }

   
   
}

PNG decoder::renderSolution(){
   PNG map = PNG(mapImg);
   for (int i=0; i < pathLength(); i++) {
      pair<int,int> p = pathPts[i];
      RGBAPixel *pixel = map.getPixel(p.first,p.second);
      *pixel->r = 255;
      *pixel->g = 0;
      *pixel->b = 0;

   }

   return map;

}

PNG decoder::renderMaze(){
   PNG map = PNG(mapImg);
   vector<bool> tempV(mapImg.height(), false);
   vector<vector<bool>> isVisited(mapImg.width(), tempV);
   vector<int> tempD(mapImg.height(), -1);
   vector<vector<int>> distances(mapImg.width(), tempD);
   

   distances[start.first][start.second] = 0;
   isVisited[start.first][start.second] = true;
   setGrey(map,start);

   Queue<pair<int,int>> q;
   q.enqueue(start);
   pair<int,int> curr;
   while(!q.isEmpty()) {
      curr = q.dequeue();
      vector<pair<int,int>> n = neighbors(curr);
      for (pair<int,int> p: n) {
         if (good(isVisited, distances, curr, p)) {
            setGrey(map,p);
            isVisited[p.first][p.second] = true;
            distances[p.first][p.second] = distances[curr.first][curr.second] + 1;
            q.enqueue(p);
         }
      }
   }


}

void decoder::setGrey(PNG & im, pair<int,int> loc){
RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
pixel->r = 2*(pixel->r/4);
pixel->g = 2*(pixel->g/4);
pixel->b = 2*(pixel->b/4);

}

pair<int,int> decoder::findSpot(){
   return pathPts[pathPts.size() - 1];
}

int decoder::pathLength(){
   return pathPts.size();
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
   
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
int x = get<0>(curr);
int y = get<1>(curr);
vector<pair<int, int>> neighbors;
neighbors.push_back(make_pair(x-1, y)); //left
neighbors.push_back(make_pair(x, y-1)); //bottom
neighbors.push_back(make_pair(x+1, y)); //right
neighbors.push_back(make_pair(x, y+1)); //top
return neighbors;
}


bool decoder::compare(RGBAPixel p, int d){
   int val = p.b % 4;
   val += (p.g % 4) << 2;
   val += (p.r % 4) << 4;
   return val == (d + 1) % 64;
}
