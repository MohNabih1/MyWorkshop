os := $(shell uname -s)
node := $(shell uname -n)

ifeq ($(node),omega.uta.edu)
   solution-dir := solution/omega
else ifeq ($(os),Linux)
   solution-dir := solution/linux
else
   solution-dir := solution/macos
endif

scanner =  ${solution-dir}/spl.scanner.o  #spl.scanner.o 
parser =   ${solution-dir}/spl.parser.o   #spl.parser.o 
typechecker = ${solution-dir}/typecheck.o #typecheck.o  

binary-files = AST.o ${scanner} ${parser} ${typechecker} interpreter.o codegen.o llvm.o print.o
binary-print = $(filter-out ${solution-dir}/print.o, $(wildcard ${solution-dir}/*.o))

cflags = -g -std=c++11 -Iinclude
libs = -lreadline -lgc -lgccpp -ldl
flex_flags =
bison_flags = # -Wcounterexamples


all: bin/solution bin/splc

bin/splc: ${binary-files}
	g++ ${cflags} src/splc.cpp ${binary-files} $(libs) -o bin/splc

%.o: src/%.cpp
	g++ ${cflags} -c $<

src/spl.scanner.cpp: spl.lex
	flex $(flex_flags) -o src/spl.scanner.cpp spl.lex

src/spl.parser.cpp: spl.y
	bison spl.y $(bison_flags)

src/AST.cpp: bin/gen AST.gen IR.gen Value.gen
	cat *.gen | bin/gen src/AST.cpp include/AST.h AST.h

bin/gen: gen/gen.y
	bison gen/gen.y -o gen.cc
	g++ ${cflags} gen.cc -o bin/gen

bin/solution: AST.o src/spl.parser.cpp src/spl.scanner.cpp llvm.o
	g++ ${cflags} src/splc.cpp AST.o llvm.o ${solution-dir}/*.o $(libs) -o bin/solution

print: bin/solution print.o
	g++ ${cflags} src/splc.cpp AST.o llvm.o ${binary-print} print.o $(libs) -o bin/splc

clean: 
	rm -f *~ */*~ a.out *.o include/AST.h bin/gen bin/splc bin/solution
	rm -f gen.cc src/AST.cpp include/spl.parser.h src/spl.parser.cpp
	rm -f src/spl.scanner.cpp tests/*ll tests/*.s
