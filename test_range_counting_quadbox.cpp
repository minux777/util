#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../k2tree/kTree.h"
#include "headers/morton.h"
#include "headers/timing.h"
#include "headers/read_MDT.h"
using namespace std;

int main(int argc, char* argv[]){

  if(argc < 5){
    fprintf(stderr,"USAGE: %s <GRAPH> <queries> <repetitions> <min_value>\n",argv[0]);
    return(-1);
  }
  TREP * trep = loadTreeRepresentation(argv[1]);
  uint reps = atoi(argv[3]);
  //uint _min_ = atoi(argv[4]);
  //uint _max_ = atoi(argv[5]);
  
  read_MDT reader;
  uint min_value = atoi(argv[4]);
  vector<pair<int, int> > W_range;
  vector<pair<pair<int,int>, pair<int, int> > > querys = reader.read_querys(argv[2], W_range);
  uint ** respuesta;
  uint count = 0;
  
  startTimer();
  for(int i = 0; i < reps; i++){
    for(int j = 0; j < querys.size(); j++){
      /*** uncomment to rebuild submatrix
	   int cols = querys[j].second.second - querys[j].first.second + 1;
	   int rows = querys[j].second.first - querys[j].first.first + 1;
	   int ** matrix = (int**) malloc(sizeof(int*) * rows);

	   for(int i = 0; i < rows; i++)
	   matrix[i] = (int *) malloc(sizeof(int) * cols);
      */
	uint ini = EncodeMorton2(querys[j].first.first, querys[j].first.second);
	uint end = EncodeMorton2(querys[j].second.first, querys[j].second.second);

	//	printf("%d, %d\n", W_range[j].first, W_range[j].second);

	respuesta = compactTreeRangeQuery(trep, W_range[j].first - min_value, W_range[j].second - min_value -1, ini, end);
	count += respuesta[0][0];
	  
	//printf("[(%d, %d) = %d] ", DecodeMorton2X(respuesta[1][l+1]), 	  
	//	 DecodeMorton2Y(respuesta[1][l+1]), 
	//	 respuesta[0][l+1]);


	/* uncomment to rebuild submatrix
	   for(int l = 0; l < respuesta[0][0]; l++){
	   matrix[DecodeMorton2X(respuesta[1][l+1]) - querys[j].first.first ][DecodeMorton2Y(respuesta[1][l+1]) - querys[j].first.second] = respuesta[0][l+1];
	   //printf("[(%d, %d) = %d] ", DecodeMorton2X(respuesta[1][l+1]), 
	   //DecodeMorton2Y(respuesta[1][l+1]), 
	   //respuesta[0][l+1]);
	   }
	*/
      
      /*
	for(int i = 0; i < rows; i++){
	for(int j  = 0; j < cols; j++)
	printf("%d ", matrix[i][j]);
	printf("\n");
	}
      */
    }
  }
  printf("k2tree zorder rq: time = %f(s),results: %u, time per query = %.2lf(us)\n", timeFromBegin(), count, (timeFromBegin()*1000000.0/querys.size()/reps));   
  //destroyTreeRepresentation(trep);
  return 0;
}
