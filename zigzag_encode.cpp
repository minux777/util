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
  
  double average_no_opt_rq = 0, average_zdivide_rq = 0, average_quadbox_rq = 0;
  int sizes[] = {5, 10, 20, 50, 100};
  int different_querys = 10;
  int repeat_for_average = 10;
  string querysfname = "querys";
  struct timeval start, end;
  long int temp_time;
  for(int i = 0; i < 4; i++){
    
    reader.make_random_range_querys( (querysfname + to_string(sizes[i]) + ".txt").c_str(), different_querys, sizes[i], sizes[i]);
    vector<pair<pair<int,int>, pair<int,int> > > querys = 
      reader.read_querys((querysfname + to_string(sizes[i]) + ".txt").c_str());
    
   
    cout << endl;
    for(int j = 0; j < different_querys; j++){
      gettimeofday(&start, NULL);
      for(int k = 0; k < repeat_for_average; k++){
	df.rangeQuery(querys[j].first.first,
		      querys[j].first.second,
		      querys[j].second.first,
		      querys[j].second.second);     
      }
      gettimeofday(&end, NULL);
      temp_time = (double)end.tv_usec - (double)start.tv_usec +  ((double)end.tv_sec - (double)start.tv_sec) *  1000000;     
      cout << "average for points("<< j + 1  << "): p1(" 
	   << querys[j].first.first << ", "
	   << querys[j].first.second << ") p2(" 
	   << querys[j].second.first << ", "
	   << querys[j].second.second << "): " << temp_time / repeat_for_average << " us" << endl;
      average_no_opt_rq += (temp_time/repeat_for_average);
    }
    cout << "\taverage no opt range query (" << sizes[i] << "x" << sizes[i] << "): " 
	 << average_no_opt_rq / (different_querys) << endl;
    average_no_opt_rq = 0;


    cout << endl;
    for(int j = 0; j < different_querys; j++){
      gettimeofday(&start, NULL);
      for(int k = 0; k < repeat_for_average; k++){
	df.rectangle_query(querys[j].first.first,
		      querys[j].first.second,
		      querys[j].second.first,
		      querys[j].second.second);     
      }
      gettimeofday(&end, NULL);
      temp_time = (double)end.tv_usec - (double)start.tv_usec +  ((double)end.tv_sec - (double)start.tv_sec) *  1000000;     
      cout << "average for points("<< j + 1  << "): p1(" 
	   << querys[j].first.first << ", "
	   << querys[j].first.second << ") p2(" 
	   << querys[j].second.first << ", "
	   << querys[j].second.second << "): " << temp_time / repeat_for_average << " us" << endl;
      average_zdivide_rq += (temp_time/repeat_for_average);
    }
    cout << "\taverage zdivide range query (" << sizes[i] << "x" << sizes[i] << "): " 
	 << average_zdivide_rq / (different_querys) << endl;
    average_zdivide_rq = 0;

    
    
    cout << endl;
    for(int j = 0; j < different_querys; j++){
      gettimeofday(&start, NULL);
      for(int k = 0; k < repeat_for_average; k++){
	df.dqb_range_query(querys[j].first.first,
		      querys[j].first.second,
		      querys[j].second.first,
		      querys[j].second.second);     
      }
      gettimeofday(&end, NULL);
      temp_time = (double)end.tv_usec - (double)start.tv_usec +  ((double)end.tv_sec - (double)start.tv_sec) *  1000000;     
      cout << "average for points("<< j + 1  << "): p1(" 
	   << querys[j].first.first << ", "
	   << querys[j].first.second << ") p2(" 
	   << querys[j].second.first << ", "
	   << querys[j].second.second << "): " << temp_time / repeat_for_average << " us" << endl;
      average_quadbox_rq += (temp_time/repeat_for_average);
    }
    cout << "\taverage quadboxes range query (" << sizes[i] << "x" << sizes[i] << "): " 
	 << average_quadbox_rq / (different_querys) << endl;
    average_no_opt_rq = 0;

  }
  return 0;
}
