#include "unitTest.h"
using namespace std;

void example1();
void example2();

int main() {
    example1();
    example2();
}

// This function is the example to be test
// This function is wrong
double mult(float x, float y) {
    return x + x * y;
}

/**
 * This is example for function with argument and return value, and no IO operation.
 */
void example1(void) {
    // TestSuite<Return Type, Argument Type>
    // The first argument "mult" is function to be tested
    // The second argument "Test Cases for mult" is the name of this test suite
    TestSuite<double, float, float> multTest{mult, "Example Suite"};

    // This is an example with only Args and Return value set
    //     and this test does not have name and comment
    TestCase<double, float, float> test1;
    test1.setArguments({0.0, 0.0});
    test1.setReturnValue(0.0);
   
    // This is an example that name and comment are set
    TestCase<double, float, float> test2;
    test2.setArguments({1.0, 1.0});
    test2.setReturnValue(1.0);
    test2.setNameComment("simple test 1", "1 * 1 = 1 by math");

    // This is an example that name and comment are set, but result is hidden.
    TestCase<double, float, float> test3;
    test3.setArguments({1.0, 1.0});
    test3.setReturnValue(1.0);
    test3.setNameComment("simple test 2", "1 * 1 = 1 by math");
    test3.hideResult();

    // Add these tests to suite
    multTest.registerNewCase(test1);
    multTest.registerNewCase(test2);
    multTest.registerNewCase(test3);
    
    // Run it!
    multTest.runSuite();
}


// This function is the example to be test
// This function is wrong
void mult_io(void) {
    float x, y;
    cin >> x >> y;
    cout << x + x * y;
}

/**
 * This is example for function with argument and return value, and no IO operation.
 */
void example2(void) {
    // TestSuite<Return Type, Argument Type>
    // The first argument "mult" is function to be tested
    // The second argument "Test Cases for mult" is the name of this test suite
    TestSuite<void> multTest{mult_io, "Example Suite with IO"};

    // This is an example with only Input and Output
    //     and this test does not have name and comment
    TestCase<void> test1;
    test1.setIO("0.0 0.0", "0");
   
    // Add to suite
    multTest.registerNewCase(test1);
    
    // Run it!
    multTest.runSuite();
}





