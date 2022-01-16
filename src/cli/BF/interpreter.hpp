#ifndef CLI_BF_INTERPRETER_HPP_
#define CLI_BF_INTERPRETER_HPP_

#include <algorithm>
#include <istream>
#include <ostream>
#include <vector>

#include "memory.hpp"
#include "preprocessor.hpp"

namespace BF
{
class InterpreterBase
{
 protected:
    std::ostream &os;
    std::istream &is;

 public:
    InterpreterBase();
    InterpreterBase(std::ostream &os, std::istream &is) : os(os), is(is){};

    virtual ~InterpreterBase(){};
    InterpreterBase(const InterpreterBase &) = delete;
    InterpreterBase(InterpreterBase &&)      = delete;
    InterpreterBase &operator=(const InterpreterBase &) = delete;
    InterpreterBase &operator=(InterpreterBase &&) = delete;

    virtual bool run(const std::vector<char> &prog) = 0;
    virtual bool run(const Preprocessor &prep)      = 0;
    virtual void reset_memory()                     = 0;
};

} // namespace BF

#endif // CLI_BF_INTERPRETER_HPP_
