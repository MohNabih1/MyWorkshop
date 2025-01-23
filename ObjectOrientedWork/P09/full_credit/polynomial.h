#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <vector>
#include <stdexcept>

class Polynomial {
protected:
    std::vector<double> coefficients;
public:
    Polynomial(const std::vector<double>& coefficients);
    virtual ~Polynomial();
    virtual std::vector<double> solve() const = 0; 
};

#endif 

