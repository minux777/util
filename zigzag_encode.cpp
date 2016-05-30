#include <sdsl/vectors.hpp>
#include <vector>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support_sada.hpp>
#include <iostream>
//#include "morton.h"// mdt-parse-morton
#include "read_MDT.h"
#include "difftree.h"//diferencial encoder

using namespace std;
using namespace sdsl;

int main(int argc, char * argv[]){
  if(argc < 3){
    cout << "use: " << argv[0] << " <filename> <0=without 0s|1=with 0s>" << endl;
    return -1;
  }
  read_MDT reader;
  vector <int> V = reader.read_morton_mdt(argv[1], atoi(argv[2]));

  DiffTree df(V, 0);//binary heap embbeding
  cout << "size of zigzag encoder (bytes): " << df.getSize() << endl;
  
  int x1, x2, y1, y2;
  x1 = (rand() % 4001);//filas MDT500
  x2 = x1 + 50;//rand() % 4001;
  y1 = (rand() % 5841);//columnas MDT500
  y2 = y1 + 50;//rand() % 5841;
  cout << "p1: ("<< x1 << ", " << y1 << ")" << endl;
  cout << "p2: ("<< x2 << ", " << y2 << ")" << endl;
  
  struct timeval start, end;

  gettimeofday(&start, NULL);  
  int cols = y2 - y1 + 1;
  int rows = x2 - x1 + 1;

  int ** matrix0 = (int**)malloc(rows * sizeof(int *));
  for(int i = 0; i < rows; i++)
    matrix0[i] = (int*)malloc(cols * sizeof(int)) ;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++)
      matrix0[i][j] = df.access(x1 + i,y1 + j);
  
  gettimeofday(&end, NULL);
  printf("access range query: %ldus\n",
	 end.tv_usec - start.tv_usec +  (end.tv_sec - start.tv_sec) *  1000000);
  
  
  gettimeofday(&start, NULL);
  int **matrix2 = df.rangeQuery(x1,y1,x2,y2);  
  gettimeofday(&end, NULL);
  printf("bad range query: %ldus\n",
	 end.tv_usec - start.tv_usec +  (end.tv_sec - start.tv_sec) *  1000000);
 
  
  gettimeofday(&start, NULL);
  int **matrix3 = df.rectangle_query(x1,y1,x2,y2);
  gettimeofday(&end, NULL);
  printf("zdivide range query: %ldus\n",
	 end.tv_usec - start.tv_usec +  (end.tv_sec - start.tv_sec) *  1000000);


  gettimeofday(&start, NULL);
  int **matrix1 = df.dqb_range_query(x1,y1,x2,y2);
  gettimeofday(&end, NULL);
  printf("quadboxes descomposition range query: %ldus\n",
	 end.tv_usec - start.tv_usec +  (end.tv_sec - start.tv_sec) *  1000000);

  
  
  
  /*cout << endl << "quadbox_desc" << endl;
  for(int i = 0; i < x2 - x1 + 1; i++){
    for(int j = 0; j < y2 - y1 + 1; j++)
      cout << ((matrix0[i][j] != matrix1[i][j])?"x":"o" )<< " ";
    cout << endl;
    }

 cout << endl << "naive" << endl;
  for(int i = 0; i < x2 - x1 + 1; i++){
    for(int j = 0; j < y2 - y1 + 1; j++)
      cout << ((matrix0[i][j] != matrix2[i][j])?"x":"o" )<< " ";
    cout << endl;
  }

 cout << endl << "zdivide" << endl;
  for(int i = 0; i < x2 - x1 + 1; i++){
    for(int j = 0; j < y2 - y1 + 1; j++)
      cout << ((matrix0[i][j] != matrix3[i][j])?"x":"o" )<< " ";
    cout << endl;
  }
  */
  return 0;
}

