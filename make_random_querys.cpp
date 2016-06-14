#include <vector>
#include <iostream>
#include "headers/read_MDT.h"//mdt-parse-morton

using namespace std;
//using namespace sdsl;

int main(int argc, char * argv[]){
  cout << argc << endl;
  if(argc < 5 || (argc % 2 == 0)){
    cout << "use: " << argv[0] << " <MDT filename> <query filename out> <nquerys> <size of range> <w1> <h1> [<w2> <h2>] ... [<wn> <hn>]" << endl;
    return -1;
  }
  read_MDT reader;
  vector <int> V = reader.read_morton_mdt(argv[1], 1);
  int nquerys = atoi(argv[3]);
  string querysfname(argv[2]);
  querysfname += "-";
  vector< pair <int,int> > sizes;
  for(int i = 0; i < (argc - 5) / 2; i++)
    sizes.push_back(make_pair(atoi(argv[5 + 2*i]), atoi(argv[5 + 2*i + 1])));
 
  int range  = rand() % (reader.get_max() - reader.get_min() - atoi(argv[4]));
  range += reader.get_min();
  
  for(int i = 0; i < sizes.size(); i++)
    reader.make_random_range_querys((
				     querysfname + 
				     to_string(sizes[i].first) + "x" 
				     + to_string(sizes[i].second) + "-" + to_string(atoi(argv[4])) + ".txt").c_str(), 
				    nquerys, sizes[i].first, sizes[i].second, range, range + atoi(argv[4]));
  return 0;
}
