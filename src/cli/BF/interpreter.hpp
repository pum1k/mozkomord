#ifndef CLI_BF_INTERPRETER_HPP_
#define CLI_BF_INTERPRETER_HPP_

#include <algorithm>
#include <codecvt>
#include <fstream>
#include <iomanip>
#include <istream>
#include <ostream>
#include <sstream>

#include "../../common/debugprint.hpp"
#include "../language.hpp"
#include "memory.hpp"
#include "preprocessor.hpp"
#include "program.hpp"

namespace BF
{
/**
 * This class defines interface for interpreter classes.
 */
class InterpreterBase
{
 protected:
    // streams for I/O operations performed by the interpreted program
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
    using fptr = void (*)(BF::Interpreter<T> *);

    // pointer to memory used by the interpreted program
    // if it is not equal to nullptr it gets deleted by destructor
    MemoryBase<T> *mem;

    // lookup table of functions, that should be executed based the interpreted
    // program
    const int function_table_size;
    fptr *function_table;

    // iterators pointing to current instruction and end of  program
    Program::container::const_iterator inst_ptr;
    Program::container::const_iterator prog_end;

    // this function reads the program and calls functions from function_table
    void run_();

    // when set to true, one level of `run_` recursion will return and this will
    // be set back to false
    bool return_one_level;

    // set function to the specified index of function_table
    void register_handler(unsigned char command, fptr function);

    // does nothing, when character does not have any meaning, it calls this
    static void noop_(BF::Interpreter<T> * /* inter */) {}

    // functions implementing logic of individual commands in the interpreted
    // program
    static void plus_sign_(BF::Interpreter<T> *inter);
    static void minus_sign_(BF::Interpreter<T> *inter);
    static void dot_(BF::Interpreter<T> *inter);
    static void comma_(BF::Interpreter<T> *inter);
    static void less_than_sign_(BF::Interpreter<T> *inter);
    static void greater_than_sign_(BF::Interpreter<T> *inter);
    static void left_square_bracket_(BF::Interpreter<T> *inter);
    static void right_square_bracket_(BF::Interpreter<T> *inter);

 public:
    Interpreter(std::ostream &os, std::istream &is, MemoryBase<T> *mem);
    virtual ~Interpreter() override;

    virtual bool run(const Program::container &prog) override;
    virtual bool run(const BF::Program &prog) override;
    virtual void reset_memory() override;
};

template <class T>
class DebugInterpreter : public Interpreter<T>
{
 protected:
    MemDbgrBase<T> *mem_dbgr;

    // these streams are used to redirect I/O operations of the interpreted
    // program
    // using these, both input and output of the program can be easily read
    // multiple times
    std::ostringstream fake_os;
    std::istringstream fake_is;
    // stream through which the interpreter sends all output and recieves all
    // input
    std::ostream &real_os;
    std::istream &real_is;

    // this function is called when program should be debugged at current
    // instruction
    // it contains a loop comunicating with the user and executing it commands
    // based on user input
    void debug_here();

    // iterator pointing to the beginning of the program
    Program::container::const_iterator prog_begin;

    // functions handling debugging commands
    void print_help();
    void print_context(int size);
    void print_memory();
    void dump_memory();
    void print_all_input();
    void print_all_output();

    // these functions override the default dot and comma characters handlers
    // these are wrappers around the basic ones - they handle sending data
    // between real and fake streams
    static void debug_dot_(BF::Interpreter<T> *inter);
    static void debug_comma_(BF::Interpreter<T> *inter);
    // starts debugging at current instruction
    static void vertical_line_(BF::Interpreter<T> *inter);

 public:
    DebugInterpreter(std::ostream &os, std::istream &is, MemoryBase<T> *mem,
                     MemDbgrBase<T> *mem_dbgr);
    virtual ~DebugInterpreter() override;

    virtual bool run(const Program::container &prog) override;
};

template <class T>
class OptimizedInterpreter : public Interpreter<T>
{
 protected:
    static void multi_value_inc_(BF::Interpreter<T> *inter);
    static void multi_value_dec_(BF::Interpreter<T> *inter);
    static void multi_memptr_inc_(BF::Interpreter<T> *inter);
    static void multi_memptr_dec_(BF::Interpreter<T> *inter);

 public:
    OptimizedInterpreter(std::ostream &os, std::istream &is,
                         MemoryBase<T> *mem);
};

enum class InterClass
{
    none,
    standard,
    standard_debug,
};

class InterFactory
{
 private:
    InterClass inter_class;
    MemFactory *mem_factory;

    template <class T>
    InterpreterBase *new_inter_standard(std::ostream &os, std::istream &is);
    template <class T>
    InterpreterBase *new_inter_standard_debug(std::ostream &os,
                                              std::istream &is);

 public:
    InterFactory();

    void set_inter_class(InterClass value);
    void set_mem_factory(MemFactory *value);

    template <class T>
    InterpreterBase *new_inter(std::ostream &os, std::istream &is);
};

} // namespace BF

#include "interpreter.tpp"

#endif // CLI_BF_INTERPRETER_HPP_
