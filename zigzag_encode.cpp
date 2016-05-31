//#include <sdsl/vectors.hpp>
#include <vector>
//#include <sdsl/bit_vectors.hpp>
//#include <sdsl/bp_support_sada.hpp>
#include <iostream>
#include "headers/read_MDT.h"//mdt-parse-morton
#include "headers/difftree.h"//diferencial encoder

using namespace std;
//using namespace sdsl;

int main(int argc, char * argv[]){
  if(argc < 3){
    cout << "use: " << argv[0] << " <filename> <0=without 0s|1=with 0s>" << endl;
    return -1;
  }
  read_MDT reader;
  vector <int> V = reader.read_morton_mdt(argv[1], atoi(argv[2]));

  DiffTree df(V, 0);//binary heap embbeding
  cout << "size of zigzag encoder (bytes): " << df.getSize() << endl;
  
  int x1, x2, y1, y2, w, h;

  int sizes[] = {5, 10, 20, 50};
  for(int p = 0; p < 4; p++){
    w = h = sizes[p];
    cout << endl << "query window size: " << w << "x" << h << endl;
    int times = 10;
    double average_arq = 0, average_brq = 0, average_zdrq = 0, average_qbrq = 0;
    while(times--){
      x1 = (rand() % reader.get_rows());
      x2 = x1 + h;//height
      y1 = (rand() % reader.get_cols());
      y2 = y1 + w;//width
      
      cout << "Q" << 10 - times << " :";
      cout << "p1("<< x1 << ", " << y1 << ") ---\t";
      cout << "p2("<< x2 << ", " << y2 << ")" << "\t";
    
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
      double temp_time = (double)end.tv_usec - (double)start.tv_usec +  ((double)end.tv_sec - (double)start.tv_sec) *  1000000;
      average_arq += temp_time;
      cout << "access rq: " << temp_time << "\t";

      gettimeofday(&start, NULL);
      int **matrix1 = df.rangeQuery(x1,y1,x2,y2);  
      gettimeofday(&end, NULL);
      temp_time = (double)end.tv_usec - (double)start.tv_usec +  ((double)end.tv_sec - (double)start.tv_sec) *  1000000;     
      average_brq += temp_time;
      cout << "bad rq: " << temp_time << "\t";


      gettimeofday(&start, NULL);
      int **matrix2 = df.rectangle_query(x1,y1,x2,y2);
      gettimeofday(&end, NULL);
      temp_time = (double)end.tv_usec - (double)start.tv_usec +  ((double)end.tv_sec - (double)start.tv_sec) *  1000000;   
      average_zdrq += temp_time;
      cout << "zdivide rq: " << temp_time << "\t";
      
      gettimeofday(&start, NULL);
      int **matrix3 = df.dqb_range_query(x1,y1,x2,y2);
      gettimeofday(&end, NULL);
      temp_time = (double)end.tv_usec - (double)start.tv_usec +  ((double)end.tv_sec - (double)start.tv_sec) *  1000000;
      average_qbrq += temp_time;
      cout << "quadboxes rq: " << temp_time << endl;

      free(matrix0);
      free(matrix1);
      free(matrix2);
      free(matrix3);
    }
    cout << "average access range query: " << (average_arq / 10) << endl;
    cout << "average bad range query: " << (average_brq / 10) << endl;
    cout << "average zdidive range query: " << (average_zdrq / 10) << endl;
    cout << "average quadboxes range query: " << (average_qbrq / 10) << endl;

  }
  return 0;
}
