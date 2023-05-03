#include "except.hpp"

namespace utils
{

void unexpected_token_error::init_msg()
{
    if (!this->has_pos())
    {
        this->msg = "Syntax error: ";
        this->msg += "Unexpected token.";
        return;
    }
    this->msg = "Syntax error: ";
    this->msg += "Unexpected token at line: ";
    this->msg += std::to_string(this->pos.first);
    this->msg += ", char: ";
    this->msg += std::to_string(this->pos.second);
    this->msg += '.';
}

const char *unexpected_token_error::what() const noexcept
{
    return this->msg.c_str();
}

bool unexpected_token_error::has_pos() const noexcept
{
    return !(this->pos.first == 0 || this->pos.second == 0);
}

missing_token_error::missing_token_error(std::string token)
{
    this->msg = "Syntax error: ";
    this->msg += "Missing token: \"";
    this->msg += token;
    this->msg += "\"";
}

const char *missing_token_error::what() const noexcept
{
    return this->msg.c_str();
}

load_file_error::load_file_error(const std::string &filename)
{
    this->msg = "Could not load file: \"";
    this->msg += filename;
    this->msg += "\" Try checking the path.";
}

const char *load_file_error::what() const noexcept { return this->msg.c_str(); }

} // namespace utils
