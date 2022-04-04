#ifndef SECRETLEAKAGE_H
#define SECRETLEAKAGE_H

#include "getVariables.hh"
#include "getAssertions.hh"

// for every (key,mask) pair
// variable must be dependent on key as well as mask
void checkSecretLeakage();


#endif