#ifndef RANDOMDEPENDENCE_H
#define RANDOMDEPENDENCE_H

#include "getVariables.hh"
#include "getAssertions.hh"
#include <set>

bool checkDependenceUtil(pairStringType ivar, z3::solver& xs, z3::context& c, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1);

varListType checkRandomDependence(varListType randomList, varListType secretList, varListType secretMaskList, varListType intermList, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1, z3::solver& s, z3::context& c);

#endif