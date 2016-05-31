include /home/ale/sdsl-lite/Make.helper
CXX_FLAGS=$(MY_CXX_FLAGS) $(MY_CXX_OPT_FLAGS) -I$(INC_DIR) -L$(LIB_DIR) 
CCLIB=-lsdsl -ldivsufsort -ldivsufsort64 

OBJECTS=headers/difftree.o headers/read_MDT.o headers/directcodes.o headers/morton.o headers/basickTree.o headers/bitrankw32intkTree.o
CFLAGS=-g -O3
CXX=g++
SOURCES=$(wildcard *.cpp)
EXECS=$(SOURCES:.cpp=.x)

all: $(EXECS)

zigzag_encode.x: zigzag_encode.cpp headers/difftree.o headers/read_MDT.o
	$(CXX) $(CFLAGS) $(OBJECTS) $< -o $@

rle.x: rle.cpp headers/read_MDT.o
	$(CXX) $(CXX_FLAGS) $(OBJECTS) $< -o $@ $(CCLIB)


headers/difftree.o: headers/difftree.cpp headers/directcodes.o headers/morton.o

headers/read_MDT.o: headers/read_MDT.cpp headers/morton.o

headers/directcodes.o: headers/directcodes.cpp headers/basickTree.o headers/bitrankw32intkTree.o

headers/morton.o: headers/morton.cpp

headers/basickTree.o: headers/basickTree.cpp

headers/bitrankw32intkTree.o: headers/bitrankw32intkTree.cpp headers/basickTree.o

#headers/morton.cpp headers/read_MDT.cpp headers/bitrankw32intkTree.cpp headers/directcodes.cpp headers/basickTree.cpp headers/difftree.cpp -o zigzag.x2

