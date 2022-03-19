#include "getAssertions.hh"

z3::expr processArithmetic(z3::expr_vector &varVector, std::vector<z3::expr_vector> &arrayVector, varMapType &varMap, varMapType &arrayMap) {
    
}

z3::expr getArrayElement(std::string arrayElementName, std::vector<z3::expr_vector> &arrayVector, varMapType &arrayMap) {
    // indx = processArithmetic(ind)
    // return arrayVector[arrayMap['arrName']]
    pairStringType arrDetails = getArrayDetails(arrayElementName);
    
}

void addAssertions(z3::solver &s, z3::expr_vector &varVector, std::vector<z3::expr_vector> &arrayVector, varMapType &varMap, varMapType &arrayMap) {

}