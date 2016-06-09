#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../k2tree/kTree.h"
#include "headers/morton.h"
#include "headers/timing.h"
#include "headers/read_MDT.h"
using namespace std;

int main(int argc, char* argv[]){

  if(argc < 6){
    fprintf(stderr,"USAGE: %s <GRAPH> <queries> <repetitions> <min value> <max value>\n",argv[0]);
    return(-1);
  }
  TREP * trep = loadTreeRepresentation(argv[1]);
  uint reps = atoi(argv[3]);
  uint _min_ = atoi(argv[4]);
  uint _max_ = atoi(argv[5]);

  read_MDT reader;
  vector<pair<pair<int,int>, pair<int, int> > > querys = reader.read_querys(argv[2]);
  
  uint ** respuesta;
  uint count = 0;


  //vector <int[3]> out;
  /*int cols = querys.second.second - querys.first.second + 1;
  int rows = querys.second.first - querys.first.first + 1;
  uint ** matrix = (int**) malloc(sizeof())
  */
  //hacer esto viernes


  startTimer();
  for(int i = 0; i < reps; i++){
    for(int j = 0; j < querys.size(); j++){
      vector <pair <pair< int, int>, int> > sub_query = 
	decomposeWindow(querys[j].first.first, querys[j].first.second,
			 querys[j].second.first - querys[j].first.first + 1, 
			 querys[j].second.second - querys[j].first.second + 1);
      
      for(int k = 0; k < sub_query.size(); k++){
	uint ini = EncodeMorton2(sub_query[k].first.first, sub_query[k].first.second);
	uint end = EncodeMorton2(sub_query[k].first.first + sub_query[k].second - 1, 
				 sub_query[k].first.second + sub_query[k].second - 1);

	//printf("[%d, %d] ini = %d, end = %d\n",0, _max_ - _min_, ini, end);
	respuesta = compactTreeRangeQuery(trep, 1, _max_ - _min_, ini, end);
	/*
	  fprintf(stderr, "--------------\n");
	  fprintf(stderr, "Range: [%d,%d]-[%d,%d], total of links %d\n", 0, _max_ - _min_, ini, end, respuesta[0][0]);
	*/
	/*
	for(int l = 0; l < respuesta[0][0]; l++){
	  printf ("[(%d, %d) = %d] ", DecodeMorton2X(respuesta[1][l+1]), 
		  DecodeMorton2Y(respuesta[1][l+1]), 
		  respuesta[0][l+1]);
	}
	*/
      //fprintf(stderr, "(%d,%d)\t", respuesta[0][l+1], respuesta[1][l+1]);
	    //fprintf(stderr, "\n");
	
      }
    }
  }
  printf("k2tree zorder rq: time = %f(s), time per query = %.2lf(us)\n", timeFromBegin(), (timeFromBegin()*1000000.0/querys.size()/reps));   
  destroyTreeRepresentation(trep);
  return 0;
}
