#include <vector>
#include <iostream>
#include "headers/read_MDT.h"//mdt-parse-morton

using namespace std;

int main(int argc, char * argv[]){
if(argc < 3){
  cout << "use: " << argv[0] << " <filename> <outname>" << endl;
  return -1;
 }
 read_MDT reader;
 reader.make_tree_graph(argv[1], argv[2]);
 return 0;
}
