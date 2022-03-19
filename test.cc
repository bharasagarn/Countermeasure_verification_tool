#include "getVariablesList.hh"

int main() {
    std::vector<std::pair<std::string,std::string>> varList;
    getVariables(varList, "input.txt");

    return 0;
}