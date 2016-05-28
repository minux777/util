/* difftree.cpp
 * Copyright (C) 2011, Francisco Claude.
 * Copyright (C) 2011, Diego Seco.
 *
 * DiffTree implementation
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
#include <iostream>

#include "difftree.h"
#include "morton.h"

DiffTree::DiffTree(vector<int> _v, size_t _noEncodedLen) {
  /* We first create a copy of the vector and sort the
     values. If we are allowed to mutate the input, the
     copying can be omitted. Both can be omitted if we 
     assume the input to be sorted. 
  */
  vector<int> values(_v.begin(),_v.end());
  //sort(values.begin(),values.end());

  /* Store the length of the resulting array, in number
     if elements. 
  */
  len = values.size();
  noEncodedLen = min(_noEncodedLen, len);
    
  /* Store the lowest value */
  lowestValue = values.at(0);
  
  /* We will keep two structures, an array with the resulting
     values to be stored, and a queue with ranges of positions
     to be processed. We are traversing the tree by levels, so
     we need a queue (FIFO). Each range corresponds to a node,
     the values in the range are the positions represented by 
     the subtree located at that node.
  */
  uint *tree = new uint[len];
  size_t tree_pos = 0;
  queue<pair<size_t,size_t> > range_queue;

  /* We initialize the queue with the first range. This 
     corresponds to the root and covers the whole range of
     elements in the tree. 
  */
  range_queue.push(make_pair((size_t)0,len-1));

  /* Now we are going to iterate until no more nodes have to be
     processed, this means, we have traversed the whole tree. 
     Every time we traverse a node, we add it to the tree vector,
     thus this iteration finished storing all values of the tree.
  */
  while(!range_queue.empty()) {
    /* We retrieve the next range to be processed. We refer to
       p.fisrt as the lower limit and p.second as the upper limit.
     */
    pair<size_t,size_t> range = range_queue.front(); 
    range_queue.pop();

    /* If the range has size 1, then we just add the element and 
      continue processing elements in it.
    */
    if(range.first==range.second) {
      if(range.first==0) {
	lowestPos = tree_pos+1;
      }
      tree[tree_pos++] = values[range.first];
      continue;
    }

    /* n represents the number of elements in the range we
       are processing at this iteration.
    */
    double n = (range.second-range.first+1.0);

    /* We compute the distance from a border of the range
       where the element that is going to be stored as the root
       for this range is located in values. This distance is 
       obtained by computing $2^{\lfloor \log(n/3)+1 \rfloor}$.
    */
    //size_t dist = 1<<(int)floor(log(n/3+1)); --> log(n/3+1) should be log(n/3)+1
    size_t dist = 1<<bits(n/3);//1<<((bits((2*n+2)/3))-1);//
    
    /* The value part corresponds to the position where the element
       stored in the root of this subtree goes. It is the lowest
       limit plus dist or the upper limit minus dist. This depends
       on whether the first option covers more than half of the 
       elements or not.
    */
    size_t part;//0..n-1
    if(dist>n/2)
      part = range.first+dist-1;
    else
      part = range.second+1-dist;

    /* We store the element at the root */
    tree[tree_pos++] = (values[part]);

    /* Just add the left subrange, it has at least one element. */
    range_queue.push(make_pair(range.first,part-1));

    /* If part is less than the upper limit, then we have 
       a right subrange to process.
    */
    if(part<range.second)
      range_queue.push(make_pair(part+1,range.second));
  }

  /* Compute differences for each node with respect to their parent. */
  takeDiffs(tree,1,tree[0]);
  
  treeNoEncoded = new uint[len];
  for(size_t i = 0; i < len; i++) {
    treeNoEncoded[i] = tree[i];
  }
  if(len > noEncodedLen) {
    treeEncoded = createFT(tree+noEncodedLen, len-noEncodedLen);
  } else {
    treeEncoded = NULL;
  }
  
  delete []tree;
  
  /* Trace variables to improve searches */
  traceValues = new uint[bits(len)];
  tracePositions = new uint[bits(len)];
}

DiffTree::~DiffTree() {
  delete [] traceValues;
  delete [] tracePositions;
  delete []treeNoEncoded;
  if(treeEncoded != NULL)
    destroyFT(treeEncoded);
}

