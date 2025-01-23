#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "university.h"


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

    
    std::string user_input;
    while (true) {
        std::cout << "Enter the state 2 letters abbreviation or empty to exit: ";
        std::getline(std::cin, user_input);

        if (user_input.empty()) {
            break; 
        }

        if (universityMap.find(user_input) == universityMap.end()) {
            std::cout << "universities are not found in " << user_input << "\n";
        } else {
            std::cout << "Universities in " << user_input << ":\n";
            for (const auto& uni : universityMap[user_input]) {
                std::cout << ": " << uni << "\n";
            }
        }
    }

    return 0; 
}
