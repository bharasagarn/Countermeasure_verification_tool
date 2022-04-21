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

z3::expr getExpression(std::string expression, z3::expr_vector &varVector, varMapType &varMap, std::string f) {
    // std::cout << "Expression under getExpression : " << expression << "\n";

    // arithmeticExpression
    int opPosition = hasOperator(expression);
    if(opPosition!=-1) {// expects no whitespaces between operands and operator
        if(expression[opPosition]=='^') return getExpression(expression.substr(0,opPosition), varVector, varMap, f)^getExpression(expression.substr(opPosition+1), varVector, varMap, f);
        if(expression[opPosition]=='&') return getExpression(expression.substr(0,opPosition), varVector, varMap, f)&getExpression(expression.substr(opPosition+1), varVector, varMap, f);
        if(expression[opPosition]=='*') return getExpression(expression.substr(0,opPosition), varVector, varMap, f)*getExpression(expression.substr(opPosition+1), varVector, varMap, f);
        if(expression[opPosition]=='/') return getExpression(expression.substr(0,opPosition), varVector, varMap, f)/getExpression(expression.substr(opPosition+1), varVector, varMap, f);
        if(expression[opPosition]=='+') return getExpression(expression.substr(0,opPosition), varVector, varMap, f)+getExpression(expression.substr(opPosition+1), varVector, varMap, f);
        if(expression[opPosition]=='-') return getExpression(expression.substr(0,opPosition), varVector, varMap, f)-getExpression(expression.substr(opPosition+1), varVector, varMap, f);
        if(expression[opPosition]=='|') return getExpression(expression.substr(0,opPosition), varVector, varMap, f)|getExpression(expression.substr(opPosition+1), varVector, varMap, f);
        if(expression[opPosition]=='%') return getExpression(expression.substr(0,opPosition), varVector, varMap, f)%getExpression(expression.substr(opPosition+1), varVector, varMap, f);
    }
    
    // arrayElement
    if(expression.back()==']') {
        pairStringType arrayDetails = getArrayDetails(expression);
        return z3::select(varVector[varMap[arrayDetails.first+f]],getExpression(arrayDetails.second, varVector, varMap, f));

    }

    return varVector[varMap[expression+f]];
}

void addAssertions(std::string fileName, z3::expr_vector &varVector_0, varMapType &varMap_0, z3::expr_vector &varVector_1, varMapType &varMap_1, z3::solver &s, z3::context& c) {
        
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

            s.add(getExpression(stmt[0], varVector_0, varMap_0, "_0") == getExpression(stmt[1], varVector_0, varMap_0, "_0"));
            s.add(getExpression(stmt[0], varVector_1, varMap_1, "_1") == getExpression(stmt[1], varVector_1, varMap_1, "_1"));

        }

        inputProg.close();
}