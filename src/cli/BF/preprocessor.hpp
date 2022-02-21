#ifndef CLI_BF_PREPROCESSOR_HPP_
#define CLI_BF_PREPROCESSOR_HPP_

#include <algorithm>
#include <functional>
#include <list>

#include "except.hpp"
#include "optimizer.hpp"
#include "program.hpp"

namespace BF
{
/**
 * This class holds pointers to functions used for checking and preprocessing
 * BF code.
 * Once set up, you can use these functions without knowing, which exact
 * function is used as back-end for the operation.
 */
class Preprocessor
{
 protected:
    std::function<void(const Program::container &)> check_;
    std::function<void(Program::container &, bool)> process_;

 public:
    void set_check(const std::function<void(const Program::container &)> &func);
    void set_process(
        const std::function<void(Program::container &, bool)> &func);

    void check(Program &prog);
    void process(Program &prog, bool keep_debug = true);
};

namespace check
{
/**
 * Check if all square brackets are in pairs.
 * Stops and throws error on first recognised problem.
 */
void standard(const Program::container &prog);
} // namespace check

namespace prep
{
/**
 * Does nothing.
 * Used as preprocessing function if no preprocessing should be done.
 */
void noop(Program::container &, bool);

/**
 * Removes all symbols that are not used by BF language. (symbols that are NOT
 * removed: +-<>[]., )
 * If `keep_debug` is true, vertical line symbol (|) is also kept.
 */
void remove_unused(Program::container &prog, bool keep_debug);

void optimize(Program::container &prog, bool keep_debug);
} // namespace prep

} // namespace BF

#endif // CLI_BF_PREPROCESSOR_HPP_
