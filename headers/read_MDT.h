#ifndef READ_MDT_H
#define READ_MDT_H

#include "morton.h"
//#include <vector>

class read_MDT
{
 private:
  int cols_, rows_;
  int min_, max_, pow2_, original_no_data_;
  int new_no_data_;
  
 public:  
  std::vector<int> read_morton_mdt(const char name[], int flag);  
  std::vector<std::pair <std::pair<int,int>, std::pair<int,int> > > read_querys(const char filename[]);
  void make_random_range_querys(const char filename[], int n_querys, int height, int width);
  int get_cols();
  int get_rows();
  int get_min();
  int get_max();
  int get_pow2();
  int get_no_data();
  int get_new_no_data();
};
#endif
