#include "getVariables.hh"

void getVariablesList(varListType& varList, std::string fileName) {
    
    std::string s;
    std::ifstream inputVarList(fileName);

    while(getline(inputVarList, s)) {
        std::stringstream ss(s);
        std::string st;
        std::pair<std::string,std::string> varPair;
        ss >> st; varPair.first = st;
        ss >> st; varPair.second = st;
        varList.push_back(varPair);
    }

    inputVarList.close();
}

pairStringType getArrayDetails(std::string s) {
    pairStringType res;
    size_t i = 0;
    while(s[i]!='[') i++;
    res.first = s.substr(0,i);
    res.second = s.substr(i+1,s.size()-i-2);
    return res;
}

std::vector<int> getVariableType(pairStringType rawVariable, std::string& varName) {

    // std:: cout << "Entered getVariableType\n";
    // std:: cout << "rawVriable: ("+rawVariable.first+","+rawVariable.second+")\n";
    
    std::vector<int> res = {1,8,0};
    // {{1->bv,2->bv_arr},{bv_sz},{bv_arr_sz}}

    if(rawVariable.first.substr(0,4)=="uint") {
        res[1] = std::stoi(rawVariable.first.substr(4,rawVariable.first.size()-6));
        varName = rawVariable.second.substr(0,rawVariable.second.size());
        if(rawVariable.second.back()==']') {
            res[0] = 2;
            pairStringType temp = getArrayDetails(rawVariable.second);
            varName = temp.first;
            res[2] = stoi(temp.second);
        }
    } else throw std::exception();

    // std:: cout << "Exiting getVariableType\n";

    return res;
}

void declareVariables(varListType varList, z3::context& c, z3::expr_vector& varVector_0, varMapType& varMap_0, z3::expr_vector& varVector_1, varMapType& varMap_1) {

    // std:: cout << "Entered declareVariables\n";


    for(auto rawVariable: varList) {

        std::string varName;
        std::vector<int> varType = getVariableType(rawVariable, varName);
        

        z3::expr x(c);
        if(varType[0]==1) {
            // std::cout << "bv_" << varType[1] << "\n";
            x = c.bv_const((varName+"_0").c_str(), varType[1]);
            varVector_0.push_back(x);
            varMap_0[varName] = int(varVector_0.size()-1);
            x = c.bv_const((varName+"_1").c_str(), varType[1]);
            varVector_1.push_back(x);
            varMap_1[varName] = int(varVector_0.size()-1);
        } 
        else if(varType[0]==2) {

            pairStringType temp = getArrayDetails(rawVariable.second);
            // std::cout << "Declaring " << varName << "\n";
            // std::cout << "array : " << int(std::log2(varType[2])) << "->" << varType[1] << "\n";

            z3::sort BVin = c.bv_sort(8); // fixed at 8 now (every array can have upto 256 elements, perhaps not all used)
            z3::sort BVout = c.bv_sort(varType[1]);
            z3::sort BVArray = c.array_sort(BVin,BVout);

            z3::expr exprArr = c.constant(varName.c_str(),BVArray);

            varVector.push_back(exprArr);
            varMap[temp.first] = int(varVector.size()-1);
        }
        
    }

    // std:: cout << "Exiting declareVariables\n";
}

void declareConstants(z3::context& c, z3::expr_vector& varVector, varMapType& varMap) {
    // bv contants 0-255
    for(int i=0; i<256; i++) {
        varVector.push_back(c.bv_val(i,8));
        varMap[std::to_string(i)] = varVector.size()-1;
    }
}