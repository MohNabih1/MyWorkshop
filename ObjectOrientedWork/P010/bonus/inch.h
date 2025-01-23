#ifndef INCH_H
#define INCH_H

#include <iostream>
#include <numeric> 

class Inch {
public:
    Inch(); 
    Inch(int whole, int numerator, int denominator); 

    Inch operator+(const Inch& rhs) const; 
    friend std::ostream& operator<<(std::ostream& os, const Inch& obj); 
    friend std::istream& operator>>(std::istream& is, Inch& obj); 

    
 auto operator<=>(const Inch& rhs) const = default;

private:
    int _whole;
    int _numerator;
    int _denominator;

    void validate(); 
};

#endif 

