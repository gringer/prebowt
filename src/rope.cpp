/** <header>
 *
 * Copyright 2014 David Eccles (gringer) <bioinformatics@gringene.org>
 *
 * This file is part of preBowt
 *
 * preBowt is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * preBowt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with preBowt. If not, see <http://www.gnu.org/licenses/>
 *
</header> **/

#include <string>
#include <iostream>
#include <memory>

#include "rope.hpp"
#include "prebowtconfig.hpp"

//#define PTR_DEBUG 1
//#define OUTPUT_DEBUG 1

using namespace std;

// non-destructive algorithms are based on Boehm, Atkinson and Plass (1995)
// http://citeseer.ist.psu.edu/viewdoc/download?doi=10.1.1.14.9450&rep=rep1&type=pdf

int Rope::nextNodeNum = 0;

ostream &operator<<(ostream& out, const Rope& src){
#if OUTPUT_DEBUG
  cerr << "{";
#endif
  if(src.left){
    out << *src.left;
  }
  if(src.isLeaf()){
    out << src.sequence;
  } else {
#if OUTPUT_DEBUG
    cerr << ",";
#endif
  }
  if(src.right){
    out << *src.right;
  }
#if OUTPUT_DEBUG
    cerr << "}";
#endif
  return out;
}

// Copy constructor (shallow copy)
// [note: left and right for src and this point to the same node]
Rope::Rope(const Rope& src){
  nodeNum = nextNodeNum++;
#if PTR_DEBUG
  cerr << "[C#" << nodeNum << "<-#" << src.nodeNum << "]";
#endif
  sequence = src.sequence;
  left = src.left;
  right = src.right;
}

// Create a new leaf node out of a string
Rope::Rope(const string& tSeq){
  nodeNum = nextNodeNum++;
#if PTR_DEBUG
  cerr << "[L#" << nodeNum << "(" << tSeq << ")]";
#endif
  sequence = tSeq;
}

// Create a new concatenation node from two child ropes
Rope::Rope(const Rope& rL, const Rope& rR){
  nodeNum = nextNodeNum++;
  shared_ptr<Rope> pRL(new Rope(rL));
  shared_ptr<Rope> pRR(new Rope(rR));
#if PTR_DEBUG
  cerr << "[T#" << nodeNum << "<-(#" 
       << pRL->nodeNum << ",#" << pRR->nodeNum << ")]";
#endif
  left = pRL;
  right = pRR;
}

// Assignment operator (shallow copy)
Rope& Rope::operator=(const Rope& src){
  if(this != &src){ // gracefully handle self assignment
    nodeNum = nextNodeNum++;
#if PTR_DEBUG
    cerr << "[A#" << nodeNum << "<-#" << src.nodeNum << "]";
#endif
    left = src.left;
    right = src.right;
  }
  return *this;
}

// destructor
Rope::~Rope(){
#if PTR_DEBUG
  cerr << "[~#" << nodeNum << "]\n";
#endif
}

// static public methods
Rope Rope::concat(const Rope& rL, const Rope& rR){
  if(rL.isShortLeaf() && rR.isShortLeaf()){
    // If both arguments are short leaves, we produce a flat rope
    // (leaf) consisting of the concatenation.
    Rope retVal(rL.sequence + rR.sequence);
    return(retVal);
  }
  if(rL.isConcatNode() && rL.right->isShortLeaf() && rR.isShortLeaf()){
    // If the left argument is a concatenation node whose right son is
    // a short leaf, and the right argument is also a short leaf, then
    // we concatenate the two leaves, and then concatenate the result
    // to the left son of the left argument
    Rope rightVal(rL.right->sequence + rR.sequence);
    Rope retVal(*rL.left, rightVal);
    return(retVal);
  }
  // In the general case, concatenation involves simply allocating a
  // concatenation node containing two pointers to the two arguments.
  return(Rope(rL, rR));
}

