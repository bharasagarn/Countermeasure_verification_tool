How to execute:
$ g++ -o main main.cc getVariablesList.cc -lz3

Structure & workflow:
{list.txt} -> [getVariablesList] -> {varList} -> [declareVariables] -> {varVector, varMap, arrayVector, arrayMap}

{prog.cc, varVector, arrayVector, varMap, arrayMap} -> [getAssertions] -> {solver (with added assertions)}

- getVariables.cc
  - getVariablesList
  - getArrayDetails
  - getVariableType
  - declareVariables

- getAssertions.cc
  - processArithmetic
  - getArrayElement
  - getStatementType
  - processLoop
  - addAssertion?

To-Do:
- Finish processArithmetic function [getAssertions.cc]
- Finish getArrayElement function [getAssertions.cc]
- Babysteps to addAssertion function [getAssertions.cc]