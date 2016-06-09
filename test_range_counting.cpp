#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../k2tree/kTree.h"
#include "headers/morton.h"
using namespace std;
/* Time meassuring */
double ticks;
struct tms t1,t2;

void start_clock() {
  times (&t1);
}

double stop_clock() {
  times (&t2);
  return (t2.tms_utime-t1.tms_utime)/ticks;
}
/* end Time meassuring */


int main(int argc, char* argv[]){

  if(argc<4){
    fprintf(stderr,"USAGE: %s <GRAPH> <queries> <repetitions>\n",argv[0]);
    return(-1);
  }
  FILE *f;	
  TREP * trep = loadTreeRepresentation(argv[1]);
  uint reps = atoi(argv[3]);
  readMDT reader;
  vector<pair<pair<int,int>, pair<int, int> > > querys = reader.read_qeurys(argv[2]);

  uint ** respuesta;
  uint count = 0;

  for(int i = 0; i < reps; i++){
    for(int j = 0; j < querys.size(); j++){
      respuesta = compactTreeRangeQuery(trep, 0, 15, querys[j].first, querys[j].);
    }
  }

  double t1, t2;
  ticks = (double)sysconf(_SC_CLK_TCK);
  t1 = t2 = 0;

  while(nqueries--) {
    uint p1,p2,q1,q2,i;

    fread(&p1,sizeof(uint),1,f);
    fread(&p2,sizeof(uint),1,f);
    fread(&q1,sizeof(uint),1,f);
    fread(&q2,sizeof(uint),1,f);

    start_clock();
  
    for(i=0; i<reps; i++)
      respuesta = compactTreeRangeQuery(trep, p1, p2, q1, q2);

    t1 += stop_clock(); 

    
      fprintf(stderr,"--------------\n");
      fprintf(stderr,"Range: [%d,%d]-[%d,%d], total of links %d\n",p1,p2,q1,q2,respuesta[0][0]);

      for(i=0;i<respuesta[0][0];i++)
      fprintf(stderr,"(%d,%d)\t",respuesta[0][i+1],respuesta[1][i+1]);
      fprintf(stderr,"\n");
    


    start_clock();
  
    for(i=0; i<reps; i++)
      count = compactTreeRangeQueryCount(trep, p1, p2, q1, q2);

    t2 += stop_clock(); 


    if(respuesta[0][0]!= count) {
      fprintf(stderr,"--------------\n");
      fprintf(stderr,"ERROR\n");
      fprintf(stderr, "%d != %d\n", respuesta[0][0],count);
      return -1;
    }

    //fprintf(stderr,"--------------\n");
    //fprintf(stderr,"Range: [%d,%d]-[%d,%d], total of links %d\n",p1,p2,q1,q2,count);

  }
  
  //fprintf(stderr,"--------------\n");
  fprintf(stderr, "%.2lf;%.2lf\n", t1/reps, t2/reps);

  fclose(f);
  destroyTreeRepresentation(trep);

  
  return 0;
}
