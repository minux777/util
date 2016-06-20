#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <math.h>
#include <vector>

using namespace std;

int divide(int x1, int y1, int x2, int y2, int times, vector<vector<int> >&V);
int main(int argc, const char *argv[]){
  if(argc < 4){
    cout << "usage: " << argv[0] << " <cols> <rows> <len (a pow2)>" << endl;
    return -1;
  }
  int cols = atoi(argv[1]);
  int rows = atoi(argv[2]);
  int size = pow(2, floor(log2(atoi(argv[3]))));
  int pow2 = pow(2, floor(log2(std::min(cols, rows))));
  if(size >= pow2){
    cout << "len should be less..." << endl;
    return -1;
  }

  vector<vector<int> > V;
  divide(0, 0, pow2, pow2, size, V);
  for(int i = 0; i < V.size(); i++){
    for(int j = 0; j < V[i].size(); j++)
      cout << V[i][j] << " ";
    cout << endl;
  }
  
}

int divide(int x1, int y1, int x2, int y2, int size, vector<vector<int> >&V){
  int midx = (x2 - x1) / 2;
  int midy = (y2 - y1) / 2;
  
  if(size == midx) {
    int arr1[] = {x1, y1, x1 + midx - 1, y1 + midy - 1, midx};
    int arr2[] = {x1 + midx, y1, x1 + midx + midx - 1, y1 + midy - 1, midx};
    int arr3[] = {x1, y1 + midy, x1 + midx - 1, y1 + midy + midy - 1, midx};
    int arr4[] = {x1 + midx, y1 + midy, x1 + midx + midx - 1, y1 + midy + midy - 1, midx};
    vector<int> v1(arr1, arr1 + 5);
    vector<int> v2(arr2, arr2 + 5);
    vector<int> v3(arr3, arr3 + 5);
    vector<int> v4(arr4, arr4 + 5);
    V.push_back(v1);
    V.push_back(v2);
    V.push_back(v3);
    V.push_back(v4);
    /*printf("(%d %d) (%d %d)\n", x1, y1, x1 + midx - 1, y1 + midy - 1);
    printf("(%d %d) (%d %d)\n", x1 + midx, y1, x1 + midx + midx - 1, y1 + midy - 1);
    printf("(%d %d) (%d %d)\n", x1, y1 + midy, x1 + midx - 1, y1 + midy + midy - 1);
    printf("(%d %d) (%d %d)\n", x1 + midx, y1 + midy, x1 + midx + midx - 1, y1 + midy + midy - 1);*/
    return 0;
  }
  else{
    divide(x1, y1, x1 + midx, y1 + midy, size, V);
    divide(x1 + midx, y1, x1 + midx + midx, y1 + midy, size, V);
    divide(x1, y1 + midy, x1 + midx, y1 + midy + midy, size, V);
    divide(x1 + midx, y1 + midy, x1 + midx + midx, y1 + midy + midy, size, V);
  }
}
