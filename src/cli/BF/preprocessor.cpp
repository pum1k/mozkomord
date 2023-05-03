#include "preprocessor.hpp"

namespace BF
{

namespace check
{

void standard(const utils::Program::container &prog)
{
    utils::check::check_balance(prog, '[', ']');
}

} // namespace check

namespace prep
{

void remove_unused(utils::Program::container &prog, bool keep_debug)
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

void optimize(utils::Program::container &prog, bool keep_debug)
{
    remove_unused(prog, keep_debug);

    std::list<utils::Program::value_type> prog_l(prog.begin(), prog.end());

    optimizers::remove_consecutive_operations(prog_l);

    prog.resize(prog_l.size());
    std::copy(prog_l.begin(), prog_l.end(), prog.begin());
}

} // namespace prep

} // namespace BF
