/** <header>

 * This file is part of preBowt -- a prefix-based BWT-like transform of
 * genetic data.
 *
 * Copyright 2014 David Eccles (gringer) <bioinformatics@gringene.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

</header> **/

#include <iostream>

#include "dtree.hpp"
#include "prebowtconfig.hpp"

size_t DTree::leafDepth = 0;
size_t DTree::nextNodeNum = 0;

#define NODE_DEBUG 1
//#define MEMORY_DEBUG 1

ostream& operator<<(ostream& out, const DTree& src){
#if NODE_DEBUG
  out << "{";
#if MEMORY_DEBUG
  out << "#" << src.nodeNum;
#endif
#endif
  if(src.nodes[0]){
    out << *(src.nodes[0]);
  }
  for(size_t i = 0; i < src.nodeCount; i++){
#if NODE_DEBUG
    out << "<" << src.deltas[i] << ">";
    out << "[";
#endif
    out << src.sequences[i];
#if NODE_DEBUG
    out << "]";
#endif
    if(src.nodes[i+1]){
      out << *(src.nodes[i+1]);
    }
  }
#if NODE_DEBUG
  out << "<" << src.deltas[src.nodeCount] << ">";
  out << "}";
#endif
  return out;
}

// constructors

DTree::DTree(){
  initialise();
}

DTree::DTree(const string& src){
  initialise();
  inplaceAppend(src);
#if MEMORY_DEBUG
  cerr << "%% " << *this << endl;
#endif
}

// Copy constructor (shallow copy)
DTree::DTree(const DTree& src){
  initialise();
  depth = src.depth;
  inplaceAppend(src);
}

// Assignment operator (shallow copy)
DTree& DTree::operator=(const DTree& src){
  if(this != &src){ // gracefully handle self assignment
    initialise();
    depth = src.depth;
    inplaceAppend(src);
  }
  return *this;
}

// public methods

DTree DTree::substr(const uint64_t& start, const uint64_t& len) const{
  uint64_t end = start + len;
  DSplit startSplit = split(start);
  DTree retVal = split(start).right.split(len).left;
  return(retVal);
}

// splits a tree into two component DTrees at location pos
// retVal.left: this[0,pos)
// retVal.right: this[pos,this->length())
DSplit DTree::split(const uint64_t& splitPos) const{
  if(splitPos < deltas[0]){
    // split point is inside the first child
    DSplit preSplit = nodes[0]->split(splitPos);
    DSplit retVal;
    retVal.right = *this;
    shared_ptr<DTree> preRight(&preSplit.right);
    retVal.right.nodes[0] = preRight;
    retVal.left = preSplit.left;
    return retVal;
  }
  size_t splitNode = 0;
  uint64_t splitStart = 0;
  uint64_t startPosSplit = 0;
  uint64_t pos = deltas[0] + deltas[1];
  for(size_t i = 1; (i < nodeCount) && (splitPos > pos); i++){
    splitNode++;
    startPosSplit = pos;
    pos+= deltas[i+1];
  }
  DSplit retVal;
  string preSplit = (splitPos == 0) ? 
    "" : sequences[splitNode].substr(0,splitPos - startPosSplit);
  string postSplit = sequences[splitNode].substr(splitPos - startPosSplit);
  retVal.left.inplaceAppend(*this,0,splitNode);
  retVal.left.inplaceAppend(preSplit);
  retVal.right.inplaceAppend(postSplit);
  retVal.right.inplaceAppend(*this,splitNode+1);
  return retVal;
}

DTree DTree::append(const DTree& src) const{
  DTree retVal = *this;
  if((nodeCount + src.nodeCount) <= SEQ_MAX){
    retVal.deltas[retVal.nodeCount] += src.deltas[0];
    for(int i = 0; i < src.nodeCount; i++){
      // TODO: need to check right node here to prevent overwrite
      retVal.nodes[retVal.nodeCount] = src.nodes[i];
      retVal.sequences[retVal.nodeCount] = src.sequences[i];
      retVal.deltas[++retVal.nodeCount] = src.deltas[i+1];
    }
  }
  retVal.seqLength += src.seqLength;
  return retVal;
}

DTree DTree::insert( const uint64_t& pos, const DTree& src) const{
  DSplit insertSplit = split(pos);
  return insertSplit.left.append(src).append(insertSplit.right);
}

uint64_t DTree::length() const{
  return seqLength;
}

// private accessory methods

