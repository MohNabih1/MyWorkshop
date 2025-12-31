%{
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

#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>

using namespace std;

template< typename T >
vector<T>* elem ( T x ) {
  auto a { new vector<T>({ x }) };
  return a;
}

vector<string> cxx_externals { "Environment" };
 
class gtype {
public:
  string* name;
  vector<gtype*>* params;
  bool isRef;

  string toString ( bool byref = false ) const {
    string s { *name };
    if (params && params->size() > 0) {
      s += "<";
      int i = 0;
      for ( auto t: *params ) {
        if (i > 0) s += ",";
        s += (t == nullptr) ? "null" : (t->toString());
        i++;
      }
      s += ">";
    }
    for ( auto c: cxx_externals )
      if (*name == c)
	s = "shared_ptr<" + s + ">";
    if (!byref)
      return s;
    else return "const "+s+"&";
  }

  string meta () const { return toString(); }
};

string new_var () {
  static int new_var_count = 0;
  return "x$"+to_string(new_var_count++);
}

tuple<string,string> make_meta ( string v, gtype tp ) {
  if (*tp.name == "int")
    return tuple<string,string>("make_shared<NInt>(NInt("+v+"))","");
  else if (*tp.name == "string")
    return tuple<string,string>("make_shared<NString>(NString("+v+"))","");
  else if (*tp.name == "float")
    return tuple<string,string>("make_shared<NFloat>(NFloat("+v+"))","");
  else if (*tp.name == "bool")
    return tuple<string,string>("make_shared<NBool>(NBool("+v+"))","");
  else if (tp.params == nullptr || tp.params->size() == 0)
    return tuple<string,string>("("+v+"==nullptr)?nullptr:("+v+"->meta())","");
  else if (tp.params != nullptr && tp.params->size() > 0) {
    if (*tp.name == "list")
      return tuple<string,string>("make_shared<Nid>(NId(\"[...]\"))","");  // to avoid recursive env
    else if (*tp.name == "vector" || *tp.name == "list") {
      string nv { new_var() };
      string vs { new_var() };
      auto t { make_meta(nv,*(*tp.params)[0]) };
      string gs { "   vector<shared_ptr<Node>> "+vs+";\n" };
      gs += "   for ( auto "+nv+": "+v+" ) {\n";
      gs += get<1>(t)+"      "+vs+".push_back("+get<0>(t)+");\n   }\n";
      return tuple<string,string>("make_shared<NVec>(NVec("+vs+"))",gs);
    } else if (*tp.name == "tuple") {
      string gs { "" };
      string e { "{ " };
      int i = 0;
      for ( auto ptp: *tp.params ) {
        if (i > 0) e += ", ";
        auto t { make_meta("get<"+to_string(i)+">("+v+")",*ptp) };
        e += get<0>(t);
        gs += get<1>(t);
        i++;
      }
      e += " }";
      return tuple<string,string>("make_shared<NTuple>(NTuple(vector<shared_ptr<Node>>("+e+")))",gs);
    } else return tuple<string,string>("make_shared<NId>(NId(\""+*tp.name+"\"))","");
  } else return tuple<string,string>("error","");
}

tuple<string,string> make_equal ( const string &v, const string &w, const gtype &tp ) {
  if (*tp.name == "int" || *tp.name == "string" || *tp.name == "float" || *tp.name == "bool")
    return tuple<string,string>(v+" == "+w,"");
  else if (tp.params == nullptr || tp.params->size() == 0)
    return tuple<string,string>("*"+v+" == *"+w,"");
  else if (*tp.name == "vector") {
    auto t { make_equal(""+v+"[i]",""+w+"[i]",*(*tp.params)[0]) };
    string gs { "   if ("+v+".size() != "+w+".size()) return false;\n" };
    gs += "   for ( int i = 0; i < "+v+".size(); i++ )\n";
    gs += "      if (!("+get<0>(t)+")) return false;\n";
    return tuple<string,string>("true",gs);
  } else if (*tp.name == "list") {
    auto t { make_equal("get<0>(zipped)","get<1>(zipped)",*(*tp.params)[0]) };
    string gs = "   if ("+v+".size() != "+w+".size()) return false;\n";
    gs += "   for ( auto&& zipped: zip("+v+","+w+") )\n";
    gs += "      if (!("+get<0>(t)+")) return false;\n";
    return tuple<string,string>("true",gs);
  } else if (*tp.name == "tuple") {
    string gs { "" };
    string e { "" };
    int i = 0;
    for ( auto ptp: *tp.params ) {
      if (i > 0) e += " && ";
      auto t = make_equal("get<"+to_string(i)+">("+v+")",
                          "get<"+to_string(i)+">("+w+")",*ptp);
      e += get<0>(t);
      gs += get<1>(t);
      i++;
    }
    return tuple<string,string>(e,gs);
  } else return tuple<string,string>("true","");
}

template<typename ... Args>
const char* sf ( const string& format, Args ... args ) {
  int size_s = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
  if (size_s <= 0)
    throw runtime_error( "Error during formatting." );
  auto size = static_cast<size_t>(size_s);
  char* buf { new char[size] };
  snprintf( buf, size, format.c_str(), args ... );
  return buf;
}

static vector<tuple<string,vector<tuple<string*,gtype*>*>*,vector<string>*>> abstract_classes;
static vector<tuple<string,string,vector<tuple<string*,gtype*>*>*>> sub_classes;

static int line_number = 1;

int yylex ();

void yyerror ( const string &error ) {
  cerr << error << endl;
}

ofstream hout;
ofstream sout;

char* include_file;

%}

