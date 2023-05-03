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

void check_balanced_brackets(const Program::container &prog, size_t size,
                             Program::value_type *opening_tokens,
                             Program::value_type *closing_tokens)
{
    auto find = [size](Program::value_type *haystack,
                       Program::value_type needle) -> int
    {
        for (size_t i = 0; i < size; i++)
        {
            if (haystack[i] == needle)
            {
                return i;
            }
        }

        return -1;
    };

    std::vector<Program::value_type> opened_stack;
    int line_num = 1;
    int char_num = 0;

    int tmp;

    for (char c : prog)
    {
        ++char_num;

        if ((tmp = find(opening_tokens, c)) != -1)
        {
            opened_stack.push_back(c);
        }
        else if ((tmp = find(closing_tokens, c)) != -1)
        {
            if (opened_stack.size() == 0 ||
                (opened_stack.back() != opening_tokens[tmp]))
            {
                throw unexpected_token_error(line_num, char_num);
            }
            opened_stack.pop_back();
        }
        else if (c == '\n')
        {
            ++line_num;
            char_num = 0;
        }
    }
    if (opened_stack.size() > 0)
    {
        throw missing_token_error(std::string() + (char)opened_stack.back());
    }
}

} // namespace check

namespace prep
{

void noop(utils::Program::container &, bool) {}

} // namespace prep

} // namespace utils
