#include "unitTest.h"
using namespace std;

void email(string domain) {
    string username;
    cin >> username;
    cout << username << '@' << domain;
}

int fib(int n) {
    if (n == 1 or n == 2) {
        return 1;
    } else {
        return fib(n-1) + fib(n-2);
    }
}

// int foo(void) {
//     return 1;
// }


int main() {
    UnitTest<void, string> testEmail{email, "email"};
    SingleTest<void, string> test1;
    test1.arguments = {"gmail.com"};
    test1.input = "Francis";
    test1.output = "Francis@gmail.com";
    test1.name = "gmail";
    
    SingleTest<void, string> test2;
    test2.arguments =  {"hotmail.com"};
    test2.input = "Franci";
    test2.output = "Francis@hotmail.com";
    test2.name = "hotmail";
    test2.comment = "this is a boring comment";
    test2.hiddenResult = true;
    testEmail.registerNewCase(test1);
    testEmail.registerNewCase(test2);
    testEmail.runSuite();

    UnitTest<int, int> testFib{fib, "fib"};
    SingleTest<int, int> test3;
    test3.name = "base";
    test3.arguments = {7};
    test3.rev = 3;
    testFib.registerNewCase(test3);
    testFib.runSuite();
    
}
