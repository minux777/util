#include <vector>
#include <iostream>
#include "headers/read_MDT.h"//mdt-parse-morton

using namespace std;
//using namespace sdsl;

int main(int argc, char * argv[]){
  cout << argc << endl;
  if(argc < 7 || ((argc - 4) % 3 != 0)){
    cout << "use: " << argv[0] << " <MDT filename> <query filename out> <nquerys> <size of range 1> <w1> <h1> [<size of range 2> <w2> <h2>] ... [<size of range n> <wn> <hn>]" << endl;
    return -1;
  }
  read_MDT reader;
  vector <int> V = reader.read_morton_mdt(argv[1], 1);
  reader.print_info();
  int nquerys = atoi(argv[3]);
  string querysfname(argv[2]);
  querysfname += "-";
  vector< pair <int,int> > sizes;
  vector<int> ranges;
  for(int i = 0; i < (argc - 4) / 3; i++){
    sizes.push_back(make_pair(atoi(argv[4 + 3*i + 1]), atoi(argv[4 + 3*i + 2])));
    ranges.push_back(atoi(argv[4 + 3*i]));
  }

  for(int i =0 ; i < ranges.size(); i++)
    cout << sizes[i].first << " " << sizes[i].second << " " << ranges[i] << endl;
  
  //int range  = atoi(argv[4]);
  
  
  for(int i = 0; i < sizes.size(); i++)
    reader.make_random_range_querys((
				     querysfname + 
				     to_string(sizes[i].first) + "x" 
				     + to_string(sizes[i].second) + "-" + to_string(ranges[i]) + ".txt").c_str(), 
				    nquerys, sizes[i].first, sizes[i].second, ranges[i]);
  
  return 0;
}
