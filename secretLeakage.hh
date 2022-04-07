#ifndef SECRETLEAKAGE_H
#define SECRETLEAKAGE_H

#include "getVariables.hh"
#include "getAssertions.hh"
#include "randomDependence.hh"

// for every (key,mask) pair
// variable must be dependent on key as well as mask
std::set<std::string> checkSecretLeakage(varListType nonDepList, varListType secretList, std::vector<std::string> secretMaskList, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1, z3::solver& s, z3::context& c);


#endif