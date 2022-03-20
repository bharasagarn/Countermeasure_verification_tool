How to execute:
```console
user@system:~$ g++ -o main main.cc getVariablesList.cc -lz3
user@system:~$ ./main
```

Program flow:
> {list.txt} -> [getVariablesList] -> {varList} -> [declareVariables] -> {varVector, varMap}

> {prog.cc, varVector, varMap} -> [getAssertions] -> {solver (with added assertions)}

Functions:
```
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
  ```

To-Do:
- [x] Design program flow and data structures
- [x] Extract variables and variable types from input list
- [x] Declare corresponding z3 variables/constants
- [ ] Finish processArithmetic function 
- [ ] Finish getArrayElement function 
- [ ] Babysteps to addAssertion function 