%union {
  vector<gtype*>*                 Targs;
  vector<tuple<string*,gtype*>*>* Tattrs;
  gtype*                          Ttp;
  string*                         Tstring;
}

%nterm <Targs> args;
%nterm <Tattrs> attrs atrs;
%nterm <Ttp> tp;
%nterm <Tstring> cdef;
%nterm <Tstring> classes;

%token <Tstring> ID;
%token class_token;

%%

prog: { hout << "#include \"Node.h\"\n\n";
        string defname;
	for ( char c: string(include_file) )
	  defname += (c == '.' || c == '/') ? '_' : c;
        hout << "#ifndef " << defname << "_incl\n#define "
	     << defname << "_incl 1\n";
        sout << "#include \"" << include_file << "\"\n\n"; }
      classes
      { for ( auto x: abstract_classes ) {
          hout << "class " << get<0>(x) << "_;\n";
	  hout << "typedef shared_ptr<" << get<0>(x) << "_> " << get<0>(x) << ";\n";
          for ( auto c: *get<2>(x) ) {
            hout << "class " << c << "_;\n";
	    hout << "typedef shared_ptr<" << c << "_> " << c << ";\n";
	  }
        }
        hout << endl;
        for ( auto x: abstract_classes ) {
          hout << "class " << get<0>(x) << "_ { public:\n";
          for ( auto attr: *get<1>(x) )
            hout << get<1>(*attr)->toString() << " " << *get<0>(*attr) << ";\n";
          if (get<2>(x)->empty()) {
            hout << "shared_ptr<Node> meta () const;\n";
            hout << "bool operator== ( const " << get<0>(x) << "_ &that ) const;\n";
          } else {
            hout << "virtual shared_ptr<Node> meta () const = 0;\n";
            hout << "virtual bool operator== ( const " << get<0>(x) << "_ &that ) const = 0;\n";
          }
          hout << "string toString () const { return meta()->toString(); }\n";
          hout << "string pretty ( const string &prefix = \"\" ) const { return meta()->pretty(prefix); }\n";
          for ( auto c: *get<2>(x) ) {
            hout << c << "_* to" << c << " () const;\n";
            hout << "inline bool is" << c << " () const { return to" << c << "() != nullptr; };\n";
          }
          hout << "};\n\n";
        }
        hout << *$2;
        for ( auto x: abstract_classes ) {
          if (get<2>(x)->empty()) {
            sout << get<0>(x) << " mk" << get<0>(x) << "( ";
            hout << get<0>(x) << " mk" << get<0>(x) << "( ";
            int i = 0;
            for ( auto attr: *get<1>(x) )
              if (*get<0>(*attr) != "line_number" && *get<0>(*attr) != "position") {
                if (i > 0) { sout << ", "; hout << ", "; }
                auto tpn { get<1>(*attr)->toString(true) };
                sout << tpn << " " << *get<0>(*attr);
                hout << tpn << " " << *get<0>(*attr);
                i++;
              }
            sout << " ) {\n   " << get<0>(x) << " o = make_shared<" << get<0>(x) << "_>(" << get<0>(x) << "_());\n";
            hout << " );\n";
            for ( auto attr: *get<1>(x) )
              sout << sf("   o->%s = %s;\n",get<0>(*attr)->c_str(),get<0>(*attr)->c_str());
            sout << "   return o;\n};\n";
            sout << "shared_ptr<Node> " << get<0>(x) << "_::meta () const {\n";
            if (get<1>(x)->size() > 0) {
              string gs = "";
              string e = "{ ";
              int i = 0;
              for ( auto attr: *get<1>(x) ) {
                if (i > 0) e += ", ";
                auto t = make_meta(*get<0>(*attr),*get<1>(*attr));
                e += get<0>(t);
                gs += get<1>(t);
                i++;
              }
              e += " }";
              sout << gs << "   return make_shared<NCall>(NCall(\"" << get<0>(x)
                   << "\",vector<shared_ptr<Node>>("+e+")));\n};\n\n";
            } else sout << "return make_shared<NId>(NId(\"" << get<0>(x) << "\"));\n};\n";
            sout << "bool " << get<0>(x) << "_::operator== ( const " << get<0>(x) << "_ &that ) const  {\n";
            string e = "   return ";
            if (get<1>(x)->size() > 0) {
              string gs = "";
              int i = 0;
              for ( auto attr: *get<1>(x) )
                if (*get<0>(*attr) != "line_number" && *get<0>(*attr) != "position") {
                  if (i > 0) e += " && ";
                  auto t = make_equal(*get<0>(*attr),"that."+*get<0>(*attr),*get<1>(*attr));
                  e += get<0>(t);
                  gs += get<1>(t);
                  i++;
                }
              sout << gs << e << ";\n};\n";
            }
          }
        }
        for ( auto x: sub_classes ) {
          sout << sf("shared_ptr<Node> %s""_::meta () const {\n",get<0>(x).c_str());
          if (get<2>(x)->size() > 0) {
            string gs = "";
            string e = "{ ";
            int i = 0;
            for ( auto attr:  *get<2>(x) ) {
              if (i > 0) e += ", ";
              auto t = make_meta(*get<0>(*attr),*get<1>(*attr));
              e += get<0>(t);
              gs += get<1>(t);
              i++;
            }
            e += " }";
            sout << gs << "   return make_shared<NCall>(NCall(\"" << get<0>(x)
                 << "\",vector<shared_ptr<Node>>("+e+")));\n};\n";
          } else sout << "return make_shared<NId>(NId(\"" << get<0>(x) << "\"));\n};\n\n";
          sout << "bool " << get<0>(x) << "_::operator== ( const " << get<1>(x) << "_ &that ) const {\n";
          if (get<2>(x)->size() > 0) {
            string gs = "";
            sout << "   if (that.is" << get<0>(x) << "()) {\n";
            sout << "      auto that_ = that.to" << get<0>(x) << "();\n";
            string e = "      return ";
            int i = 0;
            for ( auto attr: *get<2>(x) ) {
              if (i > 0) e += " && ";
              auto t = make_equal(*get<0>(*attr),"that_->"+*get<0>(*attr),*get<1>(*attr));
              e += get<0>(t);
              gs += get<1>(t);
              i++;
            }
            sout << gs << e << ";\n   } else return false;\n};\n";
          } else sout << "   return that.is" << get<0>(x) << "();\n}\n";
        }
	hout << "#endif\n";
      }
      ;

