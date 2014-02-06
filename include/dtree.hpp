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

#ifndef __ROPE_HPP__
#define __ROPE_HPP__

#include <string>
#include <memory>

using namespace std;

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
  // fields
  uint64_t deltas[PTR_MAX];
  shared_ptr<DTree> nodes[PTR_MAX];
  string sequences[SEQ_MAX];
  size_t length;
  size_t depth;
  // constructors
  DTree(const string& src);
  // public methods
  DTree substr(const uint64_t& start, const uint64_t& len);
  void append(const string& src);
  void insert(const uint64_t& pos, const string src);
private:
  static size_t leafDepth;
};

#endif //__ROPE_HPP_
