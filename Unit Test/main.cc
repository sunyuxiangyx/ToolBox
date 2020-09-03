#include <functional>
#include <iostream>
#include <sstream>
#include <tuple>
#include <algorithm>
using namespace std;

#include <functional>
#include <iostream>
template <class Ret, class... Args>
class UnitTest {
    public:
    function<Ret(Args...)> func;
    tuple<Args...> arguments;
    Ret rev;
    string input;
    string output;
    string name;
    string comment;
    UnitTest(function<Ret(Args...)> func, tuple<Args...> args, Ret rev): func{func}, arguments{args}, rev{rev} {} 
    bool check() const {

        istringstream iss{input};
        streambuf* cinbuf = std::cin.rdbuf();
        cin.rdbuf(iss.rdbuf());

        ostringstream oss;
        streambuf* coutbuf = std::cout.rdbuf();
        cout.rdbuf(oss.rdbuf());

        Ret actullyRev = apply(func, arguments);

        std::cin.rdbuf(cinbuf);
        std::cout.rdbuf(coutbuf);


        if (rev == actullyRev and oss.str() == output) {
            cerr << "Success: " << name << endl;
            return true;
        } else {
            cerr << "Fail: " << name << endl;
            cerr << "Arguments:";
            apply([](const auto&... args) {((cout << " "<< args), ...);}, arguments);
            cerr << endl;
            cerr << "Input: " << input << endl;
            cerr << "Expected Output: " << output << endl;
            cerr << "Actual Output: " << oss.str() << endl;
            cerr << "Comment: " << comment << endl;
            return false;
        }
    }
};

int foo(int b, bool c, string d) {
    int x;
    cin >> x;
    cout << x * x;
    if (c) {
        cerr << d << endl;
    } else {
        return b;
    }
    return d.length();
}

int main() {
    UnitTest<int, int, bool, string> f{foo, {4, false, "str"}, 4};
    f.name = "Basic Test";
    f.comment = "failure comment";
    f.output = "16";
    f.check();
}