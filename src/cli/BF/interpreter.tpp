#include "interpreter.hpp"

namespace BF
{

// #############################################################################
// #############################################################################
// #### INTERPRETER ############################################################
// #############################################################################
// #############################################################################

template <class T>
void Interpreter<T>::run_()
{
    const auto end         = this->prog_end;
    this->return_one_level = false;

    for (; (!this->return_one_level) && (this->inst_ptr < end); ++inst_ptr)
    {
        (this->function_table[(*this->inst_ptr)])(this);
    }

    this->return_one_level = false;
}

template <class T>
void Interpreter<T>::register_handler(unsigned char command, fptr function)
{
    this->function_table[command] = function;
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
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;

    inter->os << conv.to_bytes(inter->mem->ref());
}

template <>
inline void Interpreter<uint8_t>::comma_(BF::Interpreter<uint8_t> *inter)
{
    int c = inter->is.get();
    if (inter->is.eof())
    {
        c = 0;
    }
    else if (inter->is.fail())
    {
        throw std::runtime_error(strings::prog_read_input_error);
    }
    inter->mem->ref() = c;
}

// This specialization currently works in the same way as the one for uint8_t
// Reading UTF-8 values is not currently supported because of Windows. When
// setting input codepage to UTF-8 it sends null characters instead of the UTF-8
// encoded input.
// Article about this being broken: https://nullprogram.com/blog/2021/12/30/
template <>
inline void Interpreter<uint16_t>::comma_(BF::Interpreter<uint16_t> *inter)
{
    int c = inter->is.get();
    if (inter->is.eof())
    {
        c = 0;
    }
    else if (inter->is.fail())
    {
        throw std::runtime_error(strings::prog_read_input_error);
    }
    inter->mem->ref() = c;
}

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

// recursive implementation of BF loops
// this calls `run_` for each iteration of the loop
// if the loop should not be run at all, it finds a matching closing bracket and
// moves the intruction pointer right after it
template <class T>
void Interpreter<T>::left_square_bracket_(BF::Interpreter<T> *inter)
{
    if (inter->mem->ref() != 0)
    {
        utils::Program::container::const_iterator tmp = inter->inst_ptr;
        do
        {
            inter->inst_ptr = tmp + 1;

            inter->run_();
        } while (inter->mem->ref() != 0);

        --inter->inst_ptr;
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
    inter->return_one_level = true;
}

template <class T>
Interpreter<T>::Interpreter(std::ostream &os, std::istream &is,
                            MemoryBase<T> *mem)
    : InterpreterBase(os, is),
      mem(mem),
      function_table_size(256),
      return_one_level(false)
{
    if (this->mem == nullptr)
    {
        throw std::invalid_argument(strings::mem_ptr_null_error);
    }

    this->function_table = new fptr[this->function_table_size];
    for (int i = 0; i < this->function_table_size; i++)
    {
        this->register_handler(i, &Interpreter<T>::noop_);
    }

    this->register_handler('+', &Interpreter<T>::plus_sign_);
    this->register_handler('-', &Interpreter<T>::minus_sign_);
    this->register_handler('.', &Interpreter<T>::dot_);
    this->register_handler(',', &Interpreter<T>::comma_);
    this->register_handler('<', &Interpreter<T>::less_than_sign_);
    this->register_handler('>', &Interpreter<T>::greater_than_sign_);
    this->register_handler('[', &Interpreter<T>::left_square_bracket_);
    this->register_handler(']', &Interpreter<T>::right_square_bracket_);
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
bool Interpreter<T>::run(const utils::Program::container &prog)
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
bool Interpreter<T>::run(const utils::Program &prog)
{
    return this->run(prog.get_const());
}

template <class T>
void Interpreter<T>::reset_memory()
{
    this->mem->reset();
}

// #############################################################################
// #############################################################################
// #### DEBUG INTERPRETER ######################################################
// #############################################################################
// #############################################################################

// this function handles user interaction with the debugger
// it reads comand from the user and calls functions that perform them
template <class T>
inline void DebugInterpreter<T>::debug_here()
{
    bool done = false;

    std::ostream &os = this->real_os;
    std::istream &is = this->real_is;

    if (is.eof())
    {
        os << "Warning: debugger cannot be entered because comunication "
              "stream was closed."
           << std::endl;
        return;
    }

    os << "\n################################\n"
          "Breakpoint hit.\n"
       << strings::debugger_questionmark_to_show_help << std::endl;

    char option;

    while (!done)
    {
        os << "Enter command: ";
        is >> option;
        switch (option)
        {
        case 'c':
            done = true;
            break;

        case 'm':
            this->print_memory();
            break;

        case 'f':
            this->dump_memory();
            break;

        case 'q':
            throw debugger_terminate("User requested termination.");
            break;

        case 'i':
            this->print_all_input();
            break;

        case 'o':
            this->print_all_output();
            break;

        case 'w':
            os << "######## Source ########\n";
            this->print_context(160);
            os << "########################\n";
            break;

        case '?':
            this->print_help();
            break;

        default:
            os << "unrecognised option: " << option << '\n';
            os << strings::debugger_questionmark_to_show_help << '\n';
            break;
        }
    }

    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    os << "################################\n";
}

template <class T>
void DebugInterpreter<T>::print_help()
{
    std::ostream &os = this->real_os;

    os << strings::debugger_help << std::flush;
}

template <class T>
void DebugInterpreter<T>::print_context(int size)
{
    std::ostream &os = this->real_os;

    int front           = this->inst_ptr - this->prog_begin;
    int back            = this->prog_end - this->inst_ptr;
    bool front_ellipsis = false;
    bool back_ellipsis  = false;

    if (front > size)
    {
        front          = size;
        front_ellipsis = true;
    }

    if (back > size)
    {
        back          = size;
        back_ellipsis = true;
    }

    int pos          = 0;
    bool pos_printed = false;

    if (front_ellipsis)
    {
        os << "(...)";
        pos += 5;
    }

    // print characters before the breakpoint
    for (auto it = this->inst_ptr - front; it < this->inst_ptr; ++it)
    {
        os << (char)*it;
        if ((char)*it == '\n')
        {
            pos = 0;
        }
        else
        {
            ++pos;
        }
    }

    // print the breakpoint character itself
    os << *this->inst_ptr;

    // print characters after the breakpoint character
    for (auto it = this->inst_ptr + 1; it < this->inst_ptr + back; ++it)
    {
        os << (char)*it;
        // if there are multiple lines after the breakpoint character, print the
        // message showing where the current breakpoint is after finishing the
        // line with the breakpoint
        if ((char)*it == '\n')
        {
            if (!pos_printed)
            {
                for (int i = 0; i < pos; i++)
                {
                    os << " ";
                }
                os << strings::debugger_breakpoint_pos << '\n';
                pos_printed = true;
            }
        }
    }

    if (back_ellipsis)
    {
        os << "(...)";
    }

    // if all characters after the breakpoint were on the same line, message
    // showing position of the current breakpoint was not yet printed, so do it
    // now
    if (!pos_printed)
    {
        os << '\n';
        for (int i = 0; i < pos; i++)
        {
            os << " ";
        }
        os << strings::debugger_breakpoint_pos << '\n';
        pos_printed = true;
    }
    os << '\n';
}

template <class T>
void DebugInterpreter<T>::print_memory()
{
    std::ostream &os = this->real_os;

    auto flags = os.flags();
    os << std::hex;

    os << "Memory pointer index (hex): " << this->mem_dbgr->get_current_index()
       << '\n';

    std::size_t mem_size = this->mem_dbgr->get_current_size();

    for (std::size_t i = 0; i < mem_size; i++)
    {
        // line starts with address
        if ((i % 16) == 0)
        {
            os << std::right << std::setw(8) << std::setfill('0');
            os << i;
            os << "  ";
        }

        os << std::setw(2 * sizeof(T));
        os << static_cast<uint64_t>(this->mem_dbgr->get_ref(i));

        // print newline after 16th cell, double space after 8th, otherwise
        // single space is printed
        if ((i % 16) == 15)
        {
            os << '\n';
        }
        else if ((i % 16) == 7)
        {
            os << "  ";
        }
        else
        {
            os << " ";
        }
    }

    if (!((mem_size % 16) == 0))
    {
        os << '\n';
    }
    os << std::right << std::setw(8) << std::setfill('0');
    os << mem_size << "\n";

    os << std::flush;
    os.flags(flags);
}

template <class T>
void DebugInterpreter<T>::dump_memory()
{
    std::ostream &os = this->real_os;
    std::istream &is = this->real_is;

    std::string filename;
    os << "enter file name: ";
    is >> filename;

    std::ofstream file(filename, std::ios::out | std::ios::binary);

    std::size_t mem_size = this->mem_dbgr->get_current_size();

    for (std::size_t i = 0; i < mem_size; i++)
    {
        file << this->mem_dbgr->get_ref(i);
    }
    os << "Memory saved\n";
}

template <class T>
void DebugInterpreter<T>::print_all_input()
{
    std::ostream &os = this->real_os;

    os << "Program input: \"" << this->fake_is.str() << "\"\n";
}

template <class T>
void DebugInterpreter<T>::print_all_output()
{
    std::ostream &os = this->real_os;

    os << "Program output: \"" << this->fake_os.str() << "\"\n";
}

// call standard BF dot instruction handler, then send the output produced by it
// to the real output stream stored in the class
template <class T>
void DebugInterpreter<T>::debug_dot_(BF::Interpreter<T> *inter)
{
    DebugInterpreter<T> *d_inter = static_cast<DebugInterpreter<T> *>(inter);

    Interpreter<T>::dot_(inter);
    d_inter->real_os << *(d_inter->fake_os.str().end() - 1);
}

// if there is no input waiting for the program in the input stringstream, it
// reads a single line from the real input stream and adds it to the end of the
// string stream
// at the end it calls standard BF comman instruction handler
template <class T>
inline void DebugInterpreter<T>::debug_comma_(BF::Interpreter<T> *inter)
{
    DebugInterpreter<T> *d_inter = static_cast<DebugInterpreter<T> *>(inter);

    if (static_cast<std::size_t>(d_inter->fake_is.tellg()) ==
        d_inter->fake_is.str().size())
    {
        std::string line;
        if (!d_inter->real_is.eof())
        {
            std::getline(d_inter->real_is, line);
            line += '\n';
        }
        DEBUG_PRINT("[DebugInterpreter]: loading next input line. Content: \""
                    << line << "\"");
        auto pos = d_inter->fake_is.tellg();
        d_inter->fake_is.str(d_inter->fake_is.str() + line);
        d_inter->fake_is.seekg(pos);
    }
    Interpreter<T>::comma_(inter);
}

template <class T>
void DebugInterpreter<T>::vertical_line_(BF::Interpreter<T> *inter)
{
    DebugInterpreter *d_inter = static_cast<DebugInterpreter<T> *>(inter);
    d_inter->debug_here();
}

template <class T>
DebugInterpreter<T>::DebugInterpreter(std::ostream &os, std::istream &is,
                                      MemoryBase<T> *mem,
                                      MemDbgrBase<T> *mem_dbgr)
    : Interpreter<T>(fake_os, fake_is, mem),
      mem_dbgr(mem_dbgr),
      real_os(os),
      real_is(is)
{
    if (this->mem_dbgr == nullptr)
    {
        throw std::invalid_argument(
            "Memory debugger pointer cannot be nullptr.");
    }

    this->register_handler('.', &DebugInterpreter::debug_dot_);
    this->register_handler(',', &DebugInterpreter::debug_comma_);
    this->register_handler('|', &DebugInterpreter::vertical_line_);
}

template <class T>
DebugInterpreter<T>::~DebugInterpreter()
{
    delete this->mem_dbgr;
}

template <class T>
bool DebugInterpreter<T>::run(const utils::Program::container &prog)
{
    this->prog_begin = prog.begin();
    return this->Interpreter<T>::run(prog);
}

// #############################################################################
// #############################################################################
// #### OPTIMIZED INTERPRETER ##################################################
// #############################################################################
// #############################################################################

template <class T>
void OptimizedInterpreter<T>::multi_value_inc_(BF::Interpreter<T> *inter)
{
    OptimizedInterpreter *o_inter =
        static_cast<OptimizedInterpreter<T> *>(inter);

    o_inter->mem->ref() += utils::prog_to_uint(o_inter->inst_ptr);
}

template <class T>
void OptimizedInterpreter<T>::multi_value_dec_(BF::Interpreter<T> *inter)
{
    OptimizedInterpreter *o_inter =
        static_cast<OptimizedInterpreter<T> *>(inter);

    o_inter->mem->ref() -= utils::prog_to_uint(o_inter->inst_ptr);
}

template <class T>
void OptimizedInterpreter<T>::multi_memptr_inc_(BF::Interpreter<T> *inter)
{
    OptimizedInterpreter *o_inter =
        static_cast<OptimizedInterpreter<T> *>(inter);

    o_inter->mem->move_ptr(utils::prog_to_uint(o_inter->inst_ptr));
}

template <class T>
void OptimizedInterpreter<T>::multi_memptr_dec_(BF::Interpreter<T> *inter)
{
    OptimizedInterpreter *o_inter =
        static_cast<OptimizedInterpreter<T> *>(inter);

    o_inter->mem->move_ptr(-utils::prog_to_uint(o_inter->inst_ptr));
}

// same as Interpreter::left_square_bracket_, but has different code for
// skipping the loops
// this is done because multi character optimized instructions may contain
// square brackets in one of the later bytes and it would break the standard
// version of this function
template <class T>
void OptimizedInterpreter<T>::optim_left_square_bracket_(
    BF::Interpreter<T> *inter)
{
    OptimizedInterpreter *o_inter =
        static_cast<OptimizedInterpreter<T> *>(inter);

    if (o_inter->mem->ref() != 0)
    {
        utils::Program::container::const_iterator tmp = o_inter->inst_ptr;
        do
        {
            o_inter->inst_ptr = tmp + 1;

            o_inter->run_();
        } while (o_inter->mem->ref() != 0);

        --o_inter->inst_ptr;
    }
    else
    {
        int stop_after = 1;
        do
        {
            ++o_inter->inst_ptr;
            if (*o_inter->inst_ptr == ']')
                --stop_after;
            else if (*o_inter->inst_ptr == '[')
                ++stop_after;
            if (optimizers::op_mark_3_byte_begin <= *o_inter->inst_ptr &&
                *o_inter->inst_ptr < optimizers::op_mark_3_byte_end)
            {
                o_inter->inst_ptr += 2;
            }

        } while (stop_after != 0);
    }
}

template <class T>
OptimizedInterpreter<T>::OptimizedInterpreter(std::ostream &os,
                                              std::istream &is,
                                              MemoryBase<T> *mem)
    : Interpreter<T>(os, is, mem)
{
    this->register_handler(BF::optimizers::op_multi_inc,
                           &OptimizedInterpreter::multi_value_inc_);
    this->register_handler(BF::optimizers::op_multi_dec,
                           &OptimizedInterpreter::multi_value_dec_);
    this->register_handler(BF::optimizers::op_multi_ptr_inc,
                           &OptimizedInterpreter::multi_memptr_inc_);
    this->register_handler(BF::optimizers::op_multi_ptr_dec,
                           &OptimizedInterpreter::multi_memptr_dec_);
    this->register_handler('[',
                           &OptimizedInterpreter::optim_left_square_bracket_);
}

// #############################################################################
// #############################################################################
// #### INTERPRETER FACTORY ####################################################
// #############################################################################
// #############################################################################

template <class T>
InterpreterBase *InterFactory::new_inter_standard(std::ostream &os,
                                                  std::istream &is)
{
    InterpreterBase *rv = nullptr;
    DEBUG_PRINT("[InterFactory]: Creating standard interpreter");
    rv = new Interpreter<T>(os, is, this->mem_factory->new_mem<T>());

    return rv;
}

template <class T>
InterpreterBase *InterFactory::new_inter_standard_debug(std::ostream &os,
                                                        std::istream &is)
{
    InterpreterBase *rv = nullptr;
    DEBUG_PRINT("[InterFactory]: Creating standard debugging interpreter");
    auto mem_and_dbgr = this->mem_factory->new_debug_mem<T>();
    rv                = new DebugInterpreter<T>(os, is, mem_and_dbgr.first,
                                 mem_and_dbgr.second);

    return rv;
}
template <class T>
InterpreterBase *InterFactory::new_inter_optimized(std::ostream &os,
                                                   std::istream &is)
{
    InterpreterBase *rv = nullptr;
    DEBUG_PRINT("[InterFactory]: Creating optimized interpreter");
    rv = new OptimizedInterpreter<T>(os, is, this->mem_factory->new_mem<T>());

    return rv;
}

inline InterFactory::InterFactory()
    : inter_class(InterClass::none), mem_factory(nullptr)
{
}

inline void InterFactory::set_inter_class(InterClass value)
{
    this->inter_class = value;
}

inline void InterFactory::set_mem_factory(MemFactory *value)
{
    this->mem_factory = value;
}

template <class T>
InterpreterBase *InterFactory::new_inter(std::ostream &os, std::istream &is)
{
    InterpreterBase *rv = nullptr;

    if (this->mem_factory == nullptr)
    {
        throw std::logic_error(
            "Cannot create new interpreter instance because memory factory is "
            "not set.");
    }

    switch (this->inter_class)
    {
    case InterClass::standard:
        rv = this->new_inter_standard<T>(os, is);
        break;

    case InterClass::standard_debug:
        rv = this->new_inter_standard_debug<T>(os, is);
        break;

    case InterClass::optimized:
        rv = this->new_inter_optimized<T>(os, is);
        break;

    case InterClass::none:
    default:
        throw inter_type_error();
        break;
    }

    return rv;
}

} // namespace BF
