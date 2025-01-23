//Mohamad Nabih Alkhateeb

#include "inch.h"
#include <iostream>

int main() {
    Inch in, last_in, sum;
    while (std::cin.good()) {
        std::cin >> in;
        if (!std::cin.good()){ 
	break;
}
        
        sum = sum + in;

        std::cout << in << std::endl;
        
        if (in == last_in) std::cout << " == ";
        if (in != last_in) std::cout << " != ";
        if (in < last_in) std::cout << " < ";
        if (in <= last_in) std::cout << " <= ";
        if (in > last_in) std::cout << " > ";
        if (in >= last_in) std::cout << " >= ";
        std::cout << std::endl;

        std::cout << "total: " << sum << std::endl;

        last_in = in;
    }

    return 0;
}

