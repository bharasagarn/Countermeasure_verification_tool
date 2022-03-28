#ifndef GETVARIABLES_H
#define GETVARIABLES_H

#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include "z3++.h"
#include <map>
#include <cmath>

typedef std::pair<std::string, std::string> pairStringType;
typedef std::vector<std::pair<std::string,std::string>> varListType;
typedef std::map<std::string, int> varMapType;

void getVariablesList(varListType& varList, std::string fileName);

pairStringType getArrayDetails(std::string s);

std::vector<int> getVariableType(pairStringType rawVariable, const char* varName);

void declareVariables(varListType varList, z3::context& c, z3::expr_vector& varVector, varMapType& varMap);

#endif