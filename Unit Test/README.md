Compile with \
    g++ -std=c++17 example.cc\

How to use this tool?
1. Declare Test Suite by \
    TestSuite<Return Type, Argument List Type> variableName{function, SuiteName} \
2. Declare a Test Case by \
    TestCase<Return Type, Argument List Type> testCase; \
3. Modify the Test Case \
    testCase.setArguments({ArgList}); Set Arguments\
    testCase.setReturnValue(RetValue); Set Return Value\
    testCase.setIO(input, output); Set Input and Output\
    testCase.setNameComment(name, comment); Set name and comment\
    testCase.hideResult(); Hide the result if this test is failed\

For function return void, don't use setReturnValue\
For function with no argument, just declare as TestSuite<Return Type>\