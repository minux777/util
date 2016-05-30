#include "read_MDT.h"

std::vector<int> read_MDT::read_morton_mdt(const char  name[], int flag = 1){
  FILE *file;
  std::vector<int> to_ret;
  file = fopen(name, "r");
  if(file ==  NULL){
    printf("null");
    return to_ret;
  }
  char buff[1024];
  int c, r, pow2, no_data, _min = 2000000000, _max = 0;
  float temp;
  int unused __attribute__((unused));

  unused = fscanf(file, "%s %d", buff, &c);//cols
  unused = fscanf(file, "%s %d", buff, &r);//rows
  for(int i = 0; i < 4; i++){
    unused = fscanf(file, "%s %f", buff, &temp);
  }
  no_data = (int)temp;
  pow2 = pow(2, ceil(log2(std::max(c, r))));
  
  if(flag == 1){
    int *curve_map = (int*)calloc(pow2 * pow2, sizeof(int));
    std::fill_n(curve_map, pow2 * pow2, no_data);

    for(int i = 0; i < r; i++){
      for(int j = 0; j < c; j++){
	unused = fscanf(file, "%f", &temp);
	if(temp == no_data) continue;
	if(temp < _min) _min = temp;
	if(temp > _max) _max = temp;
	curve_map[EncodeMorton2(i,j)] = (int)(temp);
      }
    }

    fclose(file);
    std::vector <int> V(curve_map, curve_map + pow2 * pow2);
    for(unsigned int i = 0; i < V.size(); i++)
      V[i] = (V[i] == no_data) ? (_max - _min) + 1 : V[i] - _min;
    to_ret = V;

  }else if(flag == 0){
    std::map<unsigned int, int> curve_map;
    for(int i = 0; i < r; i++){
      for(int j = 0; j < c; j++){
	unused = fscanf(file, "%f", &temp);
	if(temp == no_data) continue;
	if(temp < _min) _min = temp;
	if(temp > _max) _max = temp;
	curve_map[EncodeMorton2(i,j)] = (int)(temp);
      }
    }
    fclose(file);
    std::vector <int> V;
    for(std::map<unsigned int, int>::iterator it = curve_map.begin(); it != curve_map.end(); it++)
      V.push_back( (it->second == no_data) ? (_max - _min) + 1 : it -> second - _min );
    to_ret =  V;
  }
    
  
  printf("%s %d\n", ((flag == 1)? "pow2: ": "pow2 not used: "), pow2);

  printf("%s %d\n", "no data: ",no_data);
  printf("%s %d\n", "min: ", _min);
  printf("%s %d\n", "max: ", _max);
  printf("%s %d\n", "new no data: ", (_max - _min) + 1);
  cols_ = c;
  rows_ = r;
  min_ = _min;
  max_ = _max;
  pow2_ = pow2;
  original_no_data_ = no_data;
  new_no_data_ = (_max - _min) + 1;
  return to_ret;
}

int read_MDT::get_cols(){
  return cols_;
}

int read_MDT::get_rows(){
  return rows_;
}

int read_MDT::get_min(){
  return min_;
}

int read_MDT::get_max(){
  return max_;
}

int read_MDT::get_pow2(){
  return pow2_;
}

int read_MDT::get_no_data(){
  return original_no_data_;
}
int read_MDT::get_new_no_data(){
  return new_no_data_;
}