#include "getAssertions.hh"

bool isExpression(std::string expression) {// "=" is second word in the line (3-OP form assumption)
    std::stringstream ss(expression);
    std::string st;
    ss >> st; ss >> st;
    if(st=="=") return true;
    return false;
}

int hasOperator(std::string expression) {
    std::string ops = "+-*/^&|%";
    int i;
    for(i=0; i<expression.size(); i++) {
        for(auto op:ops) {
            if(expression[i]==op) return i;
        }
    }
    return -1;
}

z3::expr getExpression(std::string expression, z3::expr_vector &varVector, varMapType &varMap) {
    std::cout << "Expression under getExpression : " << expression << "\n";

    // arithmeticExpression
    int opPosition = hasOperator(expression);
    if(opPosition!=-1) {
        if(expression[opPosition]=='^') return getExpression(expression.substr(0,opPosition), varVector, varMap)^getExpression(expression.substr(opPosition+1), varVector, varMap);
        if(expression[opPosition]=='&') return getExpression(expression.substr(0,opPosition), varVector, varMap)&getExpression(expression.substr(opPosition+1), varVector, varMap);
        if(expression[opPosition]=='*') return getExpression(expression.substr(0,opPosition), varVector, varMap)*getExpression(expression.substr(opPosition+1), varVector, varMap);
        if(expression[opPosition]=='/') return getExpression(expression.substr(0,opPosition), varVector, varMap)/getExpression(expression.substr(opPosition+1), varVector, varMap);
        if(expression[opPosition]=='+') return getExpression(expression.substr(0,opPosition), varVector, varMap)+getExpression(expression.substr(opPosition+1), varVector, varMap);
        if(expression[opPosition]=='-') return getExpression(expression.substr(0,opPosition), varVector, varMap)-getExpression(expression.substr(opPosition+1), varVector, varMap);
        if(expression[opPosition]=='|') return getExpression(expression.substr(0,opPosition), varVector, varMap)|getExpression(expression.substr(opPosition+1), varVector, varMap);
        if(expression[opPosition]=='%') return getExpression(expression.substr(0,opPosition), varVector, varMap)%getExpression(expression.substr(opPosition+1), varVector, varMap);
    }
    
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
            for(auto v:stmt) std::cout << "|"+v+"|";
            std::cout << "\n";
            
            s.add(getExpression(stmt[0], varVector, varMap) == getExpression(stmt[1], varVector, varMap));
        }

        inputProg.close();
}