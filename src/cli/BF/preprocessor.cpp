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
