#include "preprocessor.hpp"

namespace BF
{
void Preprocessor::set_program(const std::vector<char> &prog)
{
    this->program = prog;
}

const std::vector<char> &Preprocessor::get_program() { return this->program; }

void Preprocessor::set_check(
    const std::function<void(const std::vector<char> &)> &func)
{
    this->check_ = func;
}

void Preprocessor::set_process(
    const std::function<void(std::vector<char> &, bool)> &func)
{
    this->process_ = func;
}

void Preprocessor::check() { return this->check_(this->program); }

void Preprocessor::process(bool keep_debug /* = true */)
{
    return this->process_(this->program, keep_debug);
}

void standard_check(const std::vector<char> &prog)
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
void noop(std::vector<char> &, bool) {}

void remove_unused(std::vector<char> &prog, bool keep_debug)
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
