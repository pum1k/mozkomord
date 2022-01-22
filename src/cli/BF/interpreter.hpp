#ifndef CLI_BF_INTERPRETER_HPP_
#define CLI_BF_INTERPRETER_HPP_

#include <algorithm>
#include <istream>
#include <ostream>

#include "memory.hpp"
#include "preprocessor.hpp"
#include "program.hpp"

namespace BF
{
class InterpreterBase
{
 protected:
    std::ostream &os;
    std::istream &is;

 public:
    InterpreterBase(std::ostream &os, std::istream &is) : os(os), is(is){};

    virtual ~InterpreterBase(){};
    InterpreterBase(const InterpreterBase &) = delete;
    InterpreterBase(InterpreterBase &&)      = delete;
    InterpreterBase &operator=(const InterpreterBase &) = delete;
    InterpreterBase &operator=(InterpreterBase &&) = delete;

    virtual bool run(const Program::container &prog) = 0;
    virtual bool run(const BF::Program &prog)        = 0;
    virtual void reset_memory()                      = 0;
};

template <class T>
class Interpreter : public InterpreterBase
{
 protected:
    MemoryBase<T> *mem;

    const int function_table_size;

    using fptr = void (*)(BF::Interpreter<T> *);
    fptr *function_table;
    Program::container::const_iterator inst_ptr;
    Program::container::const_iterator prog_end;

    void run_();

    struct return_one_level
    {
    };

    static void noop_(BF::Interpreter<T> * /* inter */) {}
    static void plus_sign_(BF::Interpreter<T> *inter);
    static void minus_sign_(BF::Interpreter<T> *inter);
    static void dot_(BF::Interpreter<T> *inter);
    static void comma_(BF::Interpreter<T> *inter);
    static void less_than_sign_(BF::Interpreter<T> *inter);
    static void greater_than_sign_(BF::Interpreter<T> *inter);
    static void left_square_bracket_(BF::Interpreter<T> *inter);
    static void right_square_bracket_(BF::Interpreter<T> * /* inter */);

 public:
    Interpreter(std::ostream &os, std::istream &is, MemoryBase<T> *mem);
    virtual ~Interpreter();

    virtual bool run(const Program::container &prog) override;
    virtual bool run(const BF::Program &prog) override;
    virtual void reset_memory() override;
};

} // namespace BF

#include "interpreter.tpp"

#endif // CLI_BF_INTERPRETER_HPP_
