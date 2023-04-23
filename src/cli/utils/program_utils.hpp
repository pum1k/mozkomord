#ifndef CLI_UTILS_PROGRAM_UTILS_HPP_
#define CLI_UTILS_PROGRAM_UTILS_HPP_

#include <type_traits>

#include "program.hpp"

namespace utils
{

// `uint_to_prog` and `prog_to_uint` iterator & argument must be iterator
// reference
// it can be used with std::vector, std::list, etc.
// `uint_to_prog` needs to be able to write data to the iterator
template <class iterator,
          std::enable_if_t<std::is_same_v<typename iterator::value_type,
                                          Program::value_type>,
                           bool> = true>
void uint_to_prog(iterator &inst_ptr, uint16_t num);

template <class iterator,
          std::enable_if_t<std::is_same_v<typename iterator::value_type,
                                          Program::value_type>,
                           bool> = true>
uint16_t prog_to_uint(iterator &inst_ptr);

} // namespace utils

#include "program_utils.tpp"

#endif // CLI_UTILS_PROGRAM_UTILS_HPP_