void DiffTree::takeDiffs(uint *tree, size_t pos, int my_val) {
  
  /* If the node is "null", we don't have anything to compute. The value
     pos is in the range [1..len]. This assumes that the arrays start at
     position 1. For accessing elements we would have to substract one
     to this value. It turns out that it is easier to express the 
     operations in this "standard positions" ([1..len]).
   */
  if(pos>len+1) return;
  
  /* Recursively compute the left child if it's not "null". The value
     left stores the position in tree where the left child is located,
     to map it to standard positions ([1..len]) we need to add 1 to
     left.
  */
  size_t left = 2*pos-1;
  if(left<len) {
    /* On the left child, we store the value of the actual node,
       that is, my_val, minus the value on the left node. This
       difference is always greater or equal than zero. If all
       elements are different, the difference is positive.
    */
    int next_val = tree[left];
    //tree[left] = my_val - tree[left];
    int temp_t = my_val - tree[left];
    if(temp_t < 0){
      temp_t *= (-1);
      tree[left] = temp_t * 2 - 1;
    }else{
      tree[left] = temp_t * 2;
    }
    /* Recurse on the left subtree, next_val remembers the actual
       value of that key for further computation.
    */
    takeDiffs(tree,left+1,next_val);
  }

  /* Recursively compute the left child if it's not "null". The value
     right stores the position in tree where the right child is located.
     To map it to standard positions, we need to add 1 to it.
   */
  size_t right = 2*pos;
  if(right<len) {
    /* On the right child, we store the value of the child minus
       the value of the actual node (my_val). Since the right child
       is greater or equal, this difference is non-negative. If all
       elements are different, the difference is positive.
    */
    int next_val = tree[right];
    int temp_t = my_val - tree[right];
    if(temp_t < 0){
      temp_t *= (-1);
      tree[right] = temp_t * 2 - 1;
    }else{
      tree[right] = temp_t * 2;
    }
    /* Recurse on the right subtree, next_val remembers the actual
       value of that key for further computation.
    */
    takeDiffs(tree, right+1,next_val);
  }
}

size_t DiffTree::search(int key) {
  /* We start looking at the root. We use pos to track
     the position in the array tree shifted by one. By shifting
     by one we can use the standard operations for moving
     to the left/right child in the array. When accessing the 
     array, we have to shift in the other direction.
  */
  size_t pos = 1;

  /* We keep track of the actual value, which gets assigned every
     time we enter a node. For the initialization, it is the value
     stored in the root (the node we are at).
  */
  int actual_value = accessDT(pos);//tree[pos-1];

  /* We traverse searching for the element until we reach an empty
     node. This condition can be checked as having a position
     that is greater than the length+1, since those nodes are
     "null".
  */
  while(pos < len+1) {
    /* If we find the element, we return the position. */
    if(key == actual_value) {
      return pos;
    }

    /* If the key is less than the actual value, we move to the
       left child. The actual_value changes to the actual_value
       minus the value stored in the left child. Since the left
       child is less, the value stored is >0.
    */
    if(key < actual_value) {
      pos = 2*pos;
      if(pos > len)
	break;
      actual_value ^= accessDT(pos);////-tree[pos-1];
    } 
    /* If the key is greater than actual_value, we move to the
       right. In this case we add the value stored in the right
       child to actual_value.
    */
    else {
      pos = 2*pos+1;
      if(pos > len)
	break;
      actual_value ^= accessDT(pos);//tree[pos-1];
    }
  }
  
  /* We didn't find the element, so we return (size_t)-1; */
  return (size_t)-1;
}

size_t DiffTree::searchPS(int key) {
  uint lPos = 1;
  uint uPos = len;
  size_t n = len;
  size_t part;
  size_t dist = 1<<bits(n/3);
  if(dist>n/2)
    part = lPos+dist-1;
  else
    part = uPos-dist+1;
    
  size_t pos = 1;
  int actual_value = accessDT(pos);//tree[pos-1];

  while(pos < len+1) {
    if(key == actual_value) {
      return part;
    }

    if(key < actual_value) {
      pos = 2*pos;
      if(pos > len)
	break;
      actual_value -= accessDT(pos);////-tree[pos-1];
      uPos = part - 1;
    } else {
      pos = 2*pos+1;
      if(pos > len)
	break;
      actual_value += accessDT(pos);//tree[pos-1];
      lPos = part + 1;
    }
    n = uPos - lPos + 1;
    dist = 1<<bits(n/3);
    if(dist>n/2)
      part = lPos+dist-1;
    else
      part = uPos-dist+1;
  }
  
  if(actual_value > key)
    return part-1;
  return part;
}

