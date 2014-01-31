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

#include "rope.hpp"
#include "prebowtconfig.hpp"

using namespace std;

// non-destructive algorithms are based on Boehm, Atkinson and Plass (1995)
// http://citeseer.ist.psu.edu/viewdoc/download?doi=10.1.1.14.9450&rep=rep1&type=pdf

// Copy constructor (shallow copy)
// [note: left and right for src and this point to the same node]
Rope::Rope(const Rope& src){
  sequence = src.sequence;
  if(left != NULL){
    delete(left);
  }
  if(right != NULL){
    delete(right);
  }
  left = src.left;
  right = src.right;
}

// Assignment operator (shallow copy)
Rope& Rope::operator=(const Rope& src){
  if(this != &src){ // gracefully handle self assignment
    if(left != NULL){
      delete(left);
    }
    if(right != NULL){
      delete(right);
    }
    left = src.left;
    right = src.right;
  }
  return *this;
}



// Create a new leaf node out of a string
Rope::Rope(const string& tSeq){
  sequence = tSeq;
}

// Create a new concatenation node from two child ropes
Rope::Rope(Rope* rL, Rope* rR){
  left = rL;
  right = rR;
}

Rope Rope::concat(Rope& rL, Rope& rR){
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
    Rope retVal(rL.left, &rightVal);
    return(retVal);
  }
  // In the general case, concatenation involves simply allocating a
  // concatenation node containing two pointers to the two arguments.
  return(Rope(&rL, &rR));
}

const bool Rope::isShortLeaf(){
  return(!hasChildren() && (sequence.size() <= Rope::SHORT_THRESHOLD));
}

const bool Rope::hasChildren(){
  return((left != NULL) || (right != NULL));
}

const bool Rope::hasRight(){
  return(right != NULL);
}

const bool Rope::isConcatNode(){
  return(hasRight());
}

Rope::~Rope(){
  if(left != NULL){
    delete(left);
  }
  if(right != NULL){
    delete(right);
  }
}

int main(){
  cout << "Testing leaf node creation...";
  Rope a("The quick brown ");
  Rope b("fox jumps over ");
  Rope c("the lazy dog");
  cout << " done\n";
  cout << "Testing simple string concatenation...";
  Rope d = Rope::concat(a,b);
  cout << " done\n";
  cout << "Testing standard concatenation containing long leaf strings...";
  Rope e = Rope::concat(d,c);
  cout << " done\n";
}
