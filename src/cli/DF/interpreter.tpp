#include "interpreter.hpp"

namespace DF
{

template <class T>
void DFInterpreter<T>::df_ptr_inc(BF::Interpreter<T> *inter)
{
    DFInterpreter<T> *df_inter = static_cast<DFInterpreter<T> *>(inter);
    df_inter->mem2->inc_ptr();
}

template <class T>
void DFInterpreter<T>::df_ptr_dec(BF::Interpreter<T> *inter)
{
    DFInterpreter<T> *df_inter = static_cast<DFInterpreter<T> *>(inter);
    df_inter->mem2->dec_ptr();
}

template <class T>
void DFInterpreter<T>::df_mem_inc(BF::Interpreter<T> *inter)
{
    DFInterpreter<T> *df_inter = static_cast<DFInterpreter<T> *>(inter);
    df_inter->mem2->inc_val();
}

template <class T>
void DFInterpreter<T>::df_mem_dec(BF::Interpreter<T> *inter)
{
    DFInterpreter<T> *df_inter = static_cast<DFInterpreter<T> *>(inter);
    df_inter->mem2->dec_val();
}

template <class T>
void DFInterpreter<T>::df_write(BF::Interpreter<T> *inter)
{
    DFInterpreter<T> *df_inter = static_cast<DFInterpreter<T> *>(inter);

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;

    df_inter->os << conv.to_bytes(df_inter->mem2->ref());
}

template <class T>
void DFInterpreter<T>::df_read(BF::Interpreter<T> *inter)
{
    DFInterpreter<T> *df_inter = static_cast<DFInterpreter<T> *>(inter);

    int c = df_inter->is.get();
    if (df_inter->is.eof())
    {
        c = 0;
    }
    else if (df_inter->is.fail())
    {
        throw std::runtime_error(BF::strings::prog_read_input_error);
    }
    df_inter->mem2->ref() = c;
}

template <class T>
void DFInterpreter<T>::df_left_cond(BF::Interpreter<T> *inter)
{
    DFInterpreter<T> *df_inter = static_cast<DFInterpreter<T> *>(inter);

    if (df_inter->mem2->ref() != 0)
    {
        utils::Program::container::const_iterator tmp = df_inter->inst_ptr;
        do
        {
            df_inter->inst_ptr = tmp + 1;

            df_inter->run_();
        } while (df_inter->mem2->ref() != 0);

        --df_inter->inst_ptr;
    }
    else
    {
        int stop_after = 1;
        do
        {
            ++df_inter->inst_ptr;
            if (*df_inter->inst_ptr == '}')
                --stop_after;
            else if (*df_inter->inst_ptr == '{')
                ++stop_after;

        } while (stop_after != 0);
    }
}

template <class T>
DFInterpreter<T>::DFInterpreter(std::ostream &os, std::istream &is,
                                BF::MemoryBase<T> *mem, BF::MemoryBase<T> *mem2)
    : BF::Interpreter<T>(os, is, mem), mem2(mem2)
{
    this->register_handler('v', &df_ptr_inc);
    this->register_handler('^', &df_ptr_dec);
    this->register_handler('/', &df_mem_inc);
    this->register_handler('\\', &df_mem_dec);
    this->register_handler(':', &df_write);
    this->register_handler(';', &df_read);
    this->register_handler('{', &df_left_cond);

    this->register_handler('}', &BF::Interpreter<T>::right_square_bracket_);
}

} // namespace DF
