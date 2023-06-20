#ifndef CLI_BF_OPTIMIZER_HPP_
#define CLI_BF_OPTIMIZER_HPP_

#include <list>

#include "../utils/program.hpp"
#include "../utils/program_utils.hpp"

namespace BF
{

namespace optimizers
{

/**
 * This enum defines values used for optimized BF instructions.
 */
enum : unsigned char
{
    op_mark_3_byte_begin = 128,
    op_multi_inc         = 128,
    op_multi_dec,
    op_multi_ptr_inc,
    op_multi_ptr_dec,
    op_mark_3_byte_end,
    op_set_zero,
};

/**
 * Replaces consecutive operations +-<> with a single instruction that has the
 * same effect.
 */
void remove_consecutive_operations(
    std::list<utils::Program::value_type> &prog_l);

/**
 * Replace the "[-]" and "[+]" patterns with a single instruction that has the
 * same effect.
 */
void replace_set_zero(std::list<utils::Program::value_type> &prog_l);

} // namespace optimizers

} // namespace BF

#endif // CLI_BF_OPTIMIZER_HPP_
