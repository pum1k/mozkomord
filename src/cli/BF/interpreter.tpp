#include "interpreter.hpp"

namespace BF
{
template <class T>
void Interpreter<T>::run_()
{
    const auto end = this->prog_end;
    for (; this->inst_ptr < end; ++inst_ptr)
    {
        (this->function_table[(unsigned)(*this->inst_ptr)])(this);
    }
}

template <class T>
void Interpreter<T>::plus_sign_(BF::Interpreter<T> *inter)
{
    inter->mem->inc_val();
}

template <class T>
void Interpreter<T>::minus_sign_(BF::Interpreter<T> *inter)
{
    inter->mem->dec_val();
}

template <class T>
void Interpreter<T>::dot_(BF::Interpreter<T> *inter)
{
    inter->os << inter->mem->ref();
}

template <>
inline void Interpreter<char>::comma_(BF::Interpreter<char> *inter)
{
    inter->is.get(inter->mem->ref());
}
// TODO: Add specializations for other types

template <class T>
void Interpreter<T>::less_than_sign_(BF::Interpreter<T> *inter)
{
    inter->mem->dec_ptr();
}

template <class T>
void Interpreter<T>::greater_than_sign_(BF::Interpreter<T> *inter)
{
    inter->mem->inc_ptr();
}

template <class T>
void Interpreter<T>::left_square_bracket_(BF::Interpreter<T> *inter)
{
    if (inter->mem->ref() != 0)
    {
        std::vector<char>::const_iterator tmp = inter->inst_ptr;
        do
        {
            inter->inst_ptr = tmp + 1;
            try
            {
                inter->run_();
            }
            catch (const return_one_level &)
            {
            }

        } while (inter->mem->ref() != 0);
    }
    else
    {
        int stop_after = 1;
        do
        {
            ++inter->inst_ptr;
            if (*inter->inst_ptr == ']')
                --stop_after;
            else if (*inter->inst_ptr == '[')
                ++stop_after;

        } while (stop_after != 0);
    }
}

template <class T>
void Interpreter<T>::right_square_bracket_(BF::Interpreter<T> *inter)
{
    throw return_one_level{};
}

template <class T>
Interpreter<T>::Interpreter(std::ostream &os, std::istream &is,
                            MemoryBase<T> *mem)
    : InterpreterBase(os, is), mem(mem), function_table_size(256)
{
    this->function_table = new fptr[this->function_table_size];
    for (int i = 0; i < this->function_table_size; i++)
    {
        this->function_table[i] = &Interpreter<T>::noop_;
    }

    this->function_table[(unsigned)'+'] = &Interpreter<T>::plus_sign_;
    this->function_table[(unsigned)'-'] = &Interpreter<T>::minus_sign_;
    this->function_table[(unsigned)'.'] = &Interpreter<T>::dot_;
    this->function_table[(unsigned)','] = &Interpreter<T>::comma_;
    this->function_table[(unsigned)'<'] = &Interpreter<T>::less_than_sign_;
    this->function_table[(unsigned)'>'] = &Interpreter<T>::greater_than_sign_;
    this->function_table[(unsigned)'['] = &Interpreter<T>::left_square_bracket_;
    this->function_table[(unsigned)']'] =
        &Interpreter<T>::right_square_bracket_;
}

template <class T>
Interpreter<T>::~Interpreter()
{
    delete[] this->function_table;

    if (this->mem != nullptr)
    {
        delete this->mem;
    }
}

template <class T>
bool Interpreter<T>::run(const std::vector<char> &prog)
{
    if (this->mem == nullptr)
    {
        throw std::runtime_error("Memory pointer cannot be nullptr.");
    }

    this->inst_ptr = prog.begin();
    this->prog_end = prog.end();

    this->run_();

    return this->inst_ptr == this->prog_end;
}

template <class T>
bool Interpreter<T>::run(const Preprocessor &prep)
{
    return this->run(prep.get_program());
}

template <class T>
void Interpreter<T>::reset_memory()
{
    this->mem->reset();
}

} // namespace BF
