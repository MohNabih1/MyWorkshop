#include "quadratic.h"
#include <cmath>

Quadratic::Quadratic(const std::vector<double>& coefficients)
    : Polynomial(coefficients) {
    if (coefficients.size() != 3 || coefficients[0] == 0) {
        throw std::runtime_error("a may not be 0");
    }
}

std::vector<double> Quadratic::solve() const {
    double a = coefficients[0];
    double b = coefficients[1];
    double c = coefficients[2];
    double discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
     throw std::runtime_error("No roots!");
    }
    double root1 = (-b + sqrt(discriminant)) / (2*a);
    double root2 = (-b - sqrt(discriminant)) / (2*a);
  return {root1, root2};
}

