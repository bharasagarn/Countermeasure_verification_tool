#ifndef GETASSERTION_H
#define GETASSERTION_H

#include "getVariables.hh"

std::vector<std::string> arith_operators = {"^","|","&","*","/","+","-","%"};

void addAssertions(z3::solver &s, z3::expr_vector &varVector, varMapType &varMap);

int getStatementType(std::string statement);

void processLoop(z3::solver &s, z3::expr_vector &varVector, varMapType &varMap);

z3::expr processArithmetic(std::string expression, z3::expr_vector &varVector, varMapType &varMap, z3::context c);

z3::expr getArrayElement(std::string arrayElementName);

#endif