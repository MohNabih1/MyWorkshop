#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "university.h"

template <typename T>
void print_container(const T& container) {
   
    typename T::const_iterator it;
    for (it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << std::endl;
    }
}


using State = std::string;
using Universities = std::vector<University>;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "using: " << argv[0] << " <enrollment.txt>\n";
        return -1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << argv[1] << ": can't find file\n";
        return -2;
    } 

    std::map<State, Universities> universityMap;
    State state;
    University university;

    
    while (file >> state >> university) {
        if (universityMap.find(state) == universityMap.end()) {
            universityMap[state] = Universities(); 
        }
        universityMap[state].push_back(university); 
    }

    
    std::string input;
    while (true) {
        std::cout << "Enter the state 2 letters abbreviation or empty to exit: ";
        std::getline(std::cin, input);

        if (input.empty()) {
            break; 
        }

        if (universityMap.find(input) == universityMap.end()) {
            std::cout << "universities are not found in " << input << "\n";
        } else {
            const Universities& universities = universityMap[input];
            print_container(universities);
        }
    }

    return 0; 
}
