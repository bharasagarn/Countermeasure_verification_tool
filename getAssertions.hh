#ifndef GETASSERTION_H
#define GETASSERTION_H

#include "getVariables.hh"

void addAssertions(z3::solver &s, z3::expr_vector &varVector, std::vector<z3::expr_vector> &arrayVector, varMapType &varMap, varMapType &arrayMap);

int getStatementType(std::string statement);

void processLoop(z3::solver &s, z3::expr_vector &varVector, std::vector<z3::expr_vector> &arrayVector, varMapType &varMap, varMapType &arrayMap);

z3::expr processArithmetic(z3::expr_vector &varVector, std::vector<z3::expr_vector> &arrayVector, varMapType &varMap, varMapType &arrayMap);

z3::expr getArrayElement(std::string arrayElementName, std::vector<z3::expr_vector> &arrayVector, varMapType &arrayMap);

#endif