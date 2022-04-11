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
    if(opPosition!=-1) {
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
        // std::cout << "getExpression :: array"+f+"\n";
        pairStringType arrayDetails = getArrayDetails(expression);
        // // return varVector[z3::select(varVector[varMap[arrayDetails.first+"[pos]"+f]],getExpression(arrayDetails.second, varVector, varMap, f))];
        return z3::select(varVector[varMap[arrayDetails.first+f]],getExpression(arrayDetails.second, varVector, varMap, f));

    }

    // singleExpression (varName)
    // std::cout << "getExpression :: singleExpr"+f+"\n";
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
            // for(auto v:stmt) std::cout << "|"+v+"|";
            // std::cout << "\n";

            // z3::expr e1_0 = getExpression(stmt[0], varVector_0, varMap_0, "_0");std::cout << "LHS expr_0 : " << e1_0 << "\n";
            // z3::expr e2_0 = getExpression(stmt[1], varVector_0, varMap_0, "_0");std::cout << "RHS expr_0 : " << e2_0 << "\n";
            // z3::expr e1_1 = getExpression(stmt[0], varVector_1, varMap_1, "_1");std::cout << "LHS expr_1 : " << e1_1 << "\n";
            // z3::expr e2_1 = getExpression(stmt[1], varVector_1, varMap_1, "_1");std::cout << "RHS expr_1 : " << e2_1 << "\n";

            s.add(getExpression(stmt[0], varVector_0, varMap_0, "_0") == getExpression(stmt[1], varVector_0, varMap_0, "_0"));
            s.add(getExpression(stmt[0], varVector_1, varMap_1, "_1") == getExpression(stmt[1], varVector_1, varMap_1, "_1"));

            // if(stmt[0]=="sboxm[0]") {
            //     z3::solver rs(c);
            //     rs.add(s.assertions());
            //     rs.add(getExpression("b", varVector_0, varMap_0, "_0").extract(0,0) == 0);
            //     rs.add(getExpression("b", varVector_1, varMap_1, "_1").extract(0,0) == 1);

            //     z3::solver irs(c);
            //     irs.add(rs.assertions());
            //     irs.add(getExpression("a", varVector_0, varMap_0, "_0") == getExpression("a", varVector_1, varMap_1, "_1"));
            //     irs.add(getExpression(stmt[0], varVector_0, varMap_0, "_0") == getExpression(stmt[0], varVector_1, varMap_1, "_1"));
            //     std::cout << irs.assertions() << std::endl;
            //     std::cout << irs.check() << std::endl;
            // }
        }

        inputProg.close();
}