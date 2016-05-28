#include <iostream>
#include "morton.h"

using namespace std;
void some_points_in_rectangle(pair<uint, uint> start_point, int **toret, uint start, uint end, uint tol);
int zdivide(unsigned int start, unsigned int end, unsigned int *litmax, unsigned int *bigmin);
unsigned int msb32(unsigned int x, unsigned int y);
void points_in_rectangle(pair<uint, uint> start_point, int **toret, uint start, uint end, uint last, uint tol);
void rectangle_query(uint x1, uint y1, uint x2, uint y2);

unsigned int msb32(unsigned int x, unsigned int y){
  unsigned int val = x^y;

  static const unsigned int bval[] =
    {0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4};

  unsigned int r = 0;
  if (val & 0xFFFF0000) { r += 16/1; val >>= 16/1; }
  if (val & 0x0000FF00) { r += 16/2; val >>= 16/2; }
  if (val & 0x000000F0) { r += 16/4; val >>= 16/4; }
  return 31 - ((r + bval[val]) - 1);
}

///calulate litmax and bigmin

int zdivide(unsigned int start, unsigned int end, unsigned int *litmax, unsigned int *bigmin){

  int bitdiff = -1;
  //int k = 0;
  *litmax = 0;
  *bigmin = 0;

  bitdiff = msb32(start, end);

  if(bitdiff > 31) {
    //cout << "? equal" << endl;
    return false;
  }

  if(bitdiff % 2 == 0){
    *litmax = end & (1431655765 | ((-1) << (31 - bitdiff + 1)));
    *bigmin = start & (1431655765 | ((-1) << (31 - bitdiff + 1)));

    *bigmin |= (1 << (31 - bitdiff)); 
    *litmax |= (((1 << (31 - bitdiff)) - 1) & 2863311530);
  }else{
    *litmax = end & (2863311530 | ((-1) << (31 - bitdiff + 1)));
    *bigmin = start & (2863311530 | ((-1) << (31 - bitdiff + 1)));

    *bigmin |= (1 << (31 - bitdiff)); 
    *litmax |= (((1 << (31 - bitdiff)) - 1) & 1431655765);
  }
  return false;//linea para warning
}



void points_in_rectangle(pair<uint, uint> start_point, int **toret, uint start, uint end, uint last, uint tol){
  uint litmax, bigmin;
  uint default_tol = tol;
  zdivide(start, end, &litmax, &bigmin);
  if(last < litmax)
    points_in_rectangle(start_point, toret, start, litmax, last, default_tol);
  some_points_in_rectangle(start_point, toret, bigmin, end, default_tol);
}



void some_points_in_rectangle(pair<uint, uint> start_point, int **toret, uint start, uint end, uint tol = 3){
  int curr = start;
  uint default_tol = tol;
  
  uint x1 = DecodeMorton2X(start);
  uint y1 = DecodeMorton2Y(start);
  uint x2 = DecodeMorton2X(end);
  uint y2 = DecodeMorton2Y(end);

  while(curr <= end){
    uint x, y;
    x = DecodeMorton2X(curr);
    y = DecodeMorton2Y(curr);
    if(x < x1 || x > x2 || y < y1 || y > y2){
      tol--;
      if(tol <= 0){
	points_in_rectangle(start_point, toret, start, end, curr, default_tol);
	curr = end;
	tol = default_tol;
	break;
      }
    }else{
      toret[x - start_point.first][y - start_point.second] = curr;
      tol = default_tol;
      curr++;
    }
  }
}

void rectangle_query(uint x1, uint y1, uint x2, uint y2){
  //return NULL;
  int min_point = EncodeMorton2(x1, y1);
  int max_point = EncodeMorton2(x2, y2);
  int cols = y2 - y1 + 1;
  int rows = x2 - x1 + 1;

  int ** matrix = (int**)malloc(rows * sizeof(int *));
  for(int i = 0; i < rows; i++)
    matrix[i] = (int*)malloc(cols * sizeof(int)) ;

  some_points_in_rectangle(make_pair(x1, y1), matrix, min_point, max_point);
  
  for(int i = 0; i < x2 - x1 + 1; i++){
    for(int j = 0; j < y2 - y1 + 1; j++)
      cout << matrix[i][j] << "\t";
    cout << endl;
  }
  cout << endl;
  //return matrix;

}


int main(){
  rectangle_query(21,14,25,19);
  
}
