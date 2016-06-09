include /home/ale/sdsl-lite/Make.helper
HEADERS=headers
K2TREE_PATH=../k2tree
K2TREE_OBJECTS=$(K2TREE_PATH)/basic.o $(K2TREE_PATH)/bitrankw32int.o $(K2TREE_PATH)/directcodes.o $(K2TREE_PATH)/kTree.o $(K2TREE_PATH)/hash.o $(K2TREE_PATH)/MemoryManager.o

OBJECTS=$(HEADERS)/difftree.o $(HEADERS)/read_MDT.o $(HEADERS)/directcodes.o $(HEADERS)/morton.o $(HEADERS)/basickTree.o $(HEADERS)/bitrankw32intkTree.o

CXX_FLAGS=$(MY_CXX_FLAGS) $(MY_CXX_OPT_FLAGS) -I$(INC_DIR) -L$(LIB_DIR) 
CCLIB=-lsdsl -ldivsufsort -ldivsufsort64 
OPT=-O3 -ffast-math -funroll-loops -msse4.2 -DHAVE_CXA_DEMANGLE
ALL_FLAGS=-std=c++11 $(OPT)
DEBUG_FLAGS= -g -O0

CXX=g++

SOURCES=$(wildcard *.cpp)
EXECS=$(SOURCES:.cpp=.x)

all: $(EXECS) $(OBJECTS)

%.debug: %.cpp $(OBJECTS)
	$(CXX) $(DEBUG_FLAGS) $(OBJECTS) $< -o $@


zigzag_encode.x: zigzag_encode.cpp $(HEADERS)/difftree.o $(HEADERS)/read_MDT.o
	$(CXX) $(ALL_FLAGS) $(OBJECTS) $< -o $@

make_random_querys.x: make_random_querys.cpp $(HEADERS)/read_MDT.o
	$(CXX) $(ALL_FLAGS) $(OBJECTS) $< -o $@

make_tree_file.x: make_tree_file.cpp $(HEADERS)/read_MDT.o
	$(CXX) $(ALL_FLAGS) $(OBJECTS) $< -o $@


test_range_counting.x: test_range_counting.cpp
	$(CXX) $(ALL_FLAGS) $(K2TREE_OBJECTS) $< -o $@




#use especial libraries
rle.x: rle.cpp $(HEADERS)/read_MDT.o
	$(CXX) $(CXX_FLAGS) $(OBJECTS) $< -o $@ $(CCLIB)



%.o: %.c
	$(CXX) $(ALL_FLAGS) -c $< -o $@

#$(HEADERS)/difftree.o: $(HEADERS)/difftree.cpp $(HEADERS)/directcodes.o $(HEADERS)/morton.o

#$(HEADERS)/read_MDT.o: $(HEADERS)/read_MDT.cpp $(HEADERS)/morton.o

#$(HEADERS)/directcodes.o: $(HEADERS)/directcodes.cpp $(HEADERS)/basickTree.o $(HEADERS)/bitrankw32intkTree.o

#$(HEADERS)/morton.o: $(HEADERS)/morton.cpp

#$(HEADERS)/basickTree.o: $(HEADERS)/basickTree.cpp

#$(HEADERS)/bitrankw32intkTree.o: $(HEADERS)/bitrankw32intkTree.cpp $(HEADERS)/basickTree.o

clean:
	rm *.x *~
#$(HEADERS)/morton.cpp $(HEADERS)/read_MDT.cpp $(HEADERS)/bitrankw32intkTree.cpp $(HEADERS)/directcodes.cpp $(HEADERS)/basickTree.cpp $(HEADERS)/difftree.cpp -o zigzag.x2
