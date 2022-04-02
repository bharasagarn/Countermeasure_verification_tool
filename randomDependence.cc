#include "randomDependence.hh"

void checkRandomDependence(varListType randomList, varListType intermList, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1, z3::solver& s) {
    for(auto v:randomList) {
        std::cout << "Checking for random variable "+v.second+" :\n";

        std::string rvarName;
        std::vector<int> rvarDetails = getVariableType(v, rvarName); // {{1->bv,2->bv_arr},{bv_sz},{bv_arr_sz}} // assumed single bv
        for(int bvPos=0; bvPos<rvarDetails[1]; bvPos++) {
            // std::cout << "...at pos " << bvPos << " : ";
            std::cout << getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) << "\n";
            s.add(getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) == getExpression(rvarName, varVector_1, varMap_1, "_1").extract(bvPos, bvPos));

            // check dependence of intermediate variables
            std::string ivarName;
            std::vector<int> ivarDetails;
            for(auto w:intermList) {
                ivarDetails = getVariableType(w, ivarName);
                std::cout << "   ...checking with intermediate variable "+ivarName+"\n";
                // single bv
                if(ivarDetails[0]==1) {
                    s.push();
                    s.add(getExpression(ivarName, varVector_0, varMap_0, "_0") == getExpression(ivarName, varVector_1, varMap_1, "_1"));
                    std::cout << "   ...result : " << s.check() << "\n";
                    s.pop();
                } else if(ivarDetails[0]==2) {
                    bool isDep = false;
                    for(int iarrPos=0; iarrPos<ivarDetails[2]; iarrPos++) {
                        // std::cout << "   ...checking "+ivarName+"[" << iarrPos << "]\n";
                        s.push();
                        s.add(z3::select(getExpression(ivarName, varVector_0, varMap_0, "_0"), iarrPos) == z3::select(getExpression(ivarName, varVector_1, varMap_1, "_1"), iarrPos));
                        if(s.check()==z3::unsat) {
                            isDep=true;
                            s.pop();
                            break;
                        }
                        s.pop();
                    }
                    std::cout << "   ...result : ";
                    if(isDep) std::cout << "unsat\n";
                    else std::cout << "sat\n";
                }
            }
        }
    }
}