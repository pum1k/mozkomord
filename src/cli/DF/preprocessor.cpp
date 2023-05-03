#include "preprocessor.hpp"

namespace DF
{

namespace check
{

void standard(const utils::Program::container &prog)
{
    utils::Program::value_type opening[] = {'[', '{'};
    utils::Program::value_type closing[] = {']', '}'};
    utils::check::check_balanced_brackets(prog, 2, opening, closing);
}

} // namespace check

namespace prep
{

} // namespace prep

} // namespace DF
