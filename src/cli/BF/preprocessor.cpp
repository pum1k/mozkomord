#include "preprocessor.hpp"

namespace BF
{

void Preprocessor::set_check(
    const std::function<void(const Program::container &)> &func)
{
    this->check_ = func;
}

void Preprocessor::set_process(
    const std::function<void(Program::container &, bool)> &func)
{
    this->process_ = func;
}

void Preprocessor::check(Program &prog) { return this->check_(prog.get()); }

void Preprocessor::process(Program &prog, bool keep_debug /* = true */)
{
    return this->process_(prog.get(), keep_debug);
}

namespace check
{

void standard(const Program::container &prog)
{
    int opening_bracket_count = 0;
    int line_num              = 1;
    int char_num              = 0;
    for (char c : prog)
    {
        ++char_num;

        if (c == '[')
        {
            ++opening_bracket_count;
        }
        else if (c == ']')
        {
            --opening_bracket_count;
            if (opening_bracket_count < 0)
            {
                throw unexpected_bracket_error(line_num, char_num);
            }
        }
        else if (c == '\n')
        {
            ++line_num;
            char_num = 0;
        }
    }
    if (opening_bracket_count > 0)
    {
        throw missing_bracket_error();
    }
}

} // namespace check

namespace prep
{

void noop(Program::container &, bool) {}

void remove_unused(Program::container &prog, bool keep_debug)
{
    std::function<bool(char)> pred;
    if (keep_debug)
    {
        pred = [](char c) -> bool
        {
            return !((c == '+') || (c == '-') || (c == '<') || (c == '>') ||
                     (c == '[') || (c == ']') || (c == '.') || (c == ',') ||
                     (c == '|'));
        };
    }
    else
    {
        pred = [](char c) -> bool
        {
            return !((c == '+') || (c == '-') || (c == '<') || (c == '>') ||
                     (c == '[') || (c == ']') || (c == '.') || (c == ','));
        };
    }

    prog.erase(std::remove_if(prog.begin(), prog.end(), pred), prog.end());
}

void optimize(Program::container &prog, bool keep_debug)
{
    remove_unused(prog, keep_debug);

    std::list<Program::value_type> prog_l(prog.begin(), prog.end());

    optimizers::remove_consecutive_operations(prog_l);

    prog.resize(prog_l.size());
    std::copy(prog_l.begin(), prog_l.end(), prog.begin());
}

} // namespace prep

} // namespace BF
