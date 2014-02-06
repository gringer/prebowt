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

#include "dtree.hpp"
#include <iostream>

ostream& operator<<(ostream& out, const DTree& src){
  for(size_t i = 0; i < src.length; i++){
    out << src.sequences[i];
  }
  return out;
}

DTree::DTree(const string& src){
  deltas[0] = 0;
  sequences[0] = src;
  length = 1;
}

DTree DTree::substr(const size_t& start, const size_t& len){
  return *this;
}

void DTree::append(const string& src){
  if(length < SEQ_MAX){
    deltas[length] = sequences[length-1].length();
    // TODO: should account for right children
    sequences[length] = src;
    length++;
  }
}

void DTree::insert( const size_t& pos, const string src){
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
