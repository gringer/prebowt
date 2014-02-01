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

using namespace std;

// non-destructive algorithms are based on Boehm, Atkinson and Plass (1995)
// http://citeseer.ist.psu.edu/viewdoc/download?doi=10.1.1.14.9450&rep=rep1&type=pdf

int Rope::nextNodeNum = 0;

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

ostream &operator<<(ostream& out, const Rope& src){
  if(src.isLeaf()){
    out << src.sequence;
  }
  return out;
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
#if PTR_DEBUG
  cerr << "[T#" << nodeNum << "<-(" 
       << rL.nodeNum << "," << rR.nodeNum << ")]";
#endif
  shared_ptr<Rope> pRL(new Rope(rL));
  shared_ptr<Rope> pRR(new Rope(rR));
  left = pRL;
  right = pRR;
}

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

Rope Rope::substr(const Rope& src, size_t start, size_t len){
  if(src.isLeaf()){
    // The substring operation on structured ropes can be easily
    // implemented. We assume that the substring operation on leaves
    // simply copies the relevant section of the leaf, and deals with
    // negative start arguments and over-length arguments correctly.
    Rope retVal(src.sequence.substr(start, len));
    return(retVal);
  }
  Rope retVal("Dummy return value");
  return(retVal);
}

bool Rope::isLeaf() const{
  return(!hasChildren());
}

bool Rope::isShortLeaf() const{
  return(!hasChildren() && (sequence.size() <= Rope::SHORT_THRESHOLD));
}

bool Rope::hasChildren() const{
  return((left != NULL) || (right != NULL));
}

bool Rope::hasRight() const{
  return(right != NULL);
}

bool Rope::isConcatNode() const{
  return(hasRight());
}

Rope::~Rope(){
#if PTR_DEBUG
  cerr << "[~#" << nodeNum << "]\n";
#endif
}

int main(){
  cout << "Testing leaf node creation...";
  Rope a("The quick brown ");
  Rope b("fox jumps over ");
  Rope c("the lazy ");
  Rope d("dog");
  cout << " done\n";
  cout << "Testing substring on simple string... ";
  cout << "'" << Rope::substr(a,4,5) << "' == 'quick'...";
  cout << " done\n";
  cout << "Testing concatenation of two short leaves...";
  Rope e = Rope::concat(a,b);
  cout << " done\n";
  cout << "Testing basic node concatenation...";
  Rope f(a,b);
  cout << " done\n";
  cout << "Testing concatenation with right of left node a short leaf...";
  Rope g = Rope::concat(f,c);
  cout << " done\n";
  cout << "Testing non-trivial node concatenation...";
  Rope h = Rope::concat(g,d);
  cout << " done\n";
}