size_t DiffTree::searchTrace(int key) {
  while(traceStackPos > 1 && key > traceValues[traceStackPos-1]) {
    traceStackPos--;
  }
  
  size_t pos = tracePositions[traceStackPos-1];
  int actual_value = traceValues[traceStackPos-1];

  if(key == actual_value) {return pos;}
  if(key < actual_value) {
    pos = 2*pos;
    if(pos > len)
      return (size_t)-1;
    actual_value -= accessDT(pos);
  } else {
    pos = 2*pos+1;
    if(pos > len)
      return (size_t)-1;
    actual_value += accessDT(pos);
  }
  
  while(pos < len+1) {
    if(key == actual_value) {      
      return pos;
    }
    if(key < actual_value) {      
      tracePositions[traceStackPos] = pos;
      traceValues[traceStackPos] = actual_value;
      traceStackPos++;
      
      pos = 2*pos;
      if(pos > len)
	break;
      actual_value -= accessDT(pos);
    } 
    else {
      pos = 2*pos+1;
      if(pos > len)
	break;
      actual_value += accessDT(pos);
    }
  }
  
  return (size_t)-1;
}

DiffTreeIterator DiffTree::searchIt(int key) {
  /* We start looking at the root. We use pos to track
     the position in the array tree shifted by one. By shifting
     by one we can use the standard operations for moving
     to the left/right child in the array. When accessing the 
     array, we have to shift in the other direction.
  */
  size_t pos = 1;

  /* We keep track of the actual value, which gets assigned every
     time we enter a node. For the initialization, it is the value
     stored in the root (the node we are at).
  */
  int actual_value = accessDT(pos);//tree[pos-1];
  int prev_value = 0;

  /* We traverse searching for the element until we reach an empty
     node. This condition can be checked as having a position
     that is greater than the length+1, since those nodes are
     "null".
  */
  while(pos < len+1) {
    /* We use prev_value to avoid check the condition pos < len+1 inside
    the loop */
    actual_value = actual_value + prev_value;

    /* If we find the element, we return the position. */
    if(key == actual_value) {
      return DiffTreeIterator(this, pos, actual_value);
    }

    /* If the key is less than the actual value, we move to the
       left child. The actual_value changes to the actual_value
       minus the value stored in the left child. Since the left
       child is less, the value stored is >0.
    */
    if(key < actual_value) {
      pos = 2*pos;
      prev_value = -accessDT(pos);//-tree[pos-1];
    } 
    /* If the key is greater than actual_value, we move to the
       right. In this case we add the value stored in the right
       child to actual_value.
    */
    else {
      pos = 2*pos+1;
      prev_value = accessDT(pos);//tree[pos-1];
    }
  }
  
  /* We didn't find the element, so we return (size_t)-1; */
  return DiffTreeIterator(this, pos/2, actual_value);
}
/*
uint DiffTree::access(int pos) {
  uint cPos = 1;
  uint cValue = accessDT(cPos);
  uint lPos = 1;
  uint uPos = len;
  size_t n = len;
  size_t part;
  size_t dist = 1<<bits(n/3);
  int temp_t;
  if(dist>n/2)
    part = lPos+dist-1;
  else
    part = uPos-dist+1;
  
  while(part != pos) {
    //cout  << pos << " " << cValue << " " << lPos << " " << uPos << " " << n << " " << part << " " << dist << endl;
    if(pos < part) {
      cPos = 2*cPos;
      temp_t = accessDT(cPos);
      if(temp_t % 2 == 0){
	temp_t = temp_t / 2;
	cValue -= temp_t;


      }
      else{
	temp_t = temp_t / 2 + 1;
	temp_t *= (-1);
	cValue -= temp_t;
      }
      
      //cValue ^= accessDT(cPos);
      uPos = part - 1;
    } else {
      cPos = 2*cPos+1;

      temp_t = accessDT(cPos);
      if(temp_t % 2 == 0){
	temp_t = temp_t / 2;
	cValue -= temp_t;
      }
      else{
	temp_t = temp_t / 2 + 1;
	temp_t *= (-1);
	cValue -= temp_t;
      }


      //cValue ^= accessDT(cPos);
      lPos = part + 1;
    }
    n = uPos - lPos + 1;
    dist = 1<<bits(n/3);
    if(dist>n/2)
      part = lPos+dist-1;
    else
      part = uPos-dist+1;
  }

  return cValue;
}
*/

