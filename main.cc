#include "getVariables.hh"
#include "main.hh" 
#include "getAssertions.hh"
#include "randomDependence.hh"
#include "secretLeakage.hh"
#include "time.h"

int main(int argc, char** argv) {
    clock_t tStart = clock();

    // temporary vector used to store var names and types extracted from src
    std::vector<std::pair<std::string,std::string>> inputList, randomList, intermList, secretList, secretMaskList;

    // z3 context instantiation
    z3::context c;
    z3::expr_vector varVector_0(c), varVector_1(c);
    // varMap->{varName,varVector_index}
    varMapType varMap_0, varMap_1;
    std::cout << "Initializing z3 context...\n\n";
    std::string inputFolder = argv[1];
    std::string srcProgName = argv[2];

    // z3 variable declaration
    try {
        std::cout << "Extracting list of variables...\n\n";
        std::cout << "Declaring z3 variables corresponding to :\n";

        getVariablesList(inputList, inputFolder+"/input.txt");
        std::cout << "...input variables...\n";
        declareVariables(inputList, c, varVector_0, varMap_0, varVector_1, varMap_1);

        getVariablesList(randomList, inputFolder+"/random.txt");
        std::cout << "...random variables...\n";
        declareVariables(randomList, c, varVector_0, varMap_0, varVector_1, varMap_1);

        getVariablesList(intermList, inputFolder+"/intermediate.txt");
        std::cout << "...intermediate variables...\n";
        declareVariables(intermList, c, varVector_0, varMap_0, varVector_1, varMap_1);

        getVariablesList(secretList, inputFolder+"/secret.txt");
        std::cout << "...secret variables...\n";
        declareVariables(secretList, c, varVector_0, varMap_0, varVector_1, varMap_1);


        getVariablesList(secretMaskList, inputFolder+"/secretmask.txt");
        std::cout << "...secret key masking variables...\n";
        declareVariables(secretMaskList, c, varVector_0, varMap_0, varVector_1, varMap_1);

        std::cout << "...bv constants 0-255...\n\n";
        declareConstants(c, varVector_0, varMap_0, varVector_1, varMap_1);


    } catch(const std::exception& e) {
        std::cout << "Variable type or declaration error.\n";
        return EXIT_FAILURE;
    }

    // z3 solver and stmt assertions
    std::cout << "Adding assertions :\n";
    std::cout << "...initializing solver...\n";
    z3::solver s(c);
    std::cout << "...asserting input variables equal for _0,_1...\n";
    for(auto v:inputList) {
        std::string varName;
        std::vector<int> varType = getVariableType(v, varName);
        if(varType[0]==1) {
            s.add(varVector_0[varMap_0[varName+"_0"]] == varVector_1[varMap_1[varName+"_1"]]);
        } else {
            for(int pos=0; pos<256; pos++) {
                s.add((z3::select(varVector_0[varMap_0[varName+"_0"]],pos)) == (z3::select(varVector_1[varMap_1[varName+"_1"]],pos)));
            }
        }
    }
    std:: cout << "...reading statements...\n";
    addAssertions(inputFolder+"/"+srcProgName, varVector_0, varMap_0, varVector_1, varMap_1, s, c);
    std::cout << "...added all stmt assertions for bv_0 & bv_1\n";
    std::cout << std::endl;
    

    // random variable dependence
    std::cout << "Checking dependence on random variables : \n";
    varListType nonDepList = checkRandomDependence(randomList, secretList, secretMaskList, intermList, varVector_0, varMap_0, varVector_1, varMap_1, s, c);

    // final secret leakage check
    std::cout << "\nChecking secret variable leakage : \n";
    std::set<std::string> leakList = checkSecretLeakage(nonDepList, randomList, secretList, secretMaskList, varVector_0, varMap_0, varVector_1, varMap_1, s, c);

    std::cout << "\nTotal time elapsed : " << (clock()-tStart)/CLOCKS_PER_SEC << " seconds.\n";

    return 0;
}