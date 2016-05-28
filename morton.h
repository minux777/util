#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
//#include <algorithm>
#include <math.h>
#include <map>
#include <limits.h>
#include <ctime>
#include <sys/time.h>

unsigned int Part1By1(unsigned int x);


unsigned int EncodeMorton2(unsigned int x, unsigned int y);
unsigned int Compact1By1(unsigned int x);

unsigned int DecodeMorton2X(unsigned int code);

unsigned int DecodeMorton2Y(unsigned int code);

unsigned int msb32(unsigned int x, unsigned int y);
int zdivide(unsigned int start, unsigned int end, unsigned int *litmax, unsigned int *bigmin);
std::vector<int> read_morton_mdt(const char name[], int flag);
std::vector<std::pair< std::pair<int,int>, int> > decomposeWindow(int x, int y, int n1, int n2);