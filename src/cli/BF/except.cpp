#include "except.hpp"

namespace BF
{

const char *mem_type_error::what() const noexcept
{
    return "Cannot create memory of this type";
}

const char *inter_type_error::what() const noexcept
{
    return "Cannot create interpreter of this type";
}

} // namespace BF