classes: cdef ';'
         { $$ = $1; }
       | classes cdef ';'
         { $$ = new string(*$1+*$2); }
       ;

cdef: class_token ID ':' ID '(' attrs ')'
      { ostringstream out;
        for ( auto x: abstract_classes )
          if (get<0>(x) == *$4)
            get<2>(x)->push_back(*$2);
        out << sf("class %s""_: public %s""_ { public:\n",
                  $2->c_str(),$4->c_str());
        for ( auto attr: *$6 )
          out << sf("%s %s;\n",get<1>(*attr)->toString().c_str(),get<0>(*attr)->c_str());
        out << "shared_ptr<Node> meta () const;\n";
        out << "bool operator== ( const " << *$4 << "_ &that ) const;\n};\n";
        out << *$2 << " mk" << *$2 << " ( ";
        sout << *$2 << " mk" << *$2 << " ( ";
        int i = 0;
        for ( auto attr: *$6 ) {
          if (i > 0) { out << ", "; sout << ", "; }
	  auto tpn { get<1>(*attr)->toString(true) };
          out << tpn << " " << *get<0>(*attr);
          sout << tpn << " " << *get<0>(*attr);
          i++;
        }
        sout << sf(" ) {\n   %s o = make_shared<%s""_>(%s""_());\n",$2->c_str(),$2->c_str(),$2->c_str());
        out << " );\n";
        sout << "   o->line_number = line_number;\n";
        sout << "   o->position = position;\n";
        for ( auto attr: *$6 )
          sout << sf("   o->%s = %s;\n",get<0>(*attr)->c_str(),get<0>(*attr)->c_str());
        sout << "   return o;\n};\n\n";
        out << sf("inline %s""_* %s""_::to%s () const { return dynamic_cast<%s""_*>((%s""_*)this); }\n\n",
                  $2->c_str(),$4->c_str(),$2->c_str(),$2->c_str(),$4->c_str());
        sub_classes.push_back(tuple<string,string,vector<tuple<string*,gtype*>*>*>
                              (*$2,*$4,$6));
        $$ = new string(out.str());
      }
    | class_token ID '(' attrs ')'
      { auto gint = new gtype(); gint->name = new string("int");
        gint->params = nullptr; gint->isRef = false;
        $4->push_back(new tuple<string*,gtype*>(new string("line_number"),gint));
        $4->push_back(new tuple<string*,gtype*>(new string("position"),gint));
        abstract_classes.push_back(tuple<string,vector<tuple<string*,gtype*>*>*,
                           vector<string>*>(*$2,$4,new vector<string>));
        $$ = new string("");
      }
    ;

