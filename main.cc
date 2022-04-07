#include "getVariables.hh"
#include "main.hh" 
#include "getAssertions.hh"
#include "randomDependence.hh"
#include "secretLeakage.hh"
#include "time.h"

int main() {
    clock_t tStart = clock();

    // temporary vector used to store var names and types extracted from src
    std::vector<std::pair<std::string,std::string>> inputList, randomList, intermList, secretList;
    std::vector<std::string> secretMasksList;

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

        getVariablesList(inputList, "input.txt");
        std::cout << "...input variables...\n";
        declareVariables(inputList, c, varVector_0, varMap_0, varVector_1, varMap_1);

        getVariablesList(randomList, "random.txt");
        std::cout << "...random variables...\n";
        declareVariables(randomList, c, varVector_0, varMap_0, varVector_1, varMap_1);

        getVariablesList(intermList, "intermediate.txt");
        std::cout << "...intermediate variables...\n";
        declareVariables(intermList, c, varVector_0, varMap_0, varVector_1, varMap_1);

        getVariablesList(secretList, "secret.txt");
        std::cout << "...secret variables...\n\n";
        declareVariables(secretList, c, varVector_0, varMap_0, varVector_1, varMap_1);
        secretMasksList = getSecretMasks("secretmask.txt");

        std::cout << "Declaring bv constants 0-255...\n";
        declareConstants(c, varVector_0, varMap_0, varVector_1, varMap_1);

        
        std::cout << "Testing variable declarations :\n";
        std::cout << "...Position of sbox in varVector_0: " << varMap_0["sbox_0"] << std::endl;
        std::cout << "...Position of sbox in varVector_1: " << varMap_1["sbox_1"] << std::endl;
        std::cout << "...Expr ind_a252 in varVector_0: " << varVector_0[varMap_0["ind_a252_0"]] << std::endl;
        std::cout << "...Expr ind_a252 in varVector_1: " << varVector_1[varMap_1["ind_a252_1"]] << std::endl;
        std::cout << "...Position of t3x in varVector_0: " << varMap_0["t3x_0"] << std::endl;
        std::cout << "...Position of t3x in varVector_1: " << varMap_1["t3x_1"] << std::endl;
        std::cout << std::endl;


    } catch(const std::exception& e) {
        std::cout << "Variable type or declaration error.\n";
        return EXIT_FAILURE;
    }

    // z3 solver and stmt assertions
    std::cout << "Adding assertions :\n";
    std::cout << "...initializing solver...\n";
    z3::solver s(c);
    std:: cout << "...reading statements...\n";
    addAssertions("AES_mixcols_masked_src.txt", varVector_0, varMap_0, varVector_1, varMap_1, s);
    std::cout << "...added all stmt assertions for bv_0 & bv_1\n";
    std::cout << std::endl;
    

    // random variable dependence
    // varListType nonDepList = checkRandomDependence(randomList, intermList, varVector_0, varMap_0, varVector_1, varMap_1, s, c);

    // final secret leakage check
    varListType nonDepList = {{"uint8_t", "temp[4]"}};
    std::set<std::string> leakList = checkSecretLeakage(nonDepList, secretList, secretMasksList, varVector_0, varMap_0, varVector_1, varMap_1, s, c);

    std::cout << "\n\nTotal time elapsed : " << (clock()-tStart)/CLOCKS_PER_SEC << " seconds.\n";

    return 0;
}