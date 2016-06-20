#(for i in `ls querys/500-q-quad*`; do echo -n `basename $i .txt`" "; ./test_range_counting_quadbox.x ../prueba/output/500-10 $i 20 3371; echo; done) > range-counting-qb-500

#(for i in `ls querys/500-q-*`; do echo -n `basename $i .txt`" "; ./test_range_counting.x ../prueba/output/500-10 $i 20 3371; echo; done) > range-counting-500

(for i in `ls querys/500-q*`; do echo -n `basename $i .txt`" ";  ./../dseco/k3base/bin/query ../dseco/k3base/bin/presition/500-10 $i 20; echo; done) > k3base-500


#(for i in `ls querys/random-quad*`; do echo -n `basename $i .txt`" "; ./test_range_counting_quadbox.x ../prueba/output/MDT05-0500-H29 $i 20 337; echo; done) > out_range-counting-qb
#(for i in `ls querys/random-quad*`; do echo -n `basename $i .txt`" ";  ./../dseco/k3base/bin/query ../dseco/k3base/bin/original/500 $i 20; echo; done) > out_query_k3base-qb

#(for i in `ls querys/ra*`; do echo -n `basename $i .txt`" "; ./test_range_counting.x ../prueba/output/MDT05-0500-H29 $i 20 337; echo; done) > out_text_range_counting
#(for i in `ls querys/ra*`; do echo -n `basename $i .txt`" ";  ./../dseco/k3base/bin/query ../dseco/k3base/bin/original/500 $i 20; echo; done) > out_query_k3base



#create random quadbox queries
#for i in 10 50 100 200 400; 
#do 
#    for j in 16 64 128 256 512; 
#    do 
#	./create_quadboxes_querys.x 5841 4001 $j $i 337 914 > random-quadbox-"$j"x"$j"-$i.txt; 
#    done; 
#done
