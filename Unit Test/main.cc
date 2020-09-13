#include "unitTest.h"
using namespace std;

void email(string domain) {
    string username;
    cin >> username;
    cout << username << '@' << domain;
}
int sqr(int x) {
    return x * x;
}

void sqr_io() {
    int x;
    cin >> x;
    cout << x * x;
}

int main() {
    UnitTest<int, int> sqrTest{sqr, "sqr"};

    SingleTest<int, int> test1;
    test1.arguments = {1};
    test1.rev = 1;
    test1.name = "base test";
    test1.comment = "1 * 1 = 1";

    SingleTest<int, int> test2;
    test2.arguments = {2};
    test2.rev = 4;
    test2.comment = "2 * 2 = 4";

    SingleTest<int, int> test3;
    test3.arguments = {4};
    test3.rev = 16;
    test3.comment = "4 * 4 = 16";

    SingleTest<int, int> test4;
    test4.arguments = {5};
    test4.rev = 24;
    test4.name = "mistake test";
    test4.comment = "This is a mistake test, and this test has a name";

    SingleTest<int, int> test5;
    test5.arguments = {5};
    test5.rev = 24;
    test5.name = "mistake test and result is hidden ";
    test5.comment = "This is a mistake test and result is hidden";
    test5.hiddenResult = true;

    sqrTest.registerNewCase(test1);
    sqrTest.registerNewCase(test2);
    sqrTest.registerNewCase(test3);
    sqrTest.registerNewCase(test4);
    sqrTest.registerNewCase(test5);
    sqrTest.runSuite();


    UnitTest<void> sqrIOTest{sqr_io, "sqr_io"};

    SingleTest<void> test11;
    test11.input = "1";
    test11.output = "1";
    test11.name = "base test";
    test11.comment = "1 * 1 = 1";

    SingleTest<void> test12;
    test12.input = "2";
    test12.output = "4";
    test12.comment = "2 * 2 = 4";

    SingleTest<void> test13;
    test13.input = "4";
    test13.output = "4";
    test13.name = "wrong test";
    test13.comment = "Wrong test and comment won't display";
    test13.hiddenResult = true;

    sqrIOTest.registerNewCase(test11);
    sqrIOTest.registerNewCase(test12);
    sqrIOTest.registerNewCase(test13);
    sqrIOTest.runSuite();

}
