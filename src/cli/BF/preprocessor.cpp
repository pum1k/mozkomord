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

void standard_check(const Program::container &prog)
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
} // namespace prep

} // namespace BF
