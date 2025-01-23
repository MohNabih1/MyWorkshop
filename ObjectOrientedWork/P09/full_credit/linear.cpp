#include "linear.h"

Linear::Linear(const std::vector<double>& coefficients)
    : Polynomial(coefficients) {
    if (coefficients.size() != 2 || coefficients[0] == 0) {
        throw std::runtime_error("a may not be 0");
 }
}

std::vector<double> Linear::solve() const {
    double a = coefficients[0];
    double b = coefficients[1];
    return {-b / a};
}

