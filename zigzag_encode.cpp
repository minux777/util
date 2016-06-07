#include <vector>
#include <iostream>
#include "headers/read_MDT.h"//mdt-parse-morton
#include "headers/difftree.h"//diferencial encoder
#include "headers/timing.h"

using namespace std;

int main(int argc, char * argv[]){
if(argc < 3){
  cout << "use: " << argv[0] << " <filename> <0=without 0s|1=with 0s> [nºrep querys] [<query file name 1>] [<query file name 2>] ... [<query file name n>]" << endl;
  return -1;
 }
 read_MDT reader;
 vector <int> V = reader.read_morton_mdt(argv[1], atoi(argv[2]));

 DiffTree df(V, 0);//binary heap embbeding
 cout << "size of zigzag encoder (bytes): " << df.getSize() << endl;
 
 double average_no_opt_rq = 0, average_zdivide_rq = 0, average_quadbox_rq = 0;

 struct timeval start, end;

 int repeat_for_average;
 if(argc > 3) repeat_for_average = atoi(argv[3]);

 for(int files = 4; files < argc; files++){
   vector<pair<pair<int,int>, pair<int,int> > > querys = reader.read_querys(argv[files]);
   
   startTimer();
   for(int i = 0; i < repeat_for_average; i++){
     for(int j = 0; j < querys.size(); j++){
       df.rangeQuery(querys[j].first.first, querys[j].first.second, querys[j].second.first, querys[j].second.second);     
     }
   }
   printf("no optimization rq: time = %f(s), time per query = %.2lf(us)\n", timeFromBegin(), (timeFromBegin()*1000000.0/querys.size()/repeat_for_average));   
   

   startTimer();
   for(int i = 0; i < repeat_for_average; i++){
     for(int j = 0; j < querys.size(); j++){
       df.rectangle_query(querys[j].first.first, querys[j].first.second, querys[j].second.first, querys[j].second.second);     
     }
   }
   printf("zdivide rq: time = %f(s), time per query = %.2lf(us)\n", timeFromBegin(), (timeFromBegin()*1000000.0/querys.size()/repeat_for_average));   
   

   startTimer();
   for(int i = 0; i < repeat_for_average; i++){
     for(int j = 0; j < querys.size(); j++){
       df.dqb_range_query(querys[j].first.first, querys[j].first.second, querys[j].second.first, querys[j].second.second);     
     }
   }
   printf("quadboxes rq: time = %f(s), time per query = %.2lf(us)\n", timeFromBegin(), (timeFromBegin()*1000000.0/querys.size()/repeat_for_average));   

 }
 return 0;
}
