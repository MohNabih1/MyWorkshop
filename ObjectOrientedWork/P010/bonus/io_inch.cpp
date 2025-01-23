//Mohamad Nabih Alkhateeb

#include "inch.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    std::ifstream ifs;
    std::ofstream ofs;
    Inch in, last_in, sum;

    
    if (argc > 1) {
        ifs.open(argv[1]);
        if (!ifs) {

            std::cerr << "Output file failed to open: " << argv[1] << std::endl;
            return 1;
        }
    }

    
    if (argc > 2) {
        ofs.open(argv[2]);
        if (!ofs) {

            std::cerr << "Output file failed to open: " << argv[2] << std::endl;
            return 1;
     }
    }

    
    if (ifs.is_open()) {
        while (ifs >> in) {

            sum = sum + in;
            if (ofs) ofs << in << std::endl;
     }
    }

    while (std::cin >> in) {
        sum = sum + in;
        if (ofs) ofs << in << std::endl;
    }

    if (!ofs.is_open()) {
        std::cout << "total: " << sum << std::endl;
    } else {

        ofs << "total: " << sum << std::endl;
    }

    return 0;
}

