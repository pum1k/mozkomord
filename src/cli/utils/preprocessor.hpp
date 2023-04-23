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

} // namespace utils

#endif // CLI_UTILS_PREPROCESSOR_HPP_
