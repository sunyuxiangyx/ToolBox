#include "unitTest.h"
#include <vector>
using namespace std;

int foo(string domain) {
    string username;
    cin >> username;
    cout << username << '@' << domain << endl;
    return username.length() + domain.length() + 1;
}

int main() {
    vector<UnitTest<int, string>> tests;
    tests.push_back({foo, {"gmail.com"}, 4});
    tests.back().input = "Francis";
    tests.back().output = "Francis@gmail.com\n";
    tests.back().rev = 17;
    tests.back().name = "gmail test";

    tests.push_back({foo, {"hotmail.com"}, 4});
    tests.back().input = "Francis";
    tests.back().output = "Francis@hotmail.com\n";
    tests.back().rev = 19;
    tests.back().name = "hotmail test";
    for (const auto& test: tests) {
        test.check();
    }
}
