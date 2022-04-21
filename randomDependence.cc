#include "randomDependence.hh"

bool checkDependenceUtil(pairStringType ivar, z3::solver& xs, z3::context& c, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1) {

    std::string ivarName;
    std::vector<int> ivarDetails;
    ivarDetails = getVariableType(ivar, ivarName);

    z3::solver ixs(c);
    ixs.add(xs.assertions());
    // single bv
    if(ivarDetails[0]==1) {
        ixs.add(getExpression(ivarName, varVector_0, varMap_0, "_0") == getExpression(ivarName, varVector_1, varMap_1, "_1"));
        if(ixs.check()==z3::unsat) return true;

    }
    // bv array 
    else if(ivarDetails[0]==2) {
        bool isDep = false;
        for(int iarrPos=0; iarrPos<ivarDetails[2]; iarrPos++) {
            ixs.push();
            ixs.add(z3::select(getExpression(ivarName, varVector_0, varMap_0, "_0"), iarrPos) == z3::select(getExpression(ivarName, varVector_1, varMap_1, "_1"), iarrPos));

            if(ixs.check()==z3::unsat) {
                return true;
            }
            ixs.pop();
        }
    }

    return false;
}

varListType checkRandomDependence(varListType randomList, varListType secretList, varListType secretMaskList, varListType intermList, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1, z3::solver& s, z3::context& c) {

    std::set<int> depIntermIndex;
    varListType nonDepList;
    for(auto v:randomList) {

        std::string rvarName;
        std::vector<int> rvarDetails = getVariableType(v, rvarName); // {{1->bv,2->bv_arr},{bv_sz},{bv_arr_sz}} // assumed single bv
        std::cout << "...for random variable "+rvarName+"...\n";

        for(int bvPos=0; bvPos<rvarDetails[1]; bvPos++) {

            z3::solver rs(c);
            rs.add(s.assertions());
            rs.add(getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) == 0);
            rs.add(getExpression(rvarName, varVector_1, varMap_1, "_1").extract(bvPos, bvPos) == 1);

            // asserting other random variables equal for _0 and _1
            for(auto orv:randomList) {
                if(orv.second != rvarName) {
                    rs.add(getExpression(orv.second, varVector_0, varMap_0, "_0") == getExpression(orv.second, varVector_1, varMap_1, "_1"));
                }
            }
            for(auto sv:secretList) {
                std::string svarName;
                std::vector<int> svarDetails = getVariableType(sv, svarName);
                if(svarDetails[0]==2) {
                    for(int pos=0; pos<svarDetails[2]; pos++) {
                        rs.add(getExpression(svarName+"["+std::to_string(pos).c_str()+"]", varVector_0, varMap_0, "_0") == getExpression(svarName+"["+std::to_string(pos).c_str()+"]", varVector_1, varMap_1, "_1"));
                    }
                } else {
                    rs.add(getExpression(sv.second, varVector_0, varMap_0, "_0") == getExpression(sv.second, varVector_1, varMap_1, "_1"));
                }
                
            }
            for(auto sm:secretMaskList) {
                rs.add(getExpression(sm.second, varVector_0, varMap_0, "_0") == getExpression(sm.second, varVector_1, varMap_1, "_1"));
            }

            // check dependence of intermediate variables
            std::string ivarName;
            std::vector<int> ivarDetails;
            for(int intermIndex=0; intermIndex<intermList.size(); intermIndex++) {
                auto w = intermList[intermIndex];
                ivarDetails = getVariableType(w, ivarName);

                if(checkDependenceUtil(w, rs, c, varVector_0, varMap_0, varVector_1, varMap_1)) {
                    depIntermIndex.insert(intermIndex);
                }
            }

        }
    }

    std::cout << "   ...non-dependent variables...:\n";
    for(auto w:intermList) {
        bool isDep = false;
        for(auto d:depIntermIndex) {
            if(w.second==intermList[d].second) {
                isDep = true;
                break;
            }
        }
        if(!isDep) {
            nonDepList.push_back(w);
            std::cout << "   ...|"+w.second << "|...\n";
        }
    }

    return nonDepList;
}