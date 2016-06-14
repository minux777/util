(for i in `ls querys/ra*`; do echo `basename $i .txt`; ./test_range_counting.x ../prueba/output/MDT05-0500-H29 $i 20 337; done) > out_text_range_counting
#(for i in `ls querys/ra*`; do echo `basename $i .txt`;  ./../dseco/k3base/bin/query ../dseco/k3base/bin/original/500 $i 20; done) > out_query_k3base
