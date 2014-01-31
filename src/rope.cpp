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

#include "rope.hpp"

// non-destructive algorithms are based on Boehm, Atkinson and Plass (1995)
// http://citeseer.ist.psu.edu/viewdoc/download?doi=10.1.1.14.9450&rep=rep1&type=pdf

// Create a new leaf node out of a string
Rope Rope::Rope(string& tSeq){
  sequence = tSeq;
}

// Create a new concatenation node from two child ropes
Rope Rope::Rope(Rope* rL, Rope* rR){
  left = rL;
  right = rR;
}

Rope Rope::concat(const Rope& rL, const Rope& rR){
  if(rL.isShortLeaf() && rR.isShortLeaf()){
    // If both arguments are short leaves, we produce a flat rope (leaf) consisting
    // of the concatenation.
    return(Rope(rL.sequence + rR.sequence));
  }
  if(rL.isConcatNode() && rL.right->isShortLeaf() && rR.isShortLeaf()){
    // If the left argument is a concatenation node whose right son is a
    // short leaf, and the right argument is also a short leaf, then we
    // concatenate the two leaves, and then concatenate the result to the
    // left son of the left argument
    return(Rope::concat(*rL.left, Rope(rL.right->sequence + rR.sequence)));
  }
  // In the general case, concatenation involves simply allocating a concatenation node
  // containing two pointers to the two arguments.
  return(Rope(&rL, &rR));
}

bool Rope::isShortLeaf(){
  return(!hasChildren() && (sequence.size() <= Rope::SHORT_THRESHOLD));
}

bool Rope::hasChildren(){
  return((left != NULL) || (right != NULL));
}

bool Rope::hasRight(){
  return(right != NULL);
}

bool Rope::isConcatNode(){
  return(hasRight());
}