attrs:      { $$ = new vector<tuple<string*,gtype*>*>(); }
     | atrs { $$ = $1; }
     ;

atrs: tp ID          { $$ = elem(new tuple<string*,gtype*>($2,$1)); }
    | atrs ',' tp ID { $$ = $1; $$->push_back(new tuple<string*,gtype*>($4,$3)); }
    ;

args: tp          { $$ = elem($1); }
    | args ',' tp { $$ = $1; $$->push_back($3); }
    ;

tp: ID              { $$ = new gtype(); $$->name = $1; $$->params = nullptr; $$->isRef = false; }
  | ID '<' args '>' { $$ = new gtype(); $$->name = $1; $$->params = $3; $$->isRef = false; }
  | tp '*'          { $$ = $1; $$->isRef = true; }
  ;

%%

int yylex () {
  int c = getchar();
  while (c == ' ' || c == '\t' || c == '\n')
    c = getchar();
  if (c == '/') {
    int c2 = getchar();
    if (c2 == '*') {
      while (c != '*' || c2 !='/') {
        c = c2;
        c2 = getchar();
      }
      return yylex();
    } else ungetc(c2,stdin);
  }
  if (isalpha(c)) {
    string s;
    do {
      s += c;
      c = getchar();
    } while (isalnum(c) || c == '_');
    ungetc(c,stdin);
    if (s == "class")
      return class_token;
    yylval.Tstring = new string(s);
    return ID;
  }
  else if (c == EOF)
    return YYEOF;
  else
    return c;
}

int main ( int argc, char* argv[] ) {
  sout.open(argv[1]);
  hout.open(argv[2]);
  include_file = argv[3];
  yyparse();
  sout.close();
  hout.close();
  return 0;
}
