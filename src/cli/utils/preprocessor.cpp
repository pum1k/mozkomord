#include "preprocessor.hpp"

namespace utils
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

void check_balance(const Program::container &prog,
                   Program::value_type open_token,
                   Program::value_type close_token)
{
    int opening_bracket_count = 0;
    int line_num              = 1;
    int char_num              = 0;
    for (char c : prog)
    {
        ++char_num;

        if (c == open_token)
        {
            ++opening_bracket_count;
        }
        else if (c == close_token)
        {
            --opening_bracket_count;
            if (opening_bracket_count < 0)
            {
                throw unexpected_token_error(line_num, char_num);
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
        throw missing_token_error(std::string() + (char)close_token);
    }
}

} // namespace check

namespace prep
{

void noop(utils::Program::container &, bool) {}

} // namespace prep

} // namespace utils
