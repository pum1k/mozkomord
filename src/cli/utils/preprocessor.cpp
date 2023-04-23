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

} // namespace utils
