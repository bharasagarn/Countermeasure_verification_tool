How to execute:
$ g++ -o main main.cc getVariablesList.cc -lz3
$ ./main

Structure & workflow:
{list.txt} -> [getVariablesList] -> {varList} -> [declareVariables] -> {varVector, varMap}

{prog.cc, varVector, varMap} -> [getAssertions] -> {solver (with added assertions)}

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