void DTree::initialise(){
  nodeCount = 0;
  depth = 0;
  nodeNum = DTree::nextNodeNum++;
  seqLength = 0;
  deltas[0] = 0;
}

// append nodes and sequences in-place from another DTree
void DTree::inplaceAppend(const DTree& src, size_t fromNode, size_t toNode){
  if((fromNode >= toNode) || (fromNode >= src.nodeCount)){
    return;
  }
  //cerr << "Appending to: " << *this << endl;
  deltas[nodeCount] += src.deltas[fromNode];
  if(fromNode > 0){
    deltas[nodeCount] -= src.sequences[fromNode-1].length();
  }
  // Append to pre-existing node if it exists
  if(nodes[nodeCount]){
    DTree toAppend = *src.nodes[fromNode];
    nodes[nodeCount]->inplaceAppend(toAppend);
  } else {
    nodes[nodeCount] = src.nodes[fromNode];
  }
  for(int i = fromNode; (i < toNode) && (i < src.nodeCount); i++){
    sequences[nodeCount] = src.sequences[i];
    deltas[nodeCount+1] = src.deltas[i+1];
    nodes[nodeCount+1] = src.nodes[i+1];
    nodeCount++;
    seqLength += src.deltas[i+1];
  }
  //cerr << "Result: " << *this << endl;
}

// append sequence in-place
void DTree::inplaceAppend(const string& src){
  sequences[nodeCount] = src;
  deltas[++nodeCount] = src.length();
  seqLength += src.length();
}

// test function

int main(){
  size_t nextTestID = 0;
  string sA("The quick brown ");
  string sB("fox jumps over ");
  string sC("the lazy ");
  string sD("dog");
  cout << "[" << ++nextTestID << "] Testing tree creation... ";
  DTree dA(sA);
  DTree dB = sB;
  DTree dC(sC);
  DTree dD = sD;
  cout << " done\n";
  cout << "    Result[dA]: " << dA << endl;
  cout << "    Result[dB]: " << dB << endl;
  cout << "    Result[dC]: " << dC << endl;
  cout << "    Result[dD]: " << dD << endl;
  cout << "[" << ++nextTestID << "] Testing append of DTree A and DTree B...";
  DTree dE = dA.append(dB);
  cout << " done\n";
  cout << "    Result[dE]: " << dE << endl;
  cout << "[" << ++nextTestID << "] Testing append of DTree E and DTree C...";
  DTree dF = dE.append(dC);
  cout << " done\n";
  cout << "    Result[dF]: " << dF << endl;
  cout << "[" << ++nextTestID << "] Testing split in middle...";
  DSplit sG = dA.split(10);
  cout << " done\n";
  cout << "    Result[sG.left]: " << sG.left << endl;
  cout << "    Result[sG.right]: " << sG.right << endl;
  cout << "[" << ++nextTestID << "] Testing split at left...";
  DSplit sH = dA.split(0);
  cout << " done\n";
  cout << "    Result[sH.left]: " << sH.left << endl;
  cout << "    Result[sH.right]: " << sH.right << endl;
  cout << "[" << ++nextTestID << "] Testing split at right...";
  DSplit sI = dA.split(dA.length());
  cout << " done\n";
  cout << "    Result[sI.left]: " << sI.left << endl;
  cout << "    Result[sI.right]: " << sI.right << endl;
  cout << "[" << ++nextTestID 
       << "] Testing complete substring on single node using length()... ";
  cout << "'" << dA.substr(0,dA.length()) << "' == 'The quick brown '...";
  cout << " done\n";
  cout << "[" << ++nextTestID 
       << "] Testing partial substring on single node... ";
  cout << "'" << dA.substr(4,5) << "' == 'quick'...";
  cout << " done\n";
  cout << "[" << ++nextTestID 
       << "] Testing substring with range [0,len(left)+1]...";
  cout << "'" << dE.substr(0,sA.length()+1)
       << "' == 'The quick brown f'...";
  cout << " done\n";
  cout << "[" << ++nextTestID 
       << "] Testing substring with range [4,len(left+right)]...";
  cout << "'" << dE.substr(4,sA.length()+sB.length() - 4)
       << "' == 'quick brown fox jumps over '...";
  cout << " done\n";
  cout << "[" << ++nextTestID 
       << "] Testing insertion...";
  DTree dJ = dA.append(dC).insert(dA.length(), dB);
  cout << " done\n";
  cout << "     Result[dJ]: " << dJ << endl;
}
