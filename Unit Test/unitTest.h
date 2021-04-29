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
struct TestCase;

/**
 * Storage a function and arbitrary number of test cases
 *
 * @tparam Ret: Type of return value of function to be tested.
 * @tparam Args: Type of arguments of the function to be tested
 * 
 * @param suiteName: The name of this test suite.
 * @param func: Function to be tested.
 * @param tests: vector of Test Case that will be used.
 * 
 */
template <class Ret, class... Args>
class TestSuite {
    std::string suiteName;
    std::function<Ret(Args...)> func;
    std::vector<TestCase<Ret, Args...>> tests;
    public:
    /**
     * Constructor
     * @param func: Function to be tested
     * @param name: The name of function to be tested
     */
    TestSuite(const std::function<Ret(Args...)>& func, std::string name);

    /**
     * Run all tests and show the result
     */
    void runSuite();

    /**
     * Add a new test for these test suit
     * @param test: New test that will be added to this test suit
     */
    void registerNewCase(const TestCase<Ret, Args...>& test);
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
class TestCase {
    std::tuple<Args...> arguments;
    std::any rev, actualRev;
    std::string input, output, actualOutput, name, comment;
    bool hiddenResult = false;

    public:
    void hideResult();
    void setArguments(std::tuple<Args...> args);
    void setReturnValue(std::any rev);
    void setIO(std::string input, std::string output);
    void setNameComment(std::string name, std::string comment = "");
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
TestSuite<Ret, Args...>::TestSuite(const std::function<Ret(Args...)>& func, std::string name): func{func}, suiteName{name} {}

template <class Ret, class... Args>
void TestSuite<Ret, Args...>::runSuite() {
    using std::cerr;
    using std::endl;
    cerr << "Testing " << suiteName <<std::endl;
    for (int i = 0; i < tests.size(); i++) {
        if (tests[i].check(func)) {
            cerr << "Test Passed (" << i+1 << "/" << tests.size()<< ")"<<endl;
        } else {
            cerr << "---------------------------------------------------------\n";
            cerr << "Test Failed (" << i+1 << "/" << tests.size()<< ")"<<endl;
            tests[i].resultDetail();
            cerr << "---------------------------------------------------------\n\n";
            // return;
        } 
    }
}

template <class Ret, class... Args>
void TestSuite<Ret, Args...>::registerNewCase(const TestCase<Ret, Args...>& test) {
    tests.push_back(test);
}


template <class Ret, class... Args>
void TestCase<Ret, Args...>::resultDetail() const {
    using std::cerr, std::endl;

    if (name != "") {
        cerr << "Test Name: " << name << endl;
    }

    if (hiddenResult) {
        cerr << "Result is hidden" << endl;
        return;
    }

    if constexpr (std::tuple_size<decltype(arguments)>::value > 0) {
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
bool TestCase<Ret, Args...>::check(const std::function<Ret(Args...)>& func) {
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

template <class Ret, class... Args>
void TestCase<Ret, Args...>::hideResult() { hiddenResult = true; }

template <class Ret, class... Args>
void TestCase<Ret, Args...>::setArguments(std::tuple<Args...> args) { arguments = args; }

template <class Ret, class... Args>
void TestCase<Ret, Args...>::setReturnValue(std::any rev) { this -> rev = rev; }

template <class Ret, class... Args>
void TestCase<Ret, Args...>::setIO(std::string input, std::string output) { 
    this -> input = input; 
    this -> output = output;
}

template <class Ret, class... Args>
void TestCase<Ret, Args...>::setNameComment(std::string name, std::string comment) {
    this -> name = name;
    this -> comment = comment;
}

#endif
