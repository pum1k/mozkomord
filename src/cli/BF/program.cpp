#include "program.hpp"

namespace BF
{

Program::Program(const std::string &prog) { this->set(prog); }
Program::Program(const container &prog) { this->set(prog); }

void Program::set(const std::string &prog)
{
    this->prog = container(prog.begin(), prog.end());
}

void Program::set(const container &prog) { this->prog = prog; }

void Program::load(const std::string &file)
{
    std::ifstream in(file, std::ios::in);
    if (!in)
    {
        throw load_file_error(file);
    }

    std::istreambuf_iterator<char> begin(in), end;
    container prog(begin, end);

    this->prog = std::move(prog);
}

Program::container &Program::get() { return this->prog; }
const Program::container &Program::get_const() const { return this->prog; }

} // namespace BF
