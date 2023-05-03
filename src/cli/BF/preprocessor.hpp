#ifndef CLI_BF_PREPROCESSOR_HPP_
#define CLI_BF_PREPROCESSOR_HPP_

#include <algorithm>
#include <functional>
#include <list>

#include "../utils/preprocessor.hpp"
#include "../utils/program.hpp"
#include "except.hpp"
#include "optimizer.hpp"

namespace BF
{

namespace check
{

/**
 * Check if all square brackets are in pairs.
 * Stops and throws error on first recognised problem.
 */
void standard(const utils::Program::container &prog);

} // namespace check

namespace prep
{

/**
 * Removes all symbols that are not used by BF language. (symbols that are NOT
 * removed: +-<>[]., )
 * If `keep_debug` is true, vertical line symbol (|) is also kept.
 */
void remove_unused(utils::Program::container &prog, bool keep_debug);

/**
 * First calls `remove_unused` to clean up the code.
 * After that, it tries to optimize the code with custom operations.
 */
void optimize(utils::Program::container &prog, bool keep_debug);

} // namespace prep

} // namespace BF

#endif // CLI_BF_PREPROCESSOR_HPP_
