#include <iostream>
#include <string>
#include <vector>
 

int main( int argc, char* argv[]) {

std::vector<std::string> odds;
std::vector<std::string>* evens = new std::vector<std::string>();

for (int i =1; i< argc; i++) {

std::string word = argv[i];
if ( word.length() %2==0) {
evens -> push_back(word);
  }
else {
odds.push_back(word);
  }
}

std::cout <<"Odd lengths:" << std::endl;
for (const auto& word: odds) {
std::cout << word << std::endl;
}

std:: cout <<"Even lengths:" << std::endl;
for (const auto& word: *evens) {
std::cout << word << std::endl;
}


delete evens;
return 0;}




