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

class Rope{
  friend ostream& operator<<(ostream& out, const Rope& src);
public:
  // fields
  static const int SHORT_THRESHOLD = 20;
  static int nextNodeNum;
  int nodeNum;
  // constructors
  Rope(const Rope& src);
  Rope(const string& tSeq);
  Rope(const Rope& rL, const Rope& rR);
  // overloaded operators
  Rope& operator=(const Rope& src);
  // destructor
  ~Rope();
  // static public methods
  static Rope concat(const Rope& rL, const Rope& rR);
  static Rope substr(const Rope& src, const size_t& start, const size_t& len);
  // public methods
  size_t length() const;
  Rope* getLeft() const;
  Rope* getRight() const;
private:
  // fields
  shared_ptr<Rope> left; // shared_ptr used to avoid excess copying
  shared_ptr<Rope> right;
  string sequence;
  // accessory methods
  bool isLeaf() const;
  bool isShortLeaf() const;
  bool hasChildren() const;
  bool hasLeft() const;
  bool hasRight() const;
  bool isConcatNode() const;
};

#endif //__ROPE_HPP_
