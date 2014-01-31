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

using namespace std;

class Rope{
public:
  static const int SHORT_THRESHOLD = 20;
  Rope(const Rope& src);
  Rope(Rope* rL, Rope* rR);
  Rope(const string& tSeq);
  Rope& operator=(const Rope& src);
  ~Rope();
  static Rope concat(Rope& rL, Rope& rR);
  const bool isShortLeaf();
  const bool hasChildren();
  const bool isConcatNode();
  const bool hasRight();
private:
  Rope* left = NULL;
  Rope* right = NULL;
  string sequence;
};

#endif //__ROPE_HPP_
