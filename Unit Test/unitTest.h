#ifndef __UNITTEST_H__
#define __UNITTEST_H__
#include <functional>
#include <iostream>
#include <sstream>
#include <tuple>

struct RedirectIOStream {
    std::ios& stream;
    std::streambuf* buf;
    std::stringstream ss;
    RedirectIOStream(std::ios& stream, std::string s=""): stream{stream}, buf{stream.rdbuf()}, ss{s} {
        stream.rdbuf(ss.rdbuf());
    }
    void reset() {
        stream.rdbuf(buf);
    }
    ~RedirectIOStream() {
        reset();
    }
};

template <class Ret, class... Args>
struct UnitTest {
    std::function<Ret(Args...)> func;
    std::tuple<Args...> arguments;
    Ret rev;
    std::string input;
    std::string output;
    std::string name;
    std::string comment;
    bool hiddenResult = false;
    UnitTest(std::function<Ret(Args...)> func, std::tuple<Args...> args, Ret rev): func{func}, arguments{args}, rev{rev} {} 
    bool check() const {
        using std::cerr;
        using std::endl;
        RedirectIOStream in{std::cin, input};
        RedirectIOStream out{std::cout};
    
        Ret actullyRev = apply(func, arguments);

        in.reset();
        out.reset();

        if (rev == actullyRev and out.ss.str() == output) {
            cerr << "Success: " << name << endl;
            return true;
        } else {
            cerr << "Fail: " << name << endl;
            if (!hiddenResult) {
                cerr << "Arguments:";
                apply([](const auto&... args) {((cerr << " "<< args), ...);}, arguments);
                cerr << endl;
                cerr << "Input: " << input << endl;
                cerr << "Expected Return : " << rev << endl;
                cerr << "Actual Return: " << actullyRev << endl;
                cerr << "Expected Output: " << output << endl;
                cerr << "Actual Output: " << out.ss.str() << endl;
            }
            cerr << "Comment: " << comment << endl;
            return false;
        }
    }
};


#endif