/*
 * pos > 0
*/
DiffTreeIterator DiffTree::accessIt(int pos) {
  uint cPos = 1;
  uint cValue = accessDT(cPos);
  uint lPos = 1;
  uint uPos = len;
  size_t n = len;
  size_t part;
  size_t dist = 1<<bits(n/3);
  int temp_t;
  if(dist>n/2)
    part = lPos+dist-1;
  else
    part = uPos-dist+1;
  
  while(part != pos) {
    //cout  << pos << " " << cValue << " " << lPos << " " << uPos << " " << n << " " << part << " " << dist << endl;
    if(pos < part) {
      cPos = 2*cPos;
      temp_t = accessDT(cPos);
      if(temp_t % 2 == 0){
	temp_t = temp_t / 2;
	cValue -= temp_t;


      }
      else{
	temp_t = temp_t / 2 + 1;
	temp_t *= (-1);
	cValue -= temp_t;
      }
      
      //cValue ^= accessDT(cPos);
      uPos = part - 1;
    } else {
      cPos = 2*cPos+1;

      temp_t = accessDT(cPos);
      if(temp_t % 2 == 0){
	temp_t = temp_t / 2;
	cValue -= temp_t;
      }
      else{
	temp_t = temp_t / 2 + 1;
	temp_t *= (-1);
	cValue -= temp_t;
      }


      //cValue ^= accessDT(cPos);
      lPos = part + 1;
    }
    n = uPos - lPos + 1;
    dist = 1<<bits(n/3);
    if(dist>n/2)
      part = lPos+dist-1;
    else
      part = uPos-dist+1;
  }
  DiffTreeIterator it(this, cPos, cValue);
  return it;
}



/*
 * pos > 0
 */
uint DiffTree::access(int pos) {
  //DiffTreeIterator it = DiffTree::accessIt(pos);
  //return 0;
  return accessIt(pos).getCurrentValue();
}


/*
* x entre 0 y filas - 1
* y entre 0 y columnas -1
*
*/
int** DiffTree::rangeQuery(uint x1, uint y1, uint x2, uint y2){
  int pos = EncodeMorton2(x1, y1) + 1;
  DiffTreeIterator it = accessIt(pos);

  int ** toret;
  uint cols = y2 - y1 + 1;
  uint rows = x2 - x1 + 1;
  toret = (int**) malloc(sizeof(int*) * rows);

  for(int i = 0 ; i < rows; i++)
    toret[i] = (int*) malloc(sizeof(int) * cols);
  
  
  

  for(int i = 0; i <= EncodeMorton2(x2, y2) + 1 - pos; i++){
    int x = DecodeMorton2X(pos - 1 + i), y = DecodeMorton2Y(pos - 1 + i);
    if(x >= x1 && x <= x2 && y >= y1 && y <= y2)
      toret[x - x1][y - y1] = it.getCurrentValue();
    it.getSucc();
  }
  return toret;
}





/*****************************************Rectangle Query*******************************************/
int DiffTree::points_in_rectangle(pair<uint, uint> start_point, int **toret, uint start, uint end, uint last, uint tol){
  uint litmax, bigmin;
  uint default_tol = tol;
  zdivide(start, end, &litmax, &bigmin);
  if(last < litmax)
    points_in_rectangle(start_point, toret, start, litmax, last, default_tol);
  some_points_in_rectangle(start_point, toret, bigmin, end, default_tol);
}



