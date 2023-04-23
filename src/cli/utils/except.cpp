#include "except.hpp"

namespace utils
{

load_file_error::load_file_error(const std::string &filename)
{
    this->msg = "Could not load file: \"";
    this->msg += filename;
    this->msg += "\" Try checking the path.";
}

const char *load_file_error::what() const noexcept { return this->msg.c_str(); }

} // namespace utils
