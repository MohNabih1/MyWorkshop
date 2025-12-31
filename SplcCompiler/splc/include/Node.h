/*
 * Copyright © 2024 Leonidas Fegaras, University of Texas at Arlington
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gc.h>
#include <gc_cpp.h>
#include <list>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <climits>
#include <cstdarg>

using namespace std;

#ifndef Node_incl
#define Node_incl 1

#include "env.h"

extern int line_number;
extern int position;

#define splc_error(msg) throw runtime_error("("+string(__FILE__)+":"+to_string(__LINE__)+") "+msg)

extern int screen_size;

class Node {
public:
  virtual int size () const = 0;

  virtual string pretty ( int pos, const string &prefix ) const = 0;

  string pretty ( const string &prefix ) const { return prefix+pretty(0,prefix); }

  string toString () const {
    int ss = screen_size;
    screen_size = INT_MAX;
    string s { pretty("") };
    if (s.length() > ss)
      s = s.substr(0,ss)+" ...";
    screen_size = ss;
    return s;
  }
};

class NId: public Node {
public:
  string name;

  NId ( const string &name ): name { name } {}

  int size () const {
    return name.length();
  }

  string pretty ( int pos, const string &prefix ) const {
    return name;
  }
};

class NCall: public Node {
public:
  string name;
  vector<Node*>* args;

  NCall ( const string &name, vector<Node*>* args ): name { name }, args { args } {}

  int size () const {
    int n = 0;
    for ( auto a: *args )
      n += a->size()+1;
    return n+name.length()+2;
  }

  string pretty ( int pos, const string &prefix ) const {
    if (pos+size() <= screen_size) {
      string s { name+"(" };
      int i = 0;
      for ( auto a: *args ) {
        if (i++ > 0) s += ",";
        s += (a == nullptr) ? "null" : a->pretty(pos,prefix);
      }
      return s+")";
    } else {
      string s { name+"(" };
      int i = 0;
      for ( auto a: *args ) {
        if (i++ > 0)
          s += ",\n"+prefix+string(pos+name.length()+1,' ');
        s += (a == nullptr) ? "null" : a->pretty(pos+name.length()+1,prefix);
      }
      return s+")";
    }
  }
};

class NVec: public Node {
public:
  vector<Node*>* args;

  NVec ( vector<Node*>* args ): args { args } {}

  int size () const {
    if (args->size() == 0)
      return 3;
    int n = 0;
    for ( auto a: *args )
      n += a->size()+2;
    return n+4;
  }

  string pretty ( int pos, const string &prefix ) const {
    if (args->size() == 0)
      return "[ ]";
    else if (pos+size() <= screen_size) {
      string s { "[ " };
      int i = 0;
      for ( auto a: *args ) {
        if (i++ > 0) s += ", ";
        s += (a == nullptr) ? "null" : a->pretty(pos,prefix);
      }
      return s+" ]";
    } else {
      string s { "[ " };
      int i = 0;
      for ( auto a: *args ) {
        if (i++ > 0)
          s += ",\n"+prefix+string(pos+2,' ');
        s += (a == nullptr) ? "null" : a->pretty(pos+2,prefix);
      }
      return s+" ]";
    }
  }
};

class NTuple: public Node {
public:
  vector<Node*>* args;

  NTuple ( vector<Node*>* args ): args { args } {}

  int size () const {
    int n = 0;
    for ( auto a: *args )
      n += a->size()+2;
    return n+4;
  }

  string pretty ( int pos, const string &prefix ) const {
    if (pos+size() <= screen_size) {
      string s { "< " };
      int i = 0;
      for ( auto a: *args ) {
        if (i++ > 0) s += ", ";
        s += (a == nullptr) ? "null" : a->pretty(pos,prefix);
      }
      return s+" >";
    } else {
      string s { "< " };
      int i = 0;
      for ( auto a: *args ) {
        if (i++ > 0)
          s += ",\n"+prefix+string(pos+2,' ');
        s += (a == nullptr) ? "null" : a->pretty(pos+2,prefix);
      }
      return s+" >";
    }
  }
};

class NInt: public Node {
public:
  int value;

  NInt ( int value ): value { value } {}

  int size () const {
    return to_string(value).length();
  }

  string pretty ( int pos, const string &prefix ) const {
    return to_string(value);
  }
};

class NFloat: public Node {
public:
  float value;

  NFloat ( float value ): value { value } {}

  int size () const {
    ostringstream out;
    out << value;
    return out.str().length();
  }

  string pretty ( int pos, const string &prefix ) const {
    ostringstream out;
    out << value;
    return out.str();
  }
};

class NString: public Node {
public:
  string value;

  NString ( string value ): value { value } {}

  int size () const {
    return value.length()+2;
  }

  string pretty ( int pos, const string &prefix ) const {
    return "\""+value+"\"";
  }
};

class NBool: public Node {
public:
  bool value;

  NBool ( bool value ): value { value } {}

  int size () const {
    return (value) ? 4 : 5;
  }

  string pretty ( int pos, const string &prefix ) const {
    return (value) ? "true" : "false";
  }
};

#endif
