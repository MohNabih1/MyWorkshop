//Mohamad Nabih Alkhateeb

#include "inch.h"
#include <stdexcept>



Inch::Inch() : Inch(0, 0, 2) {} 

Inch::Inch(int whole, int numerator, int denominator)
    : _whole(whole), _numerator(numerator), _denominator(denominator) {
    validate();
}



Inch Inch::operator+(const Inch& rhs) const {
    int new_numerator = _numerator * (64 / _denominator) + rhs._numerator * (64 / rhs._denominator);

    return Inch(_whole + rhs._whole, new_numerator, 64);

}

std::ostream& operator<<(std::ostream& os, const Inch& obj) {
    os << obj._whole << " " << obj._numerator << "/" << obj._denominator;

    return os;
}

std::istream& operator>>(std::istream& is, Inch& obj) {
    char slash;
    is >> obj._whole >> obj._numerator >> slash >> obj._denominator;
      if (is) {
        obj.validate();
    }
    return is;
}

void Inch::validate() {
    static const int valid_denoms[] = {1,2, 4, 8, 16, 32,64};
    bool valid = false;
      for (int denom : valid_denoms) {
           if (_denominator == denom) {
            valid = true;
            break;
        }
    }
    if (!valid) {

        throw std::invalid_argument("Denominators allowed:1, 2,4,8,16,32,64");
    }

    while (_numerator >= _denominator)
 {
        _numerator -= _denominator;
        _whole++;
    }

    int gcd = std::gcd(_numerator, _denominator);
    _numerator /= gcd;
    _denominator /= gcd;
}

