#ifndef GETASSERTIONS_H
#define GETASSERTIONS_H

#include "getVariables.hh"

bool isExpression(std::string expression);

int hasOperator(std::string expression);

z3::expr getExpression(std::string expression, z3::expr_vector &varVector, varMapType &varMap, std::string f);

void addAssertions(std::string fileName, z3::expr_vector &varVector_0, varMapType &varMap_0, z3::expr_vector &varVector_1, varMapType &varMap_1, z3::solver &s);

#endif