#include "morton.h"

unsigned int Part1By1(unsigned int x)
{
  x &= 0x0000ffff;                  // x = ---- ---- ---- ---- fedc ba98 7654 3210
  x = (x ^ (x <<  8)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
  x = (x ^ (x <<  4)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
  x = (x ^ (x <<  2)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
  x = (x ^ (x <<  1)) & 0x55555555; // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
  return x;
}


unsigned int EncodeMorton2(unsigned int x, unsigned int y)//x fila y columna
{
  return (Part1By1(x) << 1) + Part1By1(y);
  //return (Part1By1(y) << 1) + Part1By1(x);
}


unsigned int Compact1By1(unsigned int x)
{
  x &= 0x55555555;                  // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
  x = (x ^ (x >>  1)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
  x = (x ^ (x >>  2)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
  x = (x ^ (x >>  4)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
  x = (x ^ (x >>  8)) & 0x0000ffff; // x = ---- ---- ---- ---- fedc ba98 7654 3210
  return x;
}


unsigned int DecodeMorton2X(unsigned int code)//devuelve la fila
{
  return Compact1By1(code >> 1);
}

unsigned int DecodeMorton2Y(unsigned int code)//devuelve la columna
{
  return Compact1By1(code >> 0);
}

/*bool printbit(unsigned int val){
  cout << val << " en binario es: ";
  for(int i = 0; i < 32; i++)
    cout << ((val >> 31 - i) & 1) << " ";
  cout << endl;
}

bool pb(unsigned int val){
  printbit(val);
}
*/

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


std::vector<int> read_morton_mdt(const char  name[], int flag = 1){
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
  return to_ret;
}


std::vector<std::pair< std::pair<int,int>, int> > decomposeWindow(int x, int y, int n1, int n2){
  int k = 1;
  int two_k = 2;
  int two_k_minus_one = 1;
  int x_plus_n2 = 0;
  int y_plus_n1 = 0;
  int n1_divided = 0;
  int n2_divided = 0;	
  int i = 0;
  std::vector<std::pair <std::pair<int, int >, int> >to_ret;
  while(1){
    if( x % two_k ){
      n1_divided = n1 / two_k_minus_one;
      for(i = 0; i < n1_divided; i++){
	//printf("%d %d %d\n", x, y + two_k_minus_one * i, two_k_minus_one);
	to_ret.push_back(std::make_pair(std::make_pair(x, y + two_k_minus_one * i), two_k_minus_one));
      }

      x += two_k_minus_one;
      n2 -= two_k_minus_one;
    }
 
    if(!n1 || !n2){
      break;
    }

    if( y % two_k ){
      n2_divided = n2 / two_k_minus_one;
      for(i = 0; i < n2_divided; i++){
	//printf("%d %d %d\n", x + two_k_minus_one * i, y, two_k_minus_one);
	to_ret.push_back(std::make_pair(std::make_pair(x + two_k_minus_one * i, y), two_k_minus_one));
      }

      y += two_k_minus_one;
      n1 -= two_k_minus_one;
    }

    x_plus_n2 = x + n2;
    y_plus_n1 = y + n1;

    if(!n1 || !n2){
      break;
    }

    if( x_plus_n2 % two_k ){
      n1_divided = n1 / two_k_minus_one;
      for(i = 0; i < n1_divided; i++){
	//printf("%d %d %d\n", x + n2 - two_k_minus_one, y + two_k_minus_one * i, two_k_minus_one);
	to_ret.push_back(std::make_pair(std::make_pair(x + n2 - two_k_minus_one, y + two_k_minus_one * i), two_k_minus_one));
      }

      n2 -= two_k_minus_one;
    }

    if(!n1 || !n2){
      break;
    }

    if( y_plus_n1 % two_k ){
      n1_divided = n2 / two_k_minus_one;
      for(i = 0; i < n1_divided; i++){
	//printf("%d %d %d\n", x + two_k_minus_one * i, y + n1 - two_k_minus_one, two_k_minus_one);
	to_ret.push_back(std::make_pair(std::make_pair(x + two_k_minus_one * i, y + n1 - two_k_minus_one), two_k_minus_one));
      }

      n1 -= two_k_minus_one;
    }

    if(!n1 || !n2){
      break;
    }

    two_k <<= 1;
    two_k_minus_one <<= 1;
  }
  return to_ret;
}
