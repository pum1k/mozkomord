#include "except.hpp"

namespace BF
{

void unexpected_bracket_error::init_msg()
{
    if (!this->has_pos())
    {
        this->msg = "Unexpected closing bracket.";
        return;
    }
    this->msg = "Unexpected closing bracket at line: ";
    this->msg += std::to_string(this->pos.first);
    this->msg += ", char: ";
    this->msg += std::to_string(this->pos.second);
    this->msg += '.';
}

const char *unexpected_bracket_error::what() const noexcept
{
    return this->msg.c_str();
}

bool unexpected_bracket_error::has_pos() const noexcept
{
    return !(this->pos.first == 0 || this->pos.second == 0);
}

const char *missing_bracket_error::what() const noexcept
{
    return "Missing closing bracket.";
}

load_file_error::load_file_error(const std::string &filename)
{
    this->msg = "Could not load file: \"";
    this->msg += filename;
    this->msg += "\" Try checking the path.";
}

const char *load_file_error::what() const noexcept { return this->msg.c_str(); }

const char *mem_type_error::what() const noexcept
{
    return "Cannot create memory of this type";
}

const char *inter_type_error::what() const noexcept
{
    return "Cannot create interpreter of this type";
}

} // namespace BF
