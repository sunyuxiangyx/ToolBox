#ifndef __UNITTEST_H__
#define __UNITTEST_H__
#include <functional>
#include <iostream>
#include <sstream>
#include <tuple>
#include <any>
#include <vector>

template <class Ret, class... Args>
struct SingleTest;

template <class Ret, class... Args>
class UnitTest {
    std::string functionName;
    std::function<Ret(Args...)> func;
    std::vector<SingleTest<Ret, Args...>> tests;
    public:
    UnitTest(const std::function<Ret(Args...)>& func, std::string name): func{func}, functionName{name} {}
    void runSuite() {
        using std::cerr;
        using std::endl;
        std::cerr << "Testing " << functionName <<std::endl;
        for (int i = 0; i < tests.size(); i++) {
            std::cerr << "Test " << tests[i].name;
            if (tests[i].check(func)) {
                cerr << " Passed (" << i+1 << "/" << tests.size()<< ")"<<endl;
            } else {
                cerr << " Failed (" << i+1 << "/" << tests.size()<< ")"<<endl;
                tests[i].resultDetail();
                cerr << endl;
                return;
            } 
        }
    }
    void registerNewCase(const SingleTest<Ret, Args...>& test) {
        tests.push_back(test);
    }
};

template <class Ret, class... Args>
struct SingleTest {
    struct RedirectIOStream {
        std::ios& stream;
        std::streambuf* buf;
        std::stringstream ss;
        RedirectIOStream(std::ios& stream, std::string s=""): stream{stream}, buf{stream.rdbuf()}, ss{s} {
            stream.rdbuf(ss.rdbuf());
        }
        void reset() { stream.rdbuf(buf); }
        ~RedirectIOStream() { reset(); }
    };

    std::tuple<Args...> arguments;
    std::any rev, actualRev;
    std::string input, output, actualOutput, name, comment;
    bool hiddenResult = false;

    void resultDetail() const {
        if (hiddenResult) {
            return;
        }

        using std::cerr, std::endl;
        if constexpr (std::tuple_size<decltype(arguments)>::value) {
            cerr << "Arguments:";
            apply([](const auto&... args) {((cerr << " "<< args), ...);}, arguments);
            cerr << endl;
        }
        
        if (input != "") {
            cerr << "Input: " << input << endl;
        }
        
        if constexpr(!std::is_void<Ret>::value) {
            cerr << "Expected Return : " << std::any_cast<Ret>(rev) << endl;
            cerr << "Actual Return: " << std::any_cast<Ret>(actualRev) << endl;
        }

        if (output != "" or actualOutput != "") {
            cerr << "Expected Output: " << output << endl;
            cerr << "Actual Output: " << actualOutput << endl;
        }

        if (comment != "") {
            cerr << "Comment: " << comment << endl;
        }
    }

    bool check(const std::function<Ret(Args...)>& func) {
        using std::cerr, std::endl;
        RedirectIOStream in{std::cin, input};
        RedirectIOStream out{std::cout};

        if constexpr(std::is_void<Ret>::value) {
            apply(func, arguments);
            actualOutput = out.ss.str();
            return actualOutput == output;
        } else {
            actualRev = apply(func, arguments);
            actualOutput = out.ss.str();
            return actualOutput == output and std::any_cast<Ret>(rev) == std::any_cast<Ret>(actualRev);
        }
    }
};

#endif
