#include "randomDependence.hh"

void checkRandomDependence(varListType randomList, varListType intermList, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1, z3::solver& s, z3::context& c) {

    std::set<std::string> depList;
    std::vector<std::set<std::string>> depListVector;
    for(auto v:randomList) {

        std::string rvarName;
        std::vector<int> rvarDetails = getVariableType(v, rvarName); // {{1->bv,2->bv_arr},{bv_sz},{bv_arr_sz}} // assumed single bv
        if(rvarName!="x") continue;
        std::cout << "Checking for random variable "+rvarName+" :\n";
        depList.clear();

        for(int bvPos=0; bvPos<rvarDetails[1]; bvPos++) {
            // std::cout << "...at pos " << bvPos << " : ";
            // std::cout << getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) << "\n";
            // s.push(); s.add(getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) == 0);
            // s.push(); s.add(getExpression(rvarName, varVector_1, varMap_1, "_1").extract(bvPos, bvPos) == 1);
            z3::solver rs(c);
            rs.add(s.assertions());
            rs.add(getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) == 0);
            rs.add(getExpression(rvarName, varVector_1, varMap_1, "_1").extract(bvPos, bvPos) == 1);

            // check dependence of intermediate variables
            std::string ivarName;
            std::vector<int> ivarDetails;
            for(auto w:intermList) {
                ivarDetails = getVariableType(w, ivarName);
                if(ivarName!="t3x") continue;
                std::cout << "   ...checking with intermediate variable "+ivarName+"\n";

                z3::solver irs(c);
                irs.add(rs.assertions());

                // single bv
                if(ivarDetails[0]==1) {
                    // s.push();
                    irs.add(getExpression(ivarName, varVector_0, varMap_0, "_0") == getExpression(ivarName, varVector_1, varMap_1, "_1"));
                    if(irs.check()==z3::unsat) depList.insert(ivarName);
                    // s.pop();
                }
                // bv array 
                else if(ivarDetails[0]==2) {
                    bool isDep = false;
                    for(int iarrPos=0; iarrPos<ivarDetails[2]; iarrPos++) {
                        std::cout << "      ...checking "+ivarName+"[" << iarrPos << "]\n";
                        // s.push();
                        irs.add(z3::select(getExpression(ivarName, varVector_0, varMap_0, "_0"), iarrPos) == z3::select(getExpression(ivarName, varVector_1, varMap_1, "_1"), iarrPos));
                        if(irs.check()==z3::unsat) {
                            std::cout << "      ...unsat...\n";
                            isDep=true;
                            // s.pop();
                            break;
                        } else {
                            std::cout << "      ...sat...\n";
                        }
                        // s.pop();
                    }
                    if(isDep) depList.insert(ivarName);
                }
            }

            // s.pop();
            // s.pop();
        }

        std::cout << "...dependent variables...\n";
        for(auto v:depList) {
            std::cout << "|"+v+"|";
        }
        std::cout << std::endl;
        depListVector.push_back(depList);
    }

}