Rope Rope::substr(const Rope& src, const size_t& start, const size_t& len){
#if SUBSTR_DEBUG
  cerr << "[SS(" << start << "," << len << ")]";
#endif
  if(src.isLeaf()){
    // The substring operation on structured ropes can be easily
    // implemented. We assume that the substring operation on leaves
    // simply copies the relevant section of the leaf, and deals with
    // negative start arguments and over-length arguments correctly.
#if SUBSTR_DEBUG
    cerr << "[Leaf substring (" << start << ","
         << len << ") of "<< src.length() << "]";
#endif
    Rope retVal(src.sequence.substr(start, len));
    return(retVal);
  }
  size_t leftLength = src.left->length();
  // left = if start <= 0 and len >= length(rope1) then
  //           rope1
  //        else
  //           substr(rope1,start,len)
  Rope left(((start == 0) && (len >= leftLength)) ?
            *src.left :
            Rope::substr(*src.left, start, len));
  // right = if start <= length(rope1)
  //  and start + len >= length(rope1) + length(rope2) then
  //            rope2
  //         else
  //            substr(rope2,start-length(rope1), len-length(left))
  Rope right(((start <= leftLength) 
              && ((start + len) >= (leftLength + src.right->length()))) ?
             *src.right :
             Rope::substr(*src.right, 
                          (leftLength >= start) ? 0 : start - leftLength,
                          len - left.length()));
  return(Rope::concat(left,right));
}

// public methods

size_t Rope::length() const{
  // this should probably be cached
  size_t tLen = sequence.length();
  if(left){
    tLen += left->length();
  }
  if(right){
    tLen += right->length();
  }
  return tLen;
}

Rope* Rope::getLeft() const{
  return left.get();
}

Rope* Rope::getRight() const{
  return right.get();
}

// private accessory methods

bool Rope::isLeaf() const{
  return((!left) && (!right));
}

bool Rope::isShortLeaf() const{
  return(!hasChildren() && (sequence.size() <= Rope::SHORT_THRESHOLD));
}

bool Rope::hasChildren() const{
  return((left) || (right));
}

bool Rope::hasLeft() const{
  return((bool)left);
}

bool Rope::hasRight() const{
  return((bool)right);
}

bool Rope::isConcatNode() const{
  return(hasRight());
}

int main(){
  cerr << "Testing leaf node creation...";
  Rope a("The quick brown ");
  Rope b("fox jumps over ");
  Rope c("the lazy ");
  Rope d("dog");
  cerr << " done\n";
  cerr << "Result[a]: " << a << endl;
  cerr << "Result[b]: " << b << endl;
  cerr << "Result[c]: " << c << endl;
  cerr << "Result[d]: " << d << endl;
  cerr << "Testing substring on simple string... ";
  cerr << "'" << Rope::substr(a,4,5) << "' == 'quick'...";
  cerr << " done\n";
  cerr << "Testing concatenation of two short leaves...";
  Rope e = Rope::concat(a,b);
  cerr << " done\n";
  cerr << "Result[e]: " << e << endl;
  cerr << "Testing basic node concatenation...";
  Rope f(a,b);
  cerr << " done\n";
  cerr << "Result[f]: " << f << endl;
  cerr << "Testing substring with range [0,len(left)+1]...";
  cerr << "'" << Rope::substr(f,0,f.getLeft()->length()+1)
       << "' == 'The quick brown f'...";
  cerr << " done\n";
  cerr << "Testing substring with range [4,len(left+right)+1]...";
  cerr << "'" << Rope::substr(f,0,f.length())
       << "' == 'quick brown fox jumps over '...";
  cerr << " done\n";
  cerr << "Testing concatenation with right of left node a short leaf...";
  Rope g = Rope::concat(f,c);
  cerr << " done\n";
  cerr << "Result[g]: " << g << endl;
  cerr << "Testing non-trivial node concatenation...";
  Rope h = Rope::concat(g,d);
  cerr << " done\n";
  cerr << "Result[h]: " << h << endl;
}
