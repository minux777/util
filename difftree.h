/* difftree.h
 * Copyright (C) 2011, Francisco Claude.
 * Copyright (C) 2011, Diego Seco.
 *
 * DiffTree definition.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#pragma once

#include<queue>
#include<vector>
#include<algorithm>
#include<cmath>

#include "directcodes.h"

#define isroot(p) (p==1)
#define isleft(p) (!(p%2))
#define isright(p) ((p%2) && p!=1)
#define hasrightchild(p,n) (2*p+1<=n)
#define hasleftchild(p,n) (2*p<=n)


using namespace std;

class DiffTreeIterator;

/** DiffTree stores the values of a binary search tree taking differences
    between the value in each node and their parent. This is dumped into
    and array and supports fast searching while storing values that tend
    to be frendlier for integer-coding.
*/
class DiffTree {
 public:

  /** Build the tree for a set of values. */
  DiffTree(vector<int> _v, size_t _noEncodedLen);

  /** Free the memory used by the tree. */
  ~DiffTree();

  /** Search for a given key. */
  size_t search(int key);
  size_t searchPS(int key);
  size_t searchTrace(int key);
  void clearTrace() {traceStackPos = 1;tracePositions[0]=1;traceValues[0]=accessDT(1);}
  
  /** Returns the value at position pos of the original list of elements. */
  uint access(int pos);
  DiffTreeIterator accessIt(int pos);
  /** Returns the range between p1 y p2*/
  


  int**rangeQuery(uint x1, uint y1, uint x2, uint y2);
  /** Obtain an iterator to a given key. 
      TODO: define what we do if the element is not present.
      Probably prev or next are equally good. NULL is not an option.
      Diego: in the intial implementation in this case it positions
      the iterator in the closest leaf that could be greater or 
      fewer than the target value*/

  int** rectangle_query(uint x1, uint y1, uint x2, uint y2);
  int** dqb_range_query(uint x1, uint y1, uint x2, uint y2);



  DiffTreeIterator searchIt(int key);

  /** Compute the memory, in bytes, required by the tree. */
  size_t getSize();
  
  /** Number of elements in the tree. */
  size_t getLen() {return len;}
  
 protected:
  int some_points_in_rectangle(pair<uint, uint> start_point, int **toret, uint start, uint end, uint tol);
  int points_in_rectangle(pair<uint, uint> start_point, int **toret, uint start, uint end, uint last, uint tol);


  /** Computed the difference between each node and its parent. */
  void takeDiffs(uint *tree, size_t pos, int my_val);

  /** Value stored at postion pos. */
  inline uint accessDT(size_t pos);

  /** Stores the tree in an array, as typically done for heaps. */
  //uint * tree;
  size_t noEncodedLen;
  uint *treeNoEncoded;
  FTRep *treeEncoded;

  /** The lenght of the array <code>tree</code>. */
  size_t len;
  
  /** Stores the value and position of the lowest element */
  uint lowestValue;
  uint lowestPos;
  
  /** Stores the trace during a set of searches */
  uint traceStackPos;
  uint *traceValues;
  uint *tracePositions;

  /* The iterator has access to the protected fields. */
  friend class DiffTreeIterator;
};

class DiffTreeIterator {
 public:
  //    DiffTreeIterator(DiffTree *_dTree):
  //      dTree(_dTree),cPos(1),cValue(_dTree->accessDT(1)){};
 DiffTreeIterator(DiffTree *_dTree):
  dTree(_dTree),cPos(_dTree->lowestPos),cValue(_dTree->lowestValue){};
 DiffTreeIterator(DiffTree *_dTree, size_t _cPos, uint _cValue):
  dTree(_dTree),cPos(_cPos),cValue(_cValue){};
  uint getCurrentValue() {return cValue;}
  inline bool hasSucc();
  inline bool hasPred();
  inline uint getSucc();
  inline uint getPred();
    
 protected:
  DiffTree *dTree;
  size_t cPos;//currentPosition: 1..treeSize. Position in the base tree
  uint cValue;//currentValue
};




/* DiffTree inline functions implementation */

uint DiffTree::accessDT(size_t pos) {
  if(pos <= noEncodedLen) {
    return treeNoEncoded[pos-1];
  } else {
    return accessFT(treeEncoded, pos-noEncodedLen);
  }
}




/* DiffTreeIterator inline functions implementation */

bool DiffTreeIterator::hasSucc() {
  if(dTree->len == (uint)(1<<bits(dTree->len))-1)//complete tree
    return cPos != dTree->len;
  else //non-complete tree
    return cPos != (uint)(1<<(bits(dTree->len)-1))-1;
}

bool DiffTreeIterator::hasPred() {
  return cPos != (uint)1<<(bits(dTree->len)-1);
}

uint DiffTreeIterator::getSucc() {
  uint tValue;
  if(hasrightchild(cPos,dTree->len)) { //INTERNAL NODE WITH RIGHT CHILD
    cPos = 2*cPos+1;//go right
    tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
    if(tValue % 2 == 0)
      tValue = tValue/2;
    else{
      tValue = tValue/2 + 1;
      tValue *= (-1);
    }
    cValue = cValue - tValue;
    while(hasleftchild(cPos,dTree->len)) {//go down until left-most child
      cPos = 2*cPos;
      tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
      if(tValue % 2 == 0)
	tValue = tValue/2;
      else{
	tValue = tValue/2 + 1;
	tValue *= (-1);
      }
      cValue = cValue - tValue;
    }
    return cValue;
  } else {
    if(isleft(cPos)) { //LEFT LEAF OR INTERNAL NODE WITHOUT RIGHT CHILD
      tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
      if(tValue % 2 == 0)
	tValue = tValue/2;
      else{
	tValue = tValue/2 + 1;
	tValue *= (-1);
      }
      cValue = cValue + tValue;
      cPos = cPos/2;//go up
      return cValue;
    } else { //RIGHT LEAF
      while(isright(cPos)) { //go up until the first ancestor that is not a right node
	tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
	if(tValue % 2 == 0)
	  tValue = tValue/2;
	else{
	  tValue = tValue/2 + 1;
	  tValue *= (-1);
	}
	cValue = cValue + tValue;
	cPos = cPos/2;
      }
      tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
      if(tValue % 2 == 0)
	tValue = tValue/2;
      else{
	tValue = tValue/2 + 1;
	tValue *= (-1);
      }
      cValue = cValue + tValue;
      cPos = cPos/2;//go up to its parent that is the successor
      return cValue;
    }
  }
}

uint DiffTreeIterator::getPred() {
  uint tValue;
  if(hasleftchild(cPos,dTree->len)) { //INTERNAL NODE
    cPos = 2*cPos;//go left
    tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
    cValue = cValue - tValue;
    while(hasrightchild(cPos,dTree->len)) {//go down until right-most child
      cPos = 2*cPos+1;
      tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
      cValue = cValue + tValue;
    }
    return cValue;
  } else {
    if(isright(cPos)) { //RIGHT LEAF
      tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
      cValue = cValue - tValue;
      cPos = cPos/2;//go up
      return cValue;
    } else { //LEFT LEAF
      while(isleft(cPos)) { //go up until the first ancestor that is not a left node
	tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
	cValue = cValue + tValue;
	cPos = cPos/2;
      }
      tValue = dTree->accessDT(cPos);//dTree->tree[cPos-1];
      cValue = cValue - tValue;
      cPos = cPos/2;//go up to its parent that is the successor
      return cValue;
    }
  }
}
