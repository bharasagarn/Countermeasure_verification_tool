#include "getVariables.hh"
#include "main.hh" 
#include "getAssertions.hh"

int main() {
    // temporary vector used to store var names and types extracted from src
    std::vector<std::pair<std::string,std::string>> varList;

    // z3 context instantiation
    z3::context c;
    z3::expr_vector varVector_0(c), varVector_1(c);
    // varMap->{varName,varVector_index}
    varMapType varMap_0, varMap_1;
    std::cout << "Initializing z3 context...\n\n";

    // z3 variable declaration
    try {
        std::cout << "Extracting list of variables...\n\n";
        std::cout << "Declaring z3 variables corresponding to :\n";

        getVariablesList(varList, "input.txt");
        std::cout << "...input variables...\n";
        declareVariables(varList, c, varVector_0, varMap_0);
        declareVariables(varList, c, varVector_1, varMap_1);

        varList.clear();getVariablesList(varList, "random.txt");
        std::cout << "...random variables...\n";
        declareVariables(varList, c, varVector_0, varMap_0);
        declareVariables(varList, c, varVector_1, varMap_1);

        varList.clear();getVariablesList(varList, "intermediate.txt");
        std::cout << "...intermediate variables...\n";
        declareVariables(varList, c, varVector_0, varMap_0);
        declareVariables(varList, c, varVector_1, varMap_1);

        varList.clear();getVariablesList(varList, "secret.txt");
        std::cout << "...secret variables...\n\n";
        declareVariables(varList, c, varVector_0, varMap_0);
        declareVariables(varList, c, varVector_1, varMap_1);

        std::cout << "Declaring bv constants 0-255...\n";
        declareConstants(c, varVector_0, varMap_0);
        declareConstants(c, varVector_1, varMap_1);

        
        std::cout << "Testing variable declarations :\n";
        std::cout << "...Position of sbox in varVector_0: " << varMap_0["sbox"] << std::endl;
        std::cout << "...Position of sbox in varVector_1: " << varMap_1["sbox"] << std::endl;
        std::cout << "...Position of ind_a252 in varVector_0: " << varMap_0["ind_a252"] << std::endl;
        std::cout << "...Position of ind_a252 in varVector_1: " << varMap_1["ind_a252"] << std::endl;
        std::cout << "...Position of t4 in varVector_0: " << varMap_0["t4"] << std::endl;
        std::cout << "...Position of t4 in varVector_1: " << varMap_1["t4"] << std::endl;
        std::cout << std::endl;

        // std::cout << "Testing assertions :\n";
        // std::cout << "...initializing solver...\n";
        // z3::solver s(c);
        // s.add( z3::select(varVector[varMap["sbox"]],255) == 80 );
        // std::cout << "...added assertion : 'sbox[255]==80'...\n";
        // s.push();
        // z3::expr test_expr = z3::select(varVector[varMap["sbox"]],255) == 250;
        // s.add(z3::select(varVector[varMap["sbox"]],255) == 82 );
        // std::cout << "...added assertion : 'sbox[255]==82'...\n";
        // std::cout << "...Solver check : " << s.check() << "..." << std::endl;
        // s.pop();
        // std::cout << "...Popped last added assertion...\n";
        // std::cout << "...Solver check : " << s.check() << "..." << std::endl;
        // std::cout << "Test successful." << std::endl;

    } catch(const std::exception& e) {
        std::cout << "Variable type or declaration error.\n";
        return EXIT_FAILURE;
    }

    // z3 solver and assertions
    std::cout << "Adding assertions :\n";
    std::cout << "...initializing solver...\n";
    z3::solver s(c);
    std:: cout << "...reading statements...\n";
    addAssertions("AES_mixcols_masked_src.txt", varVector_0, varMap_0, s);
    

    return 0;
}