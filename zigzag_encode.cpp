#include <sdsl/vectors.hpp>
#include <vector>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support_sada.hpp>
#include <iostream>
#include "morton.h"// mdt-parse-morton
#include "difftree.h"//diferencial encoder

using namespace std;
using namespace sdsl;

int main(int argc, char * argv[]){
  if(argc < 3){
    cout << "use: " << argv[0] << " <filename> <0|1>" << endl;
    return -1;
  }
  cout << "starting" << endl;
  vector <int> V = read_morton_mdt(argv[1], atoi(argv[2]));//zorder vector

  DiffTree df(V, 0);//binary heap embbeding
  cout << df.getSize() << endl;
  
  int x1, x2, y1, y2;
  x1 = rand() % 4001;//fillas MDT500
  x2 = x1 + 50;//rand() % 4001;
  y1 = rand() % 5841;//columnas MDT500
  y2 = y1 + 50;//rand() % 5841;
  /*x1 = 4;
  y1 = 3;
  x2 = 8;
  y2 = 7;
  */
  /*if(x2 < x1){
    int temp = x2;
    x2 = x1;
    x1 = temp;
  }
  if(y2 < y1){
    int temp = y2;
    y2 = y1;
    y1 = temp;
    }*/
  
  cout << "p1: "<< x1 << ", " << y1 << endl;
  cout << "p2: "<< x2 << ", " << y2 << endl;

  
  struct timeval start, end;
  gettimeofday(&start, NULL);
 
  int **matrix = df.dqb_range_query(x1,y1,x2,y2);
  //(int**) malloc(sizeof(int *) * (y2 - y1 + 1));
  /*for(int i = 0; i < (y2 - y1 + 1); i++){
    matrix[i] = (int*) malloc(sizeof(int) * (x2 - x1 + 1));
    if(matrix[i] == NULL)
      cout << "malloc error" << endl;
  }

  vector<pair<pair<int,int>, int> >  querys;
  querys = decomposeWindow(x1, y1, (x2 - x1 + 1), (y2 - y1 + 1));

  for(unsigned int i = 0; i < querys.size(); i++){
    int ** temp;
    temp = df.rangeQuery(querys[i].first.first, 
			 querys[i].first.second,
			 querys[i].first.first + querys[i].second - 1,
			 querys[i].first.second + querys[i].second - 1);

    for(int j = 0; j < querys[i].second; j++){
      for(int k = 0; k < querys[i].second; k++){
	matrix[querys[i].first.first + j - x1][querys[i].first.second + k - y1] =  temp[j][k];
      }
    }
    }*/
  gettimeofday(&end, NULL);
  printf("quadboxes desc query: %fus\n",(double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec));
  
  /*
  cout << "quadboxes query" << endl;
  for(int i = 0; i < x2 - x1 + 1; i++){
    for(int j = 0; j < y2 - y1 + 1; j++)
      cout << matrix[i][j] + (-3) << " ";
    cout << endl;
    }*/
  
  gettimeofday(&start, NULL);
  int **matrix2 = df.rangeQuery(x1,y1,x2,y2);  
  gettimeofday(&end, NULL);
  printf("bad query: %fus\n",(double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec));
  
  /*cout << endl << "rangeQuery" << endl;
  for(int i = 0; i < x2 - x1 + 1; i++){
    for(int j = 0; j < y2 - y1 + 1; j++)
      cout << matrix2[i][j] + (-3) << " ";
    cout << endl;
  }
  */
  
  gettimeofday(&start, NULL);
  int **matrix3 = df.rectangle_query(x1,y1,x2,y2);
  gettimeofday(&end, NULL);
  printf("zdivide query: %fus\n",(double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec));
  
  /*cout << endl << "rectangle_quuery" << endl;
  for(int i = 0; i < x2 - x1 + 1; i++){
    for(int j = 0; j < y2 - y1 + 1; j++)
      cout << matrix3[i][j] + (-3) << " ";
    cout << endl;
  }
  */

  cout << endl << "rectangle_quuery" << endl;
  for(int i = 0; i < x2 - x1 + 1; i++){
    for(int j = 0; j < y2 - y1 + 1; j++)
      cout << ((matrix3[i][j] != matrix[i][j])?"x":"o" )<< " ";
    cout << endl;
  }
  return 0;
}

