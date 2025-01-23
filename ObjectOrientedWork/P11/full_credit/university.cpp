#include "university.h"

University::University(const std::string& name, int enrollment) 
    : _name(name), _enrollment(enrollment) {
    validate(); 
}

std::string University::name() const {
    return _name;
}
int University::enrollment() const {
    return _enrollment;
}

void University::validate() const {
    if (_enrollment < 0) {
        throw std::invalid_argument("University enrollment must be greater than zero, enter a positive number");
    }
    if (_enrollment > 0 && _name.empty()) {
        throw std::invalid_argument("please provide a character");
    }
}

std::istream& operator>>(std::istream& is, University& uni) {
    std::getline(is, uni._name); 
    is >> uni._enrollment;       
    is.ignore();                 
    uni.validate();             
    return is;
}

std::ostream& operator<<(std::ostream& os, const University& uni) {
    os << uni.name() << " has an enrollment of " << uni.enrollment();
    return os;
}
