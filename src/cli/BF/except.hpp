/**
 * This file defines exceptions used by other BF classes/functions.
 * All of these exceptions inherit (possibly indirectly) from std::exception.
 */

#ifndef CLI_BF_EXCEPT_HPP_
#define CLI_BF_EXCEPT_HPP_

#include <exception>
#include <string>
#include <utility>

#include "strings.hpp"

namespace BF
{

/**
 * This exception will be thrown by BF memory factory when the requested type is
 * not valid.
 */
class mem_type_error : public std::bad_alloc
{
 private:
 public:
    virtual const char *what() const noexcept override;
};

/**
 * This exception will be thrown by BF interpreter factory when the requested
 * type is not valid.
 */
class inter_type_error : public std::bad_alloc
{
 private:
 public:
    virtual const char *what() const noexcept override;
};

} // namespace BF

#endif // CLI_BF_EXCEPT_HPP_
