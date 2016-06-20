#include <vector>
#include <iostream>
#include "headers/read_MDT.h"//mdt-parse-morton
#include "headers/difftree.h"//diferencial encoder
//#include "headers/timing.h"

using namespace std;

int main(int argc, char * argv[]){
  if(argc < 3){
    cout << "use: " << argv[0] << " <filename> <0=without 0s|1=with 0s> <query file name 1>" << endl;
    return -1;
  }
  read_MDT reader;
  vector <int> V = reader.read_morton_mdt(argv[1], atoi(argv[2]));
  reader.print_info();

  DiffTree df(V, 0);//binary heap embbeding
  cout << "size of zigzag encoder (bytes): " << df.getSize() << endl;
 
  vector<pair<pair<int,int>, pair<int,int> > > querys = reader.read_querys(argv[3]);
  
  int **matrix1, **matrix2;
 
  for(int j = 0; j < querys.size(); j++){
    int cols = querys[j].second.second - querys[j].first.second;
    int rows = querys[j].second.first - querys[j].first.first;
    matrix1 = df.rectangle_query(querys[j].first.first, querys[j].first.second, querys[j].second.first, querys[j].second.second);     
    matrix2 = df.dqb_range_query(querys[j].first.first, querys[j].first.second, querys[j].second.first, querys[j].second.second);     
    for(int i = 0; i < rows; i++)
      for(int k = 0; k < cols; k++)
	if(matrix1[i][k] != matrix2[i][k]){
	  cout << "diferents" << endl;
	  break;
	}

    reader.write_matrix(matrix1, cols, rows, (argv[3] + to_string(j)).c_str());
  }

return 0;
}
