#include "getAssertions.hh"

bool isExpression(std::string expression) {// "=" is second word in the line (3-OP form assumption)
    std::stringstream ss(expression);
    std::string st;
    ss >> st; ss >> st;
    if(st=="=") return true;
    return false;
}

z3::expr getExpression(std::string expression, z3::expr_vector &varVector, varMapType &varMap) {
    // arithmeticExpression
    
    
    // arrayElement
    if(expression.back()==']') {
        pairStringType arrayDetails = getArrayDetails(expression);
        return z3::select(varVector[varMap[arrayDetails.first]],getExpression(arrayDetails.second, varVector, varMap));
    }

    // singleExpression
    return varVector[varMap[expression]];
}

void addAssertions(std::string fileName, z3::expr_vector &varVector, varMapType &varMap, z3::solver &s) {
        
        std::string temp;
        std::ifstream inputProg(fileName);

        while(getline(inputProg, temp)) {
            if(!isExpression(temp)) continue;
            std::stringstream ss(temp);
            std::string st;
            std::vector<std::string> stmt;
            ss >> st; stmt.push_back(st);
            ss >> st;
            ss >> st; st.pop_back(); stmt.push_back(st);
            // for(auto v:stmt) std::cout << "|"+v+"|";
            // std::cout << std::endl;
            s.add(getExpression(stmt[0], varVector, varMap) == getExpression(stmt[1], varVector, varMap));
        }

        inputProg.close();
}