int DiffTree::some_points_in_rectangle(pair<uint, uint> start_point, int **toret, uint start, uint end, uint tol = 3){
  int curr = start;
  uint default_tol = tol;
  DiffTreeIterator it = accessIt(start + 1);
  uint x1 = DecodeMorton2X(start);
  uint y1 = DecodeMorton2Y(start);
  uint x2 = DecodeMorton2X(end);
  uint y2 = DecodeMorton2Y(end);

  while(curr <= end){
    uint x, y;
    x = DecodeMorton2X(curr);
    y = DecodeMorton2Y(curr);
    if(x < x1 || x > x2 || y < y1 || y > y2){
      tol--;
      if(tol <= 0){
	points_in_rectangle(start_point, toret, start, end, curr, default_tol);
	curr = end;
	tol = default_tol;
	break;
      }
    }else{
      toret[x - start_point.first][y - start_point.second] = it.getCurrentValue();
      tol = default_tol;
      curr++;
    }
    it.getSucc();
  }
}

int** DiffTree::rectangle_query(uint x1, uint y1, uint x2, uint y2){
  //return NULL;
  int min_point = EncodeMorton2(x1, y1);
  int max_point = EncodeMorton2(x2, y2);
  int cols = y2 - y1 + 1;
  int rows = x2 - x1 + 1;

  int ** matrix = (int**)malloc(rows * sizeof(int *));
  for(int i = 0; i < rows; i++)
    matrix[i] = (int*)malloc(cols * sizeof(int)) ;

  some_points_in_rectangle(make_pair(x1, y1), matrix, min_point, max_point);
  
  return matrix;
}


/*************************************descomosition in quadboxes range query*********************************/
int** DiffTree::dqb_range_query(uint x1, uint y1, uint x2, uint y2){
  int **matrix = (int**) malloc(sizeof(int *) * (y2 - y1 + 1));
  for(int i = 0; i < (y2 - y1 + 1); i++){
    matrix[i] = (int*) malloc(sizeof(int) * (x2 - x1 + 1));
    if(matrix[i] == NULL)
      cout << "malloc error" << endl;
  }

  vector<pair<pair<int,int>, int> >  querys;
  querys = decomposeWindow(x1, y1, (x2 - x1 + 1), (y2 - y1 + 1));

  for(unsigned int i = 0; i < querys.size(); i++){
    int ** temp;
    temp = rangeQuery(querys[i].first.first, 
			 querys[i].first.second,
			 querys[i].first.first + querys[i].second - 1,
			 querys[i].first.second + querys[i].second - 1);

    for(int j = 0; j < querys[i].second; j++){
      for(int k = 0; k < querys[i].second; k++){
	matrix[querys[i].first.first + j - x1][querys[i].first.second + k - y1] =  temp[j][k];
      }
    }
  }
  return matrix;

}


size_t DiffTree::getSize() {
  /* The size is the addition of the space used by the object
     and the space used by the array tree. 
  */
  //size_t baseSize = sizeof(*this);
  size_t baseSize = 2*sizeof(size_t) + 2*sizeof(uint) + sizeof(uint*) + sizeof(FTRep*);
  if(treeEncoded == NULL)
    return sizeof(int)*noEncodedLen; //if the list is too small we store a plain array and we don't care about the structure
  return baseSize
    +sizeof(int)*noEncodedLen
    +sizeFT(treeEncoded);
  //return sizeof(*this)+sizeFT(treeEncoded);
  //return sizeof(*this)+sizeof(int)*len;
}

/*
int main(int argc, char **argv) {
  size_t len;
  cin >> len;
  vector<int> values;
  for(size_t i=0;i<len;i++) {
    int v;
    cin >> v;
    values.push_back(v);
  }

  DiffTree tree(values);
  
  while(true) {
    uint tValue;
    cin >> tValue;
    DiffTreeIterator dtIt = tree.searchIt(tValue);
    cout << dtIt.getCurrentValue() << ": " << endl;
    while(dtIt.hasSucc())
      cout << dtIt.getSucc() << " ";
    cout << endl;
    dtIt = tree.searchIt(tValue);
    while(dtIt.hasPred())
      cout << dtIt.getPred() << " ";
    cout << endl;
  }
  
  return 0;
}
*/
