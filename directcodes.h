#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>


#include "basickTree.h"
#include "bitrankw32intkTree.h"


#ifndef byte
      #define byte unsigned char
#endif

typedef struct sFTRep {
	  uint listLength;
	  byte nLevels;
	  uint tamCode;
	  uint * levels;
	  uint * levelsIndex;
	  uint * iniLevel;
	  uint * rankLevels;
	  bitRankW32Int * bS;
	  //uint * bits_bitmap;
	  uint * base;
	  ushort * base_bits;
	  uint * tablebase;
	  uint tamtablebase;

  	
} FTRep;



// public:
	FTRep* createFT(uint *list,uint listLength);
	uint accessFT(FTRep * listRep,uint param);
	void saveFT(FTRep * listRep, FILE * flist);
	uint * decompressFT(FTRep * listRep, uint n);
	FTRep* loadFT(FILE * flist);
	void destroyFT(FTRep * listRep);
	uint sizeFT(FTRep* rep);