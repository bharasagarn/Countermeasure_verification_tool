#include "getVariables.hh"
#include "main.hh" 
#include "getAssertions.hh"

int main() {
    // temporary vector used to store var names and types extracted from src
    std::vector<std::pair<std::string,std::string>> varList;

    // z3 context instantiation
    z3::context c;
    z3::expr_vector varVector(c);
    std::vector<z3::expr_vector> arrayVector;
    // varMap->{varName,varVector_index}
    varMapType varMap;

    // z3 variable declaration
    try {
        getVariablesList(varList, "input.txt");
        declareVariables(varList, c, varVector, varMap);

        varList.clear();getVariablesList(varList, "random.txt");
        declareVariables(varList, c, varVector, varMap);

        varList.clear();getVariablesList(varList, "intermediate.txt");
        declareVariables(varList, c, varVector, varMap);

        varList.clear();getVariablesList(varList, "secret.txt");
        declareVariables(varList, c, varVector, varMap);

        
        std::cout << "Testing rebuilt exprArr for sbox..." << std::endl;
        std::cout << "Position of sbox in varVector: " << varMap["sbox"] << std::endl;
        z3::solver s(c);
        s.add(z3::select(varVector[varMap["sbox"]],255) == 28);
        s.push();
        z3::expr test_expr = z3::select(varVector[varMap["sbox"]],255) == 250;
        s.add(z3::select(varVector[varMap["sbox"]],255) == 27);
        std::cout << s.check() << std::endl;
        s.pop();
        std::cout << s.check() << std::endl;
        std::cout << "Test successful." << std::endl;

    } catch(const std::exception& e) {
        std::cout << "Variable type or declaration error.\n";
        return EXIT_FAILURE;
    }

    // z3 solver and assertions
    z3::solver s(c);
    
    

    return 0;
}