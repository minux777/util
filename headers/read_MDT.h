#ifndef READ_MDT_H
#define READ_MDT_H

#include "morton.h"
//#include <vector>

class read_MDT
{
 private:
  int cols_, rows_;
  int min_, max_, pow2_, original_no_data_, flag_;
  int new_no_data_;
  void write_graph(int nodes, long int edges, std::vector<std::vector<int > > G, const char* filename);
  
 public: 
  /**
   * returns a vector with the MDT file readed in Z-Order
   *  
   * name: the file .asc 
   * flag: read with(1) or without(0) aditional zeros.
   */
  std::vector<int> read_morton_mdt(const char name[], int flag);  

  /**
   * returns a vector with two pairs per index, where the first 
   * pair of the index, represent the point(x, y) of the up-left corner
   * and the second the point of the corrner at the bottom-right corner.
   *  
   * filename: the query file, that has the structure:
   * in the first line has one integer n that represents the number of querys
   * the next n lines have six integers
   * the first two are the x and y coordinates of the up-left corner
   * the second two are the x and y coordinates of the bottom-right corner
   * and the last two are the min and max value for the window (for future windows query)
   *
   * flag: read with(1) or without(0) aditional zeros.
   */
  std::vector<std::pair <std::pair<int,int>, std::pair<int,int> > > read_querys(const char filename[]);

  /**
   * makes a file of random querys that could be readed by the read_querys method
   *  
   * filename: the output filename
   * n_querys: the number querys in the file
   * height: the height of the range query
   * width: the width of the range query
   */
  void make_random_range_querys(const char filename[], int n_querys, int height, int width);

  /**
   * makes a graph file that could be the input file of a k2tree buider
   *
   * name: the .asc file
   * outname: the file outname
   */
  void make_tree_graph(const char name[], const char outname[]);

  /**
   * prints the cols, rows, min, max, pow2, no_data and new_no_data values
   */
  void print_info();

  /**
   *return the cols value
   */
  int get_cols();

  /**
   * return the rows value
   */
  int get_rows();

  /**
   *return the min value
   */
  int get_min();

  /**
   *return the max value
   */
  int get_max();

  /**
   *return the pow value
   */
  int get_pow2();

  /**
   *return the no data value
   */
  int get_no_data();

  /**
   *return new no data value
   */
  int get_new_no_data();
};
#endif
