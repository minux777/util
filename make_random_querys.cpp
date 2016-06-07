#include <vector>
#include <iostream>
#include "headers/read_MDT.h"//mdt-parse-morton

using namespace std;
//using namespace sdsl;

int main(int argc, char * argv[]){
  cout << argc << endl;
  if(argc < 4 || (argc % 2 == 1)){
    cout << "use: " << argv[0] << " <MDT filename> <query filename out> <nquerys> <w1> <h1> [<w2> <h2>] ... [<wn> <hn>]" << endl;
    return -1;
  }
  read_MDT reader;
  vector <int> V = reader.read_morton_mdt(argv[1], 1);
  int nquerys = atoi(argv[3]);
  string querysfname(argv[2]);
  querysfname += "-";
  vector< pair <int,int> > sizes;
  for(int i = 0; i < (argc - 4) / 2; i++)
    sizes.push_back(make_pair(atoi(argv[4 + 2*i]), atoi(argv[4 + 2*i + 1])));
 
  
  for(int i = 0; i < sizes.size(); i++)
    reader.make_random_range_querys((
				     querysfname + 
				     to_string(sizes[i].first) + "x" 
				     + to_string(sizes[i].second) + ".txt").c_str(), 
				    nquerys, sizes[i].first, sizes[i].second);
  
  return 0;
}
