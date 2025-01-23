#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include <iostream>
#include <stdexcept>

class University {
private:
    std::string _name;
    int _enrollment;

    void validate() const; 

public:
    
    University(const std::string& name = "", int enrollment = 0);

    
    std::string name() const; 
    int enrollment() const;   
   
    friend std::istream& operator>>(std::istream& is, University& uni); 
    friend std::ostream& operator<<(std::ostream& os, const University& uni); 
};

#endif 
