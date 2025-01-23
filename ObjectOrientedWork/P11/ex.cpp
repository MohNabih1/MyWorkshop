#include <iostream>
#include <iomanip>

int main() {
    int num1 = 10;
    int num2 = 100;
    int num3 = 1000;

    std::cout << std::setw(6) << num1 << std::endl;
    std::cout << std::setw(6) << num2 << std::endl;
    std::cout << std::setw(6) << num3 << std::endl;

    return 0;
}
