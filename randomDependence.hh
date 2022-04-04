#ifndef RANDOMDEPENDENCE_H
#define RANDOMDEPENDENCE_H

#include "getVariables.hh"
#include "getAssertions.hh"
#include <set>

void checkRandomDependence(varListType randomList, varListType intermList, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1, z3::solver& s, z3::context& c);

#endif