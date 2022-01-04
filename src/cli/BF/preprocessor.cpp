#include "preprocessor.hpp"

void BFPreprocessor::set_program(const std::vector<char> &prog)
{
    this->program = prog;
}

const std::vector<char> &BFPreprocessor::get_program() { return this->program; }
