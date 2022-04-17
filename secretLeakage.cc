#include "secretLeakage.hh"

std::set<std::string> checkSecretLeakage(varListType nonDepList, varListType randomList, varListType secretList, varListType secretMaskList, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1, z3::solver& s, z3::context& c) {
    
    std::set<std::string> leakList;
    for(int i=0; i<secretList.size(); i++) {
        std::string svarName, smaskName;
        std::vector<int> svarDetails = getVariableType(secretList[i], svarName); // {{1->bv,2->bv_arr},{bv_sz},{bv_arr_sz}} // assumed single bv
        smaskName = secretMaskList[i].second;
        std::cout << "...for secret variable "+svarName+"...\n";

        z3::solver sc(c);
        sc.add(s.assertions());
        for(auto rv:randomList) {
            sc.add(getExpression(rv.second, varVector_0, varMap_0, "_0") == getExpression(rv.second, varVector_1, varMap_1, "_1"));
        }
        for(int j=0; j<secretList.size(); j++) {
            auto sv = secretList[j];
            if(sv.second==secretList[i].second) continue;
            std::string svarName;
            std::vector<int> svarDetails = getVariableType(sv, svarName);
            if(svarDetails[0]==2) {
                for(int pos=0; pos<svarDetails[2]; pos++) {
                    sc.add(getExpression(svarName+"["+std::to_string(pos).c_str()+"]", varVector_0, varMap_0, "_0") == getExpression(svarName+"["+std::to_string(pos).c_str()+"]", varVector_1, varMap_1, "_1"));
                }
            } else {
                sc.add(getExpression(sv.second, varVector_0, varMap_0, "_0") == getExpression(sv.second, varVector_1, varMap_1, "_1"));
            }
            sc.add(getExpression(secretMaskList[j].second, varVector_0, varMap_0, "_0") == getExpression(secretMaskList[j].second, varVector_1, varMap_1, "_1"));
        }

        if(svarDetails[0]==1) {// single bv variable
            for(int bvPos=0; bvPos<svarDetails[1]; bvPos++) {
                // std::cout << "...at bv pos " << bvPos << " : ";
                z3::solver ss(c);
                ss.add(sc.assertions());
                z3::expr svar_0 = getExpression(svarName, varVector_0, varMap_0, "_0");
                z3::expr svar_1 = getExpression(svarName, varVector_1, varMap_1, "_1");
                // std::cout << svar_0 << "\n";
                // int powbvPos = 1 << bvPos;
                // ss.add((((svar_0&(powbvPos)) / (powbvPos)) == 0));
                // ss.add((((svar_1&(powbvPos)) / (powbvPos)) == 1));
                ss.add((svar_0^getExpression(smaskName, varVector_0, varMap_0, "_0")).extract(bvPos,bvPos)==0);
                ss.add((svar_1^getExpression(smaskName, varVector_1, varMap_1, "_1")).extract(bvPos,bvPos)==1);

                // check dependence of the non-dependent intermediate variables
                std::string ivarName;
                std::vector<int> ivarDetails;
                for(auto w:nonDepList) {
                    ivarDetails = getVariableType(w, ivarName);
                    // if(ivarName!="t3x") continue;
                    // std::cout << "   ...checking with random dependent variable "+ivarName+"\n";

                    if(checkDependenceUtil(w, ss, c, varVector_0, varMap_0, varVector_1, varMap_1)) leakList.insert(ivarName);
                }

                // s.pop();
                // s.pop();
                // break;
            }
        } else if(svarDetails[0]==2) {// bv array
            for(int sarrPos=0; sarrPos<svarDetails[2]; sarrPos++) {
                std::cout << "checking for secret variable position : " << sarrPos << "\n";
                for(int bvPos=0; bvPos<svarDetails[1]; bvPos++) {
                    // std::cout << "...at bv pos " << bvPos << " : \n";
                    z3::solver ss(c);
                    ss.add(sc.assertions());
                    z3::expr svar_0 = z3::select(getExpression(svarName, varVector_0, varMap_0, "_0"), sarrPos);
                    z3::expr svar_1 = z3::select(getExpression(svarName, varVector_1, varMap_1, "_1"), sarrPos);
                    // int powbvPos = 1 << bvPos;
                    // ss.add((((svar_0&(powbvPos)) / (powbvPos)) == 0));
                    // ss.add((((svar_1&(powbvPos)) / (powbvPos)) == 1));
                    ss.add((svar_0^getExpression(smaskName, varVector_0, varMap_0, "_0")).extract(bvPos,bvPos)==0);
                    ss.add((svar_1^getExpression(smaskName, varVector_1, varMap_1, "_1")).extract(bvPos,bvPos)==1);

                    // check dependence of intermediate variables
                    std::string ivarName;
                    std::vector<int> ivarDetails;
                    for(auto w:nonDepList) {
                        ivarDetails = getVariableType(w, ivarName);
                        // if(ivarName!="t") continue;
                        // std::cout << "   ...checking with intermediate variable "+ivarName+"\n";
                        if(checkDependenceUtil(w, ss, c, varVector_0, varMap_0, varVector_1, varMap_1)) leakList.insert(ivarName);
                    }

                    // s.pop();
                    // s.pop();
                    // break;
                }
                // break;
            }
        }

        std::cout << "...leaking variables...:\n";
        for(auto v:leakList) {
            std::cout << "|"+v+"|";
        }
        if(leakList.size()==0) {
            std::cout << "   ...none...";
        }
        std::cout << std::endl;
    }

    return leakList;
}