#include <iostream>
#include "polynomial.h"
#include "linear.h"
#include "quadratic.h"

//Mohamad Nabih Alkhateeb

   int main(int argc, char** argv) {
    if (argc != 3 && argc != 4) {
       std::cerr << "Invalid polynomial size - must be linear or quadratic" << std::endl;
        return -1;
 }

    Polynomial* p = nullptr;
    try {
     if (argc == 3) {
        std::vector<double> coefficients = {std::stod(argv[1]), std::stod(argv[2])};
          p = new Linear(coefficients);
  } else if (argc == 4) {
         std::vector<double> coefficients = {std::stod(argv[1]), std::stod(argv[2]), std::stod(argv[3])};
        p = new Quadratic(coefficients);
  }

        auto solutions = p->solve();
       for (const auto& solution : solutions) {
          std::cout << solution << std::endl;
  }
        
        delete p;
 } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        delete p; 
        return -2;
 }

    return 0; }

