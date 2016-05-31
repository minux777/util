CXX=g++
zigzag_encode: zigzag_encode.o headers/difftree.o headers/read_MDT.o
zigzag_encode.cpp headers/read_MDT.cpp headers/difftree.cpp headers/read_MDT.h headers/difftree.h
	$(CXX) -o funciona zigzag_encode.o headers/difftree.o headers/read_MDT.o


zigzag_encode.o:

headers/difftree.o:
headers/read_MDT.o:

#headers/morton.cpp headers/read_MDT.cpp headers/bitrankw32intkTree.cpp headers/directcodes.cpp headers/basickTree.cpp headers/difftree.cpp -o zigzag.x2

