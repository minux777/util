#name=$(echo $1 | cut -d'.' -f 1)
name=`basename $1 | cut -d'.' -f 1`
k2tree_parse_dir=$2
output_dir=$3

./make_tree_file.x $1 $k2tree_parse_dir$name.k2t

/home/dseco/trabajo/rangecounting/k2tree/build_tree $k2tree_parse_dir$name.k2t $output_dir$name 4 2 6 22

/home/dseco/trabajo/rangecounting/k2tree/compress_leaves $output_dir$name

du -bc $output_dir$name.tr $output_dir$name.lv $output_dir$name.cil $output_dir$name.voc | tail -1
