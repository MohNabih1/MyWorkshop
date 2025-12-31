Gen is a compiler that translates algebraic data types to C++ classes to make
abstract syntax trees used by a compiler easier to specify and code.

For example, the following gen specification:
```cpp
class Expr ( Type tpe );
class BinOpExp: Expr ( string op, Expr left, Expr right );
```
generates the following code:
```cpp
class Expr {
public:
  Type* tpe;
  int lineno;                // last line number (set by the scanner)
  int position;              // last position number (set by the scanner)
  string toString ();        // print this Expr into a string
  string pretty ();          // pretty-print it in multiple lines
  bool isBinOpExp ();        // true if this Expr is a BinopExp
  BinOpExp* toBinOpExp ();   // coerce this Expr to a BinopExp
}

class BinOpExp: public Expr {
public:
  string op;
  Expr* left;
  Expr* right;
  bool operator== ( const Expr &that );  // deep equality
};
// construct a new BinOpExp
BinOpExp* mkBinOpExp ( const string &op, Expr* left, Expr* right );
```
