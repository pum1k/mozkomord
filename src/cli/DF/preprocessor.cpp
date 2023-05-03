#include "preprocessor.hpp"

namespace DF
{

namespace check
{

void standard(const utils::Program::container &prog)
{
    utils::check::check_balance(prog, '[', ']');
    utils::check::check_balance(prog, '{', '}');
}

} // namespace check

namespace prep
{

} // namespace prep

} // namespace DF
