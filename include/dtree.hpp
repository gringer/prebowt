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

#ifndef __DTREE_HPP__
#define __DTREE_HPP__

#include <string>
#include <memory>
#include <limits>

using namespace std;

class DSplit;

class DTree{
  friend ostream& operator<<(ostream& out, const DTree& src);
public:
  // constants
  enum Base : size_t {END, A, C, G, T, AMBIG, ALL};
  enum Limits {
    SEQ_MIN = 2,
    SEQ_MAX = 4,
    PTR_MAX = 5
  };
  // constructors
  DTree(); // create empty tree
  DTree(const string& src); // create initial tree from string
  DTree(const DTree& src); // copy constructor
  // operators
  DTree& operator=(const DTree& src); // assignment operator
  // public methods
  DTree substr(const uint64_t& start, const uint64_t& len) const;
  DSplit split(const uint64_t& splitPos) const;
  DTree append(const DTree& src) const;
  DTree insert(const uint64_t& pos, const DTree& src) const;
  uint64_t length() const;
protected:
private:
  // shared fields
  static size_t leafDepth;
  static size_t nextNodeNum;
  // personal fields
  uint64_t deltas[PTR_MAX];
  shared_ptr<DTree> nodes[PTR_MAX];
  string sequences[SEQ_MAX];
  size_t nodeCount;
  size_t depth;
  size_t nodeNum;
  uint64_t seqLength;
  // accessory methods
  void initialise();
  void inplaceAppend(const DTree& src, size_t fromNode = 0,
                     size_t toNode = numeric_limits<size_t>::max());
  void inplaceAppend(const string& src);
};

class DSplit{
public:
  DTree left;
  DTree right;
};

#endif //__DTREE_HPP_
