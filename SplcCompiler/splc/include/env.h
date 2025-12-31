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
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>

using namespace std;

/** A map from names to values of type T with nested scopes */
template< typename T >
class Environment {
private:

  // the environment is a hash table (an array of buckets)
  typedef vector<tuple<string,T>*>* HashTableBucket;

  const int hash_table_size = 997;

  HashTableBucket* hash_table;
  vector<int> scopes;

  /** a hashing function for strings */
  int string_hash ( const string &s ) const {
    return abs((int)hash<string>()(s)) % hash_table_size;
  }

public:

  Environment () {
    hash_table = new (GC) HashTableBucket[hash_table_size];
    for ( int i = 0; i < hash_table_size; i++ )
      hash_table[i] = nullptr;
  }

  /** true if the item exists in the environment */
  bool exists ( const string &key ) const {
    auto b { hash_table[string_hash(key)] };
    if (b)
      for ( auto it = b->rbegin(); it != b->rend(); it++ )
        if (get<0>(**it) == key)
          return true;
    return false;
  }

  /** lookup for an item in the map */
  const T& lookup ( const string &key ) const {
    auto b { hash_table[string_hash(key)] };
    if (b)
      for ( auto it = b->rbegin(); it != b->rend(); it++ )
        if (get<0>(**it) == key)
          return get<1>(**it);
    throw runtime_error("Name "+key+" has not been defined");
  }

  /** insert a new binding in the map */
  void insert ( const string &key, const T &value ) {
    int loc { string_hash(key) };
    if (!hash_table[loc])
      hash_table[loc] = new (GC) vector<tuple<string,T>*>();
    hash_table[loc]->push_back(new (GC) tuple<string,T>(key,value));
    scopes.push_back(loc);
  }
 
  /** if the item exists, replace its value; otherwise insert a new binding */
  void replace ( const string &key, const T &value ) {
    auto b { hash_table[string_hash(key)] };
    if (b)
      for ( auto it = b->rbegin(); it != b->rend(); it++ )
        if (get<0>(**it) == key) {
          get<1>(**it) = value;
          return;
        }
    insert(key,value);
  }

  /** start a new scope */
  void begin_scope () {
    scopes.push_back(-1);
  }

  /** pop the last scope */
  void end_scope () {
    int loc { 0 };
    while ( loc >= 0 ) {
      loc = scopes.back();
      scopes.pop_back();
      if (loc >= 0)
        if (hash_table[loc])
          hash_table[loc]->pop_back();
    }
  }

  /** return the current scope as a list of pairs */
  list<tuple<string,T>*>* current_scope () {
    auto scope { new (GC) list<tuple<string,T>*>() };
    vector<int> counts(hash_table_size);
    for ( int loc = scopes.size()-1; loc >= 0 && scopes[loc] >= 0; loc-- ) {
      auto v { hash_table[scopes[loc]] };
      if (v) {
        scope->push_front((*v)[v->size()-counts[scopes[loc]]-1]);
        counts[scopes[loc]]++;
      }
    }
    return scope;
  }

  /** clone the current environment to make it available to a closure */
  Environment<T>* clone () {
    Environment<T>* env { new (GC) Environment<T>() };
    for ( int i = 0; i < hash_table_size; i++ )
      if (hash_table[i]) {
        env->hash_table[i] = new (GC) vector<tuple<string,T>*>();
        for ( auto t: *hash_table[i] )
          env->hash_table[i]->push_back(t);
      }
    for ( int i: scopes )
      env->scopes.push_back(i);
    return env;
  }

  /** display the content of the symbol table (type T must have a toString method) */
  void print () {
    cout << "*** Symbol table:\n";
    vector<int> counts(hash_table_size);
    for ( int i = 0; i < hash_table_size; i++ )
      counts[i] = 0;
    for ( int loc = scopes.size()-1; loc >= 0; loc-- ) {
      if (scopes[loc] < 0)
        cout << "--------------------------\n";
      else {
        auto v { hash_table[scopes[loc]] };
        if (v != nullptr) {
          auto t { (*v)[v->size()-counts[loc]-1] };
          cout << get<0>(*t) << ": " << get<1>(*t)->toString() << endl;
          counts[scopes[loc]]++;
        }
      }
    }
    cout << "--------------------------\n";
  }
};
