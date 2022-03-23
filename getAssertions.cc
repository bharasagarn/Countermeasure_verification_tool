#include "getAssertions.hh"

z3::expr processArithmetic(std::string expression, z3::expr_vector &varVector, varMapType &varMap, z3::context c) {
    z3::expr res(c);
    
}

z3::expr getArrayElement(std::string arrayElementName) {
    // indx = processArithmetic(ind)
    // return arrayVector[arrayMap['arrName']]
    pairStringType arrDetails = getArrayDetails(arrayElementName);
    
}

void addAssertions(z3::solver &s, z3::expr_vector &varVector, varMapType &varMap) {

}