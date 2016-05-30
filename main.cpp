#include "read_MDT.h"
#include <iostream>

using namespace std;

int main(){
  read_MDT reader;
  reader.read_morton_mdt("example.MDT", 1);
  cout << reader.get_min() << endl;
  cout << reader.get_max() << endl;
}
