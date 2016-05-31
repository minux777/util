#include <sdsl/vectors.hpp>
#include <vector>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support_sada.hpp>
#include <iostream>
#include "headers/directcodes.h"//FTRep -- dacs
#include "headers/read_MDT.h"// mdt-parse-morton

using namespace std;
using namespace sdsl;


int rle(vector <int> V);
int size_dacs(int_vector <16> v);
int calc_differences(vector<int> &V, int no_data);


int main(int argc, char * argv[]){
  if(argc < 3){
    cout << "use: " << argv[0] << " <filename> <0|1>" << endl;
    return -1;
  }
  read_MDT reader;
  vector <int> V = reader.read_morton_mdt(argv[1], atoi(argv[2]));
  calc_differences(V, -999);
  rle(V);
}

int rle(vector <int> V){
  int_vector<16> v(V.size(), 0);
  bit_vector bv(v.size(), 0);
  int pos = 0;
  cout << ".";
  for(unsigned int i = 0; i < V.size(); i++)
    v[i] = V[i];
  cout << ".";
  
  bv[0] = 1;
  for(unsigned int i = 1; i < v.size(); i++)
    if(v[i] != v[i - 1]){
      pos++;
      bv[i] = 1;
      v[pos] = v[i];
    }
  cout << "." << endl;
  v.resize(pos + 1);
  cout << "original: ";
  cout << "\tsize of v in  bytes: " << size_in_bytes(v) << endl;
  cout << "\tlength of v: " << v.size() << endl;
  cout << "\tsize of bv in bytes: " << size_in_bytes(bv) << endl;  
  
  dac_vector<> dac_v(v);
  cout << ".";
  rrr_vector<63> rrr_bv(bv);
  cout << ".";
  sd_vector<> sd_bv(bv);
  cout << endl;
  cout << "compressed: ";
  cout << "\tsize of dac_v in bytes: " << size_in_bytes(dac_v) << endl;
  cout << "\tsize of rrr_bv in bytes: " << size_in_bytes(rrr_bv) << endl;
  cout << "\tsize of sd_bv in bytes: " << size_in_bytes(sd_bv) << endl;
  cout << endl;
  cout << "size_dacs opt: " << size_dacs(v) << endl;
  return 0;
}
int calc_differences(vector<int> &V, int no_data){
  int _max = -1, _min = 1000000;
  int prev = V[0];
  
  if(V[0] != no_data){
    if(V[0] >= 0)
      V[0] = 2 * V[0];
    else
      V[0] = (-2) * V[0] - 1;
    if(V[0] > _max)_max = V[0];
    if(V[0] < _min)_min = V[0];
  }
  
  for(unsigned int i = 1; i < V.size(); i++){
    if(V[i] == no_data) continue;
    if(V[i - 1] == no_data) prev = 0;

    int temp = V[i];
    V[i] = V[i] - prev;
    prev = temp;

    if(V[i] >= 0)
      V[i] = 2 * V[i];
    else
      V[i] = (-2) * V[i] - 1;

    if(V[i] > _max)_max = V[i];
    if(V[i] < _min)_min = V[i];
  }

  for(unsigned int i = 0; i < V.size(); i++)
    V[i] -= _min;
  
  cout << "differences vector:" << endl;
  cout << "\tmax: " << _max << ", min: "  << _min << endl;
  return _max - _min + 1;
}

int size_dacs(int_vector <16> v){
  vector<int> temp(v.size());
  for(unsigned int i = 0; i < v.size(); i++)
    temp[i] = v[i];
  cout << ".";
  FTRep* f = createFT((unsigned int *)&temp[0] , temp.size());
  destroyFT(f);
  return sizeFT(f);
}
