#ifndef GETASSERTIONS_H
#define GETASSERTIONS_H

#include "getVariables.hh"

bool isExpression(std::string expression);

int hasOperator(std::string expression);

z3::expr getExpression(std::string expression, z3::expr_vector &varVector, varMapType &varMap);

void addAssertions(std::string fileName, z3::expr_vector &varVector, varMapType &varMap, z3::solver &s);

#endif