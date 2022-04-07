#include "secretLeakage.hh"

std::set<std::string> checkSecretLeakage(varListType nonDepList, varListType secretList, std::vector<std::string> secretMaskList, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1, z3::solver& s, z3::context& c) {
    
    std::set<std::string> leakList;
    for(int i=0; i<secretList.size(); i++) {
        std::string svarName, smaskName;
        std::vector<int> svarDetails = getVariableType(secretList[i], svarName); // {{1->bv,2->bv_arr},{bv_sz},{bv_arr_sz}} // assumed single bv
        smaskName = secretMaskList[i];
        // if(rvarName!="x") continue;
        std::cout << "Checking for secret variable "+svarName+" :\n";
        // depList.clear();

        if(svarDetails[0]==1) {// single bv variable
            for(int bvPos=0; bvPos<svarDetails[1]; bvPos++) {
                // std::cout << "...at pos " << bvPos << " : ";
                // std::cout << getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) << "\n";
                // s.push(); s.add(getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) == 0);
                // s.push(); s.add(getExpression(rvarName, varVector_1, varMap_1, "_1").extract(bvPos, bvPos) == 1);
                z3::solver ss(c);
                ss.add(s.assertions());
                ss.add((getExpression(svarName, varVector_0, varMap_0, "_0")^getExpression(smaskName, varVector_0, varMap_0, "_0")).extract(bvPos, bvPos) == 0);
                ss.add((getExpression(svarName, varVector_1, varMap_1, "_1")^getExpression(smaskName, varVector_1, varMap_1, "_1")).extract(bvPos, bvPos) == 1);

                // check dependence of the non-dependent intermediate variables
                std::string ivarName;
                std::vector<int> ivarDetails;
                for(auto w:nonDepList) {
                    ivarDetails = getVariableType(w, ivarName);
                    // if(ivarName!="t3x") continue;
                    std::cout << "   ...checking with random dependent variable "+ivarName+"\n";

                    z3::solver iss(c);
                    iss.add(ss.assertions());

                    // single bv
                    if(ivarDetails[0]==1) {
                        // s.push();
                        iss.add(getExpression(ivarName, varVector_0, varMap_0, "_0") == getExpression(ivarName, varVector_1, varMap_1, "_1"));
                        if(iss.check()==z3::unsat) leakList.insert(ivarName);
                        // s.pop();
                    }
                    // bv array 
                    else if(ivarDetails[0]==2) {
                        bool isDep = false;
                        for(int iarrPos=0; iarrPos<ivarDetails[2]; iarrPos++) {
                            std::cout << "      ...checking "+ivarName+"[" << iarrPos << "]\n";
                            // s.push();
                            iss.add(z3::select(getExpression(ivarName, varVector_0, varMap_0, "_0"), iarrPos) == z3::select(getExpression(ivarName, varVector_1, varMap_1, "_1"), iarrPos));
                            if(iss.check()==z3::unsat) {
                                std::cout << "      ...unsat...\n";
                                isDep=true;
                                // s.pop();
                                break;
                            } else {
                                std::cout << "      ...sat...\n";
                            }
                            // s.pop();
                        }
                        if(isDep) leakList.insert(ivarName);
                    }
                }

                // s.pop();
                // s.pop();
            }
        } else if(svarDetails[0]==2) {// bv array
            for(int sarrPos=0; sarrPos<svarDetails[2]; sarrPos++) {
                for(int bvPos=0; bvPos<svarDetails[1]; bvPos++) {
                    // std::cout << "...at pos " << bvPos << " : ";
                    // std::cout << getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) << "\n";
                    // s.push(); s.add(getExpression(rvarName, varVector_0, varMap_0, "_0").extract(bvPos, bvPos) == 0);
                    // s.push(); s.add(getExpression(rvarName, varVector_1, varMap_1, "_1").extract(bvPos, bvPos) == 1);
                    z3::solver ss(c);
                    ss.add(s.assertions());
                    ss.add((z3::select(getExpression(svarName, varVector_0, varMap_0, "_0"), sarrPos)^getExpression(smaskName, varVector_0, varMap_0, "_0")).extract(bvPos, bvPos) == 0);
                    ss.add((z3::select(getExpression(svarName, varVector_1, varMap_1, "_1"), sarrPos)^getExpression(smaskName, varVector_1, varMap_1, "_1")).extract(bvPos, bvPos) == 1);

                    // check dependence of intermediate variables
                    std::string ivarName;
                    std::vector<int> ivarDetails;
                    for(auto w:nonDepList) {
                        ivarDetails = getVariableType(w, ivarName);
                        // if(ivarName!="t3x") continue;
                        std::cout << "   ...checking with intermediate variable "+ivarName+"\n";

                        z3::solver iss(c);
                        iss.add(ss.assertions());

                        // single bv
                        if(ivarDetails[0]==1) {
                            // s.push();
                            iss.add(getExpression(ivarName, varVector_0, varMap_0, "_0") == getExpression(ivarName, varVector_1, varMap_1, "_1"));
                            if(iss.check()==z3::unsat) leakList.insert(ivarName);
                            // s.pop();
                        }
                        // bv array 
                        else if(ivarDetails[0]==2) {
                            bool isDep = false;
                            for(int iarrPos=0; iarrPos<ivarDetails[2]; iarrPos++) {
                                std::cout << "      ...checking "+ivarName+"[" << iarrPos << "]\n";
                                // s.push();
                                iss.add(z3::select(getExpression(ivarName, varVector_0, varMap_0, "_0"), iarrPos) == z3::select(getExpression(ivarName, varVector_1, varMap_1, "_1"), iarrPos));
                                if(iss.check()==z3::unsat) {
                                    std::cout << "      ...unsat...\n";
                                    isDep=true;
                                    // s.pop();
                                    break;
                                } else {
                                    std::cout << "      ...sat...\n";
                                }
                                // s.pop();
                            }
                            if(isDep) leakList.insert(ivarName);
                        }
                    }

                    // s.pop();
                    // s.pop();
                }
            }
        }

        std::cout << "...dependent variables...\n";
        for(auto v:leakList) {
            std::cout << "|"+v+"|";
        }
        std::cout << std::endl;
    }

    return leakList;
}