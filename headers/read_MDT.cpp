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

  cols_ = c;
  rows_ = r;
  min_ = _min;
  max_ = _max;
  pow2_ = pow2;
  original_no_data_ = no_data;
  new_no_data_ = (_max - _min) + 1;
  flag_= flag;

  return to_ret;
}

std::vector<std::pair< std::pair<int,int>, std::pair<int,int> > > read_MDT::read_querys(const char filename[], std::vector<std::pair<int,int> > &W_vector){
  int n_querys;
  std::vector<std::pair< std::pair<int,int>, std::pair<int,int> > > querys;
  FILE *file;
  file = fopen(filename, "r");
  if(file ==  NULL){
    printf("null");
    return querys;
  }
  int unused __attribute__((unused));

  unused = fscanf(file, "%d", &n_querys);
  int x1, x2, y1, y2, bottom_r, up_r;
  for(int i = 0; i < n_querys; i++){
    unused = fscanf(file, "%d %d %d %d %d %d", &x1, &y1, &x2, &y2, &bottom_r, &up_r);
    querys.push_back(std::make_pair(std::make_pair(x1, y1), std::make_pair(x2, y2)));
    W_vector.push_back(std::make_pair(bottom_r, up_r));
  }
  fclose(file);
  return querys;
}


void read_MDT::make_random_range_querys(const char filename[], int n_querys, int height, int width, int range_min, int range_max){
  FILE *file = fopen(filename, "w");
  fprintf(file, "%d\n", n_querys);
  int x1, x2, y1, y2;
  for(int i = 0; i < n_querys; i++){
    x1 = (rand() % (cols_ - height));
    y1 = (rand() % (rows_ - width));
    x2 = x1 + height;
    y2 = y1 + width;
    fprintf(file, "%d %d %d %d %d %d\n", x1, y1, x2, y2, range_min, range_max);
  }
  fclose(file);
}


void read_MDT::make_random_range_querys(const char filename[], int n_querys, int height, int width, int range_len){
  FILE *file = fopen(filename, "w");
  fprintf(file, "%d\n", n_querys);
  int x1, x2, y1, y2;
  for(int i = 0; i < n_querys; i++){
    int range  = rand() % (get_max()- get_min() - range_len);
    range += get_min();
    x1 = (rand() % (cols_ - height));
    y1 = (rand() % (rows_ - width));
    x2 = x1 + height;
    y2 = y1 + width;
    fprintf(file, "%d %d %d %d %d %d\n", x1, y1, x2, y2, range, range + range_len);
  }
  fclose(file);
}






void read_MDT::make_tree_graph(const char name[], const char outname[] = "salida.k2t" ){
  std::vector<int> V = read_morton_mdt(name, 1);
  std::vector<std::vector<int> > graph((max_ - min_) + 2);
  for(unsigned int i = 0; i < V.size(); i++)
    graph[V[i]].push_back(i);
  int nodes = std::max((int)(((int)max_ - min_) + 2), (int)(pow2_*pow2_));
  long int edges = pow2_ * pow2_;
  write_graph(nodes, edges, graph, outname);
}

void read_MDT::write_graph(int nodes, long int edges, std::vector<std::vector<int > > G, const char* filename){
  FILE *fp;
  fp = fopen( filename , "w" );
  fwrite(&nodes , sizeof(int), 1 , fp );
  fwrite(&edges , sizeof(long int), 1 , fp );
  int temp;
  unsigned int i;
  for(i = 0; i < G.size() - 1; i++){
    temp = (i + 1)*(-1);
    fwrite(  &temp, sizeof(int), 1 , fp );
    for(unsigned int j = 0; j < G[i].size(); j++){
      temp = G[i][j] + 1;
      fwrite( &temp , sizeof(int), 1 , fp );
    }
  }
  for(;i < (unsigned)nodes; i++){
    temp = (i + 1)*(-1);
    fwrite(  &temp, sizeof(int), 1 , fp );
  }
  fclose(fp);
}


void read_MDT::print_info(){
  printf("%s %d\n", "cols: ", cols_);
  printf("%s %d\n", "rows: ", rows_);
  printf("%s %d\n", ((flag_ == 1)? "pow2: ": "pow2 not used: "), pow2_);
  printf("%s %d\n", "no data: ",original_no_data_);
  printf("%s %d\n", "min: ", min_);
  printf("%s %d\n", "max: ", max_);
  printf("%s %d\n", "new no data: ", new_no_data_);
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
void read_MDT::write_matrix(int **matrix, int cols, int rows, const char filename[]){
  FILE *file = fopen(filename, "w");
  fprintf(file, "%d %d\n", cols, rows);
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++)
      fprintf(file, "%d%s",matrix[i][j], (j==cols - 1)?"":" ");
      fprintf(file, "\n");
  }
  fclose(file);
}
