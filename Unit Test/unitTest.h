#ifndef __UNITTEST_H__
#define __UNITTEST_H__
#include <functional>
#include <iostream>
#include <sstream>
#include <tuple>
#include <any>
#include <vector>

// Forward declaration
template <class Ret, class... Args>
struct SingleTest;

/**
 * Storage a function and arbitrary number of test cases
 *
 * @tparam Ret: Type of return value of function to be tested.
 * @tparam Args: Type of arguments of the function to be tested
 * 
 * @param functionName: The name of the function to be tested.
 * @param func: Function to be tested
 * @param tests: vector of Single Test that will be used.
 * 
 */
template <class Ret, class... Args>
class UnitTest {
    std::string functionName;
    std::function<Ret(Args...)> func;
    std::vector<SingleTest<Ret, Args...>> tests;
    public:
    /**
     * Constructor
     * @param func: Function to be tested
     * @param name: The name of function to be tested
     */
    UnitTest(const std::function<Ret(Args...)>& func, std::string name);

    /**
     * Run all tests and show the result
     */
    void runSuite();

    /**
     * Add a new test for these test suit
     * @param test: New test that will be added to this test suit
     */
    void registerNewCase(const SingleTest<Ret, Args...>& test);
};

/**
 * Storage a function and arbitrary number of test cases
 *
 * @tparam Ret: Type of return value of function to be tested.
 * @tparam Args: Type of arguments of the function to be tested
 * 
 * @param arguments: The arguments to be used in this test (if applicable)
 * @param rev: The expected retuan value to be used in this test (if applicable)
 * @param actualRev: The actual return value produced by tested function (User should not change it)
 * @param input: The input from stdin to be used in this test (if applicable)
 * @param output: The output to stdout to be used in this test (if applicable)
 * @param actualOutput: The actual output produced produced by tested function (User should not change it)
 * @param name: The name of this test
 * @param comment: Comment of this test, it will show to user when this test is failed.
 * @param hiddenResult: if this is set to true, when test is failed, user won't get any information.
 * 
 */
template <class Ret, class... Args>
struct SingleTest {

    std::tuple<Args...> arguments;
    std::any rev, actualRev;
    std::string input, output, actualOutput, name, comment;
    bool hiddenResult = false;

    void resultDetail() const;
    bool check(const std::function<Ret(Args...)>& func);

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
};




template <class Ret, class... Args>
UnitTest<Ret, Args...>::UnitTest(const std::function<Ret(Args...)>& func, std::string name): func{func}, functionName{name} {}

template <class Ret, class... Args>
void UnitTest<Ret, Args...>::runSuite() {
    using std::cerr;
    using std::endl;
    std::cerr << "Testing " << functionName <<std::endl;
    for (int i = 0; i < tests.size(); i++) {
        std::cerr << "Test " << tests[i].name? tests[i].name + " " : "";
        if (tests[i].check(func)) {
            cerr << "Passed (" << i+1 << "/" << tests.size()<< ")"<<endl;
        } else {
            cerr << "Failed (" << i+1 << "/" << tests.size()<< ")"<<endl;
            tests[i].resultDetail();
            cerr << endl;
            // return;
        } 
    }
}

template <class Ret, class... Args>
void UnitTest<Ret, Args...>::registerNewCase(const SingleTest<Ret, Args...>& test) {
    tests.push_back(test);
}


template <class Ret, class... Args>
void SingleTest<Ret, Args...>::resultDetail() const {
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

template <class Ret, class... Args>
bool SingleTest<Ret, Args...>::check(const std::function<Ret(Args...)>& func) {
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
#endif
