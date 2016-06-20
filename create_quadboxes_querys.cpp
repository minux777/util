#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

int main(int argc, const char *argv[]){
  if(argc < 7){
    cout << "usage: " << argv[0] << " <cols> <rows> <len (a pow2)> <range len> <min value> <max value>" << endl;
    return -1;
  }
  int cols = atoi(argv[1]);
  int rows = atoi(argv[2]);
  int size = pow(2, floor(log2(atoi(argv[3]))));
  int range_len = atoi(argv[4]);
  int min_value = atoi(argv[5]);
  int max_value = atoi(argv[6]);
  vector<vector<int> > V;
  for(int i = 0; i < rows - size + 1; i += size){
    for(int j = 0; j < cols - size + 1; j += size){
      int temp[] = {i, j, i + size - 1, j + size - 1};
      vector<int> t(temp, temp + 4);
      V.push_back(t);
    }
  }
  random_shuffle(V.begin(), V.end());

  cout << V.size() << endl;
  for(int i = 0; i < V.size(); i++){
    int range = rand() % (max_value - min_value - range_len);
    range += min_value;
    for(int j = 0; j < V[i].size(); j++){
      cout << V[i][j] << " ";
    }
    cout << range << " " << range + range_len << endl;
  }
  
}

