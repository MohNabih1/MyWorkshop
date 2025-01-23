#include <iostream>
#include <algorithm>
#include <random>
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


// sort and shuffle functoins obtained from online resources

std::sort(odds.begin(), odds.end());
   
std::random_device rd;
std::mt19937 g(rd());
std::shuffle(evens->begin(), evens->end(), g);

std::cout << "Odd lengths (sorted):" << std::endl;
for (const auto& word : odds) {
      std::cout << word << std::endl;
    }

std::cout << "Even lengths (shuffled):" << std::endl;
for (const auto& word : *evens) {
        std::cout << word << std::endl;
    }


delete evens;
return 0;}




