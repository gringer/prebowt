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

#include <iostream>

#include "dtree.hpp"
#include "prebowtconfig.hpp"

size_t DTree::leafDepth = 0;

ostream& operator<<(ostream& out, const DTree& src){
  if(src.nodes[0]){
    out << *(src.nodes[0]);
  }
  for(size_t i = 0; i < src.length; i++){
    out << src.sequences[i];
    if(src.nodes[i+1]){
      out << *(src.nodes[i+1]);
    }
  }
  return out;
}

DTree::DTree(const string& src){
  deltas[0] = 0;
  sequences[0] = src;
  deltas[1] = src.length();
  length = 1;
}

DTree DTree::substr(const uint64_t& start, const uint64_t& len){
  if(depth == leafDepth){ // a leaf node (no children)
    size_t startSeq = 0;
    size_t endSeq = 0;
    uint64_t end = start + len;
    uint64_t pos = deltas[0];
    uint64_t posStart = 0;
    for(size_t i = 1; i <= length; i++){
      pos += deltas[i];
      if(start >= pos){
        posStart = pos;
        startSeq = i;
      }
      if(end > pos){
        endSeq = i;
      }
    }
    if(startSeq == endSeq){
      string sub = sequences[posStart].substr(start - posStart, len);
      DTree ret(sub);
      return(ret);
    }
  }
  return *this;
}

void DTree::append(const string& src){
  if(length < SEQ_MAX){
    // TODO: should account for right children
    sequences[length] = src;
    deltas[++length] = src.length();
  }
}

void DTree::insert( const uint64_t& pos, const string src){
}

int main(){
  string sA("The quick brown ");
  string sB("fox jumps over ");
  string sC("the lazy ");
  string sD("dog");
  cerr << "Testing tree creation... ";
  DTree dA(sA);
  DTree dB = sB;
  cerr << " done\n";
  cerr << "Result[dA]: " << dA << endl;
  cerr << "Result[dB]: " << dB << endl;
  cerr << "Testing substring on simple string... ";
  cerr << "'" << dA.substr(4,5) << "' == 'quick'...";
  cerr << " done\n";
  cerr << "Testing append of string...";
  DTree dC = sA;
  dC.append(sB);
  cerr << " done\n";
  cerr << "Result[dC]: " << dC << endl;
  cerr << "Testing substring with range [0,len(left)+1]...";
  cerr << "'" << dC.substr(0,sA.length()+1)
       << "' == 'The quick brown f'...";
  cerr << " done\n";
  cerr << "Testing substring with range [4,len(left+right)]...";
  cerr << "'" << dC.substr(4,sA.length()+sB.length())
       << "' == 'quick brown fox jumps over '...";
  cerr << " done\n";
  cerr << "Testing insertion...";
  DTree dD = sA;
  dD.append(sC);
  dD.insert(sA.length(),sB);
  cerr << " done\n";
  cerr << "Result[dD]: " << dD << endl;
}
