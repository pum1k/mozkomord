#ifndef CLI_BF_OPTIMIZER_HPP_
#define CLI_BF_OPTIMIZER_HPP_

#include <list>

#include "program.hpp"

namespace BF
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

namespace optimizers
{

/**
 * This enum defines values used for optimized BF instructions.
 */
enum : unsigned char
{
    op_multi_inc = 128,
    op_multi_dec,
    op_multi_ptr_inc,
    op_multi_ptr_dec,

};

/**
 * Replaces consecutive operations +-<> with a single instruction that has the
 * same effect.
 */
void remove_consecutive_operations(std::list<Program::value_type> &prog_l);

} // namespace optimizers

} // namespace BF

#include "optimizer.tpp"

#endif // CLI_BF_OPTIMIZER_HPP_
