#ifndef CLI_UTILS_PREPROCESSOR_HPP_
#define CLI_UTILS_PREPROCESSOR_HPP_

#include <functional>

#include "../utils/program.hpp"

namespace utils
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

void check_balanced_brackets(const Program::container &prog, size_t size,
                             Program::value_type *opening_tokens,
                             Program::value_type *closing_tokens);

} // namespace check

namespace prep
{

/**
 * Does nothing.
 * Used as preprocessing function if no preprocessing should be done.
 */
void noop(utils::Program::container &, bool);

/**
 * Returns a function that can be used as preprocessor's process function. The
 * returned function will remove all characters, that are not contained in the
 * used_chars string.
 */
std::function<void(utils::Program::container &, bool)> get_unused_remover(
    std::string used_chars);

} // namespace prep

} // namespace utils

#endif // CLI_UTILS_PREPROCESSOR_HPP_
