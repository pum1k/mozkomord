#ifndef CLI_DF_INTERPRETER_HPP_
#define CLI_DF_INTERPRETER_HPP_

#include "../BF/interpreter.hpp"

namespace DF
{

template <class T>
class DFInterpreter : public BF::Interpreter<T>
{
 protected:
    BF::MemoryBase<T> *mem2;

    static void df_ptr_inc(BF::Interpreter<T> *inter);
    static void df_ptr_dec(BF::Interpreter<T> *inter);
    static void df_mem_inc(BF::Interpreter<T> *inter);
    static void df_mem_dec(BF::Interpreter<T> *inter);
    static void df_write(BF::Interpreter<T> *inter);
    static void df_read(BF::Interpreter<T> *inter);
    static void df_left_cond(BF::Interpreter<T> *inter);

 public:
    DFInterpreter(std::ostream &os, std::istream &is, BF::MemoryBase<T> *mem,
                  BF::MemoryBase<T> *mem2);
};

} // namespace DF

#include "interpreter.tpp"

#endif // CLI_DF_INTERPRETER_HPP_
