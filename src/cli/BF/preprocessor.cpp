#include "preprocessor.hpp"

void BFPreprocessor::set_program(const std::vector<char> &prog)
{
    this->program = prog;
}

const std::vector<char> &BFPreprocessor::get_program() { return this->program; }

void BFPreprocessor::set_check(
    const std::function<void(std::vector<char> &)> &func)
{
    this->check_ = func;
}

void BFPreprocessor::set_process(
    const std::function<void(std::vector<char> &, bool)> &func)
{
    this->process_ = func;
}

void BFPreprocessor::check() { return this->check_(this->program); }

void BFPreprocessor::process(bool keep_debug /* = true */)
{
    return this->process_(this->program, keep_debug);
}

void BF_standard_check(std::vector<char> &prog)
{
    int opening_bracket_count = 0;
    int line_num              = 1;
    for (char c : prog)
    {
        if (c == '[')
        {
            ++opening_bracket_count;
        }
        else if (c == ']')
        {
            --opening_bracket_count;
            if (opening_bracket_count < 0)
            {
                // TODO: Change to different exception class, probably custom
                // "Unexpected closing bracket."
                throw std::exception();
            }
        }
        else if (c == '\n')
        {
            ++line_num;
        }
    }
    if (opening_bracket_count > 0)
    {
        // TODO: Change to different exception class, probably custom
        // "Missing closing bracket."
        throw std::exception();
    }
}
